
-- SUMMARY --

This module provides a neat solution for cutting off the text without taking 
any HTML tags into consideration. It uses a regular expression containing all 
the words of a trimmed textual representation of the field value to extract 
the wanted block of HTML.

This is a simple solution for anyone that just wants their teasers to have the 
same textual length regardless of any links, images or other tags that are in 
the field value. This module does not provide a way to cut off at the end of a 
sentence. It goes up to word level.

For a full description of the module, visit the project page:
  https://drupal.org/sandbox/Mywebmaster/2118939
To submit bug reports and feature suggestions, or to track changes:
  https://drupal.org/project/issues/2118939


-- REQUIREMENTS --

* Field formatter settings


-- INSTALLATION --

Place the entirety of this directory in sites/all/modules/field_html_trim. You 
must also install the Field formatter settings module 
(https://www.drupal.org/project/field_formatter_settings) to be able to use 
Field HTML trim.

Navigate to administer >> build >> modules. Enable Field HTML trim.


-- CONTACT --

Current maintainer:
* Alex Verbruggen (alexverb) - https://drupal.org/user/1129948
