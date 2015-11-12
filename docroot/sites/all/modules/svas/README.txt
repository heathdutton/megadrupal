// $Id

The SysVar AJAX Saver (SVAS) module allows to save any form element values
as system variable with AJAX.

Requirements
--------------------------------------------------------------------------------
The SVAS modules are written for Drupal 7.0+.

Installation
--------------------------------------------------------------------------------
Copy the SVAS module folder to your module directory and then enable the module
on the admin modules page.

Administration
--------------------------------------------------------------------------------
No administer interface available.

Usage
--------------------------------------------------------------------------------
It's very easy.

Extend a form element with '#ajax' and define 'svas_save' as callback function.

The name of the form element is the name of the system variable that is stored.

For an usage example please take a look at the
module included doxygen documentation.

