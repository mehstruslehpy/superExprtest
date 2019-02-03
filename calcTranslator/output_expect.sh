#!/bin/expect
spawn ../main
send "c\r"; #this program is always calc mode
send "2\r"; #binary
send "o\r"; #binary connective
send "1\r"; #unary
send "e\r"; #existential
send "c\r"; #dependent variable
send "1\r"; #unary
send "v\r"; #variable
send "C\r"; #relation name
send "c,b,d,c\r"; #tuple
send "2\r"; #binary
send "a\r"; #binary connective
send "2\r"; #binary
send "a\r"; #binary connective
send "2\r"; #binary
send "a\r"; #binary connective
send "1\r"; #unary
send "v\r"; #variable
send "B\r"; #relation name
send "b,d,d,c,b\r"; #tuple
send "1\r"; #unary
send "n\r"; #not
send "2\r"; #binary
send "c\r"; #binary connective
send "1\r"; #unary
send "u\r"; #universal
send "d\r"; #dependent variable
send "1\r"; #unary
send "n\r"; #not
send "1\r"; #unary
send "u\r"; #universal
send "c\r"; #dependent variable
send "1\r"; #unary
send "v\r"; #variable
send "B\r"; #relation name
send "d,a\r"; #tuple
send "1\r"; #unary
send "u\r"; #universal
send "b\r"; #dependent variable
send "1\r"; #unary
send "v\r"; #variable
send "C\r"; #relation name
send "d\r"; #tuple
send "1\r"; #unary
send "u\r"; #universal
send "b\r"; #dependent variable
send "2\r"; #binary
send "o\r"; #binary connective
send "1\r"; #unary
send "u\r"; #universal
send "c\r"; #dependent variable
send "1\r"; #unary
send "v\r"; #variable
send "C\r"; #relation name
send "a,d,d,c,a\r"; #tuple
send "1\r"; #unary
send "n\r"; #not
send "1\r"; #unary
send "e\r"; #existential
send "a\r"; #dependent variable
send "1\r"; #unary
send "n\r"; #not
send "1\r"; #unary
send "u\r"; #universal
send "d\r"; #dependent variable
send "1\r"; #unary
send "v\r"; #variable
send "C\r"; #relation name
send "d,b,d\r"; #tuple
send "1\r"; #unary
send "n\r"; #not
send "2\r"; #binary
send "a\r"; #binary connective
send "1\r"; #unary
send "u\r"; #universal
send "b\r"; #dependent variable
send "1\r"; #unary
send "v\r"; #variable
send "A\r"; #relation name
send "b,a,a,c\r"; #tuple
send "1\r"; #unary
send "n\r"; #not
send "1\r"; #unary
send "u\r"; #universal
send "c\r"; #dependent variable
send "1\r"; #unary
send "n\r"; #not
send "1\r"; #unary
send "u\r"; #universal
send "b\r"; #dependent variable
send "2\r"; #binary
send "c\r"; #binary connective
send "1\r"; #unary
send "e\r"; #existential
send "d\r"; #dependent variable
send "1\r"; #unary
send "v\r"; #variable
send "B\r"; #relation name
send "b,c,d,a,b\r"; #tuple
send "1\r"; #unary
send "n\r"; #not
send "1\r"; #unary
send "v\r"; #variable
send "A\r"; #relation name
send "a,c,a,b,c\r"; #tuple
send "q\r"; #last formula
interact; #hand input to user 
