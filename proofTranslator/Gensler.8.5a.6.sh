#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)((F(x)vG(x))->H(x))
n
F(m)
y
H(m)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
