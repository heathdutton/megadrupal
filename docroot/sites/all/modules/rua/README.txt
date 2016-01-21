Remove Uppercase Accents
Automatically removes accented characters (currently greek) from elements having
their text content uppercase transformed through CSS.

This project is now called 'rua'.
It used to be called 'remove_upcase_accents'.
On Drupalcon Prague I discussed with people about it and we agreed that
it should either something short or 'remove_upPERcase_accents'.
I preffered the short name :)

USAGE
The script operates automatically on the document ready event, by selecting all
the elements having their text content uppercase transformed through CSS, and by
replacing the accented characters in them by their respective non-accented.

The selection is based on the effective CSS rules defining the uppercase text
transformation, ie the following style rule:

  h1 { text-transform: uppercase; }

or

  .title { font-variant: small-caps; }

Currently the script transforms only greek text, but it can be easiy extended
to support other languages.

Original JS script is released under GPL license on github:
https://github.com/tdoumas/jquery-remove-upcase-accents

Converted to Drupal module by http://srm.gr
