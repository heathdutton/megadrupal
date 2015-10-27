<?php

/**
 * @file
 * Implements Cobros en Línea payment callbacks for use with Drupal Commerce.
 */

/**
 * Payment method callback: settings form.
 */
function commerce_cel_settings_form($settings = array()) {
  $form = array();

  // Merge default settings into the stored settings array.
  $settings = (array) $settings + commerce_cel_default_settings();

  $form['account'] = array(
    '#type' => 'textfield',
    '#title' => t('Account'),
    '#default_value' => $settings['account'],
    '#size' => 80,
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['secret_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Secret key'),
    '#description' => t('The alphanumeric secret key as informed by Cobros en Línea.'),
    '#default_value' => $settings['secret_key'],
    '#size' => 80,
    '#maxlength' => 255,
    '#required' => TRUE,
  );
  $form['mode'] = array(
    '#type' => 'radios',
    '#title' => t('Mode'),
    '#options' => array(
      'local' => t('Local'),
      'production' => t('Production'),
    ),
    '#default_value' => $settings['mode'],
    'local' => array(
      '#description' => t('No payment will be processed, the form will redirect to a local page where you can simulate the different responses from Cobros en Línea. Only users with the Use Cobros en Línea in local mode permission will be able to use the method.'),
    ),
    'production' => array(
      '#description' => t('Use this when you are ready to start processing payments through Cobros en Línea.'),
    ),
  );
  // Currently only ARS is allowed, we leave the option here just in case.
  $form['currency'] = array(
    '#type' => 'select',
    '#title' => t('Currency'),
    '#description' => t('Currently Cobros en Línea only allows transactions in ARS'),
    '#default_value' => $settings['currency'],
    '#disabled' => TRUE,
    '#options' => array(
      'ARS' => 'ARS',
    ),
    '#required' => TRUE,
  );

  $form['installments_display'] = array(
    '#type' => 'select',
    '#title' => t('Installments display'),
    '#description' => t('Select if you want to display the final price to the customet in the installments selection box.'),
    '#options' => array(
      '' => t("Don't show price"),
      'total' => t('Show final price'),
      'installments' => t('Show installment amount'),
    ),
    '#default_value' => $settings['installments_display'],
  );

  $methods = commerce_cel_available_methods();
  $form['methods'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Cobros en Línea payment methods'),
    '#description' => t('Select which methods the user will be able to choose when using Cobros en Línea. Remember to set the options below for each enabled method.'),
    '#default_value' => $settings['methods'],
    '#options' => $methods,
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'commerce_cel_ajax_callback',
      'wrapper' => 'cel-methods-settings',
      'method' => 'replace',
    ),
  );

  foreach (array_filter($settings['methods']) as $method) {
    $form['methods_settings'][$method] = commerce_cel_submethod_settings_form($method, $methods[$method], $settings['methods_settings'][$method]);
  }
  $form['methods_settings']['#type'] = 'container';
  $form['methods_settings']['#attributes']['id'] = 'cel-methods-settings';

  return $form;
}

/**
 * Ajax callback for building the payment methods form.
 */
function commerce_cel_ajax_callback($form, &$form_state) {
  $parents = array_slice($form_state['triggering_element']['#array_parents'], 0, -2);
  $settings_form = drupal_array_get_nested_value($form, $parents);
  return $settings_form['methods_settings'];
}

/**
 * Returns the default settings for the Cobros en Línea payment method.
 */
function commerce_cel_default_settings() {
  $settings = array(
    'account' => '',
    'secret_key' => '',
    'mode' => 'local',
    'currency' => 'ARS',
    'installments_display' => '',
    'methods' => array(),
  );

  return $settings;
}

/**
 * Payment method callback: submit form.
 */
