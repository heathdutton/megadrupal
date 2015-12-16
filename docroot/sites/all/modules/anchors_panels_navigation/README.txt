
-- SUMMARY --

Add user predefined anchors to the panels to provide better navigation into one page or on so called Landing Pages.
Please see instruction on http://abzats.com/blog/anchors-panels-navigation-module
Please see the demo on http://apn.yaremchuk.ru

-- REQUIREMENTS --

Modules
- ctools (>=1.4)
- panels (>=3)
- libraries (>=2)
- jquery_update (>=2)
should be installed!

Should switch to JQuery 1.8 or higher.

Jquery apper plugin should be installed (https://github.com/morr/jquery.appear).

-- INSTALLATION --

1. Turn on the module and all required modules. Normal Drupal module installation, see http://drupal.org/node/70151 for further information.

2. Put 'jquery.appear.js' into '/sites/all/libraries/appear' folder.
You can download this file from https://github.com/morr/jquery.appear

3. Add panel node and setup several panels in it.

4. Set CSS IDs for each pannel on the 'panel content' tab. For example 'foo'.

5. Add links with hash equal these IDs to the menu. For example '#foo'.

6. You can use 'Floating block module' (https://www.drupal.org/project/floating_block) to make your menu visible 
during page is scrolling to anchors.

-- CONFIGURATION --

Please find additional fieldset in Panels node edit form.

-- CUSTOMIZATION --

-- ROADMAP --

-- CONTACT --

Vasily Yaremchuk (Yaremchuk) - http://drupal.org/user/576918
Skype: vasilyyaremchuk
Home page: http://abzats.com

