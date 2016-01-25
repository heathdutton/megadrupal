CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation

INTRODUCTION
------------

Keycuts features the ability to have keyboard shortcuts in your Drupal pages
just like in your regular desktop apps. You can for example use Ctrl+S on a
form page to save the form. This module requires the Mousetrap library
available through: http://craig.is/killing/mice

INSTALLATION
------------

1. Download the module package from drupal.org.

2. Unpack this module in your modules directory.

3. Download the Mousetrap library from http://craig.is/killing/mice.

4. Install the library in sites/all/libraries so your folder structure looks
   like this:
     sites/all/libaries/mousetrap/mousetrap.min.js

5. Download and enable libraries (at least 2.x) and Token module.

6. Enable the keynav module.

7. Goto admin/config/user-interface/accessibility/keycuts
   Make sure the 'Enable' checkbox is checked and hit 'save' to save the
   default shortcuts.
