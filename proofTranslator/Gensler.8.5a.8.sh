#!/bin/bash
./translator-expect << ENDOFMESSAGE
((Some x)F(x)->(All x)G(x))
n
~G(p)
y
~F(p)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
