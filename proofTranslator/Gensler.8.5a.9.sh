#!/bin/bash
./translator-expect << ENDOFMESSAGE
(Some x)(F(x)vG(x))
y
((All x)~G(x)->(Some x)F(x))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
