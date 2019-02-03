#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)F(x)
y
(Some x)(G(x)vF(x))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
