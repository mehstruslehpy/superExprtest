#!/bin/bash
./translator-expect << ENDOFMESSAGE
(R(a)->(All x)E(x))
y
(All x)(R(a)->E(x))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
