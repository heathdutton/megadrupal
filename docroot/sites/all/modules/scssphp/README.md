SCSSPHP for Drupal
==================

Compiles [SCSS](http://sass-lang.com/) files to CSS using [SCSSPHP](http://leafo.net/scssphp/).


Dependencies
------------

* [Composer Manager](http://drupal.org/project/composer_manager)


Installation (via Drush)
------------------------

    ``` sh
    $ drush dl scssphp
    ```


Installation (Manual)
---------------------

1. Download and install Composer Manager

2. Download and install SCSSPHP module

3. Run a Composer Manager Install

    ``` sh
    $ drush composer-manager install
    ```

4. Check admin/reports/status to make sure the module is installed correctly.

5. Check admin/help/scssphp for a demonstration of it in action.


Thoughts
--------

 * Allow use of either Libraries API or Composer Autoload?
