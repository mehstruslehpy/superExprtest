#!/bin/sbcl --script
;load the correct files
(load "common.lisp")
;seed the random number generator
(setf *random-state* (make-random-state t))
;build a random argmument with up to 20 premises
;(build-pretty-argument (+ (random 5) 1) 300)
(print-expr 9 #'first-order-prop)
(fresh-line)
