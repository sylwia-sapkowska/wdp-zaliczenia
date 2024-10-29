for ((i = 0;; i++))
do
    ./gen > wejscie.in 
    ./brut < wejscie.in > brut.out
    ./wzor < wejscie.in > wzor.out

    if diff -b wzor.out brut.out > /dev/null
    then
        echo "OK $i"
    else
        echo "fatalnie sylwia"
        break
    fi
done