!#/bin/sh
#this is a calculation for the calculator
echo "P(x): x is a president"
echo "M(x): x is male"
echo "c: Bill Clinton"
echo "a: An arbitrary male non president"
cd ..
./translator-expect << MESSAGE
P(a)
n
P(c)
n
(Some x)(~P(x)&M(x))
y
MESSAGE
chmod +x output_expect.sh
echo "running expect script please select the correct true or false valuations for the prover"
./output_expect.sh
cd Calculations