function commerce_cel_submit_form($payment_method, $pane_values, $checkout_pane, $order) {
  $form = array();

  $values = array('cel_method' => '', 'installments' => '1');
  if (!empty($pane_values['payment_details']) && is_array($pane_values['payment_details'])) {
    $values = (array) $pane_values['payment_details'] + $values;
  }

  $form['cel_method'] = array(
    '#type' => 'select',
    '#title' => t('Cobros en Línea payment method'),
    '#title_display' => 'invisible',
    '#options' => commerce_cel_enabled_methods($payment_method['settings']),
    '#empty_value' => '',
    '#empty_option' => t('Please select how you want to pay'),
    '#default_value' => $values['cel_method'],
    '#required' => TRUE,
    '#ajax' => array(
      'callback' => 'commerce_cel_submit_form_ajax',
      'wrapper' => 'commerce-cel-installments',
      'method' => 'replace',
    ),
  );

  $form['installments_wrapper'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => 'commerce-cel-installments',
    ),
  );

  if ($values['cel_method'] && $values['cel_method'] != 'imprimo_pago') {
    $form['installments_wrapper']['installments'] = array(
      '#type' => 'select',
      '#title_display' => 'invisible',
      '#title' => t('Installments'),
      '#options' => commerce_cel_enabled_installments($values['cel_method'], $payment_method['settings'], $order),
      '#default_value' => $values['installments'],
      '#required' => TRUE,
    );
  }

  return $form;
}

/**
 * Submit form ajax callback, returns the installments form element.
 */
function commerce_cel_submit_form_ajax($form, &$form_state) {
  return $form['commerce_payment']['payment_details']['installments_wrapper'];
}

/**
 * Payment method callback: submit form validation.
 */
function commerce_cel_submit_form_validate($payment_method, $pane_form, $pane_values, $checkout_pane, $order, $form_parents = array()) {
  $needs_installments = !empty($pane_values['payment_method']) && $pane_values['payment_method'] != 'imprimo_pago';
  if ($needs_installments && empty($pane_values['installments_wrapper']['installments'])) {
    $parents = array_merge($form_parents, array('installments_wrapper', 'installments'));
    form_set_error(implode('][', $parents), t('Please select number of installments.'));
    return FALSE;
  }
}

/**
 * Payment method callback: submit form submit.
 *
 * We are not processing the payment here, just adding data to the order for the
 * redirect form.
 */
function commerce_cel_submit_form_submit($payment_method, $pane_form, $pane_values, $order, $charge) {
  $method = $pane_values['cel_method'];
  $method_code = commerce_cel_cel_method_code($method);
  $data = array();
  if ($method_code !== FALSE) {
    $data['method_code'] = $method_code;
    $data['method'] = $pane_values['cel_method'];
    if (!empty($pane_values['installments_wrapper']['installments'])) {
      $data['installments'] = $pane_values['installments_wrapper']['installments'];
    }
    else {
      $data['installments'] = NULL;
    }

    $settings = $payment_method['settings'];
    if ($method == 'imprimo_pago') {
      $data['promo_code'] = '';
      $data['expiration'] = $settings['methods_settings'][$method]['expiration'];
    }
    else {
      $data['promo_code'] = $settings['methods_settings'][$method]['promo_code'];
      $data['expiration'] = '';
    }

    $order->data['commerce_cel'] = $data;
    return TRUE;
  }

  drupal_set_message(t('An error has occurred, please contact a site administrator.'), 'error');
  return FALSE;
}

/**
 * Payment method callback: redirect form.
 */
