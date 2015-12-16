INTRODUCTION
------------

The Panel Parallaxe module offers parallaxe display for your project.

The Parallaxe contains an fixed image in the background and some text that moves
over the background.
The module contains:

1. Content Type Parallaxe: to specify background image and text
2. A custom display type: Parallaxe display, to be used together with the
   content of type Parallaxe
3. 8 different Panel Layouts, each containing three areas for Parallaxe display
4. 14 image styles to fit the backgound image best to screen-resolution


* For a full description of the module, visit the project page:
   https://drupal.org/sandbox/slowflyer/2426021
   https://drupal.org/project/panel_parallaxe

* To submit bug reports and feature suggestions, or to track changes:
   https://drupal.org/project/issues/2426021
   https://drupal.org/project/issues/panel_parallaxe


REQUIREMENTS
------------
This module requires the following modules:

* Panels (https://drupal.org/project/panels)
* SamrtCrop (https://drupal.org/project/smartcrop)


RECOMMENDED MODULES
-------------------
* AdaptiveTheme (https://drupal.org/project/adaptivetheme):
   The modul works best together with AdaptiveTheme, therefore with
   Drupal Commons.
   But can be used with any other themes as well.
   

INSTALLATION
------------

* Install as you would normally install a contributed Drupal module. See:
   https://drupal.org/documentation/install/modules-themes/modules-7
   for further information.

 
CONFIGURATION
-------------

* Configure usage of CSS for block behaviour on small screens Administration >>
  Media >> Configure Parallaxe CSS:

  - If you want your theme to define behaviour of block display on small screens
    disable "Use modules own CSS for panel blocks"

    When working with AdaptiveThemes it can make sense to disable it,
    to keep behaviour consistent on pages using Panel Parallaxe and pages
    not using it. 

* Create a new content of type Parallaxe

* Go to your Panels and change layout to one of the 8 Layouts coming with the
  modul (categories: Parallaxe-1-column, Parallaxe-2-column, Parallaxe-3-column)

* Add your node of type Parallaxe to an orange area,
  select Build mode: Parallaxe display
   
   
MAINTAINERS
-----------

Current maintainers:
* Ullrich Neiss (slowflyer) - https://drupal.org/u/slowflyer
* Mario Langlitz (creativeChaos) - https://drupal.org/u/creativechaos


This project has been sponsored by:
* crowd-creation GmbH
   Your specialized partner for community solutions
   Activate the innovating power of your stakeholders
