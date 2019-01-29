\
\ This file and its contents are supplied under the terms of the
\ Common Development and Distribution License ("CDDL"), version 1.0.
\ You may only use this file in accordance with the terms of version
\ 1.0 of the CDDL.
\
\ A full copy of the text of the CDDL should have accompanied this
\ source.  A copy of the CDDL is also available via the Internet at
\ http://www.illumos.org/license/CDDL.
\

\
\ Copyright 2019 Joyent, Inc.
\

2 brandX ! 1 brandY ! \ Initialize brand placement defaults

: brand+ ( x y c-addr/u -- x y' )
	2swap 2dup at-xy 2swap \ position the cursor
	type \ print to the screen
	1+ \ increase y for next time we're called
;

: brand ( x y -- ) \ "illumos" [wide] logo in B/W (5 rows x 39 columns)

	s"       #                                   " brand+ 
	s"       #  ####  #   # ###### #    # #####  " brand+ 
	s"       # #    #  # #  #      ##   #   #    " brand+ 
	s"       # #    #   #   #####  # #  #   #    " brand+ 
	s" #     # #    #   #   #      #  # #   #    " brand+ 
	s" #     # #    #   #   #      #   ##   #    " brand+ 
	s"  #####   ####    #   ###### #    #   #  TM" brand+ 

	2drop
;