function commerce_cel_redirect_form($form, &$form_state, $order, $payment_method) {
  $settings = $payment_method['settings'];
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
  $order_total = $order_wrapper->commerce_order_total->amount->value();
  $order_currency = $order_wrapper->commerce_order_total->currency_code->value();
  $user_name = $order_wrapper->commerce_customer_billing->commerce_customer_address->name_line->value();

  $data['rccpagosUsuario'] = !empty($user_name) ? $user_name : $order_wrapper->mail->value();
  $data['rccpagosEmpresa'] = $settings['account'];
  $data['rccpagosImporte'] = commerce_currency_amount_to_decimal($order_total, $order_currency);
  $data['rccpagosMoneda'] = $settings['currency'];
  $data['rccpagosOrden'] = $order_wrapper->order_number->value();

  $transaction = commerce_cel_transaction($payment_method, $order);
  $base_return_url = 'checkout/' . $order->order_id . '/payment/return/' . $order->data['payment_redirect_key'] . '/' . $transaction->transaction_id . '/###VERIF###';
  $data['rccpagosURLOk'] = url($base_return_url, array('absolute' => TRUE));
  $data['rccpagosURLError'] = url($base_return_url . '/error/###ERROR###', array('absolute' => TRUE));

  $data['rccpagosMedioDePago'] = $order->data['commerce_cel']['method_code'];
  $data['rccpagosVencimiento'] = $order->data['commerce_cel']['expiration'];
  $data['rccpagosCorreoElectronico'] = $order_wrapper->mail->value();
  $data['rccpagosCuotas'] = $order->data['commerce_cel']['installments'];

  if (!empty($order->data['commerce_cel']['promo_code'])) {
    $data['rccpagosPromocion'] = $order->data['commerce_cel']['promo_code'];
  }

  $checksum = commerce_cel_checksum($data, $payment_method);
  $data['rccpagosVerificacion'] = $checksum;
  $order->data['commerce_cel']['checksum'] = $checksum;
  commerce_order_save($order);

  foreach ($data as $field => $value) {
    $form[$field] = array('#type' => 'hidden', '#value' => $value);
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Continue to Cobros en Línea'),
  );

  if ($settings['mode'] == 'local') {
    $form['#action'] = url('commerce_cel/local/' . $order->order_id, array('absolute' => TRUE));
  }
  else {
    $form['#action'] = COMMERCE_CEL_SERVER;
  }

  return $form;
}

/**
 * Payment method callback: redirect form return validation.
 */
function commerce_cel_redirect_form_validate($order, $payment_method) {
  $args = explode('/', current_path());
  $order = menu_get_object('commerce_order', 1);
  $verification = $args[6];

  if (!commerce_cel_valid_verification($order, $payment_method, $verification)) {
    watchdog('commerce_cel',
      'Invalid verification string @verif for order @order, transaction @trans.',
      array(
        '@verif' => $verification,
        '@order' => $args[1],
        '@trans' => $args[5],
      ));
    return FALSE;
  }

  $transaction = commerce_payment_transaction_load($args[5]);
  // Invalid transaction.
  if (!$transaction) {
    watchdog('commerce_cel',
      'Invalid transaction id @trans for order @order.',
      array(
        '@order' => $args[1],
        '@trans' => $args[5],
      ));
    return FALSE;
  }

  // Processed transaction.
  if ($transaction->status != COMMERCE_PAYMENT_STATUS_PENDING) {
    watchdog('commerce_cel',
      'The Cobros en Línea response for the order @order, transaction @trans was already processed (transaction status is @status).',
      array(
        '@order' => $args[1],
        '@trans' => $args[5],
        '@status' => $transaction->status,
      ));
    drupal_set_message(t("Your payment is already being processed, please contact us to confirm the order if you don't receive our confirmation email."), 'status');
    return FALSE;
  }

  // Payment failed.
  if (isset($args[7]) && $args[7] == 'error') {
    $transaction->status = COMMERCE_PAYMENT_STATUS_FAILURE;
    $error_code = $args[8];
    $transaction->remote_status = $error_code;
    $transaction->message = commerce_cel_error_message($error_code);
    $result = FALSE;
  }
  else {
    $transaction->status = COMMERCE_PAYMENT_STATUS_SUCCESS;
    $transaction->remote_status = 0;
    $result = TRUE;
  }
  commerce_payment_transaction_save($transaction);
  return $result;
}

/**
 * Returns available Cobros en Línea payment methods.
 */
