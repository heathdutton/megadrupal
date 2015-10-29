#!/bin/sh

export LANG=utf8

[ -e currency.inc ] && rm currency.inc && touch currency.inc

echo '<?php

/**
 * @file
 * List of currencies available.
 */

/**
 * Return a list of all currencies.
 */
function ec_receipt_get_all_currencies() {
  return array(' >> currency.inc

cut -f1,4 currency_list.txt| tail +2 | while read a b
do
  printf "    '%s' => \"%s - %s\",\n" "$a" "$a" "$b" >> currency.inc
done

echo '  );
}' >> currency.inc
