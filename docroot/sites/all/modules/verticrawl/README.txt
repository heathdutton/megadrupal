
-- SUMMARY --

Verticrawl replace quickly default search from Drupal's core with a powerful
semantic full text search engine. It let you choose how your search engine
respond depending of typed text and crawled content like html, pdf, docs.
Other features like screenshot and image integration are also available.
Contextual search, word suggestion and synonyms are also useful to help your
user.

For a full description of the module, visit the project page:
  #http://drupal.org/project/verticrawl_search_engine
  https://www.drupal.org/sandbox/verticrawl/2427759

To submit bug reports and feature suggestions, or to track changes:
  #http://drupal.org/project/issues/verticrawl_search_engine
  https://www.drupal.org/project/issues/2427759


-- REQUIREMENTS --

* Date API & Date Popup :
  https://www.drupal.org/project/date


-- INSTALLATION --

* Install as usual, see http://drupal.org/node/895232 for further information.

* Modules:
  
  - Verticrawl Search Engine (verticrawl)

    This parent module helping to manage sub-modules and create the table to
    save module configuration (required).

  - Search PHP (verticrawl_search)

    The basic sub-module you always need to display the Verticrawl Search Page
    and a quick search configurable block (need API Key but a demo one is 
    provided for testings).

  - Screenshot (verticrawl_search_screenshot)

    Activate the screenshot integration in your results if available (need API
    Key but a demo one is provided for testings).


-- CONFIGURATION --

* Configure user permissions in Administration » People » Permissions:
  
  - Administer Verticrawl search

    Users need this permission to access Verticrawl Search Engine configuration 
    page.

  - Use Verticrawl search

    Users need this permission to access the Verticrawl search page.

  - Use Verticrawl advanced search

    Users need this permission to use the advanced search in the Verticrawl 
    search page.

  NOTE : Permissions management for this module is very closed to the default 
  Drupal search.

* Configure Verticrawl Search Engine in Administration » Search and metadata » 
Verticrawl Search
  
  - API Keys

    Demos API Keys are defined to help first integration. Define your own API 
    Keys in:
    
    1) Administration » Search and metadata » Verticrawl Search » Search PHP

    2) Administration » Search and metadata » Verticrawl Search » Screenshot

    NOTE : See http://verticrawl.com to get or buy API Keys.

  - Other configurations

    Simply follow instructions in the config forms.


-- CUSTOMIZATION --

* Template suggestions:

  - Search results:

    Override the default layout of result. Be careful, some variables are from 
    Verticrawl API Documentation.
    
    File: verticrawl_search_php/theme/verticrawl-search--results.tpl.php

    An example template is set to show all variables. It can help to customize
    the display of results, but all variables are rendered keeping the order
    of variables passed by Verticrawl API (due to SWITCH/CASE structure). 
    You should use the default template to manually set the order of each field
    you need.

    File: verticrawl-search--results.tpl.php.example (in module's root)

  - Answord:

    Override the default layout of ANSWORD campaigns.
    File: verticrawl_search_php/theme/verticrawl-search--answords.tpl.php

  - Pager:

    Override the default layout of pager at the bottom of the search page.

    File: verticrawl_search_php/theme/verticrawl-search--pager.tpl.php


-- CONTACT --

Current maintainers:
* Jean-François Lhuisset (VERTICRAWL) - http://drupal.org/user/3161541
* Nicolas Bédé (urashima82) - http://drupal.org/user/2802755
