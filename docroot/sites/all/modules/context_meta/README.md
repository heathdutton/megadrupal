INTRODUCTION
------------
Context meta module provides a facility to add tags likes Abstract, Description,
Keywords, Robots, Copyright and even OpenGraph meta tags to your defined page.
Whenever we create a custom module with custom menu then we are facing issue to
defined SEO tags for custom page. Also this module provide facility to defined
context based (i.e based on page URL) meta element.


REQUIREMENTS
------------
This module requires the following modules:
* Node
* User
* Taxonomy

INSTALLATION
------------
* Install as you would normally install a contributed drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.


CONFIGURATION
-------------
* Configure context meta under configuration settings tab.

FAQ
---
Q: How it works?
A: Once this module is ON then a new section of setting is automatically add
   under configuration. Admin will add meta tags using "Add new meta" link.
   When a page will render then this module check all available meta tags
   against requested PAGE URL. All enable meta tags are added to the head
   section which is available under request URL.

MAINTAINERS
-----------
Current maintainers:
* ratanphp - https://www.drupal.org/user/458884
