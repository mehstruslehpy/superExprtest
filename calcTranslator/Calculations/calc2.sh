!#/bin/sh
#this is a calculation for the calculator
echo "P(x): x is a president"
echo "S(x): x plays the saxophone"
echo "c: Bill Clinton"
echo "b: George Bush"
cd ..
./translator-expect << MESSAGE
(All x)(P(x)->S(x))
n
P(b)
n
P(c)
n
(Some x)(~P(x)vS(x))
y
MESSAGE
chmod +x output_expect.sh
echo "running expect script please select the correct true or false valuations for the prover"
./output_expect.sh
cd Calculations
