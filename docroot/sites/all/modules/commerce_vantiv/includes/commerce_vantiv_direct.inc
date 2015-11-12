<?php
/**
 * @file
 * This file implements the direct payment functionality for
 * Little and co payment gateway.
 * Payment method instance id: vantiv_direct|commerce_payment_vantiv_direct
 */

/**
 * hook_TYPE_alter().
 * Alters the checkout payment form.
 */
function commerce_vantiv_commerce_cardonfile_checkout_pane_form_alter(&$form, &$form_state) {
  if ($form_state['commerce_payment']['payment_method']['#default_value'] == 'vantiv_echeck|commerce_payment_vantiv_echeck') {
    // Change the label for card storage.
    $form['credit_card']['cardonfile_store']['#title'] = t('Store this check on file for further use.');
    $form['credit_card']['cardonfile_store']['#states'] = array(
      'visible' => array(
        ':input[name$="[cardonfile]"]' => array('value' => 'new'),
      ),
      'invisible' => array(
        ':input[name=$"[cardonfile]"]' => array('value' => '!new'),
      ),
    );

    if(!empty($form['cardonfile']['#options'])) {
      foreach ($form['cardonfile']['#options'] as $key => $value) {
        if (is_numeric($key) == TRUE) {
          $new_value = str_replace('Card', 'Account', $value);
          $piece = explode(',', t('@str', array('@str' => $new_value)));
          $new_value_replace = $piece[0];
          $new_array[$key] = $new_value_replace;
        }
      }
      $new_array['new'] = t('Use a different echeck account.');
      $form['cardonfile']['#options']  = $new_array;
      $form['cardonfile']['#default_value'] = 'new';
    }
    $form['cardonfile']['#title'] = t('Select a stored echeck');
  }
  if ($form_state['commerce_payment']['payment_method']['#default_value'] == 'vantiv_direct|commerce_payment_vantiv_direct') {
    return $form;
  }
}

/**
 * Submit callback for commerce_vantiv.
 */
