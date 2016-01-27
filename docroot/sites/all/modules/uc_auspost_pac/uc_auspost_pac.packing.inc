<?php
/**
 * @file
 * Default and core packing algorithms.
 */

/**
 * Implements hook_packing_info().
 */
function uc_auspost_pac_packing_info() {
  $info = array();

  $info['do_not_pack'] = array(
    'title' => t('Do Not Pack'),
    'callback' => 'uc_auspost_pac_packing_do_not_pack',
    'file' => 'packing/do_not_pack.inc',
    'description' => t('All products are packaged individually.'),
  );

  return $info;
}

/**
 * Packing method settings form wrapper.
 */
function uc_auspost_pac_settings_wrapper($packing_id, $method_id) {

  // Load the packing method method and see if it has a configuration form.
  $packing_method = uc_auspost_pac_get_packing_method($packing_id);

  // If there's no configuration form to display, 404 it.
  if (!isset($packing_method['config']) || !$packing_method['config']) {
    return drupal_not_found();
  }

  drupal_set_title($packing_method['title'] . ' ' . t('Packing Method Settings'));

  // Load the shipping method to see if it's valid.
  $quote_methods = uc_quote_methods(TRUE);
  if (!isset($quote_methods[$method_id])) {
    return drupal_not_found();
  }

  // If the packing method is limited and this method_id is not mentioned,
  // then again, return a 404 error.
  if (isset($packing_method['quote_methods']) && $packing_method['quote_methods'] && !in_array($method_id)) {
    return drupal_not_found();
  }

  // Load the configuration form file if set.
  if (isset($packing_method['config file'])) {
    include_once $packing_method['file path'] . '/' . $packing_method['config file'];
  }

  // If the function name to call does not exist, display an error.
  if (function_exists($packing_method['config'])) {
    return drupal_get_form($packing_method['config'], $packing_method, $quote_methods[$method_id]);
  }
  else {
    return '<p>' . t('Packing method settings form callback function is not defined.') . '</p>';
  }
}
