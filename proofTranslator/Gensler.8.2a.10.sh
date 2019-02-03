#!/bin/bash
./translator-expect << ENDOFMESSAGE
(All x)((F(x)->G(x))&(G(x)->F(x)))
n
(Some x)~G(x)
y
(Some x)~F(x)
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
