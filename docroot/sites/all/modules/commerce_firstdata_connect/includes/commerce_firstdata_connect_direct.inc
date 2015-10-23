<?php
/**
 * @file
 * Direct integration with direct (instant) payment.
 */

/**
 * Implements hook_TYPE_alter().
 */
function commerce_firstdata_connect_commerce_cardonfile_checkout_pane_form_alter(&$form, &$form_state) {
  cache_clear_all();
  $card_onfile_default = commerce_firstdata_connect_return_ajax_values($form, $form_state);
  $form_state['commerce_payment']['payment_details']['cardonfile']['#default_value'] = $card_onfile_default;
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
  $payment_method = commerce_payment_method_instance_load($form_state['commerce_payment']['payment_method']['#default_value']);
  $cardonfile_capable = module_exists('commerce_cardonfile') && !empty($payment_method['settings']['cardonfile']);
  if ($form_state['commerce_payment']['payment_method']['#default_value'] == 'commerce_firstdata_connect_hosted|commerce_payment_commerce_firstdata_connect_hosted' and $cardonfile_capable) {
    $form['cardonfile']['#default_value'] = 'new';
    foreach ($form as $keys => $values) {
      if ($keys != 'credit_card') {
        $hosted_form[$keys] = $values;
      }
    }
    $form = $hosted_form;
    if ($cardonfile_capable) {
      if (!empty($form['cardonfile']['#options'])) {
        foreach ($form['cardonfile']['#options'] as $key => $value) {
          if ($key != 'new') {
            $new_array[$key] = $value;
          }
        }
      }
      $new_array['new_card'] = t('Use a different credit card');

      $form['cardonfile']['#options'] = $new_array;

      foreach ($form as $k => $v) {
        if ($k != 'cardonfile_instance_default') {
          $new_form_array[$k] = $v;
        }
        else {
          $cardonfile_instance_default[$k] = $v;
        }
      }
      // Set the Store credit card checkbox.
      $form = $new_form_array;
      $form['cardonfile_store_card'] = array(
        '#type' => 'checkbox',
        '#title' => t('Store this credit card on file for future use.'),
        '#default_value' => '',
        '#ajax' => array(
          'callback' => 'commerce_firstdata_connect_store_card_ajax_callback',
          'wrapper' => 'replace_textfield_div',
        ),
        '#prefix' => '<div id="replace_textfield_div">',
        '#suffix' => '</div>',
      );
      // Set the default card checkbox.
      foreach ($cardonfile_instance_default as $k => $v) {
        if ($k == 'cardonfile_instance_default') {
          $form['cardonfile_instance_default'] = $v;
        }
      }
      $form['cardonfile_instance_default']['#states'] = '';
      $form['cardonfile_instance_default']['#states'] = array(
        'visible' => array(
          ':input[name$="[cardonfile_store_card]"]' => array('value' => '1'),
        ));
      $form['cardonfile_instance_default']['#states'] = array(
        'invisible' => array(
          ':input[name$="[cardonfile]"]' => array('value' => 'new'),
        ),
        'visible' => array(
          ':input[name$="[cardonfile_store_card]"]' => array('checked' => TRUE),
        ),
      );

    }
  }

  if (!empty($form['cardonfile']) and $form['cardonfile'] != 'new' and  $form['cardonfile'] != 'new_card' and $form_state['commerce_payment']['payment_method']['#default_value'] === 'commerce_firstdata_connect_full_payment|commerce_payment_commerce_firstdata_connect_full_payment') {
    cache_clear_all();
    $form['cardonfile']['#default_value'] = 'new';
    if (!empty($form_state['commerce_payment']['payment_details']['cardonfile']['#value']) and $form_state['commerce_payment']['payment_details']['cardonfile']['#value'] != 'new') {
      $form['card_code'] = array(
        '#type' => 'textfield',
        '#title' => 'CVV/CV3 <span class="star_required">*</span>',
        '#default_value' => '',
        '#attributes' => array(
          'autocomplete' => 'off',
        ),
        '#required' => FALSE,
        '#maxlength' => 4,
        '#size' => 4,
        '#validated' => TRUE,
        '#states' => array(
          'invisible' => array(
            ':input[name="commerce_payment[payment_details][cardonfile]"]' => array('value' => 'new'),
          ),
          'visible' => array(
            ':input[name="commerce_payment[payment_details][cardonfile]"]' => array('!value' => 'new'),
          ),
        ),

      );

      $form['#attached']['css'] = array(
        drupal_get_path('module', 'commerce_firstdata_connect') . '/css/style_fd.css',
      );
    }
    elseif (empty($form_state['commerce_payment']['payment_details']['cardonfile']['#value']) and $cardonfile_capable) {
      $form['card_code'] = array(
        '#type' => 'textfield',
        '#title' => 'CVV/CV3 <span class="star_required">*</span>',
        '#default_value' => '',
        '#attributes' => array(
          'autocomplete' => 'off',
        ),
        '#required' => FALSE,
        '#maxlength' => 4,
        '#size' => 4,
        '#validated' => TRUE,
        '#states' => array(
          'invisible' => array(
            ':input[name="commerce_payment[payment_details][cardonfile]"]' => array('value' => 'new'),
          ),
          'visible' => array(
            ':input[name="commerce_payment[payment_details][cardonfile]"]' => array('!value' => 'new'),
          ),
        ),

      );

      $form['#attached']['css'] = array(
        drupal_get_path('module', 'commerce_firstdata_connect') . '/css/style_fd.css',
      );
    }
  }

}

