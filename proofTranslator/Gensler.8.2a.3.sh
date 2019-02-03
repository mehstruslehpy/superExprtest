#!/bin/bash
./translator-expect << ENDOFMESSAGE
~(Some x)(F(x)&G(x))
n
(Some x)F(x)
y
(Some x)~G(x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
