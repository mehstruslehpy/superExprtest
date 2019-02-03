#!/bin/bash
./translator-expect << ENDOFMESSAGE
(Some x)~(F(x)vG(x))
n
(Some x)H(x)
n
~(Some x)F(x)
y
~(All x)(H(x)->G(x))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
