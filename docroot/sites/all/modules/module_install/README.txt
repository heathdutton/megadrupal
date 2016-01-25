CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Installation

INTRODUCTION
------------

module_install extends the core "add new module" page (admin/modules/install)
and adds a destination folder dropdown to the existing form.

You can then choose to install a new module in one of the non-module
subfolders under sites/all/modules, sites/YOUR-SITE/modules or
profiles/YOUR-PROFILE/modules

INSTALLATION
------------

1. Normally you would create 3 subfolders in your sites/all/modules folder
     * contrib
     * custom
     * patched

2. Download the module package from drupal.org.

3. Unpack this module in your modules directory.

4. Enable the module

5. Check admin/modules/install, you should see the extra dropdown
   "Destination folder"
