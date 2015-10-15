Autoloader Early
================

This module provides a generic autoloader, able to load any PSR-0 compatible
code. It also can be used as a map based autoloader, and can accept a pragmatic
list of class and associated files (such as the core autoloader).

It is generic enough, and does not rely on database so it can be used even
before Drupal has been bootstrapped, which means that it can be used for cache,
locking and session framework if needed.

Install
-------

If you don't use any module that needs it before the hook_boot() execution,
just enable the module.

Therefore, its is a good idea to put the class loading in your settings.php
file in order to allow early pre-bootstrap code to use it. Simply add into
your settings.php file:

  require_once 'sites/all/modules/autoloadearly/lib/AutoloadEarly.php';

Devel mode
----------

Additionally, before the include, you can add a variable to set the autoloader
in development mode. Development mode means that on partial class found in a
known namespace, the autoloader will send exceptions to help you debug. Just
modify your settings.php file like this:

  $conf['autoloadearly_debug'] = 1;
  require_once 'sites/all/modules/autoloadearly/lib/AutoloadEarly.php';

API
===

If you are a developer and willing to use it, you have basically two path.

Using the map-based autoloader
------------------------------

  // Register one or more classes:
  AutoloadEarly::getInstance()
    ->registerFiles(array(
      'SomeClass'      => DRUPAL_ROOT  . '/sites/all/modules/mymod/lib/foo.php'
      'SomeOtherClass' => DRUPAL_ROOT . '/sites/all/libraries/SomeOther.php',
    ));

Furthermore, if your inclusion is being done in a file into your own module,
you could do this:

  // Register one or more classes:
  $modulePath = dirname(__FILE__); // __DIR__ for PHP 5.3 (Drupal 8 and above)
  AutoloadEarly::getInstance()
    ->registerFiles(array(
      'ClassA'         => $modulePath  . '/lib/A.php'
      'ClassB'         => $modulePath  . '/lib/B.php'
      'SomeOtherClass' => DRUPAL_ROOT . '/sites/all/libraries/SomeOther.php',
    ));

Using the namespace/PSR-0 autoloader
------------------------------------

PSR-0 autoloader will probably much more useful when it comes to deal with a
more consistent library, using a lot of files. Any decent external library
will probably be PSR-0 compatible. They often ship with a custom but optional
autoloader.

For example, let's say you have the Predis library in sites/all/libraries: you
will probably have a path something like:

  sites/all/libraries/Predis/lib/Predis/Client.php

You can register the library using this statement:

  AutoloadEarly::getInstance()
    ->registerNamespace('Predis', DRUPAL_ROOT . '/sites/all/libraries/Predis/lib');

Now, consider that you have your own OOP library, embeded with your module, with
something that looks like this:

  my_module/
  my_module/lib/
  my_module/lib/MyModule/
  my_module/lib/MyModule.php   (contains the MyModule class)
  my_module/lib/MyModule/A.php (contains the MyModule\A or MyModule_A class)
  my_module/lib/MyModule/B.php (contains the MyModule\B or MyModule_B class)

You can register it either by doing this:

  AutoloadEarly::getInstance()
    ->registerNamespace('MyModule', dirname(__FILE__) . '/lib');

Into one of your module file, or:

  AutoloadEarly::getInstance()
    ->registerNamespace('MyModule', 'sites/all/modules/my_module/lib');

From anywhere else.

Additionnaly, you can combine more than one namespace registration in one
function call, just do as this:

  AutoloadEarly::getInstance()
    ->registerNamespace(array(
      'Predis'   => DRUPAL_ROOT . '/sites/all/libraries/Predis/lib',
      'MyModule' => DRUPAL_ROOT . 'sites/all/modules/my_module/lib',
    );

Some bits of advice
-------------------

If you are using this autoloader for Cache handling or such, you will
definitely need to find your module path to include the right path. One
fail-safe method seems to do it within a file into your module.

For example, if you are developping a cache backend, you can add:

  my_module/lib/                (where the library stands)
  my_module/my_module.cache.inc

Which only contain:

  <?php
  AutoloadEarly::getInstance()
    ->registerNamespace('MyModule', dirname(__FILE__) . '/lib');

You will then be able to put my_module/my_module.cache.inc as a cache backend
provider file into your settings.php as such:

  $conf['cache_backends'][] = 'sites/all/modules/my_module/my_module.cache.inc';

Then the autoloader will be initialized magically by the core itself on cache
initialization time, which is a dawn good low level enough place to spawn your
autoloader if you need it in pre-bootstrap phase.
