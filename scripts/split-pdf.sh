#!/bin/bash
conv=$PWD/split-by-bookmarks
if ! [ -x "$conv" ]; then
    echo >&2 "converter not found"
fi

i=0
pdftk "$1" dump_data_utf8 | "$conv" | while read -r a b title; do
    title=${title// /-}
    title=${title//\//-}
    title=${title//ä/ae}
    title=${title//ö/oe}
    ttile=${title//ü/ue}
    title=${title:-preamble}

    echo "$2-${i}_$title".pdf
    pdftk "$1" cat $a-$b output "$2-${i}_$title".pdf
    let i++;
done
