-- SUMMARY --

Da Vinci uses:

Sass 3 - with sourcemaps
Sass Globbing - Call your partials with /* or /** in your Main Sass.
Bourbon Refills - http://refills.bourbon.io/
Compass - http://compass-style.org/
Compass Normalize - Compass version of normalize.css
Susy 2 - Grid Framework.
Breakpoint - Helper for grid, excelent.
Sassy Buttons - http://jaredhardy.com/sassy-buttons/

and

Guard & LiveReload Browser extension.



-- INSTALLATION --

  1. Download and enable:
     https://www.drupal.org/project/libraries
     https://www.drupal.org/project/jquery_update
     https://www.drupal.org/project/chosen
     and See Readme.txt

     or

     Drush command, more easy and fast: 
     
     drush dl libraries && drush en -y libraries && drush dl jquery_update && drush en -y jquery_update && drush dl chosen && drush en -y chosen && drush chosenplugin

  2 Configure jquery update.
      2.3.1 Path to configure: admin/config/development/jquery_update
      2.3.2 Default jQuery Version major to 1.7, (recommended v1.10).

  3. Download the Plugin's 

     (https://github.com/Emergya/da_vinci_plugins/archive/master.zip) 
     and extract the file under sites/all/libraries/da-vinci-plugins.

     (https://github.com/flaviusmatis/easyModal.js/archive/master.zip) 
     and extract the file under sites/all/libraries/easymodal.

     (http://masonry.desandro.com/masonry.pkgd.min.js) 
     and extract the file under sites/all/libraries/masonry.

  4. Install Gems and RVM, very EASY, copy & paste

      4.1 Install RVM.
        http://www.ladrupalera.com/drupal/desarrollo/instalacion/rvm-gemas-y-drupal

      4.2 Install Bundle
        gem install bundle (maybe password sudo)

      4.3 Install Gemfile
        In theme directory /themes/da_vinci
        4.3.1 execute: bundle install

Finish :)

INTRODUCTION
------------
Da Vinci is a Drupal 7 theme. The theme is not dependent on any 
core theme. Its very light weight for fast loading with modern look.
  Simple and clean design
  Drupal standards compliant
  Multi-level drop-down menus
  Browser selector
  sass & compass

REQUIREMENTS (Optional)
------------

CONFIGURATION
------------
This theme is compatible with Drupal 7.x.x

USAGE
------------
Easy and Fast.

MAINTAINERS
-----------
Current maintainers:
 * Nesta Guerrero (nguerrero) - https://www.drupal.org/u/nguerrero
 * Karmen Garcia (Karmen) - https://www.drupal.org/u/karmen
