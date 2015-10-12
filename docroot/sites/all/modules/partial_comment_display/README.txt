INTRODUCTION
------------
Partial comment display module is provide a facility to show partial comments
on node view page for specific content type. Partial comment display setting is
available under comment settings of content type.


REQUIREMENTS
------------
This module requires the following modules:
* Comment

INSTALLATION
------------
* Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
* Configure partial comment display settings under comment settings of
  content type edit form page.
* Check Partial comment display checkbox.
* Select roles for which system will show partial comment on node view page.
* Select number of comments will be shown on node page for selected user roles.

FAQ
---
Q: How it works? 
A: Once this module is ON then a new section of setting is automatically add
   under Comment settings of all content types. Check the Partial comment 
   display field. A new section will be open where you have to checked
   applicable user roles and how many number of comment will be shown on 
   node page for these roles.

Q: After configuration on content type select user role has still able to see
   all posted comment?
A: This setting is not applicable for those roles who has permission to 
   post comment.

MAINTAINERS
-----------
Current maintainers:
* ratanphp - https://www.drupal.org/user/458884
