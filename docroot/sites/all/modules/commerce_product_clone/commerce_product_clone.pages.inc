<?php

/**
 * @file
 * Page callbacks and display-related code for the Commerce Product Clone module
 */

/**
 * Menu callback to configure module settings.
 *
 * @return array
 *   A system settings form for the module
 */
function commerce_product_clone_settings($form, &$form_state) {
  $product_types = array();
  $types = commerce_product_types();
  foreach ($types as $type => $value) {
    $product_types[$type] = $value['name'];
  }

  $form['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General settings'),
  );

  $form['general']['commerce_product_clone_omitted'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Do NOT allow users to clone the following product types:'),
    '#default_value' => variable_get('commerce_product_clone_omitted', array()),
    '#options' => $product_types,
    '#description' => t('Users with appropriate permissions will be able to clone all other products.'),
  );

  return system_settings_form($form);
}

/**
 * Prepopulates commerce product creation form with original product data
 *
 * This also makes small modifications to the original data to preserve
 * uniqueness.
 *
 * @return array
 *   A commerce product creation form array (pre-populated with original product's data)
 */
function commerce_product_clone_cloneform($original_product) {
  if (isset($original_product->product_id)) {
    if (commerce_product_clone_is_permitted($original_product->type)) {
      global $user;

      // Reset some values we want to be unique.
      $product = clone $original_product;
      $product->product_id = NULL;
      $product->revision_id = NULL;
      $product->uid = $user->uid;
      $product->created = NULL;
      $product->sku = NULL;
      $product->title = t('Clone of !title', array('!title' => $product->title));

      // Set the page title.
      drupal_set_title($product->title);

      // Let other modules alter the product.
      // Usage: hook_commerce_product_clone_product_alter(&$product, $original_product).
      drupal_alter('commerce_product_clone_product', $product, $original_product);

      // Make sure the file defining the commerce product form is loaded.
      module_load_include('inc', 'commerce_product', 'includes/commerce_product.forms');
      return drupal_get_form('commerce_product_ui_product_form', $product);
    }
  }
}
