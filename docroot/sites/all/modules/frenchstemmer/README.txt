Readme
------

This module implements the Paice Husk stemming algorithm to improve French-language
searching with the Drupal built-in search.module. It also does some extra job
to allow user mistakes ('hello' can be typed 'helo').

It reduces each word in the index to its basic root or stem so that variations
on a word are considered equivalent when searching. This generally results in more
relevant results.

