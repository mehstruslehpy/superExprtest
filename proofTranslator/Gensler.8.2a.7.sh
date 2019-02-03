#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)~(F(x)vG(x))
y
(All x)~F(x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
