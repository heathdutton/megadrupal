MakeUp CSS
==========

Maintainer: w3wfr (<drupal@w3w.fr>)

MakeUp CSS provide a field to define CSS that will be added to the header or imported in a file.

### Notice: 

 * Inline CSS formatter is most adapted for testing and light corrections that will pass at last and not impact other Drupal optimized files.
 * For best performance, CSS File formatter should be used as it is compatible with Drupal optimization.

### Features

MakeUp CSS provide CSS code as a content.

Using it with EntityReference helps skinning the page based on Taxonomy term, Organic group or Bean.

This approach allows webmasters and designer agencies to adapt the design simply by creating a new design content (term, block...) and attach it to any content.

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


Settings
--------

A CSS value can be set at formatter / view mode level: it is used as a default value. It is only used if no value is provided at content level.
