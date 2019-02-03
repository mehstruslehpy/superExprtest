#!/bin/bash
#NOTE: see earlier arguments using R(a)
./translator-expect << ENDOFMESSAGE
(All x)(E(x)->R(a))
y
((All x)E(x)->R(a))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
