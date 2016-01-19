------------------------------------------------------------------------------
  Token taxonomy breadcrumb module
  http://drupal.org/project/token_taxonomy_breadcrumb

  Created by David Herminghaus, http://drupal.org/user/833794
  D7 version sponsored by Compuccino GmbH, http://www.compuccino.com
------------------------------------------------------------------------------

Contents:
=========
1. ABOUT
2. REQUIREMENTS
3. QUICK REFERENCE

1. ABOUT
========
Core and contrib tokens already do a great job in providing precious meta
information on Drupal entities. However, creating a hierarchical breadcrumb
from the root term down (or up) to and especially including the current term
with a single token is impossible unless you wanted to create custom code
snippets as a workaround.

This module is intended to fill just this gap as handy as possible and thus
introduces two additional term related tokens:

[term:breadcrumb] and [term:breadcrumb-reverse]

Where the first one returns an array starting with the root term, the latter
starting with the current term.

2. REQUIREMENTS
===============
Core taxonomy and contrib token module both need to be enabled in order for
this one to work. That's all.

3. QUICK REFERENCE
==================
Just enable the module and create some tokenizable content. You will find
the two new tokens in the token browser, supporting all array-related options
the token module offers.

Enjoy!
