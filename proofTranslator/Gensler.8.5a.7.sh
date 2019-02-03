#!/bin/bash
./translator-expect << ENDOFMESSAGE
F(j)
n
(Some x)G(x)
n
(All x)((F(x)&G(x))->H(x))
y
(Some x)H(x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
