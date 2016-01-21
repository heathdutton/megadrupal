Custom search fields 7.x-1.x
--------------------------

Install
-------
* Enable custom search module
* Enable this module
* Go to Administer > Settings > Custom search to change settings
* Don't forget to set permissions, otherwise nobody will see the changes

Description
-----------
This module alters the custom search module to allow filtering by Entity fields (At present only Entity Reference Fields), 
it only expands on the custom search functionality does not control it by 
itself.

This module is inspired by the work done on the custom search module and 
leverages that to work

At present you will also need to include the Custom Search patch available here https://www.drupal.org/node/2365273 until Custom Search adds this into it's official releases. This allows Custom Search fields to send information to Custom Search.

Author
------
omaster
