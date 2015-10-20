# Definition list

## Overview
This module exposes theme_definition_list, which will theme an array as a
[definition list](http://www.w3.org/wiki/HTML/Elements/dl).

## Usage
Function theme_definition_list accepts a single keyed array as an argument:

theme('definition_list', $variables);

In its simplest form, $variables only requires an 'items' array:

```php
$variables = array(
  'items' => array(
    array('term' => 'Term 1', 'desc' => 'Definition 1'),
    array('term' => 'Term 2', 'desc' => 'Definition 2'),
  ),
);
theme('definition_list', $variables);
```
An optional 'title' key will add a title to the output:

```php
$variables = array(
  'items' => array(
    array('term' => 'Term 1', 'desc' => 'Definition 1'),
    array('term' => 'Term 2', 'desc' => 'Definition 2'),
  ),
  'title' => 'This is the title for my definition list',
);
theme('definition_list', $variables);
```

A further 'attributes' key can optionally be included, which will passed on to
drupal_attributes() to allow custom attributes to be added to the dl html
element such as class:

```php
$variables = array(
  'attributes' => array(
    'class' => 'dl_class',
  ),
  'items' => array(
    array('term' => 'Term 1', 'desc' => 'Definition 1'),
    array('term' => 'Term 2', 'desc' => 'Definition 2'),
  )
);
theme('definition_list', $variables);
```

Each term and desc can be passed as a single value (as shown above), or an
array when the value is the 'data' key.  Additional keys are passed on to
drupal_attributes() to allow custom attributes to be added the corresponding
dt/dd html elements such as class or style.  First and last classed are added
by default:

```php
$variables = array(
  'items' => array(
    array(
      'term' => array(
        'data'  => 'Term 1',
        'class' => 'term_1_class',
        'style' => 'color:red;',
      ),
      'desc' => array(
        'data'  => 'Definition 1',
        'class' => 'desc_1_class',
        'style' => 'color:blue;',
      ),
    ),
    array('term' => 'Term 2', 'desc' => 'Definition 2'),
  )
);
theme('definition_list', $variables);
```