function commerce_vantiv_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $cardonfile_capable = module_exists('commerce_cardonfile') && !empty($payment_method['settings']['cardonfile']);
  if ($payment_method['method_id'] == 'vantiv_direct') {

    module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
    $card_types = commerce_vantiv_direct_all_card_names();
    foreach ($card_types as $key => $value) {
      $cards_id[] = $key;
    }
    $form = commerce_payment_credit_card_form();
    $credit_card_settings = array(
      'type'       => $cards_id,
      'code'       => 'CVV/CV2',
      'owner' => '',
    );


    $form = commerce_payment_credit_card_form($credit_card_settings);
    $customer_profile = commerce_customer_profile_load($order->commerce_customer_billing['und'][0]['profile_id']);
    $form['credit_card']['type']['#options'] = $card_types;

      if ($cardonfile_capable) {
        $storage = variable_get('commerce_cardonfile_storage', 'opt-in');
        if (in_array($storage, array('opt-in', 'opt-out')) && !$payment_method['settings']['continuous']) {
          $form['credit_card']['cardonfile_store'] = array(
            '#type' => 'checkbox',
            '#title' => t('Store this credit card on file for future use.'),
            '#default_value' => $storage == 'opt-out',
          );
        }
        else {
          $form['credit_card']['cardonfile_store'] = array(
            '#type' => 'hidden',
            '#value' => '',
          );
        }

        $form['cardonfile_instance_default'] = array(
          '#type' => 'checkbox',
          '#title' => t('Set as your default card'),
          '#states' => array(
            'invisible' => array(
              ':input[name$="[cardonfile]"]' => array('value' => 'new'),
            ),
            'visible' => array(
              ':input[name$="[cardonfile_store]"]' => array('checked' => TRUE),
            ),
          ),
        );
      }
   $api = commerce_vantiv_api_object();
   $fraud_key = !empty($api->advance_fraud_key) ? $api->advance_fraud_key : '';
   $reminder = $order->uid . '-' . time();
   $session_id =  $fraud_key . '-' . $reminder;
   if (!empty($fraud_key) and $api->advance_fraud == '1') {
     $form['hidden_fraud_kit'] = array(
       '#type' => 'hidden',
       '#title' => t('Hidden element.'),
       '#prefix' => '<!-Begin ThreatMetrix profiling tags below -->
          <!- note: replace UNIQUE_SESSION_ID with a uniquely generated handle
          note: for production, replace h.online-metrix.net with a local URL and
          configure your web server to redirect to h.online-metrix.net -->
          <div style="background:url(https://h.online-metrix.net/fp/clear.png?org_id=
          q5ua9sl2&amp;session_id=' . $session_id . '&amp;m=1)"> </div>
          <img src="https://h.online-metrix.net/fp/clear.png?org_id=q5ua9sl2&amp;
          session_id=' . $session_id . '&amp;m=2" />
          <script src="https://h.online-metrix.net/fp/check.js?org_id=q5ua9sl2&amp;
          session_id=' . $session_id . '&amp;pageid=##PAGEID##">
          </script>
          <object type="application/x-shockwave-flash" data="https://h.onlinemetrix.
          net/fp/fp.swf?org_id=q5ua9sl2&amp;session_id=' . $session_id . '" width="1"
          height="1">
          <param name="movie" value="https://h.onlinemetrix.
          net/fp/fp.swf?org_id=q5ua9sl2&amp;session_id=' . $session_id . '" />
          <param name="wmode" value="transparent" />
          <div></div>
          </object>
          <!- End profiling tags -->',
       '#default_value' => $session_id,
       '#value' => $session_id,
     );
   }
  }
  elseif ($payment_method['method_id'] == 'vantiv_echeck') {
    $form['#attached']['css'] = array(
      drupal_get_path('module', 'commerce_vantiv') . '/css/style_vantiv.css',
    );
    $form['accType'] = array(
      '#type' => 'select',
      '#title' => t('Account type') . ' <span class="star_required">*</span>',
      '#options' => array(
        'Checking' => t('Checking'),
        'Savings'  => t('Savings'),
        'Corporate'  => t('Corporate'),
        'Corp Savings'  => t('Corp Savings'),
      ),
      '#states' => array(
        'visible' => array(
          ':input[name$="[cardonfile]"]' => array('value' => 'new'),
        ),
        'invisible' => array(
          ':input[name$="[cardonfile_store_card]"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['accNum'] = array(
      '#type' => 'textfield',
      '#title' => t('Account number') . ' <span class="star_required">*</span>',
      '#default_value' => '',
      '#attributes' => array('autocomplete' => 'off'),
      '#maxlength' => 17,
      '#size' => 17,
      '#states' => array(
        'visible' => array(
          ':input[name$="[cardonfile]"]' => array('value' => 'new'),
        ),
        'invisible' => array(
          ':input[name$="[cardonfile_store_card]"]' => array('checked' => TRUE),
        ),
      ),
    );

    $form['routingNum'] = array(
      '#type' => 'textfield',
      '#title' => t('Routing number') . ' <span class="star_required">*</span>',
      '#default_value' => '',
      '#attributes' => array('autocomplete' => 'off'),
      '#maxlength' => 9,
      '#size' => 9,
    );

    $form['checkNum'] = array(
      '#type' => 'textfield',
      '#title' => t('Check number') . ' <span class="star_required">*</span>',
      '#default_value' => '',
      '#attributes' => array('autocomplete' => 'off'),
      '#maxlength' => 15,
      '#size' => 15,
    );

    $form['credit_card']['cardonfile_store'] = array(
      '#type' => 'checkbox',
      '#title' => t('Store this check on file for further use.'),
      '#default_value' => FALSE,
      '#states' => array(
        'visible' => array(
          ':input[name$="[cardonfile]"]' => array('value' => 'new'),
        ),
        'invisible' => array(
          ':input[name$="commerce_payment[payment_details][credit_card][cardonfile_store]"]' => array('value' => TRUE),
        ),
      ),
    );

      $form['cardonfile_instance_default']['#states'] = array(
        'invisible' => array(
          ':input[name$="[cardonfile]"]' => array('value' => 'new'),
        ),
        'visible' => array(
          ':input[name$="[cardonfile_store_card]"]' => array('checked' => TRUE),
        ),
      );

    if ($cardonfile_capable) {
      $storage = variable_get('commerce_cardonfile_storage', 'opt-in');
      if (in_array($storage, array('opt-in', 'opt-out'))) {
        $form['credit_card']['cardonfile_store'] = array(
          '#type' => 'checkbox',
          '#title' => t('Store this credit card on file for future use.'),
          '#default_value' => $storage == 'opt-out',
        );
        $form['credit_card']['cardonfile_store']['#states'] = array(
          'visible' => array(
            ':input[name$="[cardonfile]"]' => array('value' => 'new'),
          ),
          'invisible' => array(
            ':input[name$="[cardonfile_store_card]"]' => array('!value' => 'new'),
          ),
        );
      }
      else {
        drupal_set_message('test', 'status');
        $form['credit_card']['cardonfile_store'] = array(
          '#type' => 'hidden',
          '#value' => TRUE,
        );
      }

      $form['cardonfile_instance_default'] = array(
        '#type' => 'checkbox',
        '#title' => t('Set as your default card'),
        '#states' => array(
          'invisible' => array(
            ':input[name$="[cardonfile]"]' => array('value' => 'new'),
          ),
          'visible' => array(
            ':input[name$="[cardonfile_store]"]' => array('checked' => TRUE),
          ),
        ),
      );
    }

  }
  if (empty($checkout_pane) and !empty($payment_method)) {
    if ($payment_method['method_id'] == 'vantiv_direct') {
      $form['credit_card']['credit'] = array(
        '#type' => 'checkbox',
        '#title' => t('Credit this card'),
        '#default_value' => FALSE,
      );
    }

    if ($payment_method['method_id'] == 'vantiv_echeck') {
      $form['credit'] = array(
        '#type' => 'checkbox',
        '#title' => t('Credit this echeck account'),
        '#default_value' => FALSE,
      );

     $form['credit_card']['number']['#states'] = array(
      'visible' => array(
        ':input[name$="[cardonfile]"]' => array('value' => 'new'),
      ),
      'invisible' => array(
        ':input[name$="[cardonfile_store]"]' => array('checked' => TRUE),
      ),
    );
    }
    // Remove the require attribute.
    $form['credit_card']['owner']['#required'] = FALSE;
    $form['credit_card']['number']['#required'] = FALSE;
    $form['credit_card']['code']['#required'] = FALSE;
    $form['credit_card']['code']['#title'] = $form['credit_card']['code']['#title'] . ' <span class="star_required">*</span>';
    $form['credit_card']['number']['#title'] = $form['credit_card']['number']['#title'] . ' <span class="star_required">*</span>';
    $form['credit_card']['owner']['#title'] = $form['credit_card']['owner']['#title'] . ' <span class="star_required">*</span>';
    $form['credit_card']['type']['#title'] = $form['credit_card']['type']['#title'] . ' <span class="star_required">*</span>';

    $form = array_merge(commerce_vantiv_load_cof($order->uid, $payment_method), $form);
    $form['credit_card']['exp_month']['#attached']['css'][] = drupal_get_path('module', 'commerce_vantiv') . '/css/style_vantiv.css';
    // Hide the credit card fields.
    $form['credit_card']['number']['#states'] = array(
      'visible' => array(
        ':input[name$="[cardonfile]"]' => array('value' => 'new'),
      ),
      'invisible' => array(
        ':input[name$="[cardonfile_store]"]' => array('checked' => TRUE),
      ),
    );
    $form['credit_card']['owner']['#states'] = array(
      'visible' => array(
        ':input[name$="[cardonfile]"]' => array('value' => 'new'),
      ),
      'invisible' => array(
        ':input[name$="[cardonfile_store]"]' => array('checked' => TRUE),
      ),
    );
    $form['credit_card']['type']['#states'] = array(
      'visible' => array(
        ':input[name$="[cardonfile]"]' => array('value' => 'new'),
      ),
      'invisible' => array(
        ':input[name$="[cardonfile_store]"]' => array('checked' => TRUE),
      ),
    );
     $form['credit_card']['exp_month']['#states'] = array(
      'visible' => array(
        ':input[name$="[cardonfile]"]' => array('value' => 'new'),
      ),
      'invisible' => array(
        ':input[name$="[cardonfile_store]"]' => array('checked' => TRUE),
       // '.commerce-month-year-divider' => array('checked' => TRUE),
      ),
    );
     $form['credit_card']['exp_year']['#states'] = array(
      'visible' => array(
        ':input[name$="[cardonfile]"]' => array('value' => 'new'),
      ),
      'invisible' => array(
        ':input[name$="[cardonfile_store]"]' => array('checked' => TRUE),
      ),
    );
    $form['credit_card']['code']['#states'] = array(
      'visible' => array(
        ':input[name$="[cardonfile]"]' => array('value' => 'new'),
      ),
      'invisible' => array(
        ':input[name$="[cardonfile_store]"]' => array('checked' => TRUE),
      ),
    );

  }

  drupal_alter('commerce_vantiv_submit_form', $form, $payment_method);

  return $form;
}

/**
 * Get all card types.
 */
function commerce_vantiv_direct_all_card_names() {
  return array(
    'visa'    => t('Visa'),
    'mastercard'      => t('Mastercard'),
    'amex'    => t('American Express'),
    'maestro' => t('Maestro'),
    'paypal' => t('PayPal'),
    'jcb' => t('JCB'),
    'bml' => t('Bill Me Later'),
  );
}

/**
 * Validates the cardholder data.
 */
function commerce_vantiv_submit_form_validate($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  if ($payment_method['method_id'] == 'vantiv_direct') {
    // Perform a check in the moto payment page.
    if ($pane_values['cardonfile'] == 'new' and $pane_values['credit_card']['credit'] == 1) {
      module_load_include('inc', 'commerce_payment', 'includes/commerce_payment.credit_card');
      $settings = array(
        'form_parents' => array_merge($form_parents, array('credit_card')),
      );
      commerce_payment_credit_card_validate($pane_values['credit_card'], $settings);
    }

    if (empty($pane_values['credit_card']['number']) and empty($pane_values['cardonfile'])) {
      form_set_error('number');
      return FALSE;
    }
    $customer_profile = commerce_customer_profile_load($order->commerce_customer_billing['und'][0]['profile_id']);
  }
  elseif ($payment_method['method_id'] == 'vantiv_echeck') {
    if (strlen($pane_values['accNum']) < 4 and (!empty($pane_values['cardonfile']) and $pane_values['cardonfile'] == 'new')) {
      form_error($pane_form['accNum'], 'The account number should be longer than 4 characters.');
      return false;
    }
    elseif (strlen($pane_values['routingNum']) < 9 and (!empty($pane_values['cardonfile']) and $pane_values['cardonfile'] == 'new')) {
      form_error($pane_form['routingNum'], 'The account number should be equal to 9 characters.');
      return false;
    }

    // Check if account number, routing number and check number are entered.
    if ((!empty($pane_values['cardonfile']) and $pane_values['cardonfile'] == 'new') and empty($pane_values['accType'])) {
      form_error($pane_values['accType'], 'The account type should not be empty.');
      drupal_set_message(t('Account type is required field'), 'error');
      return FALSE;
    }
    if ((!empty($pane_values['cardonfile']) and $pane_values['cardonfile'] == 'new') and empty($pane_values['accNum'])) {
      form_error($pane_values['accNum'], 'The account number should not be empty.');
      drupal_set_message(t('Account number is required field'), 'error');
      return FALSE;
    }
    if ((!empty($pane_values['cardonfile']) and $pane_values['cardonfile'] == 'new') and empty($pane_values['routingNum'])) {
      form_error($pane_values['routingNum'], 'The routing number should not be empty.');
      drupal_set_message(t('Routing number is required field'), 'error');
      return FALSE;
    }
    if ((!empty($pane_values['cardonfile']) and $pane_values['cardonfile'] == 'new') and empty($pane_values['checkNum'])) {
      form_error($pane_values['checkNum'], 'The check number should not be empty.');
      drupal_set_message(t('Check number is required field'), 'error');
      return FALSE;
    }
  }

}

/**
 * Performs http request.
 */
function commerce_vantiv_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $form_parents = array()) {
  $api = commerce_vantiv_api_object();
  $customer_profile = commerce_customer_profile_load($order->commerce_customer_billing['und'][0]['profile_id']);
  // Direct transaction without using card on file.
  if ($payment_method['method_id'] == 'vantiv_direct') {
    // Check if card should be credit.
    if (!empty($pane_values['credit_card']['credit']) and $pane_values['credit_card']['credit'] == TRUE) {
      if (!empty($form_parents['amount'])) {
        $amount = new stdClass();
        $amount->amount = $form_parents['amount'];
        $amount->currency_code = $form_parents['currency_code'];
      }
      else {
        $amount = '';
      }
      // Credit this card
      $result = $api->directPayments($customer_profile, $order, $pane_values, $payment_method, $amount);
    }

    if(!empty($pane_values['credit_card']['cardonfile_store']) and $pane_values['credit_card']['cardonfile_store'] == TRUE) {
      if (!empty($pane_values['hidden_fraud_kit'])) {
        $result = $api->directPayments($customer_profile, $order, $pane_values, $payment_method, '', '', $pane_values['hidden_fraud_kit']);
      }
      else {
        $result = $api->directPayments($customer_profile, $order, $pane_values, $payment_method);
      }
    }
    elseif(!empty($pane_values['cardonfile']) and is_numeric($pane_values['cardonfile']) == TRUE) {
      // Load the card.
      $card = commerce_cardonfile_load($pane_values['cardonfile']);
      $result = $api->directPayments($customer_profile, $order, $pane_values, $payment_method, '', $card->remote_id);
    }
    else {
      if (!empty($pane_values['hidden_fraud_kit'])) {
        $result = $api->directPayments($customer_profile, $order, $pane_values, $payment_method, '', '', $pane_values['hidden_fraud_kit']);
      }
      elseif (empty($pane_values['credit_card']['credit'])) {
        $result = $api->directPayments($customer_profile, $order, $pane_values, $payment_method);
      }
    }
  }
  elseif ($payment_method['method_id'] == 'vantiv_echeck') {
    // Check if card should be credit.
    if (!empty($pane_values['credit']) and $pane_values['credit'] == TRUE) {
      if (!empty($form_parents['amount'])) {
        $amount = new stdClass();
        $amount->amount = $form_parents['amount'];
        $amount->currency_code = $form_parents['currency_code'];
      }
      else {
        $amount = '';
      }
      // If it is saved echeck account provide the token.
      if (!empty($pane_values['cardonfile']) and is_numeric($pane_values['cardonfile']) == TRUE) {
        $card = commerce_cardonfile_load($pane_values['cardonfile']);
      }

      // Credit this echeck account.
      $result = $api->directPayments($customer_profile, $order, $pane_values, $payment_method, $amount, $card->remote_id, '', TRUE);
    }
    // Saves a new account.
    if(!empty($pane_values['credit_card']['cardonfile_store']) and $pane_values['credit_card']['cardonfile_store'] == TRUE and empty($pane_values['credit'])) {
      if (!empty($pane_values['hidden_fraud_kit'])) {
        $result = $api->directPayments($customer_profile, $order, $pane_values, $payment_method, '', '', $pane_values['hidden_fraud_kit'], TRUE);
      }
      else {
        $result = $api->directPayments($customer_profile, $order, $pane_values, $payment_method, '', '', '', TRUE);
      }
    }
    // Use already saved account.
    elseif(!empty($pane_values['cardonfile']) and is_numeric($pane_values['cardonfile']) == TRUE and empty($pane_values['credit'])) {
      // Load the card.
      $card = commerce_cardonfile_load($pane_values['cardonfile']);
      $result = $api->directPayments($customer_profile, $order, $pane_values, $payment_method, '', $card->remote_id, '', TRUE);
    }
    elseif (empty($pane_values['credit'])) {
      // No saved accounts to be used, just process the transaction.
      if (!empty($pane_values['hidden_fraud_kit'])) {
        $result = $api->directPayments($customer_profile, $order, $pane_values, $payment_method, '', '', $pane_values['hidden_fraud_kit'], TRUE);
      }
      else {
        $result = $api->directPayments($customer_profile, $order, $pane_values, $payment_method, '', '', '', TRUE);
      }
    }

  }
  return $result;
}
