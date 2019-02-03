#!/bin/bash
./translator-expect << ENDOFMESSAGE
F(a)
y
~(Some x)(F(x)&~E(x,a))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
