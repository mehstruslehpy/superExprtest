#!/bin/bash
./translator-expect << ENDOFMESSAGE
(Some x)F(x)
y
(All x)F(x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt