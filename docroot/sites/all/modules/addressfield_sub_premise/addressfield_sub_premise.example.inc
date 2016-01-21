<?php

/**
 * @file
 * Example file for Address Field Sub Promise.
 */

/**
 * Implements hook_addressfield_address_formats_alter().
 */
function example_addressfield_address_formats_alter(&$address_formats) {
  // Enable 'sub_premise' field.
  $address_formats['US']['used_fields'][] = 'sub_premise';
  // Set 'sub_premise' field as required.
  $address_formats['US']['required_fields'][] = 'sub_premise';
  // Set label.
  $address_formats['US']['sub_premise_label'] = t('Address 3');
}