/**
 * Helper function to select new card.
 */
function commerce_firstdata_connect_return_ajax_values($form, &$form_state) {
  return $form_state['commerce_payment']['payment_details']['cardonfile']['#default_value'] = 'new';
}

/**
 * Ajax callback for the store credit card checkbox.
 */
function commerce_firstdata_connect_store_card_ajax_callback($form, $form_state) {
  if ($form_state['triggering_element']['#value'] == 1) {
    return $form['commerce_payment']['payment_details']['cardonfile_store_card']['#default_value'] = 1;
  }
  else {
    return $form['commerce_payment']['payment_details']['cardonfile_store_card']['#default_value'] = '';
  }
}

/**
 * Payment method callback: checkout form.
 */
function commerce_firstdata_connect_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
  if ($payment_method['method_id'] != 'commerce_firstdata_connect_hosted') {
    $card_types = commerce_payment_credit_card_types();

    foreach ($card_types as $key => $value) {
      if ($key == 'visa' || $key == 'mastercard' || $key == 'amex' || $key == 'discover' || $key == 'dc' || $key == 'maestro') {
        $cards_id[] = $key;
        $card_type[$key] = $value;
      }
    }
    $card_type['maestroUK'] = t('Maestro UK');
    $credit_card_settings = array(
      'type' => $cards_id,
      'code' => 'CVV/CV2',
      'issue' => '',
    );

    $form = commerce_payment_credit_card_form($credit_card_settings);
    $form['credit_card']['type']['#options'] = $card_type;

    return $form;
  }
}

/**
 * Payment method callback: checkout form validation.
 */
function commerce_firstdata_connect_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  if ((!empty($pane_values['cardonfile']) and $pane_values['cardonfile'] != 'new') and empty($pane_values['card_code']) and $payment_method['method_id'] != 'commerce_firstdata_connect_hosted') {
    drupal_set_message(t('CVV/CV2 is required field'), 'error');
    return FALSE;
  }
  if ($payment_method['method_id'] == 'commerce_firstdata_connect_hosted') {
    return;
  }
  else {
    if (empty($pane_values['cardonfile']) or $pane_values['cardonfile'] == 'new') {
      if ($payment_method['method_id'] == 'commerce_firstdata_connect_full_payment') {
        module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
        // Validate credit card.
        $settings = array(
          'form_parents' => array_merge($form_parents, array('credit_card')),
        );
        if (empty($pane_values['card_code'])) {
          $pane_values['card_code'] = 1;
          $pane_form['card_code']['#default_value'] = 1;
          $pane_form['card_code']['#value'] = 1;
          $validate = commerce_payment_credit_card_validate($pane_values['credit_card'], $settings);
          if ($validate == TRUE or $validate == 1) {
            return;
          }
        }
        if ((commerce_payment_credit_card_validate($pane_values['credit_card'], $settings) == FALSE)) {
          return FALSE;
        }
        return TRUE;
      }
      else {
        if ($payment_method['settings']['logs'] == '0') {
          watchdog('commerce_firstdata_connect', "The system is trying to use wrong payment method!", array(), WATCHDOG_ERROR);
        }
        drupal_set_message(t('The system is trying to use wrong payment method!'), 'error');
        return FALSE;
      }
    }
    return;
  }
}

/**
 * Submit the payment request.
 */
