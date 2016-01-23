<?php

/**
 * @file
 *  Generate content, taxonomy, menu, and users via drush framework.
 */

/**
 * Implementation of hook_drush_command().
 */
function commerce_devel_generate_drush_command() {
  $items['generate-products'] = array(
    'description' => 'Create products.',
    'arguments' => array(
      'num_products' => 'Number of products to generate.',
    ),
    'options' => array(
      'kill' => 'Delete all products before generating new ones.',
      'types' => 'A comma delimited list of product types to create. Defaults to product.',
      'length' => 'Max word length of titles.',
    ),
    'aliases' => array('genp'),
    'callback' => 'drush_commerce_devel_generate_products',
  );
  return $items;
}


/**
 * Command callback. Generate a number of users.
 */
function drush_commerce_devel_generate_products($num_products = NULL) {
  if (drush_generate_is_number($num_products) == FALSE) {
    return drush_set_error('DEVEL_GENERATE_INVALID_INPUT', t('Invalid number of products.'));
  }
  drush_generate_include_devel();
  module_load_include('inc', 'commerce_devel_generate', 'devel_generate');
  $values = array(
    'kill_products' => drush_get_option('kill'),
    'num_products' => $num_products,
    'product_types' => drupal_map_assoc(explode(',', drush_get_option('types', 'product'))),
    'title_length' => drush_get_option('length', 4),
  );
  commerce_devel_generate_products(array('values' => $values));
  drush_log(t('Generated @number products.', array('@number' => $num_products)), 'success');
}