function commerce_cel_available_methods() {
  return array(
    'imprimo_pago' => t('imprimoYpago'),
    'cabal' => t('Cabal'),
    'cabal_bcc' => t('Cabal banco Credicoop'),
    'visa' => t('Visa'),
    'visa_bcc' => t('Visa banco Credicoop'),
    'mastercard' => t('Mastercard'),
    'mastercard_bcc' => t('Mastercard banco Credicoop'),
    'amex' => t('American Express'),
    'nativa' => t('Nativa'),
    'naranja' => t('Naranja'),
  );
}

/**
 * Returns enabled Cobros en Línea payment methods.
 *
 * @param array $settings
 *   Payment method settings.
 */
function commerce_cel_enabled_methods($settings) {
  $methods = commerce_cel_available_methods();
  $enabled = array_filter($settings['methods']);
  return array_intersect_key($methods, $enabled);
}

/**
 * Returns the Cobros en Línea internal code for $method.
 *
 * @param string $method
 *   Machine name of the cel payment method.
 */
function commerce_cel_cel_method_code($method) {
  $cel_payment_methods = array(
    'imprimo_pago' => '0',
    'cabal' => '1',
    'cabal_bcc' => '1',
    'visa' => '2',
    'visa_bcc' => '2',
    'mastercard' => '3',
    'mastercard_bcc' => '3',
    'amex' => '4',
    'nativa' => '5',
    'naranja' => '6',
  );
  if (isset($cel_payment_methods[$method])) {
    return $cel_payment_methods[$method];
  }
  return FALSE;
}

/**
 * Returns enabled installments for $method in Cobros en Línea.
 *
 * @param string $method
 *   Cobros en línea payment method.
 * @param array $settings
 *   Payment method settings.
 * @param object $order
 *   The order being payed.
 */
function commerce_cel_enabled_installments($method, $settings, $order) {
  $installments = array();

  if (!empty($settings['methods_settings'][$method]['installments'])) {
    $order_wrapper = entity_metadata_wrapper('commerce_order', $order);
    $order_total = $order_wrapper->commerce_order_total->amount->value();
    $order_currency = $order_wrapper->commerce_order_total->currency_code->value();

    foreach ($settings['methods_settings'][$method]['installments'] as $n => $options) {
      if ($options['enabled']) {
        $interest = floatval($options['interest']);
        switch ($settings['installments_display']) {
          case 'total':
            $final_price = commerce_round(COMMERCE_ROUND_HALF_UP, $order_total * $interest);
            $message = format_plural($n,
              '1 installment (@price)',
              '@count installments (@price)',
              array(
                '@price' => commerce_currency_format($final_price, $order_currency),
              )
            );
            break;

          case 'installments':
            $installment_amount = commerce_round(COMMERCE_ROUND_HALF_UP, ($order_total * $interest) / $n);
            $message = format_plural($n,
              '1 installment of @amount',
              '@count installments of @amount',
              array(
                '@amount' => commerce_currency_format($installment_amount, $order_currency),
              )
            );
            break;

          default:
            $message = format_plural($n, '1 installment', '@count installments');
            break;
        }

        $installments[$n] = $message;
      }
    }
  }

  return $installments;
}

/**
 * Returns the subform for configuring each available Cobros en Línea method.
 */
