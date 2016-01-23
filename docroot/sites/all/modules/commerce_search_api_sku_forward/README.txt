CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Maintainers

 INTRODUCTION
 ------------
 This module integrates with Search API. It checks if a searched value directly
 matches a products SKU. From there, it checks to see how many nodes reference
 that product, and if only a single node references the product, it takes the
 user direct to that product instead of taking them to the search results page.


  * For a full description of the module, visit the project page:
    http://www.drupal.org/project/commerce_search_api_sku_forward


  * To submit bug reports and feature suggestions, or to track changes:
    http://www.drupal.org/project/issues/commerce_search_api_sku_forward

REQUIREMENTS
------------
This module requires the following modules:
 * Commerce (https://www.drupal.org/project/commerce)
 * Search API (https://www.drupal.org/project/search_api)

INSTALLATION
------------
* Install as you would normally install a contributed Drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information.

CONFIGURATION
-------------
 * Configure user permissions in Administration » Configuration »
   Search and Metadata >> Commerce Search API SKU forward


   - Select the search index this should affect

     In an effort to prevent this module from hitting everything, you have to
     select which search indexes you desire to affect. Multiple search indexes
     can be selected, or you can ignore a search index that may not relate to
     products.

   - Select the nodes this should affect

      As multiple nodes can reference products, you will want to select the
      relevant nodes that should be targeted.

   - Select the field machine name, and data column.

      As there are variations in field machine name and data columns between
      Commerce Kickstart 1.x and 2.x, along with any custom setups, you can
      customize the relevant fields and DB column for Sku/product-id. The
      default values should work for most people, especially if you used
      Commerce Kickstart 1.x!


MAINTAINERS
-----------
Current maintainers:
 * Jesse Nicola (jnicola) - https://drupal.org/user/224754


This project has been sponsored by:
 * Commerce Guys
   Commerce Guys are the creators of and experts in Drupal Commerce, the
   eCommerce solution that capitalizes on the virtues and power of Drupal,
   the premier open-source content management system. We focus our knowledge
   and expertise on providing online merchants with the powerful, responsive,
   innovative eCommerce solutions they need to thrive.