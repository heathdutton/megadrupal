CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Configuration
 * Module Details
 * Recommended modules
 * Configuration
 * FAQ
 * TROUBLESHOOTING


INTRODUCTION
------------
Quora Module provides related questions/posts of QUORA websites in drupal as
Block or Ctools Plugin. Quora Questions/Posts to be displayed is fetched on the
basis of tags provided by one of the fields of the content type. This field
acting as the interconnection between your Drupal website and Quora Website is
configurable.

This module uses Google's Custom Search Engine (CSE) api to fetch
Quora questions/posts related to your content and display them in a block or
ctools plugin.

In short, Quora Module uses Google CSE API to bridge Drupal and Quora, as quora
website itself doesn't provide any developer api.


CONFIGURATION
-------------

Quora Administration Configurations:

* Google Custom Search Api -
  Enter your Google custom search engine api.
  To generate Google Custom Search Engine API, visit https://cse.google.com/cse.

* Google Custom Search Engine CX ID -
  Enter CX-ID of google custom search engine.

  (IMPORTANT: While creating Custom Search Engine for your Drupal application,
  do not restrict pages from *.quora.com while it is recommended to add
  *.quora.com as "Sites to Search").

* Quora TAG Field For Content Types -
  For each content type, we need to map field to be used by Quora module as to
  get tags to fetch related Quora Questions/Tags.
  By default, Quora Modules look for Tags field in the content type and in case
  it fails to get appropriate terms for building search query, Quora Module uses
  title field for the purpose.

Quora Block/Ctools Plugin Configuration:

  DISPLAY OPTIONS:
  Allows to control the display of Quora Questions/Posts in the Block or Plugin

  ADVANCED SEARCH SETTINGS:
  Admins can have control over Quora Questions/Posts to be shown using these
  advanced search options.


MODULE DETAILS
--------------

Basic Flow implemented by Quora Module is:
1. Collect Terms from the content using one of the field values of Content.
2. Preprocess the terms and builds query
3. Uses Google CSE (Custom Search Engine API) (If provided) to fetch Related
   Quora Questions/Posts.
4. Display them in the block/widget.


RECOMMENDED MODULES
-------------------

* CTOOLS
  When enabled Quora module provides Quora Plugin available as ctools
  plugin under Widgets section. Quora plugin allows user to use the
  quora feature separately for each panel.


FAQ
---

Q: What if a content doesn't have data in the field selected as Quora Tag field?

A: Quora Module intelligently collect terms from alternate fields, continues the
further process and displayes related Quora Questions/Posts.



Q: What if I don't have Custom Search API?

A: It is highly recommended to use Google Custom Search Engine API as it is
highly reliable, but in case user fails to submit appropriate Google CSE API and
CX-ID, Quora again plays intelligently to fetch related posts/Questions of Quora


Q: Do Quora Module requires Credentials to work?

A: No, Quora Module uses Google Custom Search Engine API for its internal
searching purposes.


TROUBLESHOOTING
---------------

Quora Module handles warning and errors itself, and on the same time logs into
Recent DB Logs.



This project has been sponsored by:
 * QED42
  QED42 is a web development agency focussed on helping organisations and
  individuals reach their potential, most of our work is in the space of
  publishing, e-commerce, social and enterprise.
