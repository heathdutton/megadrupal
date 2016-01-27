<?php

/**
 * @file
 * Page callbacks and form builder functions for administering products.
 */


/**
 * Menu callback: display a list of product key types that the user can create.
 */
function commerce_product_key_ui_add_page() {
  $item = menu_get_item();
  $content = system_admin_menu_block($item);

  // Bypass the admin/commerce/product-keys/add listing if only one product key type is
  // available.
  if (count($content) == 1) {
    $item = array_shift($content);
    drupal_goto($item['href']);
  }

  return theme('product_add_list', array('content' => $content));
}

/**
 * Displays the list of available product key types for product creation.
 *
 * @ingroup themeable
 */
function theme_product_key_add_list($variables) {
  $content = $variables['content'];
  $output = '';

  if ($content) {
    $output = '<dl class="commerce-product-key-type-list">';
    foreach ($content as $item) {
      $output .= '<dt>' . l($item['title'], $item['href'], $item['localized_options']) . '</dt>';
      $output .= '<dd>' . filter_xss_admin($item['description']) . '</dd>';
    }
    $output .= '</dl>';
  }
  else {
    if (user_access('administer sequences')) {
      $output = '<p>' . t('You have not created any sequences yet. Go to the <a href="@create-product-key-type">sequence creation page</a> to add a new sequence.', array('@create-product-key-type' => url('admin/commerce/product-keys/types/add'))) . '</p>';
    }
    else {
      $output = '<p>' . t('No Sequences have been created yet for you to use.') . '</p>';
    }
  }

  return $output;
}

/**
 * Form callback wrapper: create or edit a product.
 *
 * @param $product_key
 *   The product object being edited by this form.
 *
 * @see commerce_product_key_ui_product_form()
 */
function commerce_product_key_ui_product_form_wrapper($product_key) {
  // Add the breadcrumb for the form's location.
  commerce_product_key_ui_set_breadcrumb();

  // Include the forms file from the Product Key module.
  module_load_include('inc', 'commerce_product_key', 'includes/commerce_product_key.forms');

  return drupal_get_form('commerce_product_key_ui_product_form', $product_key);
}

/**
 * Form callback wrapper: confirmation form for deleting a product key.
 *
 * @param $product_key
 *   The product object being deleted by this form.
 *
 * @see commerce_product_key_product_key_delete_form()
 */
function commerce_product_key_ui_product_delete_form_wrapper($product_key) {
  // Add the breadcrumb for the form's location.
  commerce_product_key_ui_set_breadcrumb();

  // Include the forms file from the Product Key module.
  module_load_include('inc', 'commerce_product_key', 'includes/commerce_product_key.forms');

  return drupal_get_form('commerce_product_key_ui_product_delete_form', $product_key);
}