function commerce_firstdata_connect_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
  $cardonfile_capable = module_exists('commerce_cardonfile') && !empty($payment_method['settings']['cardonfile']);
  $api = commerce_firstdata_connect_api_object('', $payment_method['instance_id']);
  global $user, $base_root;

  // If it's a new card that will be saved.
  if ($payment_method['method_id'] == 'commerce_firstdata_connect_hosted') {
    if ((!empty($payment_method['settings']['cardonfile']) and $payment_method['settings']['cardonfile'] == 1) and !empty($pane_values['cardonfile']) and
        ($pane_values['cardonfile'] == 'new' or $pane_values['cardonfile'] == 'new_card')) {
      if ((!empty($pane_values['cardonfile_store']) and $pane_values['cardonfile_store'] == 1) or (!empty($pane_values['cardonfile_store_card'])
          and $pane_values['cardonfile_store_card'] == 1)) {
        // Create temporary card that will be updated with
        // The real data if the transaction is successeful.
        $pane_values['credit_card']['type'] = 'visa';
        $pane_values['credit_card']['number'] = 'XXXXXXXXXXXXXXXX';
        $pane_values['credit_card']['exp_month'] = 12;
        $pane_values['credit_card']['exp_year'] = 2100;
        commerce_firstdata_connect_cardonfile_create($order, $payment_method, $pane_values);
        $card_details = commerce_firstdata_connect_card_on_file_load_by_date($user->uid);
        if (is_array($card_details)) {
          $card_details = reset($card_details);
        }
        if (!empty($card_details)) {
          // The newly saved card.
          $form_state['values']['hosteddataid'] = $card_details->card_id;
        }
      }
    }

    // If it's existing card already saved.
    if (!empty($pane_values['cardonfile']) and $pane_values['cardonfile'] != 'new' and $pane_values['cardonfile'] != 'new_card') {
      $card = commerce_cardonfile_load($pane_values['cardonfile']);
      if (is_array($card)) {
        $card = reset($card);
      }
      $pane_values['credit_card']['type'] = $card->card_type;
      $form_state['values']['hosteddataid'] = $card->card_id;
      if (empty($pane_values['credit_card']['type'])) {
        if (!empty($card)) {
          $pane_values['credit_card']['type'] = $card->card_type;
        }
      }
      $form_state['values']['card_type'] = $pane_values['credit_card']['type'];
    }

    $result = commerce_firstdata_connect_redirect_alter_form($pane_form, $form_state, $order);

    foreach ($result as $key => $value) {
      if ($key == 'chargetotal' || $key == 'comments' || $key == 'currency' || $key == 'customerid' || $key == 'hash' || $key == 'hosteddataid' || $key == 'paymentMethod' ||
        $key == 'invoicenumber' || $key == 'language' || $key == 'mode' || $key == 'oid' || $key == 'responseFailURL' || $key == 'responseSuccessURL'
        || $key == 'storename' || $key == 'timezone' || $key == 'transactionNotificationURL' || $key == 'trxOrigin' || $key == 'txndatetime' || $key == 'txntype') {
        $new_form[$key] = $value['#value'];
      }
    }

    $new_form['authenticateTransaction'] = 'true';
    $new_form['responseSuccessURL'] = $base_root . '/firstdata_connect/hosted/success';
    $new_form['responseFailURL'] = $base_root . '/firstdata_connect/hosted/fail';
    $new_form['transactionNotificationURL'] = $base_root . '/firstdata_connect/hosted/callback';
    ksort($new_form);

    $string = '';
    foreach ($new_form as $k => $v) {
      if ($k != (count($new_form) - 1)) {
        $string .= $k . '=' . $v . '&';
      }
      elseif ($k == (count($new_form) - 1)) {
        $string .= $k . '=' . $v;
      }
    }

    drupal_goto('firstdata_connect/hosted/saved_cards', array(
      'query' => array(
        'action' => base64_encode($result['#action']),
        'form_param' => base64_encode($string),
      ),
        ));
    return FALSE;

  }
  else {
    if ($cardonfile_capable) {
      // Fully loaded card on file.
      $cardonfile = commerce_cardonfile_load($pane_values['cardonfile']);

      // Create new card on file.
      $cardonfile_capable = module_exists('commerce_cardonfile') && !empty($payment_method['settings']['cardonfile']);
      if ($pane_values['credit_card']['cardonfile_store'] == 1 and ($pane_values['cardonfile'] == 'new' or $pane_values['cardonfile'] == 'new_card') and $cardonfile_capable == TRUE) {
        commerce_firstdata_connect_cardonfile_create($order, $payment_method, $pane_values);
        $card_details = commerce_firstdata_connect_card_on_file_load_by_date($user->uid);
        $card_details = reset($card_details);
        if (empty($card_details)) {
          $card_details = '';
        }
      }
    }
    $payment_method['settings']['payment_method'] = $pane_values['credit_card']['type'];
    $customer_profile = commerce_firstdata_connect_customer_profile_load($order->commerce_customer_billing[LANGUAGE_NONE][0]['profile_id']);
    if (is_array($customer_profile) == TRUE) {
      $customer_profile = reset($customer_profile);
    }
    $api = commerce_firstdata_connect_api_object('', $payment_method['instance_id']);
    // If it's brand new card that will be saved.
    if ((!empty($pane_values['credit_card']['cardonfile_store']) and $pane_values['credit_card']['cardonfile_store'] == TRUE) and ($pane_values['cardonfile'] == 'new' or $pane_values['cardonfile'] == 'new_card') and !empty($card_details)) {
      $result = $api->directPayments($customer_profile, $order, $pane_values, '', $pane_values['cardonfile'], $card_details);
    }
    elseif (!empty($cardonfile) and $pane_values['cardonfile'] != 'new' and $pane_values['cardonfile'] != 'new_card') {
      $result = $api->crossPayment($order, '', $payment_method['settings']['transaction_type_process'], '', $cardonfile->card_id, $pane_values['card_code']);
    }
    else {
      // No card to be saved.
      $result = $api->directPayments($customer_profile, $order, $pane_values);
    }
    $response = commerce_firstdata_connect_get_response_data($result);

    if (array_key_exists('TermUrl', $response) == TRUE) {
      print ($result->data);
      drupal_goto('firstdata_connect/3d_secure/callback', array(
        'query' => array(
          'result' => base64_encode($result->data),
        ),
      ));
    }

    $respond_codes = commerce_firstdata_connect_get_response_code($response);
    if (is_array($customer_profile) == TRUE) {
      $customer_profile = reset($customer_profile);
    }
    $order_id_all = $response['oid'];
    $order_id = explode('-', $order_id_all);
    $api = commerce_firstdata_connect_api_object();
    if (!empty($response['response_hash']) and !empty($response['chargetotal']) and !empty($response['currency']) and !empty($order_id[0]) and !empty($response['approval_code'])) {
      $result_hash = $api->calculateResponseHash($response['response_hash'], $response['approval_code'], $response['chargetotal'], $response['currency'], $order_id[0]);
      $flag = 1;
    }
    if ($result->status_message == 'OK' && $result->code == '200' and $respond_codes['approval_code_status'] == 'Y' && ($result_hash == TRUE or $result_hash == 'Parameter txndatetime was not stored.')) {
      $transaction = commerce_payment_transaction_new('commerce_firstdata_connect_full_payment', $order->order_id);
      $transaction->instance_id = $payment_method['instance_id'];
      $transaction->remote_id = !empty($respond_codes['remote_id']) ? $respond_codes['remote_id'] : '';
      $transaction->amount = $order->commerce_order_total[LANGUAGE_NONE][0]['amount'];
      $transaction->currency_code = $order->commerce_order_total[LANGUAGE_NONE][0]['currency_code'];
      $transaction->status = ($payment_method['settings']['transaction_type_process'] == 'preauth') ? COMMERCE_PAYMENT_STATUS_PENDING : COMMERCE_FIRSTDATA_CONNECT_CAPTURED;
      $transaction->message_variables = array('@name' => $customer_profile[0]->commerce_customer_address[LANGUAGE_NONE][0]['name_line']);
      $transaction->message = ($payment_method['settings']['transaction_type_process'] == 'preauth') ? 'Authorised' : COMMERCE_FIRSTDATA_CONNECT_CAPTURED;
      $transaction->remote_status = $response['status'];
      $transaction->payload = $response;
      $transaction->data['tdate'] = $response['tdate'];
      if (!empty($transaction->uid)) {
        commerce_payment_transaction_save($transaction);
      }
    }
    else {
      if (!empty($flag) and (empty($result_hash) or $result_hash == FALSE)) {
        drupal_set_message(t('The response is not valid! It might be a fraud attempt!'), 'error');
      }
      if ($cardonfile_capable) {
        commerce_cardonfile_delete($card_details->card_id);
      }
      if ($response['approval_code'] != 'Y') {
        drupal_goto('firstdata_connect/hosted/fail', array(
          'query' => array(
            'data' => base64_encode($result->data),
          ),
        ));
      }
    }
  }
}
