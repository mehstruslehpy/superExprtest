#!/bin/bash
./translator-expect << ENDOFMESSAGE
((All x)F(x)v(All x)G(x))
y
(All x)(F(x)vG(x))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
