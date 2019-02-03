#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)((F(x)&G(x))->H(x))
n
(Some x)F(x)
n
(Some x)G(x)
y
(Some x)H(x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
