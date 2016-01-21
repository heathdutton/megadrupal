Symfony Drupal Module
=====================

[Symfony](http://symfony.com) is a PHP framework for web projects. Speed up the
creation and maintenance of your PHP web applications. Replace the repetitive
coding tasks by power, control and pleasure.

The Drupal module ensures that the Symfony Framework module is available to
other modules. You should not have to install it unless another module asked you
to.


Installation
------------

1. Install the module to your *sites/all/modules* directory.

2. Run [Drush Composer](http://dgo.to/composer) install on the module directory:
```
    $ cd sites/all/modules/symfony
    $ drush dl composer
    $ drush composer install
```

3. Alternatively, if you don't have Drush installed:
```
    $ cd sites/all/modules/symfony
    $ curl -s http://getcomposer.org/installer | php
    $ php composer.phar install
```


Usage
-----

Visit *admin/help/symfony* to verify that Symfony is installed correctly and a
demonstration on how to use it.
