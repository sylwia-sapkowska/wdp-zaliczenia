for ((i= 0; ; i++))
do
    ./gen > wejscie.in
    ./brut < wejscie.in > out1
    ./wzor < wejscie.in > out2
    if diff -b out1 out2
    then
        echo "OK $i"
    else
        echo "fatalnie sylwia"
        exit
    fi
done