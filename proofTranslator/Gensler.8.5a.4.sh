#!/bin/bash
./translator-expect << ENDOFMESSAGE
((Some x)F(x)v(Some x)G(x))
y
(Some x)(F(x)vG(x))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
