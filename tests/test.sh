#!/bin/sh

REF_OUT=".ref.out"
TEST_OUT=".test.out"

testrpn() {
    echo "$@ p" | dc 1> "$REF_OUT" 2> "/dev/null"
    echo -n "$@" | ./evalexpr -rpn 1> "$TEST_OUT" 2> "/dev/null"

    diff "$REF_OUT" "$TEST_OUT"
}

testnormal() {
    echo "$@" | bc 1> "$REF_OUT" 2> "/dev/null"
    echo -n "$@" | ./evalexpr 1> "$TEST_OUT" 2> "/dev/null"

    diff "$REF_OUT" "$TEST_OUT"
}

testnobc1() {
    echo "-6" 1> "$REF_OUT"
    echo -n "$@" | ./evalexpr 1> "$TEST_OUT"

    diff "$REF_OUT" "$TEST_OUT"
}

testerr1() {
    echo -n "$@" | ./evalexpr 2> "/dev/null"
    echo $? 1> "$TEST_OUT"
    echo "1" 1> "$REF_OUT"

    diff "$REF_OUT" "$TEST_OUT"
}

testerr2() {
    echo -n "$@" | ./evalexpr 2> "/dev/null"
    echo $? 1> "$TEST_OUT"
    echo "2" 1> "$REF_OUT"

    diff "$REF_OUT" "$TEST_OUT"
}

testerr3() {
    echo -n "$@" | ./evalexpr 2> "/dev/null"
    echo $? 1> "$TEST_OUT"
    echo "3" 1> "$REF_OUT"

    diff "$REF_OUT" "$TEST_OUT"
}

testerr4() {
    echo -n "" | ./evalexpr "$@" 2> "/dev/null"
    echo $? 1> "$TEST_OUT"
    echo "4" 1> "$REF_OUT"

    diff "$REF_OUT" "$TEST_OUT"
}

testrpnerr() {
    echo -n "$@" | ./evalexpr -rpn 2> "/dev/null"
    echo $? 1> "$TEST_OUT"
    echo "1" 1> "$REF_OUT"

    diff "$REF_OUT" "$TEST_OUT"
}

testnobc1 "+2*-3"
testerr1 "a+1"
testerr2 "2+"
testerr2 "1(+1)"
testerr2 "()"
testerr2 "("
testerr2 ")"
testerr3 "1%0"
testerr3 "1/0"
testerr3 "2^(-1)"
testerr4 "--toto"
testerr4 "--toto --ok"
testerr4 ""
testrpnerr "(2 3 +)"

testrpn ""
testrpn "0"
testrpn "1"
testrpn "   1"
testrpn "0 0 +"
testrpn "1 1 +"
testrpn "0 1 1 + +"
testrpn "  1  1   + "
testrpn "13 12 11 10 9 8 7 6 5 4 3 1 2 + + + + + + + + + + + +"
testrpn "50048 32678 +"
testrpn "1 1 -"
testrpn "0 1 1 - -"
testrpn "59984 65321 -"
testrpn "1 1 *"
testrpn "0 1 1 * *"
testrpn "3 2 1 * *"
testrpn "65464 2 *"
testrpn "1 1 /"
testrpn "6 2 1 / /"
testrpn "32768 2 /"
testrpn "1 1 %"
testrpn "6 2 4 % %"
testrpn "456 7 %"
testrpn "1 1 ^"
testrpn "3 7 9 9 6 3 2 + - * / % ^"
testrpn "2 9 * 1 + 13 - 2 * 2 / 3 %"

testnormal ""
testnormal "0"
testnormal "1"
testnormal "   0"
testnormal "()"
testnormal "(1)"
testnormal "-0"
testnormal "-1"
testnormal "3 +  5"
testnormal "1 + 2 +3+ 4"
testnormal "1 + (2+3)"
testnormal "(1)+"
testnormal "6565+99999"
testnormal "42+0"
testnormal "1-1"
testnormal "1 - 2 -6"
testnormal "-987"
testnormal "0-42"
testnormal "1-(3-5)"
testnormal "1*0"
testnormal "1*1"
testnormal "0*1"
testnormal "0*0"
testnormal "6*3"
testnormal "5*4*3*2*1"
testnormal "-3*2"
testnormal "2*(-5)"
testnormal "3/1"
testnormal "32/2"
testnormal "0/4"
testnormal "16/4"
testnormal "7/2"
testnormal "2/(3+4)"
testnormal "2/(6/3)"
testnormal "6/-1"
testnormal "-6/-2"
testnormal "-6/3"
testnormal "5%1"
testnormal "1%5"
testnormal "256%2"
testnormal "257%2"
testnormal "9%3"
testnormal "2%16"
testnormal "89562%101"
testnormal "49563%10"
testnormal "-1%5"
testnormal "-3%5"
testnormal "9%-2"
testnormal "0^5"
testnormal "1^0"
testnormal "1^5"
testnormal "2^3"
testnormal "2^8"
testnormal "2^2^2"
testnormal "2^(2^2)"
testnormal "2+3-4"
testnormal "6*8/2"
testnormal "3+5%2"
testnormal "5+2^3"
testnormal "6+(23+5*-2)^2"
testnormal "1+1*(1/1)-1^1"
testnormal "2+2\n"
