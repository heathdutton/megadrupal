CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Maintainers

INTRODUCTION
------------

Adds the new command ajax_command_invoke_chain(), that works pretty much like 
ajax_command_invoke(), but with the chance to chain jQuery method calls.

It receives two things: the selector and a chain of methods to apply to selected
data. The chain is an array, where every item can be a string, when the method
doesn't need additional arguments, or an array, where the first item will be the
method name, and the rest its arguments. Note that sending an array with just
one element is the same as passing just a string value instead.

See acic_test module for usage examples.

REQUIREMENTS
------------

Drupal :)

INSTALLATION
------------

* Download the module from the project page or via drush using:
  drush dl ajax_command_invoke_chain

* Install as you would normally install a contributed Drupal module. See:
  https://drupal.org/documentation/install/modules-themes/modules-7
  for further information or via drush using:
  drush en ajax_command_invoke_chain

MAINTAINERS
-----------

Current maintainers:
 * Daniel Dalgo (dalguete) - https://drupal.org/user/142457
