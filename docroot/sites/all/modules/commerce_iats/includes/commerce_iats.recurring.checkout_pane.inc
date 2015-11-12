<?php
/**
 * @file
 * Checkout pane callback functions for recurring payments.
 */

/**
 * Checkout pane callback: Recurring payments settings form.
 */
function commerce_iats_recurring_pane_settings_form($checkout_pane) {
  $form = array();

  $form['payment_duration'] = array(
    '#type' => 'textfield',
    '#title' => 'Default end date',
    '#description' => 'Number of days recurring payments should be active for.',
    '#default_value' => variable_get('commerce_iats_recurring_payment_duration',
      COMMERCE_IATS_RECURRING_PAYMENT_DURATION),
  );

  return $form;
}

/**
 * Checkout pane callback: Displays the recurring payment page.
 */
function commerce_iats_recurring_pane_checkout_form($form, &$form_state, $checkout_pane, $order) {
  $pane_form = array();
  $default_values = isset($order->data['recurring']) ?
    $order->data['recurring'] : array();

  $currency_code = $order->commerce_order_total[LANGUAGE_NONE][0]['currency_code'];

  $schedule_type = '';
  if (isset($form_state['values']['checkout_recurring']['schedule_type'])) {
    $schedule_type = $form_state['values']['checkout_recurring']['schedule_type'];
  }
  elseif (isset($default_values['schedule_type'])) {
    $schedule_type = $default_values['schedule_type'];
  }

  $pane_form['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('Amount (@currency)',
    array('@currency' => $currency_code)),
    '#default_value' => isset($default_values['amount']) ? $default_values['amount'] : '',
  );

  $pane_form['schedule_type'] = array(
    '#type' => 'select',
    '#title' => t('Payment schedule type'),
    '#options' => commerce_iats_recurring_pane_schedule_types(),
    '#default_value' => $schedule_type,
    '#ajax' => array(
      'callback' => 'commerce_iats_recurring_pane_schedule_type_callback',
      'method' => 'replace',
      'wrapper' => 'payment-date-wrapper',
    ),
  );

  if (!empty($schedule_type)) {
    $payment_dates = commerce_iats_recurring_pane_schedule_dates($schedule_type);

    $pane_form['schedule_date'] = array(
      '#type' => 'select',
      '#title' => t('Payment date'),
      '#required' => TRUE,
      '#options' => $payment_dates,
      '#default_value' => isset($default_values['schedule_date']) ?
      $default_values['schedule_date'] : 1,
    );
  }
  else {
    $pane_form['schedule_date'] = array();
  }

  $pane_form['schedule_date']['#prefix'] = '<div id="payment-date-wrapper">';
  $pane_form['schedule_date']['#suffix'] = '</div>';

  return $pane_form;
}

/**
 * Checkout pane callback: Validates the recurring payment options.
 */
function commerce_iats_recurring_pane_checkout_form_validate($form, &$form_state) {
  $valid = TRUE;

  $amount = $form_state['values']['checkout_recurring']['amount'];
  $schedule_type = $form_state['values']['checkout_recurring']['schedule_type'];

  if (isset($form_state['values']['checkout_recurring']['schedule_date'])) {
    $schedule_date = $form_state['values']['checkout_recurring']['schedule_date'];
  }

  $valid_schedule_types = commerce_iats_recurring_pane_schedule_types();
  if (!isset($valid_schedule_types[$schedule_type])) {
    form_error($form['checkout_recurring']['schedule_type'], t('Please select a valid schedule type.'));
    $valid = FALSE;
  }

  if (isset($schedule_date)) {
    $valid_schedule_dates = commerce_iats_recurring_pane_schedule_dates($schedule_type);
    if (!isset($valid_schedule_dates[$schedule_date])) {
      form_error($form['checkout_recurring']['schedule_date'], t('Please select a valid schedule date.'));
      $valid = FALSE;
    }
  }

  return $valid;
}

/**
 * Checkout pane callback: Saves the recurring payment state.
 */
function commerce_iats_recurring_pane_checkout_form_submit($form, &$form_state, $checkout_pane, &$order) {
  if (isset($form_state['values']['checkout_recurring'])) {
    $order->data['recurring'] = $form_state['values']['checkout_recurring'];
    $order->data['recurring']['begin_date'] = commerce_iats_get_formatted_date(time());

    $payment_duration = variable_get('commerce_iats_recurring_payment_duration',
      COMMERCE_IATS_RECURRING_PAYMENT_DURATION);
    $end_time = mktime(0, 0, 0, date('n'), date('j') + $payment_duration, date('y'));

    $order->data['recurring']['end_date'] = commerce_iats_get_formatted_date($end_time);
  }
  else {
    $order->data['recurring'] = array();
  }
}

/**
 * Form callback when selecting a schedule type.
 */
function commerce_iats_recurring_pane_schedule_type_callback($form, $form_state) {
  return $form['checkout_recurring']['schedule_date'];
}

/**
 * Returns an associative array of payment schedule types.
 *
 * @return array
 *   Array of recurring schedule types.
 */
function commerce_iats_recurring_pane_schedule_types() {
  $types = array(
    '' => 'None',
    'Weekly' => 'Weekly',
    'Monthly' => 'Monthly',
    'Quarterly' => 'Quarterly',
    'Annually' => 'Annually',
  );
  return $types;
}

/**
 * Returns an associative array of payment schedule dates.
 *
 * @param string $payment_type
 *   The schedule type to return schedule dates for.
 *
 * @return array
 *   Array of schedule dates.
 */
function commerce_iats_recurring_pane_schedule_dates($payment_type) {
  switch ($payment_type) {
    case 'Weekly':
      $payment_dates = array(
        1 => 'Monday',
        2 => 'Tuesday',
        3 => 'Wednesday',
        4 => 'Thursday',
        5 => 'Friday',
        6 => 'Saturday',
        7 => 'Sunday',
      );
      break;

    case 'Monthly':
      // 1st and 15th provided as default options for easier batch processing.
      // Add more dates using hook_form_alter if required.
      $payment_dates = array(
        1 => 1,
        15 => 15,
      );
      break;

    case 'Annually':
      $payment_dates = array(
        1 => 'January',
        2 => 'February',
        3 => 'March',
        4 => 'April',
        5 => 'May',
        6 => 'June',
        7 => 'July',
        8 => 'August',
        9 => 'September',
        10 => 'October',
        11 => 'November',
        12 => 'December',
      );
      break;

    default:
      // Payment types that do not require schedule dates.
      $payment_dates = array(
        '0' => $payment_type,
      );
  }
  return $payment_dates;
}
