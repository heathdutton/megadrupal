DESCRIPTION
-----------
This module provides ways to index image fields directly as markup, and allows
to choose what image styles to be used for this indexing. The module integrates
with Apachesolr Views and provides a field handler that can take advantage
of the indexed images and display them as a views field.

INSTALLATION
------------
Install the module as normal, but make sure that Apachesolr and (optionally)
Apachesolr Views modules are installed correctly. And the Solr indexing and
searching both work.

CONFIGURATION
-------------
Configure the image styles to index from admin/config/search/apachesolr/images,
then be sure to re-index existing content. If using Apachesolr Views, the field
should now be available for adding to the view with the name:
sm_thumb_{style}_{field_name}. If picked, it should display the indexed syled
image markup.

MAINTAINERS
-----------
Miroslav Vladimirov Banov: https://www.drupal.org/user/1509224
