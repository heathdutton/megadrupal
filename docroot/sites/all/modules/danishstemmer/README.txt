Danish Stemmer
================================================================================

Summary
--------------------------------------------------------------------------------

This module implements the Danish stemming algorithm from the Snowball project
to improve the usability of the core search module.

Read more about the algorithm and stemming in general at:

   http://snowball.tartarus.org/algorithms/danish/stemmer.html
   http://en.wikipedia.org/wiki/Porter_Stemmer


Installation
--------------------------------------------------------------------------------

1. Copy the danishstemmer folder to sites/all/modules or to a site-specific
   modules folder.
2. Go to Administer > Site building > Modules and enable the Danish Stemmer
   module.

The module includes unit tests based on the vocabulary from the Snowball
project. The current algorithm has an accuracy of 97%.


Configuration
--------------------------------------------------------------------------------

This module doesn't offer any configurable options.


Support
--------------------------------------------------------------------------------

Please post bug reports and feature requests in the issue queue:

   http://drupal.org/project/danishstemmer


Credits
--------------------------------------------------------------------------------

Original author: Morten Wulff <wulff@ratatosk.net> (http://drupal.org/user/7118)

Based on the Swedish Stemmer module by Fredrik Jonsson.

Module development sponsored by:

   ZenSci (www.zensci.dk)
   TravelForum (www.travelforum.dk)


