<?php

/**
 * @file
 * Rules extras supplied by the commerce webform module.
 */

/**
 * Implements hook_rules_condition_info().
 */
function commerce_webform_rules_condition_info() {
  return array(
    'commerce_webform_is_commerce_webform' => array(
      'label' => t('Webform is a commerce webform.'),
      'parameter' => array(
        'webform' => array(
          'type' => 'node',
          'label' => t('The submitted webform node'),
        ),
      ),
      'help' => t('This condition returns TRUE if the submitted webform has a commerce webform field.'),
      'group' => t('Commerce Webform'),
    ),
    'commerce_webform_is_commerce_webform_order' => array(
      'label' => t('Order contains items from a commerce webform.'),
      'parameter' => array(
        'commerce_order' => array(
          'type' => 'commerce_order',
          'label' => t('A commerce order'),
        ),
      ),
      'help' => t('This condition returns TRUE if the order contains products from a commerce webform.'),
      'group' => t('Commerce Webform'),
    ),
  );
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_webform_rules_action_info() {
  $actions = array();
  $actions['commerce_webform_order_create'] = array(
    'label' => t('Create an order from a commerce webform'),
    'parameter' => array(
      'node' => array(
       'type' => 'node',
        'label' => t('The submitted webform node.'),
      ),
      'data' => array(
        'type' => 'webform',
        'label' => t('The submitted webform data.'),
      ),
      'user' => array(
        'type' => 'user',
        'label' => t('The user who submitted the webform.'),
      ),
    ),
    'group' => t('Commerce webform'),
  );

  $actions['commerce_webform_order_update'] = array(
    'label' => t('Update an order from a commerce webform update'),
    'parameter' => array(
      'node' => array(
       'type' => 'node',
        'label' => t('The updated webform node.'),
      ),
      'data' => array(
        'type' => 'webform',
        'label' => t('The submitted webform data.'),
      ),
      'user' => array(
        'type' => 'user',
        'label' => t('The user who submitted the webform.'),
      ),
    ),
    'group' => t('Commerce webform'),
  );

  $actions['commerce_webform_mark_paid'] = array(
    'label' => t('Mark the webform submission as paid'),
    'parameter' => array(
      'node' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Commerce webform'),
  );

  return $actions;
}

/**
 * Rules action callback.
 * Adds the product stored in a webform submission
 * to the users cart.
 */
function commerce_webform_order_create($node, $webform_submission, $user) {
  $uid = empty($user) ? 0 : $user->uid;

  $product_details = commerce_webform_get_products_from_webform_submission($webform_submission);
  if (empty($product_details)) {
    return;
  }

  $order_id = 0;
  if ($uid && isset($node->webform['commerce_webform_new_order']) && $node->webform['commerce_webform_new_order']) {
    // Create the new order in checkout.
    $order = commerce_order_new($uid, 'checkout_checkout');

    // Save the order to get its ID.
    commerce_order_save($order);
    $order_id = $order->order_id;
  }

  // Add all the products chosen as line items to the order.
  _commerce_webform_create_order_line_items($product_details, $order_id, $uid, $node->nid, $webform_submission['sid']);

}

/**
 * Helper function to create new line items from the webform submission.
 * Used by webform creation and webform update rules.
 *
 * @param array $product_details
 *   The product details as supplied by a call to
 *   commerce_webform_get_products_from_webform_submission().
 * @param int $order_id
 *   The order to link the line items to (0 for current cart).
 * @param int $uid
 *   The Drupal user uid who owns the order.
 * @param int $nid
 *   The webform node nid.
 * @param int $sid
 *   The webform submission sid
 */
function _commerce_webform_create_order_line_items($product_details, $order_id, $uid, $nid, $sid) {
  if ($order_id) {
    // Setup the wrapper for the order.
    $order = commerce_order_load($order_id);
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  }

  foreach ($product_details as $product_detail) {

    $state = array(
      'commerce_webform' => array(
        'choose_quantity' => $product_detail->choose_quantity,
        'required' => $product_detail->required,
      ),
    );

    // Create a new line item.
    $line_item = commerce_product_line_item_new($product_detail->product, $product_detail->quantity, $order_id, $state, 'commerce_webform');

    // Save the line item to get its ID.
    commerce_line_item_save($line_item);

    // Add the sid to the line item.
    $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    $line_item_wrapper->commerce_webform_sid->set($sid);
    $line_item_wrapper->commerce_webform_nid->set($nid);

    // Resave the line item with sid info.
    commerce_line_item_save($line_item);

    // Update the webform submission for this product field.
    $webform_submission_object = webform_get_submission($nid, $sid);
    commerce_webform_update_webform_submission_productfield($webform_submission_object, $order_id, $line_item->line_item_id, $product_detail->product->product_id, $product_detail->quantity, FALSE);

    // Allow modules to prepare this as necessary. This hook is defined by the
    // Product Pricing module.
    drupal_alter('commerce_product_calculate_sell_price_line_item', $line_item);

    // Process the unit price through Rules so it reflects the user's actual
    // purchase price.
    rules_invoke_event('commerce_product_calculate_sell_price', $line_item);

    if ($order_id) {
      // Add the line item to the special order.
      $order_wrapper->commerce_line_items[] = $line_item;
    }
    else {
      // Add item to the shopping cart.
      commerce_cart_product_add($uid, $line_item, TRUE);
    }
  }

  if ($order_id) {
    // Save the order again to update its line item reference field.
    commerce_order_save($order);
  }
}

/**
 * Rules actions.
 * Update an order from a commerce webform update.
 */
function commerce_webform_order_update($node, $webform_submission, $user) {
  $uid = empty($user) ? 0 : $user->uid;
  $order_id = 0;

  // Find any existing line items for the submission.
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'commerce_line_item')
    ->entityCondition('bundle', 'commerce_webform')
    ->fieldCondition('commerce_webform_sid', 'value', $webform_submission['sid'], '=')
    ->execute();

  if (!isset($result['commerce_line_item'])) {
    // There were no existing line items for this submission
    // so create a new order.
    return commerce_webform_order_create($node, $webform_submission, $user);
  }

  $line_items = entity_load('commerce_line_item', array_keys($result['commerce_line_item']));

  $product_details = commerce_webform_get_products_from_webform_submission($webform_submission);

  // Loop over the existing submissions, updating or removing them.
  foreach ($line_items as $line_item) {
    $wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
    $product = $wrapper->commerce_product->value();
    $order_id = $line_item->order_id;
    if (isset($product_details[$product->product_id])) {
      // Update this line item.
      $product_detail = $product_details[$product->product_id];
      $line_item->quantity = $product_detail->quantity;
      $line_item->data['commerce_webform'] = array(
        'choose_quantity' => $product_detail->choose_quantity,
        'required' => $product_detail->required,
      );
      commerce_line_item_save($line_item);

      // Update the webform submission for this product field.
      $webform_submission_object = webform_get_submission($node->nid, $webform_submission['sid']);
      commerce_webform_update_webform_submission_productfield($webform_submission_object, $order_id, $line_item->line_item_id, $product_detail->product->product_id, $product_detail->quantity, FALSE);

      // Remove this from the list of products to update.
      unset($product_details[$product->product_id]);
    }
    else {
      // This is no longer required in the order.
      commerce_line_item_delete($line_item->line_item_id);
    }
  }

  if (!empty($product_details)) {
    // Create new line items for each new product on the submission.
    _commerce_webform_create_order_line_items($product_details, $order_id, $uid, $node->nid, $webform_submission['sid']);
  }

}

/**
 * Rules action.
 * Mark commerce webform submission as paid.
 */
function commerce_webform_mark_paid($order) {
  module_load_include('inc', 'webform', 'includes/webform.submissions');
  $loaded_order = commerce_order_load($order->order_number);
  $order_wrapper = entity_metadata_wrapper('commerce_order', $loaded_order);
  $line_items = $order_wrapper->commerce_line_items->value();
  foreach ($line_items as $line_item) {
    if ($webform_submission = commerce_webform_get_webform_submission_from_lineitem($line_item)) {
      $line_item_wrapper = entity_metadata_wrapper('commerce_line_item', $line_item);
      $product = $line_item_wrapper->commerce_product->value();
      commerce_webform_update_webform_submission_productfield($webform_submission, $order->order_id, $line_item->line_item_id, $product->product_id, $line_item_wrapper->quantity->value(), TRUE);
    }
  }
}
