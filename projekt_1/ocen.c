#include <stdbool.h>
#include <assert.h>
#include "zbior_ary.h"

int main() {
  // A = {2, 7, 12}
  zbior_ary A = ciag_arytmetyczny(2, 5, 12);
  assert(moc(A) == 3);
  assert(ary(A) == 1);
  // B = {4, 9, 14, 19}
  zbior_ary B = ciag_arytmetyczny(4, 5, 19);
  // C = {1}
  assert(moc(B) == 4);
  zbior_ary C = singleton(1);
  assert(ary(C) == 1);
  assert(nalezy(C, 2) == false);
  assert(nalezy(B, 9) == true);
  // D = {1, 4, 9, 14, 19}
  zbior_ary D = suma(B, C);
  assert(moc(D) == 5);
  // E = {1, 2, 4, 7, 9, 12, 14, 19}
  zbior_ary E = suma(D, A);
  assert(moc(E) == 8);
  // F = {14}
  zbior_ary F = singleton(14);
  zbior_ary T = iloczyn(F, A);
  assert(moc(T) == 0);
  // G = {1, 2, 4, 7, 9, 12, 19}
  zbior_ary G = roznica(E, F);
  assert(ary(G) == 4);
  // H = {4, 9, 19};
  zbior_ary H = iloczyn(G, B);
  assert(moc(H) == 3);
  assert(ary(H) == 2);
  // I = {}
  zbior_ary I = iloczyn(H, A);
  assert(moc(I) == 0);
  return 0;
}
