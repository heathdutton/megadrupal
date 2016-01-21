<?php

/**
 * @file
 * Rules events Checkout Page Event.
 */

/**
 * Helper function to return the event info to the main module.
 */
function commerce_rules_extra_process_checkout_page_event_info() {
  return array(
    'label' => t('Process a change to the checkout page'),
    'group' => t('Commerce Checkout'),
  );
}

/**
 * Implements hook_commerce_checkout_page_info_alter().
 *
 * Invoke all rules for process_checkout_page event and alter pages properties returned by rules
 */
function commerce_rules_extra_commerce_checkout_page_info_alter(&$checkout_pages) {

  global $_commerce_rules_extra_page_changes;
  if (!$_commerce_rules_extra_page_changes) {
    $_commerce_rules_extra_page_changes = array();
  }
  $urls = array('checkout', 'system');
  drupal_alter('checkout_page_urls', $urls);
  if (in_array(arg(0), $urls)) {
    rules_invoke_all('commerce_rules_extra_process_checkout_page');
    foreach ($_commerce_rules_extra_page_changes as $page_id => $changes) {
      if ($changes['title'] != '') {
        $checkout_pages[$page_id]['title'] = $changes['title'];
      }
      if ($changes['help'] != '') {
        $checkout_pages[$page_id]['help'] = $changes['help'];
      }
      if ($changes['submit_value'] != '') {
        $checkout_pages[$page_id]['submit_value'] = $changes['submit_value'];
      }
    }
  }
}
