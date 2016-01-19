# RESTful web services support for Internationalization (i18n)

This module provides support for creating and updating a node's translation 
source.

## Installation

Requires RESTful web services (restws) and i18n_node
No additional steps needed, just enable the module in Drupal.

## Usage example

1. Enable and configure restws (and optionally restws_basic_auth), making sure 
you can create/update a regular 'article' node.
2. Go to admin/config/regional/language and enable Spanish.
3. Enable and configure "Multilingual content" module. 
4. Using any REST client extension, POST a JSON body payload like the one in 
examples/article-payload.json. It should return a JSON with node ID == 1.
5. Check the newly created article, with the URL returned.
6. Using the REST client extension again, POST another JSON body payload, like
 the one in examples/article-es-payload.json. Mind the nid == 1 (change it as 
 needed)
7. It should return a JSON with node ID == 2.
8. Now again check the newly created article in Spanish, with the URL returned.
9. Go to node/1/edit and you will see the Spanish translation.

## Credits
Mariano E. Barcia Calderón (mbarcia@asmws.com)
