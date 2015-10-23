MakeUp Title
============

Maintainer: w3wfr (<drupal@w3w.fr>)

MakeUp Title helps to manage Title and Submitted visibility in the View mode UI.

### Notice: 

 * Other modules exists that do similar things on Title field.
 * This module also provides the same approach to Submitted field in the same piece of code.
 * As Title and Submitted are not fieldable, Semanticfield has no effect - even if the dropdown appears in the UI.

### Features

MakeUp Title provides a UI to manage visibility of the entity Title field in a view mode. 

It also allows to do the same for the Submitted meta-field.

Requirements
------------

 * Drupal 7.x

### Modules

 * Field
 * MakeUp

Installation
------------

Ordinary installation.
[http://drupal.org/documentation/install/modules-themes/modules-7](http://drupal.org/documentation/install/modules-themes/modules-7)


Setup
-----

Title field is natively display through `page.tpl.php` and `node.tpl.php`.

Make sure the Title and Submitted values won't display by erasing the lines or simply introducing a `and FALSE` in the test.
