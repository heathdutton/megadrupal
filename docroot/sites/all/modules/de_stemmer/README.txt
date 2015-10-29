
Readme
------

This module implements a German stemming algorithm to improve German-language
searching with the Drupal built-in search.module.


It reduces each word in the index to its basic root or stem so that variations
on a word (e.g. "Menschlichkeit -> menschlich" or "Städte -> Stadt") are
considered equivalent when searching. This generally results in more
relevant results.


The module contains two (static) lists
* one list of stop words
* one (currently very small) list of exceptions

Please feel free to comment these lists ...


Currently stemming results in finding additional documents.
And the list of search results contains these documents as well.
But the matching words are not recognized and not marked by the search module 
what makes searching not realy easy.
To solve this a patch for the search module is included in this package. Applying this
patch should not harm the search module in any way.


References:
  Algorithm:
    http://www.clef-campaign.org/workshop2002/WN/3.pdf
    http://w3.ub.uni-konstanz.de/v13/volltexte/2003/996//pdf/scherer.pdf
    http://kontext.fraunhofer.de/haenelt/kurs/Referate/Kowatschew_Lang/stemming.pdf
    http://www.cis.uni-muenchen.de/people/Schulz/SeminarSoSe2001IR/FilzmayerMargetic/referat.html
    http://www.ifi.unizh.ch/CL/broder/mue1/porter/stemming/node1.html

  For lists of stopwords see
    http://members.unine.ch/jacques.savoy/clef/index.html

  Irregular verbs:
    http://de.wiktionary.org/wiki/Wiktionary:Deutsch/Liste_der_unregelmäßigen_Verben

And please have also a loook at
http://groups.drupal.org/lucene-nutch-and-solr?page=1

