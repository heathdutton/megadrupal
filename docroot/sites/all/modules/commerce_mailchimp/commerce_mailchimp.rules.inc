<?php

/**
 * @file
 * Rules integration for the checkout process.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_action_info().
 */
function commerce_mailchimp_rules_action_info() {
  $items = array();

  $items['commerce_mailchimp_user_subscribe_order'] = array(
    'label' => t('Subscribe user to mailchimp lists of an order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order in checkout'),
      ),
    ),
    'group' => t('Mailchimp'),
    'access callback' => 'mailchimp_lists_rules_access_callback',
    'base' => 'commerce_mailchimp_rules_action_user_subscribe_order',
  );

  $items['commerce_mailchimp_ecom_update'] = array(
    'label' => t('Send order data to mailchimp and subscription users to lists'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order in checkout'),
      ),
    ),
    'group' => t('Mailchimp'),
    'callbacks' => array(
      'execute' => 'commerce_mailchimp_rules_action_ecom_update',
    ),
  );

  $items['commerce_mailchimp_ecom_delete'] = array(
    'label' => t('Delete order data from mailchimp'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
    ),
    'group' => t('Mailchimp'),
    'callbacks' => array(
      'execute' => 'commerce_mailchimp_rules_action_ecom_delete',
    ),
  );
  
  $items['commerce_mailchimp_lists_email_subscribe_list'] = array(
    'label' => t('Subscribe order email address to a specific mailchimp list'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'list' => array(
        'type' => 'text',
        'label' => t('Mailchimp List'),
        'options list' => 'commerce_mailchimp_lists_groups_options_list',
      ),
    ),
    'group' => t('Mailchimp'),
    'access callback' => 'mailchimp_lists_rules_access_callback',
    'base' => 'commerce_mailchimp_lists_rules_action_email_subscribe_list',
  );

  return $items;
}

/**
 * MailChimp Lists options callback
 */
function commerce_mailchimp_lists_groups_options_list() {
  $options = array();
  $lists = mailchimp_get_lists();
  foreach ($lists as $list_id => $list) {
    $options[$list_id] = $list['name'];
  }

  return $options;
}

/**
 * Action callback: Subscribe a user to lists of an order.
 */
function commerce_mailchimp_rules_action_user_subscribe_order($order) {
  if (!isset($order->data['commerce_mailchimp'])) {
    return;
  }
  $wrapper = entity_metadata_wrapper('commerce_order', $order);
  $lists = mailchimp_get_lists();
  foreach ($order->data['commerce_mailchimp'] as $list_id => $value) {
    if ($value) {
      $list = $lists[$list_id];
      $merge_variables = array();
      if (isset($list['mergevars'])) {
        $merge_variables = token_generate('commerce-order', $list['mergevars'], array('commerce-order' => $order));
      }
      mailchimp_subscribe($list_id, $order->mail, $merge_variables);
    }
  }
}

/**
 * Action callback: completes checkout for the given order.
 */
