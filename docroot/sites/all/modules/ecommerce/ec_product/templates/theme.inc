<?php

/**
 * @file
 * Contains all preprocess functions for product templates.
 */

/**
 * Create variables for displaying of the product price.
 */
function template_preprocess_ec_product_price(&$variables) {
  drupal_add_css(drupal_get_path('module', 'product') . '/css/product.css');

  $variables['price_prefix'] = t('Price:');
  $variables['price'] = format_currency(ec_product_get_final_price($variables['node'], 'product'));

  $variables['template_files'][] = 'ec-product-price-' . str_replace('_', '-', $variables['node']->type);
  $variables['template_files'][] = 'ec-product-price-' . str_replace('_', '-', $variables['node']->ptype);
  $variables['template_files'][] = 'ec-product-price-' . str_replace('_', '-', $variables['node']->type) . '-' . str_replace('_', '-', $variables['node']->ptype);
}

/**
 * Create variables for product ptype feature list.
 */
function template_preprocess_ec_product_admin_ptypes_feature_list(&$variables) {
  drupal_add_tabledrag('feature-table', 'order', 'sibling', 'feature-weight');

  $form = & $variables['form'];
  $variables['features'] = array();
  $variables['submit'] = '';

  if (!empty($form['features'])) {
    foreach (element_children($form['features']) as $ftype) {
      $form['features'][$ftype]['weight']['#attributes']['class'][] = 'feature-weight';
      $variables['features'][$ftype] = array(
        'name' => drupal_render($form['features'][$ftype]['name']),
        'description' => drupal_render($form['features'][$ftype]['description']),
        'weight' => drupal_render($form['features'][$ftype]['weight']),
        'ops' => isset($form['features'][$ftype]['ops']) ? drupal_render($form['features'][$ftype]['ops']) : '',
        'draggable' => isset($form['features'][$ftype]['weight']['#disabled']) && !$form['features'][$ftype]['weight']['#disabled'] ? TRUE : FALSE,
      );
    }

    $variables['submit'] = drupal_render($form['submit']);
  }
}

function template_preprocess_ec_product_admin_features_overview(&$variables) {
  module_load_include('inc', 'ec_product', 'ec_product.admin');
  drupal_add_css(drupal_get_path('module', 'ec_product') . '/css/admin.css');
  
  // Insert Types into the breadcrumb
  $bc = drupal_get_breadcrumb();
  $last = array_pop($bc);

  $bc[] = l(t('Types'), 'admin/config/store/products/types');
  $bc[] = $last;

  drupal_set_breadcrumb($bc);

  $variables['feature_list'] = drupal_render(drupal_get_form('ec_product_admin_ptypes_feature_list', $variables['ptype']));
  $variables['feature_add'] = drupal_render(drupal_get_form('ec_product_admin_ptypes_feature_form', $variables['ptype']));
}
