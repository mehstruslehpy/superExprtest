#!/bin/bash
./translator-expect << ENDOFMESSAGE
(Some x)(F(x)v~G(x))
n
(All x)(~G(x)->H(x))
n
(Some x)(F(x)->H(x))
y
(Some x)H(x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
