#!/bin/bash
./translator-expect << ENDOFMESSAGE
(Some x)F(x)
y
(Some x)~F(x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
