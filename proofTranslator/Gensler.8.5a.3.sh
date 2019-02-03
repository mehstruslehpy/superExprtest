#!/bin/bash
#NOTE: the original was R and not R(a) but the prover
#		is not designed to intermingle propositions and relations
#		this technically inserts a subject into the universe
#		so it's not entirely equivalent
./translator-expect << ENDOFMESSAGE
((All x)E(x)->R(a))
y
(All x)(E(x)->R(a))
ENDOFMESSAGE
chmod +x output_expect.txt
./output_expect.txt
