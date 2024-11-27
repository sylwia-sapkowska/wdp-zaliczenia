#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

// Precision for doubleing point comparisons
const double EPS = 1e-7;

// Point with coordinates x, y.
typedef struct {
    double x, y;
} P;

typedef struct {
    double A, B, C; // Coefficients of the line Ax + By + C = 0
    P p1, p2; // To keep track of the direction of the line
} Line;

enum Shape { rectangle, circle};

typedef struct{
    P ld, ru;
} Rect;

typedef struct{
    P center;
    double radius;
} Circle;

// Paper structure that can be either a rectangle or a circle, keeps track of the folds (as lines).
typedef struct{
    enum Shape s;
    union {
        Rect r;
        Circle c;
    };
    Line* folds;
    int number_of_folds;
} Paper;

// Reflects point X over line L
P reflect(P X, Line L) {
    double x1 = X.x, y1 = X.y;
    double denom = L.A * L.A + L.B * L.B;
    
    assert(denom > 0);
    double x2 = x1 - (2 * L.A * (L.A * x1 + L.B * y1 + L.C)) / denom;
    double y2 = y1 - (2 * L.B * (L.A * x1 + L.B * y1 + L.C)) / denom;
    
    P reflection = {x2, y2};
    return reflection;
}

// Creates a line from two points A and B
Line create_line(P A, P B){
    Line L;
    L.p1 = A;
    L.p2 = B;
    L.A = A.y - B.y;
    L.B = B.x - A.x;
    L.C = - B.x * A.y + B.y * A.x;
    return L;
}

// Finds the cross product of vectors AB and AC
double cross_product(P A, P B, P C){
    return (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
}

// Unfolds current array of points over line L
// points - pointer to the array of points to reflect
void unfold(P** points, int *size, Line L){
    int new_size = 2 * (*size);
    P* unfolded_points = (P*)malloc((unsigned)(new_size) * sizeof(P));
    
    int ptr = 0;
    for (int j = 0; j<(*size); j++){
        P current = (*points)[j];
        P reflection = reflect(current, L);

        // If the current point is on the right side of the line, it was never on the paper
        if (cross_product(L.p1, current, L.p2) > EPS) {
            continue;
        }
        unfolded_points[ptr++] = current;
        // If the current point is on the edge, we don't need to add it again
        if (fabsl(current.x - reflection.x) < EPS && fabsl(current.y - reflection.y) < EPS){
            continue;
        }
        unfolded_points[ptr++] = reflection;
    }
    // Free the old array and assign the new one
    free(*points);
    *size = ptr;
    *points = unfolded_points;
    if ((*size) == 0){
        return;
    }
    *points = realloc(*points, (unsigned)(*size) * sizeof(P));
}

// Returns whether point X is inside the paper
bool inside(P X, Paper paper){
    if (paper.s == rectangle){
        Rect r = paper.r;
        return X.x >= r.ld.x - EPS && X.x <= r.ru.x + EPS 
            && X.y >= r.ld.y - EPS && X.y <= r.ru.y + EPS;
    } else {
        Circle c = paper.c;
        double x_diff = X.x - c.center.x;
        double y_diff = X.y - c.center.y;
        return c.radius - sqrtl(x_diff * x_diff + y_diff * y_diff) >= -EPS;
    }
}

// Returns the number of layers of paper that pass through point X
int number_of_folds(Paper paper, P X){
    P* reflections = (P*)malloc(sizeof(P));
    int size = 1;
    reflections[0] = X;
    
    for (int i = (int)paper.number_of_folds-1; i >= 0; i--){
        Line L = paper.folds[i];
        unfold(&reflections, &size, L);
        // If none of the points are inside the paper, we can return 0 immediately
        if (size == 0){
            free(reflections);
            return 0;
        }
    }
    int ans = 0;
    for (int i = 0; i<size; i++){
        P current = reflections[i];
        if (inside(current, paper)){
            ans++;
        }
    }
    free(reflections);
    return ans;
}

// Copies paper B to paper A
void copy_paper(Paper *A, Paper B){
    A->s = B.s;
    if (B.s == rectangle){
        A->r = B.r;
    } else {
        A->c = B.c;
    }
    A->number_of_folds = B.number_of_folds;
    A->folds = (Line*)malloc((unsigned)A->number_of_folds * sizeof(Line));
    for (int i = 0; i<A->number_of_folds; i++){
        A->folds[i] = B.folds[i];
    }
}

// Reads a rectangle from the input
void read_rectangle(Paper *paper){
    P A, B;
    scanf("%lf %lf %lf %lf", &A.x, &A.y, &B.x, &B.y);
    paper->s = rectangle;
    paper->r.ld = A;
    paper->r.ru = B;
    paper->number_of_folds = 0;
    paper->folds = NULL;
}

// Reads a circle from the input
void read_circle(Paper *paper){
    double r; P center;
    scanf("%lf %lf %lf", &center.x, &center.y, &r);
    paper->s = circle;
    paper->c.center = center;
    paper->c.radius = r;
    paper->number_of_folds = 0;
    paper->folds = NULL;
}

// Reads a fold from the input
void read_fold(Paper *paper){
    P A, B;
    scanf("%lf %lf %lf %lf", &A.x, &A.y, &B.x, &B.y);
    Line L = create_line(A, B);
    paper->folds = (Line*)realloc(paper->folds, (unsigned)(paper->number_of_folds + 1) * sizeof(Line));
    paper->folds[paper->number_of_folds] = L;
    paper->number_of_folds++;
}

void read_data(int n, Paper* papers){
    for (int i = 0; i < n; i++){
        char c; scanf(" %c", &c);
        if (c == 'P'){
            read_rectangle(&papers[i]);
        } else if (c == 'K') {
            read_circle(&papers[i]);
        } else {
            int k; scanf("%d", &k);
            --k; // 0-indexing
            copy_paper(&papers[i], papers[k]);
            read_fold(&papers[i]);
        }
    }
}

void answer_queries(int q, Paper* papers){
    for (int i = 0; i < q; i++){
        int k; P X;
        scanf("%d %lf %lf", &k, &X.x, &X.y);
        --k; // 0-indexing
        printf("%d\n", number_of_folds(papers[k], X));
    }
}

int main(){
    int n, q;
    scanf("%d %d", &n, &q);
    Paper* papers = (Paper*)malloc((unsigned)n * sizeof(Paper));
    read_data(n, papers);
    answer_queries(q, papers);

    for (int i = 0; i<n; i++){
        free(papers[i].folds);
    }
    free(papers);

    return 0;
}
