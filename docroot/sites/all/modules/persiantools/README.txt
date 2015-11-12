A module that would solve many of the common problems with persian drupal sites.

Current list of features to include:
1. convert all displayed integers from latin to persian, without breaking page 
   and scripts. (in progress)
2. convert arabic "ي" and "ک" characters to persian "ی" and "ک" characters.
3. fix invalid alphabetic sort in persian sites (پ,چ,ژ,گ show at bottom) due to
   wrong database collations.
4. fix misplaced presentation of text with mixed persian and english strings,
   by overriding the default bidi algorithm using special symbols.
