drush_modulesmith
=================

Create simple modules with Drush.

Usage:

    drush modulesmith-forge modulename
    drush modulesmith-forge modulename sites/all/modules/custom

By default, it will try to put it in the following paths:

    sites/all/modules/custom
    sites/all/modules
    profiles/INSTALLPROFILE/modules/custom
    profiles/INSTALLPROFILE/modules

## Install

    cd ~/.drush/
    git clone git@github.com:robballou/drush_modulesmith.git

## Why this command?

You can [use module_builder](https://drupal.org/project/module_builder) to build modules. It's a great tool and fully featured. `module_builder` installs into your project though. And it does quite a lot (which is good ... unless you need something simple). This just adds a module skeleton for you and you can then add your code as needed.
