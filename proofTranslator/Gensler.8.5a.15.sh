#!/bin/bash
./translator-expect << ENDOFMESSAGE
((Some x)E(x)->R(a))
y
(All x)(E(x)->R(a))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
