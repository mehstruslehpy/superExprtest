#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)(F(x)vG(x))
n
~(All x)F(x)
y
(Some x)G(x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
