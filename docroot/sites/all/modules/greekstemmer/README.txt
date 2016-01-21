Drupal GreekStemmer module:
----------------------------
Author: Yannis Karampelas (yannisc - www.netstudio.gr)

License - GPL (see LICENSE)


Overview:
--------
The problem: 
Drupal's core search module requires an absolute match between the query word and a word e.g. in a node
in order to return the node as a result to the user. In the Greek language words tend to have many forms that
depend on many factors (mainly grammatical), a fact that almost completely ruins the core search of Drupal.

The proposed solution:
Stemming is performed to all words in the search query and to all words in all searchable content types (e.g. nodes etc..)

From Wikipedia (http://en.wikipedia.org/wiki/Stemming)
Stemming is the process for reducing inflected (or sometimes derived) words to their stem, 
base or root form – generally a written word form. The stem need not be identical to the morphological 
root of the word; it is usually sufficient that related words map to the same stem, even if this stem 
is not in itself a valid root.

As a result, the search.module returns more results (in contrast to none in most cases, if no stemming is used), with only a small drop
of precision.

The algorithm used is described in the master thesis of Georgios Ntais (Georgios.Ntais@eurodyn.com)
at Royal Institute of Technology [KTH], Stockholm Sweden 
(http://www.dsv.su.se/~hercules/papers/Ntais_greek_stemmer_thesis_final.pdf)

Installation:
------------
1. Place this module directory in your modules folder.
2. Go to "administer" -> "modules" and enable the module.
3. Go to "administer" -> "settings" -> "search" and click "Re-index site".
4. Run cron.php (http://yourdomain/cron.php)


Credits:
-------
Port to Drupal 7 was based on the UTF8 implementation of basos G ( basos.g@gmx.net ).
First implementation to Drupal CMS was done by Vassilis Spiliopoulos (http://www.psychfamily.gr) & Pantelis Nasikas
based on Spyros Saroukos PHP implementation. Spyros Saroukos implementation was based on the work of
Panos Kyriakakis (http://www.salix.gr) and Georgios Ntais (Georgios.Ntais@eurodyn.com)
Georgios firstly developed the stemmer's javascript implementation for his master thesis at Royal Institute of Technology [KTH], Stockholm Sweden
http://www.dsv.su.se/~hercules/papers/Ntais_greek_stemmer_thesis_final.pdf 
