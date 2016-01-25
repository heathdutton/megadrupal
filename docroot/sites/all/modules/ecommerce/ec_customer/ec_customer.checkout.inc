<?php


/**
 * @file
 * Provide customer implementation for checkout.
 */

/**
 * Implements hook_checkout_init().
 */
function ec_customer_checkout_init(&$txn) {
  if (empty($txn->customer)) {
    $txn->customer = ec_customer_get_customer(isset($txn->ecid) ? $txn->ecid : NULL);
  }
  $txn->mail = ec_customer_get_email($txn->customer);

  $addresses = ec_customer_get_addresses($txn->customer);

  ec_customer_prepare_addresses($txn, $addresses);
}

/**
 * Implements hook_checkout_form().
 */
function ec_customer_checkout_form(&$form, &$form_state) {
  $txn = & $form_state['txn'];

  if (!variable_get('ec_customer_billing_address', 1) && !$txn->shippable) {
    return;
  }

  $form['ec_customer'] = array(
    '#prefix' => '<div class="customer-addresses' . (!empty($txn->shippable) ? ' shippable' : '') . '">',
    '#suffix' => '</div>',
    '#theme' => 'ec_customer_address',
  );
  $form['ec_customer']['title'] = array(
    '#value' => t(!empty($txn->shippable) ? 'Shipping and billing address' : 'Billing address'),
  );
  $form['ec_customer']['address'] = array(
    '#tree' => TRUE,
  );
  $options = array('' => t('New address'));

  foreach (ec_customer_get_addresses($txn->customer) as $address) {
    $address = (array) $address;
    $options[$address['aid']] = t('@firstname @lastname, @street in @city', array('@firstname' => $address['firstname'], '@lastname' => $address['lastname'], '@street' => $address['street1'], '@city' => $address['city']));
  }

  $types = array('billing');

  if ($txn->shippable) {
    $types[] = 'shipping';
  }

  foreach ($types as $type) {
    $form['ec_customer']['address'][$type] = array(
      '#theme' => 'checkout_address',
      '#prefix' => '<div id="customer-address-' . $type . '" class="customer-address">',
      '#suffix' => '</div>',
    );
    $form['ec_customer']['address'][$type]['#address_type'] = $type;
    if (!empty($txn->{$type . '_address'})) {
      $form['ec_customer']['address'][$type]['display_address'] = array(
        '#markup' => theme('formatted_address', array('address' => $txn->address[$type])),
      );
    }
    else {
      module_load_include('inc', 'ec_store');
      $address = isset($txn->address[$type]) ? $txn->address[$type] : array();
      $addr_form = ec_store_address_form($address);
      $form['ec_customer']['address'][$type] += $addr_form;
      if (ec_customer_get_function($txn->customer->type, 'customer_address_save')) {
        $form['ec_customer']['address'][$type]['save'] = array(
          '#type' => 'checkbox',
          '#title' => t('Save this address for later use.'),
          '#default_value' => isset($txn->address[$type]['save']) ? $txn->address[$type]['save'] : 0,
          '#attributes' => array('class' => array('ignore-update')),
        );
      }
    }

    if ($type == 'billing' && $txn->shippable) {
      $form['ec_customer']['address'][$type]['use_for_shipping'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use this address as the shipping address'),
        '#default_value' => 1,
        '#id' => 'ec-customer-use-for-shipping',
      );
    }

    $form['ec_customer']['address'][$type]['buttons'] = array(
      '#prefix' => '<div class="address-buttons ec-inline-form">',
      '#suffix' => '</div>',
      '#weight' => -1,
    );
    $form['ec_customer']['address'][$type]['buttons']['select_address'] = array(
      '#type' => 'select',
      '#title' => t('Select @type address', array('@type' => $type)),
      '#options' => $options,
      '#access' => count($options) > 1,
    );
    if (isset($txn->{$type . '_address'})) {
      unset($form['ec_customer']['address'][$type]['buttons']['select_address']['#options'][$txn->{$type . '_address'}]);
    }
    $form['ec_customer']['address'][$type]['buttons']['address_submit'] = array(
      '#type' => 'submit',
      '#value' => t('Select address'),
      '#access' => count($options) > 1,
      '#submit' => array('ec_customer_checkout_alter_address'),
      '#ajax' => array(
        'callback' => 'ec_customer_checkout_update_address_js',
        'wrapper' => $type . '-address-form',
        'method' => 'replace',
      ),
    );
  }  

  // load the js to update the states
  module_load_include('inc', 'ec_store');
}

