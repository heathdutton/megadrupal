
Amazon.module API Documentation
===============================

Please read the handbook documentation at http://drupal.org/node/595464.

The Amazon package gives Drupal based web sites access to the core features of 
the Amazon Product Marketing API. The package provides several components:

1) A core API module that communicates with Amazon's web services and adds 
   support for retrieving, displaying, and storing basic product information 
   in Drupal as well as associating Amazon products with content nodes. 
2) A field type (and an accompanying set of formatters) for CCK that allows 
   Amazon products to be explicitly added to any node type. 
3) An 'Amazon Media' module that adds support for Amazon's extended product 
   information for several common product types (Books, Music, DVDs, and 
   Software).
4) An 'Amazon Search' module that allows developers to conduct API-driven 
   searches of the Amazon product database. This module also allows users to 
   search Amazon's product database from Drupal's Advanced Search page.

Most users and site builders will want to enable the basic Amazon API module, 
as well as the Amazon Field and Amazon Media modules. These provide the 
functionality that the majority of simple sites need. 

For more information, visit the project page at http://drupal.org/project/amazon 
and the project documentation at http://drupal.org/node/595464.

Amazon API conventions
======================

- All element keys are lowercased for consistency
- The Amazon ItemAttributes collection is merged into the top level of the 
  Drupal entity for simplicity.
- Information outside the 'common' ItemAttributes, Product Images, and 
  Artist/Author/Etc. data must be handled by external third-party modules.
- Authors, Directors, and other creator information is stored in the generic 
  $item['participants'] array. It's a crummy name, but until I come up with
   something better it'll have to do. A separate array for each participant type 
   is also created.