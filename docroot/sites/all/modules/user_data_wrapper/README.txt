CONTENTS OF THIS FILE
---------------------

 * Introduction
 * Requirements
 * Installation
 * Usage
 * Maintainers


INTRODUCTION
------------
user_data_wrapper is a simple API that provides a structured way to use the
$account->data array. It also provides your defined properties to the entity
meta wrapper.

REQUIREMENTS
------------
This module requires the following modules:
 * Entity (https://drupal.org/project/entity)

INSTALLATION
------------
* Install as you would normally install a contributed Drupal module. See:
https://drupal.org/documentation/install/modules-themes/modules-7
for further information.

USAGE
-----
To use this module you will first need to define your data using the
hook_user_data_info hook. See the user_data_wrapper.api.php doc for examples.

You can then use your defined data through entity meta data wrappers as
shown the user_data_wrapper.api.php

MAINTAINERS
-----------
Current maintainers:
 * Alex Farr (alexfarr) - https://drupal.org/user/913434
