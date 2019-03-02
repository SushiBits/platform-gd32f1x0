#!/bin/bash

while read name rom ram ccm junk; do
	if [ "$ccm" == "64" ]; then
		RAMBASE=0x20010000
	elif [ "$ccm" == "128" ]; then
		RAMBASE=0x20020000
	else
		RAMBASE=0x20000000
	fi
	cat > $name.ld << EOF
MEMORY
{
	ITCM	(rwx) : ORIGIN = 0x00000000, LENGTH = 16k
    IROM    (r x) : ORIGIN = 0x00200000, LENGTH = ${rom}k
    AXIM	(r x) : ORIGIN = 0x08000000, LENGTH = ${rom}k
    DTCM    (rwx) : ORIGIN = 0x20000000, LENGTH = ${ccm}k
    IRAM    (rwx) : ORIGIN = ${RAMBASE}, LENGTH = $((${ram} - ${ccm}))k
}

INCLUDE $1
EOF
done
