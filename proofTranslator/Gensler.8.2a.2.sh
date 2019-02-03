#!/bin/bash
./translator-expect << ENDOFMESSAGE
~(Some x)(F(x)&~G(x))
y
(All x)(F(x)->G(x))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
