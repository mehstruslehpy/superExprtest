#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)(F(x)->G(x))
n
(All x)(F(x)->~G(x))
y
(All x)~F(x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
