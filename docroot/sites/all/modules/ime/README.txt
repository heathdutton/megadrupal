CONTENTS OF THIS FILE
---------------------
 * Introduction
 * About Project
 * Requirements
 * Installation
 * Configuration

Introduction
-----------
jQuery.IME is a jQuery based input method editor
library supporting more than 135 input methods across more than 62 languages.

These input methods are well tested. Initially the input methods were
contributed by the Wikimedia community. By now many input methods have also
been contributed by Red Hat.

jQuery IME project URL :- https://github.com/wikimedia/jquery.ime

About Project
-------------
IME module allow you to integrate
jQuery.IME library in Drupal 7 and use it's multilingual support in
writing content in more then 62 languages.

Requirements
------------
1. jQuery.IME library :- You will need to
download jquery.ime from https://github.com/wikimedia/jquery.ime and
extract the jquery.ime files to the "/sites/all/libraries/" directory
and rename to jquery.ime(e.g:/sites/all/libraries/jquery.ime/).     or
you can directly do a "git clone
https://github.com/wikimedia/jquery.ime.git" in
"/sites/all/libraries/".     or     You can download this module using
drush command "drush dl ime" after that do "drush en ime" it will
download the jQuery.ime library as well automatically

2. Libraries module (https://drupal.org/project/libraries)  3. jQuery
update module (https://drupal.org/project/jquery_update) IME module
requires jQuery 1.7 . Please configure to use a higher jQuery version.

Installation / Configuration
----------------------------
1. Extract the module files to the "sites/all/modules" directory.
It should now contain a "ime" folder or download it using drush.
2. Enable the module in the
"Administration panel > Modules > Multilingual -Internationalization" section.
 3. Configured it from admin/config/regional/ime

Current maintainers:
----------------------------
 * Suhel Rangnekar (suhel.rangnekar) - https://www.drupal.org/user/1639016
 * Ashwini Kumar (ashwinikumar) - https://www.drupal.org/user/1866282