function commerce_cel_submethod_settings_form($method, $title, $settings) {
  $fieldset = array();

  if ($method == 'imprimo_pago') {
    $fieldset = array(
      '#type' => 'fieldset',
      '#title' => $title,
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    for ($i = 1; $i <= 31; ++$i) {
      $days[$i] = format_plural($i, '@count day', '@count days');
    }
    $fieldset['expiration'] = array(
      '#type' => 'select',
      '#title' => t('Valid time (in days)'),
      '#description' => t('The time in days your customers can make the payment since finishing the order. Please verify the valid options with Cobros en Línea.'),
      '#options' => $days,
      '#default_value' => $settings['expiration'],
    );
  }
  else {
    $fieldset = array(
      '#type' => 'fieldset',
      '#title' => $title,
      '#tree' => TRUE,
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $fieldset['promo_code'] = array(
      '#type' => 'textfield',
      '#title' => t('Promo code for @card', array('@card' => $title)),
      '#description' => t('If you have a promotion code for this card enter it here.'),
      '#default_value' => $settings['promo_code'],
      '#size' => 20,
    );
    $installments = array(
      '1' => '1',
      '3' => '3',
      '6' => '6',
      '9' => '9',
      '12' => '12',
      '18' => '18',
      '24' => '24',
    );
    $fieldset['installments'] = array(
      '#type' => 'fieldset',
      '#title' => t('Installments'),
      '#tree' => TRUE,
      '#description' => t('Select available installments for @card and set the interest to apply. The interest should be informed by @card and should be between 1.0 and 2.0.', array('@card' => $title)),
    );
    foreach ($installments as $n) {
      $fieldset['installments'][$n] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('container-inline')),
      );
      $fieldset['installments'][$n]['enabled'] = array(
        '#type' => 'checkbox',
        '#title' => $n,
        '#default_value' => $settings['installments'][$n]['enabled'],
        // We set the name of the field so we can use conditional states
        // regardless of the parents of the form.
        '#name' => $method . '_installments_' . $n,
        // Custom value callback, since we change the name of the field.
        '#value_callback' => 'commerce_cel_installments_enabled_value',
      );
      $fieldset['installments'][$n]['interest'] = array(
        '#type' => 'textfield',
        '#title' => t('Interest'),
        '#title_display' => 'invisible',
        '#field_suffix' => 'interest',
        '#default_value' => !empty($settings['installments'][$n]['interest']) ? $settings['installments'][$n]['interest'] : '1.0',
        '#size' => 4,
        '#states' => array(
          'disabled' => array(
            ':input[name="' . $method . '_installments_' . $n . '"]' => array('checked' => FALSE),
          ),
        ),
      );
    }
  }

  return $fieldset;
}

/**
 * Returns the value of the installments enabled checkbox.
 */
function commerce_cel_installments_enabled_value($element, $input = FALSE, $form_state = array()) {
  if ($input !== FALSE) {
    $name = $element['#name'];
    if (isset($form_state['input'][$name])) {
      return $form_state['input'][$name];
    }
  }
  if (!empty($element['#default_value'])) {
    return $element['#default_value'];
  }
}

/**
 * Creates a new transaction for $order.
 *
 * @param array $payment_method
 *   The payment method instance object used to charge this payment.
 * @param object $order
 *   The order object the payment applies to.
 */
function commerce_cel_transaction($payment_method, $order) {
  $order_wrapper = entity_metadata_wrapper('commerce_order', $order);

  $transaction = commerce_payment_transaction_new('commerce_cel', $order->order_id);
  $transaction->instance_id = $payment_method['instance_id'];
  $transaction->amount = $order_wrapper->commerce_order_total->amount->value();
  $transaction->currency_code = $order_wrapper->commerce_order_total->currency_code->value();
  $transaction->status = COMMERCE_PAYMENT_STATUS_PENDING;

  $payment_details = $order->data['commerce_cel'];
  $transaction->data['method'] = $payment_details['method'];
  $transaction->data['installments'] = $payment_details['installments'];
  $transaction->data['expiration'] = $payment_details['expiration'];
  $transaction->data['promo_code'] = $payment_details['promo_code'];

  if ($payment_details['method'] == 'imprimo_pago') {
    $transaction->message = 'Cobros en Línea payment method: @method';
    $transaction->message_variables = array(
      '@method' => $payment_details['method'],
    );
  }
  else {
    $transaction->message = 'Cobros en Línea payment method: @method<br />Installments: @installments';
    $transaction->message_variables = array(
      '@method' => $payment_details['method'],
      '@installments' => $payment_details['installments'],
    );
  }

  commerce_payment_transaction_save($transaction);
  return $transaction;
}