/**
 * Implements hook_checkout_validate().
 */
function ec_customer_checkout_validate(&$form, &$form_state) {
  $txn = & $form_state['txn'];
  
  // If we are changing addresses then don't do any validation of the address.
  if ($form_state['clicked_button']['#value'] == t('Select address')) {
    return;
  }
  
  if (variable_get('ec_customer_billing_address', 1) || $txn->shippable) {
    module_load_include('inc', 'ec_store');
    foreach ($form_state['values']['address'] as $type => $address) {
      if (empty($txn->{$type . '_address'}) && ($type != 'shipping' || !$form_state['values']['address']['billing']['use_for_shipping'])) {
        ec_store_address_form_validate($form['ec_customer']['address'][$type], $address);
      }
    }
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function ec_customer_checkout_alter_address(&$form, &$form_state) {
  ctools_include('object-cache');
  
  $txn = & $form_state['txn'];
  $type = $form_state['clicked_button']['#parents'][1];
  $txn->{$type . '_address'} = $form_state['values']['address'][$type]['buttons']['select_address'];
  $txn->address['billing'] = ec_customer_get_addresses($txn->customer, $txn->{$type . '_address'});
  $form_state['rebuild'] = TRUE;
  
  ctools_object_cache_set('transaction', 'ec_checkout', $txn);
}

/**
 * AJAX to allow the customer to change address.
 */
function ec_customer_checkout_update_address_js(&$form, &$form_state) {
  return $form['ec_customer']['address'][$form_state['clicked_button']['#parents'][1]];
}

/**
 * Check, prepare and sanitize addresses on a transaction.
 *
 * @param $txn
 *  An ecommerce transaction or any similar object that stores address
 *  data on an address attribute.
 *
 * @return
 *  Returns FALSE if addresses could not be resolved. This will mean
 *  that the calling code needs
 *
 */
function ec_customer_prepare_addresses(&$txn, $addresses = NULL) {
  if ($addresses) {
    // The presence of new address data means we are updating the transaction.
    $txn->address = array();
  }

  // Assume that any $txn->address is valid.
  if (!empty($txn->address)) {
    return TRUE;
  }

  // If we lack addresses, try to gather them or return FALSE
  if (!$addresses && !isset($txn->address)) {
    if (!ec_customer_get_attr($txn->customer->type, 'store_addresses', TRUE)) {
      // We don't have an address provider!
      return FALSE;
    }
    else {
      $addresses = ec_customer_get_addresses($txn->customer);
      if (!$addresses) {
        return FALSE;
      }
    }
  }

  // Manage the addresses returned from the address provider.
  if ($txn->shippable) {
    $txn->address['billing'] = $txn->address['shipping'] = reset($addresses);
    if (isset($txn->address['billing']['aid'])) {
      $txn->billing_address = $txn->shipping_address = $txn->address['billing']['aid'];
    }
  }
  elseif (variable_get('ec_customer_billing_address', 1)) {
    if (!empty($addresses)) {
      $txn->address['billing'] = reset($addresses);
      if (isset($txn->address['billing']['aid'])) {
        $txn->billing_address = $txn->address['billing']['aid'];
      }
    }
    else {
      $txn->billing_address = NULL;
    }
  }
  else {
    $txn->address = array();
  }
}

/**
 * The form to set the shipping and billing address in the checkout procedure.
 */
function ec_customer_address_form($txn) {
  // Grab the user's addressbook
  $address = ec_customer_get_addresses($txn->customer);

  if (empty($address) && ec_customer_get_attr($txn->customer->type, 'store_addresses', TRUE)) {
    $goto = ec_customer_links($txn->customer, 'add_address', drupal_get_destination());
    call_user_func_array('drupal_goto', $goto);
  }

  // Build address options
  $options = array();
  foreach ($address as $value) {
    $options[$value->aid] = t('@name, @street in @city',
        array(
      '@name' => isset($value->fullname) ? $value->fullname : "{$value->firstname} {$value->lastname}",
      '@street' => $value->street1,
      '@city' => $value->city,
    ));
  }
  if (!empty($txn->shippable)) {
    $form['shipping_address'] = array(
      '#type' => 'radios',
      '#title' => t('Shipping to'),
      '#default_value' => $txn->shipping_address,
      '#options' => $options,
      '#description' => t('Please choose where you would like the items to be delivered.') .
      (ec_customer_get_attr($txn->customer, 'add_address', TRUE) ?
        ' ' . t('You can also <a href="!add_address">add a new address</a>.', array(
        '!add_address' => call_user_func_array('url',
            ec_customer_links($txn->customer, 'add_address', drupal_get_destination())),
      )) : ''),
    );
  }
  $form['billing_address'] = array(
    '#type' => 'radios',
    '#title' => t('Billing to'),
    '#default_value' => $txn->billing_address,
    '#options' => $options,
    '#description' => t('Please choose where you would like the bill to be delivered.') .
    (ec_customer_get_attr($txn->customer, 'add_address', TRUE) ?
      ' ' . t('You can also <a href="!add_address">add a new address</a>.',
        array('!add_address' => call_user_func_array('url',
            ec_customer_links($txn->customer, 'add_address', drupal_get_destination())))) : ''),
  );
  #$form['address']['#tree'] = TRUE;
  return $form;
}

/**
 * Implements hook_checkout_update().
 */
function ec_customer_checkout_update(&$form, &$form_state) {
  $txn = & $form_state['txn'];

  if (isset($form_state['values']['mail'])) {
    $txn->mail = $form_state['values']['mail'];
  }

  if (empty($txn->billing_address) && $form_state['values']['address']['billing']) {
    $txn->address['billing'] = $form_state['values']['address']['billing'];
  }

  if ($txn->shippable) {
    if (isset($form_state['values']['address']['billing']['use_for_shipping'])) {
      $txn->address['billing']['use_for_shipping'] = $form_state['values']['address']['billing']['use_for_shipping'];
    }
    if ($txn->address['billing']['use_for_shipping']) {
      $txn->address['shipping'] = $txn->address['billing'];
      unset($txn->address['shipping']['save']);
    }
    elseif (empty($txn->shipping_address) && $form_state['values']['address']['shipping']) {
      $txn->address['shipping'] = $form_state['values']['address']['shipping'];
    }
  }
}

/**
 * Implements hook_checkout_post_submit().
 */
function ec_customer_checkout_post_submit($txn, &$form_state) {
  if ($function = ec_customer_get_function($txn->customer->type, 'customer_address_save')) {
    foreach ($txn->address as $type => $address) {
      if (isset($form_state['values']['address'][$address['type']]['save']) && $form_state['values']['address'][$address['type']]['save']) {
        $function($txn->customer, $txn->address[$type]);
      }
    }
  }
}

/**
 * Implements hook_checkout_init().
 */
function ec_customer_email_checkout_form(&$form, &$form_state) {
  $txn = & $form_state['txn'];
  $form['ec_customer_email'] = array(
    '#prefix' => '<div class="customer-email">',
    '#suffix' => '</div>',
    '#theme' => 'ec_mail_checkout_form',
    '#txn' => &$form_state['txn'],
  );
  if ($txn->customer->type == 'anonymous') {
    $form['ec_customer_email']['mail'] = array(
      '#type' => 'textfield',
      '#title' => t('Email'),
      '#default_value' => $txn->mail,
      '#validate' => array('ec_anon_validate_mail' => array()),
      '#required' => TRUE,
    );
  }
  else {
    $form['ec_customer_email']['mail'] = array(
      '#markup' => !empty($txn->mail) ? $txn->mail : ec_customer_get_email($txn->ecid),
    );
    $form['ec_customer_email']['mail']['#access'] = variable_get('ec_display_email', 1);
  }
}

/**
 * Implements hook_checkout_submit().
 */
function ec_customer_email_checkout_submit(&$form, &$form_state) {
  $txn = & $form_state['txn'];
  if (!empty($form_state['values']['mail'])) {
    $txn->mail = $form_state['values']['mail'];
  }
}

