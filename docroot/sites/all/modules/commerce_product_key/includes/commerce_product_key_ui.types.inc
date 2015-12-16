<?php

/**
 * @file
 */


/**
 * Menu callback: display an overview of available types.
 */
function commerce_product_key_ui_types_overview() {
  drupal_add_css(drupal_get_path('module', 'commerce_product_key_ui') . '/theme/commerce_product_key_ui.types.css');

  $header = array(
    t('Name'),
    t('Operations'),
  );

  $rows = array();

  // Loop through all defined sequences.
  foreach (commerce_product_key_types() as $type => $product_key_type) {
    // Build the operation links for the current product key type.
    $links = menu_contextual_links('commerce-product-key-type', 'admin/commerce/product-keys/types', array(strtr($type, array('_' => '-'))));

    // Add the product key type's row to the table's rows array.
    $rows[] = array(
      theme('product_key_type_admin_overview', array('product_key_type' => $product_key_type)),
      theme('links', array('links' => $links, 'attributes' => array('class' => 'links inline operations'))),
    );
  }

  // If no product key types are defined...
  if (empty($rows)) {
    // Add a standard empty row with a link to add a new product key type.
    $rows[] = array(
      array(
        'data' => t('There are no sequences yet. <a href="@link">Add sequence</a>.', array('@link' => url('admin/commerce/product-keys/types/add'))),
        'colspan' => 2,
      )
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Builds an overview of a product key type for display to an administrator.
 *
 * @param $variables
 *   An array of variables used to generate the display; by default includes the
 *     type key with a value of the product key type array.
 *
 * @ingroup themeable
 */
function theme_product_key_type_admin_overview($variables) {
  $product_key_type = $variables['product_key_type'];

  $output = check_plain($product_key_type['name']);
  $output .= ' <small> (Machine name: ' . check_plain($product_key_type['type']) . ')</small>';
  $output .= '<div class="description">' . filter_xss_admin($product_key_type['description']) . '</div>';

  return $output;
}

/**
 * Form callback wrapper: create or edit a product key type.
 *
 * @param $type
 *   The machine-name of the product key type being created or edited by this form
 *     or a full product key type array.
 *
 * @see commerce_product_key_product_key_type_form()
 */
function commerce_product_key_ui_product_key_type_form_wrapper($type) {
  if (is_array($type)) {
    $product_key_type = $type;
  }
  else {
    $product_key_type = commerce_product_key_type_load($type);
  }

  // Add the breadcrumb for the form's location.
  commerce_product_key_ui_set_breadcrumb(TRUE);

  // Return a message if the product key type is not governed by Product UI.
  if (!empty($product_key_type['type']) && $product_key_type['module'] != 'commerce_product_key_ui') {
    return t('This sequence cannot be edited, because it is not defined by the Product Key UI module.');
  }

  // Include the forms file from the Product module.
  module_load_include('inc', 'commerce_product_key_ui', 'includes/commerce_product_key_ui.forms');

  return drupal_get_form('commerce_product_key_ui_product_key_type_form', $product_key_type);
}

/**
 * Form callback wrapper: confirmation form for deleting a product key type.
 *
 * @param $type
 *   The machine-name of the product key type being created or edited by this form
 *     or a full product key type array.
 *
 * @see commerce_product_key_product_key_type_delete_form()
 */
function commerce_product_key_ui_product_key_type_delete_form_wrapper($type) {
  if (is_array($type)) {
    $product_key_type = $type;
  }
  else {
    $product_key_type = commerce_product_key_type_load($type);
  }

  // Add the breadcrumb for the form's location.
  commerce_product_key_ui_set_breadcrumb(TRUE);

  // Return a message if the product key type is not governed by Product UI.
  if ($product_key_type['module'] != 'commerce_product_key_ui') {
    return t('This sequence cannot be deleted, because it is not defined by the Product UI module.');
  }

  // Don't allow deletion of product key types that have products already.
  if (($count = db_query("SELECT product_key_id FROM {commerce_product_key} WHERE type = :product_key_type", array(':product_key_type' => $product_key_type['type']))->rowCount()) > 0) {
    drupal_set_title(t('Cannot delete the %name sequence', array('%name' => $product_key_type['name'])), PASS_THROUGH);
    return format_plural($count,
      'There is 1 product key of this type. It cannot be deleted.',
      'There are @count product keys of this type. It cannot be deleted.'
    );
  }

  // Include the forms file from the Product module.
  module_load_include('inc', 'commerce_product_key_ui', 'includes/commerce_product_key_ui.forms');

  return drupal_get_form('commerce_product_key_ui_product_key_type_delete_form', $product_key_type);
}
