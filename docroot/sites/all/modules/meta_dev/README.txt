# Development Meta module

This is a simple meta module.  It installs the key tools for building Drupal 
sites.  When you disable or uninstall the module it removes the dependencies
for you too.

To start using meta_dev you can use one of the following methods:

## Drush Make
Include the following entry in your drush make file

    ; Include dev tools
    projects[meta_dev][subdir] = "dev"

## Download and Enable with Drush
Run the following command to download and enable meta_dev and all the 
dependencies:

    $ drush dl meta_dev && drush en -y meta_dev

## Do it All Manually
I don't encourage this method, but you can download all of the dependencies
manually and enable meta_dev via the modules page (admin/modules).  You'll need
the following modules:

 * [Coder](https://drupal.org/project/coder)
 * [devel](https://drupal.org/project/devel)
 * [Search Krumo](https://drupal.org/project/search_krumo)

meta_dev is developed by [Dave Hall Consulting](http://davehall.com.au)
