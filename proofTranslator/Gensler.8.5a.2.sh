#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)(E(x)->R(a))
y
((Some x)E(x)->R(a))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
