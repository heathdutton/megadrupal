<?php

/**
 * @file
 * Handle our rules integration.
 */


/**
 * Builds the product sell price calculation Rules Overview page.
 */
function commerce_barcode_scanner_scanner_rules() {

  RulesPluginUI::$basePath = 'admin/config/workflow/rules/reaction';

  $rules_tables = array(
    'enabled' => array(
      'title' => t('Enabled Barcode Scanner Rules'),
      'conditions' => array(
        'active' => TRUE,
        'event' => 'commerce_barcode_scanner_scanned',
      ),
      'empty' => t('There are no active barcode scanner rules.'),
    ),
    'disabled' => array(
      'title' => t('Disabled Barcode Scanner Rules'),
      'conditions' => array(
        'active' => FALSE,
        'event' => 'commerce_barcode_scanner_scanned',
      ),
      'empty' => t('There are no disabled barcode scanner rules.'),
    ),
  );

  $content = array();
  foreach ($rules_tables as $type => $info) {
    $content[$type]['title']['#markup'] = '<h3>' . $info['title'] . '</h3>';
    $content[$type]['rules'] = RulesPluginUI::overviewTable($info['conditions'], array('show plugin' => FALSE));
    $content['enabled']['rules']['#empty'] = $info['empty'];
  }

  return $content;
}


/**
 * Implements hook_rules_event_info().
 */
function commerce_barcode_scanner_rules_event_info() {
  return array(
    'commerce_barcode_scanner_scanned' => array(
      'label' => t('Barcode Scanned'),
      'group' => t('Commerce Barcode'),
      'help' => t('Triggers when someone scans a barcode while on the website.'),
      'variables' => array(
        'commerce_product' => array(
          'label' => t('Product'),
          'type' => 'commerce_product',
        ),
        'commerce_product_display' => array(
          'label' => t('Product Display'),
          'type' => 'node',
        ),
      ),
    ),
  );
}


/**
 * Trigger a barcode scan event.
 *
 * This is a menu callback invoked directly from the client and will trigger a
 * rules event, if permission is so granted.
 *
 * @param string $sku
 *   A products SKU.
 */
function commerce_barcode_scanner_trigger_scan($sku) {

  // Ensure users are allowed to trigger barcode events.
  if (!user_access('trigger commerce barcode events')) {
    drupal_goto();
  }

  // Load our product and product display.
  $commerce_product = commerce_product_load_by_sku($sku);
  if ($commerce_product) {
    $commerce_product_display = commerce_barcode_scanner_display_referencing_product($commerce_product);
  }

  // Check we have loaded valid products and product display.
  if (!$commerce_product || !isset($commerce_product_display) || !$commerce_product_display) {
    drupal_set_message(t('Unable to find product matching sku "@sku".', array('@sku' => $sku)));
    drupal_goto();
  }

  rules_invoke_event('commerce_barcode_scanner_scanned', $commerce_product, $commerce_product_display);

  // If no redirect takes place in our rules execution, we simply return to the
  // previous page. The destination paramater is set by the javascript.
  drupal_goto();

}
