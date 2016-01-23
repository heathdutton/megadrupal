===========================================================================
dsb Portal - client module for communicating with the Swiss Digital Library
===========================================================================

This module allows a site to retrieve information from the national catalog to the Swiss digital school library (a.k.a. "dsb", Digitale Schulbibliothek). Resources can be searched, filtered, etc.

The module communicates with the official API, which can be found here: https://dsb-api.educa.ch/

Installation
============

There are 2 ways to install the module.

* Using Drush and Composer (requires the Composer Manager module; recommended if you have shell access to your server)
* Downloading third-party libraries "by hand" and uploading them to your server via FTP (requires the Libraries API and X Autoload modules)

You can choose either method, but you must choose at least one.

Using Composer, Drush and Composer Manager
------------------------------------------

First, install Drush and Composer on your host.

* http://docs.drush.org/en/master/install/
* https://getcomposer.org/doc/00-intro.md

Download and enable the Composer Manager (composer_manager) module. Change directory inside your Drupal site and call:

drush composer-json-rebuild

This will generate a composer.json file. By default, it is located in public://composer (usually, this maps to sites/default/files/composer). Now, change directory inside the folder containing this composer.json file and call:

composer install

And voilà.

Downloading third-party libraries and uploading them by FTP
-----------------------------------------------------------

This module requires the dsb Client PHP library. But dsb Client itself has many dependencies. Basically, all libraries should be uploaded to your current site's libraries folder. This is located in:

sites/*/libraries

For example:

- sites/all/libraries
- sites/example.com/libraries
- etc

If you don't see a "libraries" folder, simply create one.

After that, all downloaded libraries must be put in their own folder. You will have something like:

sites/*/libraries/{library_name_lowercase}/

dsb Client:
1. Download it from https://github.com/educach/dsb-client/releases and unzip it.
2. Upload the unzipped folder to your libraries folder (e.g., sites/all/libraries/) and rename the folder to "dsb-client" (so you will have sites/*/libraries/dsb-client/)

Guzzle:
1. Download *the source code* (NOT the release package) from https://github.com/guzzle/guzzle/releases and unzip it (version 5.3.*).
2. Upload the unzipped folder to your libraries folder (e.g., sites/all/libraries/) and rename the folder to "guzzle" (so you will have sites/*/libraries/guzzle/)

RingPHP:
1. Download it from https://github.com/guzzle/RingPHP/releases and unzip it (version 1.*).
2. Upload the unzipped folder to your libraries folder (e.g., sites/all/libraries/) and rename the folder to "ringphp" (so you will have sites/*/libraries/ringphp/)

Streams:
1. Download it from https://github.com/guzzle/streams/releases and unzip it (version 3.*).
2. Upload the unzipped folder to your libraries folder (e.g., sites/all/libraries/) and rename the folder to "streams" (so you will have sites/*/libraries/streams/)

React/Promise:
1. Download it from https://github.com/reactphp/promise/releases and unzip it (version 2.*)
2. Upload the unzipped folder to your libraries folder (e.g., sites/all/libraries/) and move the folder to "promise" (so you will have sites/*/libraries/promise/)

Finally, download and enable the following Drupal modules:
- Libraries API (https://www.drupal.org/project/libraries)
- X Autoload (https://www.drupal.org/project/xautoload)

Configuration
=============

Before you can use the module, you must configure it. Go to admin/config/services/dsb-portal and follow the instructions.

Usage
=====

You can find the search page at dsb-portal/search.

Tip: create a new menu link in the main menu so you can easily find it (admin/structure/menu/manage/main-menu/add).

