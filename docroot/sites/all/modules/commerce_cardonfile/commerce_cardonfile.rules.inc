<?php

/**
 * @file
 * Provides Rules integration
 */

/**
 * Implements hook_rules_data_info().
 */
function commerce_cardonfile_rules_data_info() {
  return array(
    'commerce_cardonfile_charge_card_response' => array(
      'label' => t('charge card response'),
      'group' => t('Card on File'),
      'wrap' => TRUE,
      'wrapper class' => 'EntityStructureWrapper',
      'token type' => 'commerce-cardonfile-charge-card-response',
      'property info' => commerce_cardonfile_charge_card_response_property_info_callback(),
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 */
function commerce_cardonfile_rules_event_info() {
  $events = array();
  $events['commerce_cardonfile_charge_failed'] = array(
    'label' => t('After a failed attempt to charge an order'),
    'group' => t('Commerce Card on File'),
    'access callback' => 'commerce_order_rules_access',
    'variables' => array(
      'card_data' => array(
        'type' => 'commerce_cardonfile',
        'label' => t('Card'),
        'optional' => TRUE,
      ),
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'charge' => array(
        'type' => 'commerce_price',
        'label' => t('Charge'),
        'optional' => TRUE,
      ),
      'response' => array(
        'type' => 'commerce_cardonfile_charge_card_response',
        'label' => t('charge Card Response'),
      ),
    ),
  );
  $events['commerce_cardonfile_charge_success'] = array(
    'label' => t('After a successful chargeing of an order'),
    'group' => t('Commerce Card on File'),
    'access callback' => 'commerce_order_rules_access',
    'variables' => array(
      'card_data' => array(
        'type' => 'commerce_cardonfile',
        'label' => t('Card'),
      ),
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'charge' => array(
        'type' => 'commerce_price',
        'label' => t('Charge'),
      ),
      'response' => array(
        'type' => 'commerce_cardonfile_charge_card_response',
        'label' => t('charge Card Response'),
      ),
    ),
  );

  return $events;
}

/**
 * Implements hook_rules_action_info().
 */
function commerce_cardonfile_rules_action_info() {
  $actions = array();
  $actions['commerce_cardonfile_order_select_card'] = array(
    'label' => t('Select card on file to charge'),
    'group' => t('Commerce Card on File'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'forced_instance_id' => array(
        'type' => 'text',
        'label' => t('Payment method instance ID'),
        'description' => t('If provided, then the instance ID must match that of the card data.'),
        'options list' => 'commerce_cardonfile_rules_payment_instance_charge_options_list',
        'optional' => TRUE,
      ),
    ),
    'provides' => array(
      'select_card_response' => array(
        'type' => 'commerce_cardonfile_charge_card_response',
        'label' => t('Select card response'),
        'save' => FALSE,
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_cardonfile_rules_action_order_select_card',
    ),
  );

  $actions['commerce_cardonfile_order_charge_card'] = array(
    'label' => t('charge an order with a card on file'),
    'group' => t('Commerce Card on File'),
    'parameter' => array(
      'order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      ),
      'charge' => array(
        'type' => 'commerce_price',
        'label' => t('Charge'),
        'description' => t('The charge amount and currency. If not provided, then the order balance will be used.'),
        'optional' => TRUE,
      ),
      'select_card_response' => array(
        'type' => 'commerce_cardonfile_charge_card_response',
        'label' => t('Select Card Response'),
        'description' => t('If provided, Card Data parameter can be omitted.'),
        'optional' => TRUE,
      ),
      'card_data' => array(
        'type' => 'commerce_cardonfile',
        'label' => t('Card'),
        'description' => t('If provided, Select Card Response will be ignored.'),
        'optional' => TRUE,
      ),
      'forced_instance_id' => array(
        'type' => 'text',
        'label' => t('Payment method instance id'),
        'description' => t('If provided and Card Data is provided, then the instance id must match that of the card data.'),
        'options list' => 'commerce_cardonfile_rules_payment_instance_charge_options_list',
        'optional' => TRUE,
      ),
    ),
    'provides' => array(
      'charge_card_response' => array(
        'type' => 'commerce_cardonfile_charge_card_response',
        'label' => t('charge Card Response'),
        'save' => FALSE,
      ),
    ),
    'callbacks' => array(
      'execute' => 'commerce_cardonfile_rules_action_order_charge_card',
    ),
  );


  return $actions;
}

/**
 * Options list for payment method instances that provide a charge callback
 */
function commerce_cardonfile_rules_payment_instance_charge_options_list() {
  $options = array('' => t('- None -'));
  $implements = commerce_cardonfile_payment_method_implements('charge callback');

  foreach ($implements as $method_id => $method_function) {
    $payment_method_instances = _commerce_cardonfile_payment_method_instances($method_id, TRUE);
    if (empty($payment_method_instances)) {
      continue;
    }

    foreach ($payment_method_instances as $instance_id => $payment_method) {
      list($method_id_part, $rule_name) = explode('|', $instance_id);

      $options[$instance_id] = t('@title (rule: @rule)', array(
        '@title' => $payment_method['title'],
        '@rule' => $rule_name,
      ));
    }
  }

  return $options;
}

/**
 * Rules action callback for commerce_cardonfile_order_select_card
 */
function commerce_cardonfile_rules_action_order_select_card($order, $forced_instance_id = NULL) {

  $response = commerce_cardonfile_order_select_card($order, $forced_instance_id);

  return array('select_card_response' => $response);
}

/**
 * Rules action callback for commerce_cardonfile_order_charge_card
 */
function commerce_cardonfile_rules_action_order_charge_card($order, $charge = NULL, $select_card_response = NULL, $card = NULL, $forced_instance_id = NULL) {
  // This action needs either the card data provided or a response returned by
  // the `Select card on file to charge` action.
  if (empty($select_card_response) && empty($card)) {
    // Return an error wrapped in a response if none of them are provided.
    $response = array(
      'code' => COMMERCE_COF_PROCESS_CODE_INSUFFICIENT_DATA,
      'message' => 'No chargeable card on file is provided for the order @order_id.',
    );
    rules_invoke_all('commerce_cardonfile_charge_failed', NULL, $order, $charge, $response);
    return array('charge_card_response' => $response);
  }

  // If provided, card data will be used regardless the response returned by
  // the `Select card on file to charge` action.
  if (empty($card)) {
    // Card data is not provided, so let's use the response by the
    // aforementioned action. Make sure it found a chargeable card.
    if ($select_card_response['code'] == COMMERCE_COF_PROCESS_CODE_CARD_OK) {
      $card = $select_card_response['card_chosen'];
    }
    else {
      rules_invoke_all('commerce_cardonfile_charge_failed', NULL, $order, $charge, $select_card_response);
      return array('charge_card_response' => $select_card_response);
    }
  }

  $response = commerce_cardonfile_order_charge_card($order, $charge, $card, $forced_instance_id);
  return array('charge_card_response' => $response);
}
