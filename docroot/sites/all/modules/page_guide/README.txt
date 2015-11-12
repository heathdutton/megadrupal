CONTENTS OF THIS FILE
---------------------
   
 * Introduction
 * Requirements
 * Recommended modules
 * Installation
 * Configuration
 * Troubleshooting
 * FAQ
 * Maintainers

 INTRODUCTION
------------
pageguide.js is an interactive visual guide to elements on web pages.
Instead of cluttering your interface with static help message,
or explanatory text,
add a pageguide and let your users learn about new features and functions.

REQUIREMENTS
------------
This module requires the following items
1. jQuery 1.7 - 2.0
2. pageguide (http://tracelytics.github.io/pageguide/)
3. Libraries API
 
Installation
---------------
1. Download and enable page_guide module
2. Download and enable libraries module from
   https://www.drupal.org/project/libraries
3. pageguide.js and pageguide.min.css (http://tracelytics.github.io/pageguide/)
   Download the pageguide and get the js from
   pageguide/js/pageguide.js  and also get the css from
   pageguide/dist/css/pageguide.min.css
4. And place the js and css file into your pageguide libraries folder.
   make sure path should be like this
   sites/all/libraries/pageguide/pageguide.js
   sites/all/libraries/pageguide/pageguide.min.css


Configuration
---------------
1. Goto Administration >> Configuration >> User interface >> Page guide
2. Click Add Page Guide link and give name and the page url
  (where do you add the page guide)
3. Then go back to the page guide page here we are listing
   you created page guide list
4. There is 3 operation link create | edit | delete
  + Create is used to create page guide information with css class name
  + edit is used to edit the page guide
  + delete is used to permanently removed the page guide

MAINTAINERS
-----------
1. Leopathu (https://www.drupal.org/u/leopathu)