function commerce_mailchimp_rules_action_ecom_update($order) {
  $store_id = variable_get('commerce_mailchimp_store_id', NULL);
  if (empty($store_id)) {
    return;
  }

  if (empty($order->mail)) {
    return;
  }

  $wrapper = entity_metadata_wrapper('commerce_order', $order);

  $mcorder = array(
    'id' => $order->order_id,
    'email' => $order->mail,
    'total' => commerce_currency_amount_to_decimal($wrapper->commerce_order_total->amount->value(), $wrapper->commerce_order_total->currency_code->value()),
    'store_id' => $store_id,
    'store_name' => variable_get('commerce_mailchimp_store_name', $_SERVER['SERVER_NAME']),
  );
  if (isset($_COOKIE[variable_get('commerce_mailchimp_campaign_cookie_name', 'mailchimp_campaign_id')])) {
    $mcorder['campaign_id'] = $_COOKIE[variable_get('commerce_mailchimp_campaign_cookie_name', 'mailchimp_campaign_id')];
  }
  if (isset($_COOKIE[variable_get('commerce_mailchimp_email_cookie_name', 'mailchimp_email_id')])) {
    $mcorder['email_id'] = $_COOKIE[variable_get('commerce_mailchimp_email_cookie_name', 'mailchimp_email_id')];
  }
  $items = array();
  foreach ($wrapper->commerce_line_items as $line_item) {
    $line_item_type = $line_item->type->value();
    if ($line_item_type === 'product') {
      $item = array();
      $item['line_num'] = $line_item->line_item_id->value();
      $item['product_id'] = $line_item->commerce_product->product_id->value();
      $item['sku'] = $line_item->commerce_product->sku->value();
      $item['product_name'] = $line_item->commerce_product->title->value();
      $item['category_id'] = 0;
      $item['category_name'] = 'None';
      $product_type = $line_item->commerce_product->type->value();
      if ($field_category = variable_get('commerce_mailchimp_category_field_' . $product_type, '')) {
        if ($terms = field_get_items('commerce_product', $line_item->commerce_product->value(), $field_category)) {
          $term = reset($terms);
          $term = taxonomy_term_load($term['tid']);
          $item['category_id'] = $term->tid;
          $item['category_name'] = $term->name;
        }
      }
      else {
        if ($reference_field = variable_get('commerce_mailchimp_category_reference_field_' . $product_type, '')) {
          $reference_field = explode('|', $reference_field);

          // Load all product displays for the product.
          $nodes = array();
          foreach (commerce_info_fields('commerce_product_reference') as $field) {
            $query = new EntityFieldQuery;
            $query->entityCondition('entity_type', 'node', '=')
              ->fieldCondition($field['field_name'], 'product_id', $line_item->commerce_product->product_id->value(), '=');

            if ($result = $query->execute()) {
              $nodes = $result['node'];
            }
          }

          // Get category info.
          foreach ($nodes as $node) {
            $node = node_load($node->nid);
            if ($node) {
              $node_wrapper = entity_metadata_wrapper('node', $node);
              if (isset($node_wrapper->{$reference_field[1]})) {
                $term = $node_wrapper->{$reference_field[1]}->value();
                if (!empty($term)) {
                  $item['category_id'] = $term->tid;
                  $item['category_name'] = $term->name;
                }
              }
            }
          }
        }
      }
      $item['qty'] = $line_item->quantity->value();
      $item['cost'] = commerce_currency_amount_to_decimal($line_item->commerce_product->commerce_price->amount->value(), $line_item->commerce_product->commerce_price->currency_code->value());

      $items[] = $item;
    }
    else {
      $custom_ids = variable_get('commerce_mailchimp_not_product_ids', array());
      if (!in_array($line_item_type, $custom_ids)) {
        $custom_ids[] = $line_item_type;
        variable_set('commerce_mailchimp_not_product_ids', $custom_ids);
      }
      $item_id = array_search($line_item_type, $custom_ids);

      $item = array();
      $item['line_num'] = $line_item->line_item_id->value();
      $item['product_id'] = $item_id;
      if (isset($line_item->commerce_product)) {
        $item['product_name'] = $line_item->commerce_product->title->value();
        $item['sku'] = $line_item->commerce_product->sku->value();
        $item['cost'] = commerce_currency_amount_to_decimal($line_item->commerce_product->commerce_price->amount->value(), $line_item->commerce_product->commerce_price->currency_code->value());
      }
      else {
        $item['product_name'] = $line_item->line_item_label->value();
        $item['cost'] = commerce_currency_amount_to_decimal($line_item->commerce_unit_price->amount->value(), $line_item->commerce_unit_price->currency_code->value());
      }
      $item['category_id'] = 0;
      $item['category_name'] = 'Other';
      $item['qty'] = $line_item->quantity->value();

      $items[] = $item;
    }

  }
  if (!empty($items)) {
    $mcorder['items'] = $items;
  }
  $mcapi = mailchimp_get_api_object();
  $mcapi->ecomm->orderAdd($mcorder);
}

/**
 * Action callback: completes checkout for the given order.
 */
function commerce_mailchimp_rules_action_ecom_delete($order) {
  $store_id = variable_get('commerce_mailchimp_store_id', NULL);
  if (empty($store_id)) {
    return;
  }

  $mcapi = mailchimp_get_api_object();
  try {
    $mcapi->ecomm->orderDel($store_id, $order->order_id);
  }
  catch (Exception $e) {
    watchdog('commerce_mailchimp', 'An error occurred deleting order @order. "%message"', array(
      '@order' => $order->order_id,
      '%message' => $e->getMessage(),
    ), WATCHDOG_ERROR);
  }
}

/**
 * Action callback: Subscribe an email address to a list.
 */
function commerce_mailchimp_lists_rules_action_email_subscribe_list($order, $list_id) {
  $list = mailchimp_get_list($list_id);
  if (!empty($list->settings['mergefields'])) {
    $mergevars = array_filter($list->settings['mergefields']);
    $mergevars = array_flip($mergevars);
    $merge_variables = token_generate('commerce-order', $mergevars, array('commerce-order' => $order));
  }
  else {
    $merge_variables = array();
  }

  mailchimp_subscribe($list_id, $order->mail, $merge_variables, FALSE);
}

/**
 * @}
 */
