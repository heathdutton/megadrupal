Zend Framework Drupal module
============================

The Zend module provides the complete Zend Framework 2.x for Drupal modules to
use. This module is a developer module providing no end user facing functionality
or configuration. Once this module is installed other modules can use any
portion of the Zend Framework starting from hook_init().

For more information on the Zend Framework please see http://framework.zend.com


Installation
------------

Using Drush:

    drush dl composer
    drush dl zend


Manual installation:

    cd sites/all/modules/zend
    curl -s http://getcomposer.org/installer | php
    php composer.phar install


Once the Zend Framework is installed it will be reported on the Status Report
page and available to all modules to take advantage of.


Maintainers
-----------

- Matt Farina (mfer) - http://drupal.org/user/25701
- Rob Loach - http://drupal.org/user/61114
- mustafa ulu (mustafau) - http://drupal.org/user/207559
