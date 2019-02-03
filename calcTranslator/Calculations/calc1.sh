!#/bin/sh
#this is a calculation for the calculator
echo "P(x): x is a person"
echo "S(x): x is a sax player"
echo "c: Bill Clinton"
echo "e: The empire state building"
echo "T(x,y): x is taller than y"
cd ..
./translator-expect << MESSAGE
P(c)
n
P(e)
n
#
S(e)
n
T(c,e)
n
T(e,c)
n
((P(c)&T(c,e))v(~P(c)&T(e,c)))
n
T(e,e)
y
MESSAGE
chmod +x output_expect.sh
echo "running expect script please select the correct true or false valuations for the prover"
./output_expect.sh
cd Calculations
