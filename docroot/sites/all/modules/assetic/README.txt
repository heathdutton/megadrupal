Assetic 1.x for Drupal 7.x
--------------------------
Assetic for Drupal adds support of the [Assetic Asset Management Framework][1]
to Drupal.

Installation
------------
The module needs the Assetic library to work properly. To include those classes,
we make use of the [Libraries API][2] and [X Autoload][3] modules. This way we
can use the Assetic classes through PSR-0 autoloading.

You also need to add the library and its dependency to your Drupal installation.
Both of these libraries need to exist in the following directories:
- 'sites/*/libraries/assetic' for the Assetic library
- 'sites/*/libraries/symfony-process' for the Symfony/Process library

You can check *admin/reports/status* if the Assetic library and
it's dependencies are correctly installed.

Usage
-----
The Assetic module doesn't do very much by itself. You have to active the
submodules to get more functionality. Check the README.txt files of each
separate module to see how you have to use them.

[1]: https://github.com/kriswallsmith/assetic
[2]: http://drupal.org/project/libraries
[3]: http://drupal.org/project/xautoload
[4]: https://github.com/symfony/Process

TODO
----
- Implement a Drush command for dumping compiled assets.
- Extend the documentation about the API.
