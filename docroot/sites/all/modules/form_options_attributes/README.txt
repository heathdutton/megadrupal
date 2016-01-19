# Form Options Attributes Module

## Overview

This module adds the ability to specify attributes for individual options
on Drupal Form API elements of the types select, checkboxes, and radios.

This is an API module, with no user interface. You would only need this
module if another module you are using requires it or if you are programming
a custom form that requires attributes on select <option> children, radio
buttons within a radios element, or checkbox elements within an checkboxes
element.


## Usage

To add attributes to a form element's options, add an '#options_attributes'
key to the form element definition. The #options_attributes value should be
an array with keys that match the keys in the #options value array. The values
in the #options_attributes array are formatted like the main #attributes array.

Example:

$form['states'] = array(
  '#type' => 'select',
  '#title' => t('States'),
  '#options' => array(
    'AL' => t('Alabama'),
    'AK' => t('Alaska'),
    'AZ' => t('Arizona'),
    'AR' => t('Arkansas'),
    // ...
    'WI' => t('Wisconsin'),
    'WY' => t('Wyoming'),
  ),
  '#options_attributes' => array(
    'AL' => array('class' => array('southeast'), 'data-bbq-meat' => 'pork')),
    'AK' => array('class' => array('non-contiguous'), 'data-bbq-meat' => 'crab')),
    'AZ' => array('class' => array('southwest'), 'data-bbq-meat' => 'rattlesnake')),
    'AR' => array('class' => array('south'), 'data-bbq-meat' => 'beef')),
    // ...
    'WI' => array('class' => array('midwest'), 'data-bbq-meat' => 'cheese')),
    'WY' => array('class' => array('flyover'), 'data-bbq-meat' => 'bison')),
  ),
  '#attributes' => array('class' => array('states-bbq-selector')),
);
