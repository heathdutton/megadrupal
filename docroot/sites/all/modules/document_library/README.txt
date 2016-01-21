CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Requirements
 * Installation
 * Configuration
 * FAQ
 * Uninstalling


INTRODUCTION
------------
This module allows the administrator to create a document library with
filtering, searching and sorting.

By Default, the module creates:
 * Document Content Type with default fields that can be tailored to specific
   requirements. All taxonomy fields can be used to filter the document
   listing in the document library.
 * Several supporting vocabularies such as categories, languages and tags.
 * Full settings page to administer the look and feel of your document
   library.
 * A Block for placing on any page you require.
 * A Block for a folder view, based on a vocabulary.


REQUIREMENTS
------------
This module requires the following core modules:
 * image
 * number
 * search
 * taxonomy
 * text

This module required the following third party modules:
 * date (https://drupal.org/project/date)
 * pathauto (https://www.drupal.org/project/pathauto)


INSTALLATION
------------
Install as you would normally install a contributed drupal module. For further
information go to:
 * https://drupal.org/documentation/install/modules-themes/modules-7


CONFIGURATION
-------------
 * Set up your Document Library Document content type as you like. Add/edit
   fields as required:
    - /admin/structure/types/manage/document-library-document/fields
 * Create one or more Document Library Documents:
    - /node/add/document-library-document
 * Configure Document Library:
    - admin/config/media/document-library
 * Add Block to desired page(s):
    - admin/structure/block


FAQ
---
Q: Document(s) not showing up in search?
A: Document searching is done against the Drupal search index and if the
   Document Library Document nodes have not been indexed by Drupal, they
   will not show up in the search results. To force them to be indexed, you
   can run cron manually (admin/reports/status/run-cron) until Drupal has
   indexed 100% of the site (check the indexing status:
   admin/config/search/settings).


UNINSTALLING
------------
 * Disable the Document Library module:
    - admin/modules
 * Uninstall the Document Library module:
    - admin/modules/uninstall
 * Remove the Document Library Document content type:
    - admin/structure/types/manage/document-library-document/delete
 * Remove any vocabularies created by or for the Document Library Document
   content type:
    - admin/structure/taxonomy
