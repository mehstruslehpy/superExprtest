;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;globals and spevial variables

; constants to use similar to C enums
; CEXPR := a conditional
(defparameter *cexpr* 0)
; OEXPR := an or
(defparameter *oexpr* 1)
; AEXPR := an and
(defparameter *aexpr* 2)
; NEXPR := a not
(defparameter *nexpr* 3)
; propexpr := a proposition
(defparameter *propexpr* 4)
; predexpr := a predicate
(defparameter *predexp* 5)

; tracks the current depth of quantification
; intended to be used with incf and decf
; this is not very useful now but could be useful later
(defparameter *quantdepth* 0)

;limits the depth of recursion
(defparameter *default-depth* 2)

;limits the arity of a relation/predicate
(defparameter *predicatearity* 5)

; quantvars := tracks what variables are currently attached to a quantifier
; ex
; to add do (push (some-func) *quant-vars*)
; to remove do (pop *quant-vars*)
; this is not very useful now but could be useful later
(defparameter *quantvars* '())

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;functions for picking random propositional letters

; use any upper case letter
;(defun random-upper ()
; (code-char (+ (random 25) 65)))
; use any lower case letter
;(defun random-lower ()
; (code-char (+ (random 25) 97)))

; use any of the first 4 upper case letters
(defun random-upper ()
 (code-char (+ (random 4) 65)))
; use any of the first 4 upper case letters
(defun random-lower ()
 (code-char (+ (random 4) 97)))


; prints a quantifier and tracks the quantifier depth and variable
(defun quantify? () 
 (let ((quantified (random 3)))
  (cond 
	 ((zerop quantified)
	  (incf *quantdepth*)
	  (let ((qvar (random-lower)))
	  (push qvar *quantvars*)
		(princ "(Some ")
    (princ qvar)
		(princ ")")))
	 ((= quantified 1)
	  (incf *quantdepth*)
	  (let ((qvar (random-lower)))
	  (push qvar *quantvars*)
		(princ "(All ")
    (princ qvar)
		(princ ")")))
   (t 
	  nil) ;the default case does nothing
		)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;functions to print random logical formulas, these generate a style warning
;due to being mutually recursive I am not sure how to disable these warnings

;pick one of the various kinds of binary operators and recurse
(defun generate-binary (exprtype iterdepth prop-func)
 (cond
  ((= exprtype *cexpr*)
   (princ "(")
	 (print-expr iterdepth prop-func)
	 (princ " -> ")
	 (print-expr iterdepth prop-func)
   (princ ")"))
  ((= exprtype *aexpr*)
   (princ "(")
	 (print-expr iterdepth prop-func)
	 (princ " & ")
	 (print-expr iterdepth prop-func)
   (princ ")"))
	((= exprtype *oexpr*)
   (princ "(")
	 (print-expr iterdepth prop-func)
	 (princ " v ")
	 (print-expr iterdepth prop-func)
   (princ ")"))
  ((zerop iterdepth)
	 (princ (funcall prop-func)))
	((princ (funcall prop-func))))
 t)

;pick a unary operator or a proposition and recurse
(defun generate-unary (exprtype iterdepth prop-func)
 (cond
  ((zerop exprtype)
   (princ (funcall prop-func)))
	((not (zerop exprtype))
	 (princ "~")
	 (print-expr iterdepth prop-func))))

;print a full propositional formula
(defun print-expr (iterdepth prop-func)
 (quantify?)
 (let ((arity (random 2)))
  (cond
   ((or (< iterdepth 0) (zerop iterdepth))
	  (princ (funcall prop-func)))
	 ((zerop arity)
	  (generate-binary (random 4) (- iterdepth 1) prop-func))
   ((not (zerop arity))
	  (generate-unary (random 2) (- iterdepth 1) prop-func))))
 (cond ; clean up some state if needed 
  ((> *quantdepth* 0) 
   (pop *quantvars*)
	 (decf *quantdepth*))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;print a nicely formatted argument
(defun build-pretty-argument (iter &optional (depth *default-depth*)(counter 0))
 (cond
  ((zerop iter)
   (fresh-line)
	 (princ "|- ")
	 (print-expr depth #'first-order-prop)
   (fresh-line))
  ((not(zerop iter))
   (fresh-line)
   (princ counter)
	 (princ ") ")
	 (print-expr depth #'first-order-prop)
   (fresh-line)
	 (build-pretty-argument (- iter 1) depth (+ counter 1)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;functions to randomly build predicates or propositions in first order logic
;(defun first-order-prop ()
; (let ((ptype (random 2)))
; (cond 
;  ((zerop ptype) 
;	 (random-upper))
;  ((= ptype 1)
;	 (build-predicate)))))
(defun first-order-prop ()
	(build-predicate))


(defun build-predicate ()
 (let ((return-string (make-adjustable-string (string (random-upper)))))
	(vector-push-extend #\( return-string)
  (loop for i from 1 to (+ (random *predicatearity*) 1) do
	 (vector-push-extend (random-lower) return-string)
	 (vector-push-extend #\, return-string))
	 (fill return-string #\) :start (-(length return-string) 1))
	 return-string))
;this function came from stack overflow
(defun make-adjustable-string (s)
 (make-array (length s)
  :fill-pointer (length s)
	:adjustable t
	:initial-contents s
	:element-type (array-element-type s)))
