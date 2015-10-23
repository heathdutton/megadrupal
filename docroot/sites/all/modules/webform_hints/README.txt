Description
-----------
Webform Hints takes the title of a Webform component and uses it as the
placeholder attribute instead. It then refrains from outputing the title as a
label, but rather hides it for only screenreaders to see.

It works on fields of type:
 * textfield
 * textarea
 * webform_email
 * select list (via #empty_option)

Requirements
------------
Drupal 7.x
Webform
Libraries 2.x

Installation
------------

*** FOR IE 9 and Below Support You must do the following ***
------------------------------------------------------------
Before installing the Webform Hints module, download jQuery Form Defaults plugin 
from the following Github repo:
https://github.com/palmerj3/jQuery-Plugin---Form-Field-Default-Value

Webform Hints requires the minified version named: jquery.form-defaults.min.js

Copy this to a new directory named 'form-defaults' within your 
"sites/all/libraries" (or equivalent) directory. Complete structure should be
the following:
-- sites/all/libraries/form-defaults/jquery.form-defaults.min.js

Warning: This will store the hint in the label attribute, which is invalid HTML.
*** End <= IE 9/Legacy Support ***
----------------------------------

The Webform Hints module itself is installed through the standard module 
installation process.

Configuration
-------------
Configure which Webforms utilize Webform Hints at
Configuration > Content Authoring > Webform hints
or at admin/config/content/webform-hints

Multilingual support
--------------------
The module's "Required indicator" option can be translated to multiple languages
using the i18n_variable module within i18n. See:
https://www.drupal.org/node/2287509#comment-8904559

Development
-----------
Webform Hints was developed and is maintained by CHROMATIC, LLC.
http://chromaticsites.com/

Support
-------
Please use the issue queue for filing bugs with this module at
http://drupal.org/sandbox/chrisfree/1128558
