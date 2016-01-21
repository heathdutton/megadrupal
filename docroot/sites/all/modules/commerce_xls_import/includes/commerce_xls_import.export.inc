<?php

/**
 * @file
 * Admin page for product exports.
 */

/**
 * Import Menu Form.
 *
 * Contains the buttons export products.
 */
function commerce_xls_import_start() {
  $form = array();
  $types = array();

  foreach (commerce_product_types() as $product_type) {
    $types[$product_type['type']] = $product_type['name'];
  }

  $form['title'] = array(
    '#markup' => '<h1>' . t('Commerce XLS Import - Export') . '</h1>',
  );

  $form['product_type'] = array(
    '#title' => t('Product Type'),
    '#type' => 'select',
    '#description' => t('Select the product type to export.'),
    '#options' => $types,
  );

  $form['export'] = array(
    '#type' => 'markup',
    '#prefix' => '<div class="product-options ' . t('export') . '">',
    '#suffix' => '</div><div style="clear:both"></div>',
  );


  if (!commerce_xls_import_is_running()) {
    $form['export_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Begin Export'),
    );
  }
  else {
    $form['warning'] = array(
      '#markup' => '<h6 style="color: red">' . t('Please wait for the current import/validation to complete') . '</h6>',
    );
    drupal_set_message(t('You are currently running an import or validation, you will not be able to run an export until it is finished or stopped manually'), 'warning', FALSE);
  }

  $form['link']['import_products'] = array(
    '#type' => 'link',
    '#href' => 'admin/commerce/products/import_commerce',
    '#title' => t('Import Products'),
    '#prefix' => '<div style="clear:both"></div>',
    '#attributes' => array(
      'id' => array('commerce-xls-import-export-products'),
    ),
  );

  return $form;
}

/**
 * Submit handler for the import start form.
 */
function commerce_xls_import_start_submit($form, &$form_state) {
  if (!commerce_xls_import_is_running()) {
    $product_type = $form_state['values']['product_type'];
    $product_info = commerce_xls_import_export_product_data($product_type);

    commerce_xls_import_export('export', array($product_info), $product_type);

    $filename = $product_type . "_export.xls";
    $path = file_build_uri($filename);

    readfile($path);

    // Exit is used to keep page from refreshing which would prevent download.
    drupal_exit();
  }
  else {
    drupal_set_message(t('You are currently running an file validation. Please wait until it is finished or cancel the current one'), 'error', FALSE);
  }
}
