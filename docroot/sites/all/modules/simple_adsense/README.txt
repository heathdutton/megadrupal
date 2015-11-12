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

This module brings Google AdSense AD unit into Drupal as block or field
simplely. We can put AD into regions, nodes or any other entities. 

Using Cases:

 * Place a AD block in the footer region.
 * Add AD field into artiles or comments.

More Implementation details visit: http://ranqiangjun.com/node/408832
(in Chinese)

Requirements
------------
 * Google Adsense Account (https://www.google.com/adsense)

RECOMMENDED MODULES
-------------------

 * Goolgle Adsense (https://www.drupal.org/project/adsense):

INSTALLATION
------------
 * Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

CONFIGURATION
-------------

Simple AdSense in the Block
===========================

 * Go to admin/config/content/simple_adsense and config. Blow are available
real world settings for testing purpose:

   - Client: ca-pub-9513614146655499
   - Slot #1: 5270097160
   - Slot #2: 5556618763
   - Slot #3: 6746830361
   - Slot #4: 8955359568
   - Slot #5: 3793363962

 * Open admin/structure/block, you'll see block(s) like this

   - simple_adsense_slot3793363962
   - simple_adsense_slot5270097160

 * Layout those block(s) as usual.

Simple AdSense in the field
===========================

 * Visit admin/config/content/simple_adsense. Check whether the Client field
 is setted correctly. The Client is required, and the Slot is optional for this.

 * Go to manage fields page, eg: admin/structure/types/manage/article/fields.

 * Add a Text field, eg: ad_field, here the slot value goes, set a default
  slot is recommanded.

 * Go to manage display page, eg: admin/structure/types/manage/article/display.
 choose the Simple AdSense format for ad_field.


MAINTAINERS
-----------

Current maintainers:
 * Qiangjun Ran (jungle) - https://drupal.org/u/jungle

This project has been sponsored by:
 * YAIYUAN LLC
   Specialized in consulting and planning of Drupal powered sites, 
   and Provides Drupal trainning as well. Visit https://www.yaiyuan.com 
   for more information.
