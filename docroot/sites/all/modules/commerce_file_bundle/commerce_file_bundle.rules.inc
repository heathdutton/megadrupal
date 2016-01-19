<?php
/**
 * @file
 * commerce_file_bundle.rules.inc
 * Provides rules api definitions.
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_file_bundle_rules_action_info() {
  $actions = array();
  $group = _commerce_file_translatables('license_label');
  $license_label = $group;
  $actions['commerce_file_bundle_license_issue_order'] = array(
    'label' => t('Create and Issue licenses for files in the order (Bundle Friendly)'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Commerce order'),
        'restriction' => 'selector',
      ),
      'license-status' => array(
        'type' => 'text',
        'label' => t('Issued license status'),
        'options list' => 'commerce_file_license_status_options_list',
        'default value' => 'pending',
      ),
    ),
    'group' => $group,
    'callbacks' => array(
      'execute' => 'commerce_file_bundle_rules_action_issue_order_licenses',
    ),
  );

  $actions['commerce_file_bundle_license_revoke_order'] = array(
    'label' => t('Revoke licenses for files in the order (Bundle Friendly)'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Commerce order'),
        'restriction' => 'selector',
      ),
    ),
    'group' => $group,
    'callbacks' => array(
      'execute' => 'commerce_file_bundle_rules_action_revoke_order_licenses',
    ),
  );

  return $actions;
}

/**
 * Rules action execute callback that assigns file licenses.
 * 
 * @param obj $order
 *   The order object on which to issue file licenses.
 * @param string $updated_license_status
 *   The status to set the file licences to. Default = 'pending'.
 */
function commerce_file_bundle_rules_action_issue_order_licenses($order, $updated_license_status = 'pending') {
  // Kick anonymous.
  if (empty($order->uid)) {
    return array();
  }

  $account = user_load($order->uid);
  if (empty($account->uid)) {
    return array();
  }

  // Wrap order and check for any line items.
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  if (empty($order_wrapper->commerce_line_items)) {
    return array();
  }

  // Issue licenses for each line item.
  foreach ($order_wrapper->commerce_line_items as $delta => $line_item_wrapper) {
    $sub_items = array();
    $sub_items = commerce_product_bundle_get_sub_line_items($line_item_wrapper->value());
    if (!empty($sub_items)) {
      foreach ($sub_items as $_delta => $sub_item) {
        commerce_file_license_issue_by_commerce_line_item($sub_item, $updated_license_status, FALSE, $order);
      }
    }
    else {
      commerce_file_license_issue_by_commerce_line_item($sub_item, $updated_license_status, FALSE, $order);
    }
  }
}

/**
 * Rules action: Revoke file licenses' on an order.
 * 
 * @param obj $order
 *   The order on which to revoke file licenses.
 */
function commerce_file_bundle_rules_action_revoke_order_licenses($order) {
  return commerce_file_bundle_license_order_update_status($order, 'revoked');
}
