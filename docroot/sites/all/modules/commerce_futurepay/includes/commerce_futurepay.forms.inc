<?php

/**
 * Alters the add transaction form if the selected payment method is FuturePay.
 * Add the refund button to allow administrators to refund orders.
 */
function commerce_futurepay_add_refund_form($form, &$form_state, $order) {
  // Ensure this include file is loaded when the form is rebuilt from the cache.
  $form_state['build_info']['files']['form'] = drupal_get_path('module', 'commerce_futurepay') . '/includes/commerce_futurepay.forms.inc';
  // Attach asset.
  $form['#attached']['css'][] = drupal_get_path('module', 'commerce_futurepay') . '/theme/commerce_futurepay.css';

  // Store the initial order in the form state.
  $form_state['order'] = $order;
  $form['#access'] = commerce_payment_transaction_order_access('create', $order);

  // If the refund has been pressed. Display the refund terminal.
  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#name'] == 'futurepay-refund-op') {
    $form['refund_payment'] = array(
      '#type' => 'fieldset',
      '#title' => t('Refund terminal: Order ' . $form_state['order']->order_id),
      '#description' => t('Refunds will be processed on accepted orders where the total refunds do no exceed the total value of the order.'),
      '#attributes' => array(
        'class' => array(
          'refund-payment-futurepay',
        ),
      ),
      '#element_validate' => array('commerce_futurepay_add_refund_form_refund_terminal_validate'),
    );

    $total_refund = commerce_futurepay_order_refund_balance($order->order_id);
    $default_amount = entity_metadata_wrapper('commerce_order', $order)->commerce_order_total->amount->value() - $total_refund['amount'];

    $form['refund_payment']['amount'] = array(
      '#type' => 'textfield',
      '#title' => t('Amount'),
      '#description' => t('only float. e.g 12.00'),
      '#value' => commerce_currency_amount_to_decimal($default_amount, $total_refund['currency_code']),
      '#size' => 10,
      '#maxlength' => 6,
      '#required' => TRUE,
      // only working for the USA.
      '#field_suffix' => t('USD')
    );

    $form['refund_payment']['notes'] = array(
      '#type' => 'textarea',
      '#title' => t('Notes'),
      '#description' => t('Used for notes about the return. This will show on the customers statement.'),
      '#default_value' => '',
    );

    $form['refund_payment']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Proceed with Refund'),
      '#name' => 'refund-payment-fp-action',
    );

    $form['refund_payment']['cancel'] = array(
      '#type' => 'markup',
      '#markup' => '<a href="' . $form['#action'] . '">' . t('cancel') . '</a>',
    );
  } else {
    $form['fp_refund_action'] = array(
      '#type' => 'button',
      '#value' => t('Proceed with Refund'),
      '#name' => 'futurepay-refund-op',
      '#ajax' => array(
        'callback' => 'commerce_futurepay_add_refund_form_ajax_refresh',
        'wrapper' => 'commerce-futurepay-add-refund-form',
      ),
    );
  }

  return $form;
}

/**
 * Returns the full refund terminal when the refund button has been pressed.
 */
function commerce_futurepay_add_refund_form_ajax_refresh($form, $form_state) {
  return $form;
}

/**
 * Element validation callback: Validate the refund terminal.
 *
 * Raise errors if found otherwise set the amount value.
 *
 * @param array $element
 *   The element array.
 * @param array $form_state
 *   The form state array.
 */
function commerce_futurepay_add_refund_form_refund_terminal_validate($element, &$form_state) {
  // Check if the entered amount is numeric.
  if (!empty($form_state['input']['amount'])) {
    if (!is_numeric($form_state['input']['amount'])) {
      form_error($element['amount'], t('You must enter a numeric amount value.'));
    }
    else {
      form_set_value($element['amount'], $form_state['input']['amount'], $form_state);
    }
  }

  // Set the notes value.
  form_set_value($element['notes'], $form_state['input']['notes'], $form_state);
}

/**
 * Validation callback: refund payment bloc.
 *
 * Validation of the refund process.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state element.
 */
function commerce_futurepay_add_refund_form_validate($form, &$form_state) {
  if (isset($form_state['triggering_element']) && $form_state['triggering_element']['#name'] == 'futurepay-refund-op') {
    return TRUE;
  }

  $values = $form_state['values'];

  if ((int) $values['amount'] == 0) {
    form_set_error('amount', t('You must enter a value greater than 0.'));
  }

  // Calculating the difference between the entered amount and total refund
  // amount.
  $total_refund = commerce_futurepay_order_refund_balance($form_state['order']->order_id);

  if (commerce_currency_decimal_to_amount($values['amount'], 'USD') > (entity_metadata_wrapper('commerce_order', $form_state['order'])->commerce_order_total->amount->value() - $total_refund['amount'])) {
    form_set_error('amount', t('The entered refund amount is greater than the total refundable.'));
  }
}

/**
 * Submission callback: refund payment bloc.
 *
 * Submission of the refund process.
 *
 * @param array $form
 *   The form array.
 * @param array $form_state
 *   The form state element.
 */
function commerce_futurepay_add_refund_form_submit($form, &$form_state) {
  if ($transaction = commerce_futurepay_api_refund_transaction($form_state['order']->order_id, $form_state['values']['amount'], $form_state['values']['notes'])) {
    if ($transaction->status == COMMERCE_FUTUREPAY_PAYMENT_STATUS_REFUND) {
      drupal_set_message(t('Refund Successful.'), 'status');
    } elseif ($transaction->status == COMMERCE_FUTUREPAY_PAYMENT_STATUS_REFUND_FAILS) {
      drupal_set_message(t('Refund was not accepted by FuturePay services. Please read the message of the transaction line.'), 'warning');
    }
  } else {
    drupal_set_message(t('Something goes wrong during the refund process. Please contact your administrator.'), 'error');
    $form_state['rebuild'] = TRUE;
  }
}
