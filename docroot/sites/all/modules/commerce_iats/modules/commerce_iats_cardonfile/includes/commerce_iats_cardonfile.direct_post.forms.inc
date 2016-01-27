<?php
/**
 * @file
 * Forms for use in Direct Post Method transactions.
 */

function commerce_iats_cardonfile_direct_post_credit_card_customer_code_form($payment_method, $order) {
  $form = commerce_iats_cardonfile_direct_post_customer_code_form($payment_method, $order, 'commerce_iats_direct_post_credit_card_form');

  return $form;
}

function commerce_iats_cardonfile_direct_post_ach_eft_customer_code_form($payment_method, $order) {
  $form = commerce_iats_cardonfile_direct_post_customer_code_form($payment_method, $order, 'commerce_iats_direct_post_ach_eft_form');

  return $form;
}

function commerce_iats_cardonfile_direct_post_customer_code_form($payment_method, $order, $form_method) {
  $form = $form_method($payment_method, $order);

  // Creating token set to TRUE.
  $form['iats_dpm_creating_token'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_CreatingToken',
    '#default_value' => 'TRUE',
  );

  $recurring = FALSE;
  if (isset($payment_method['settings']['allow_recurring'])
    && ($payment_method['settings']['allow_recurring'] == 1)
    && isset($order->data['recurring'])) {
    $recurring = TRUE;
  }

  // Recurring set to TRUE or FALSE.
  $form['iats_dpm_recurring_on'] = array(
    '#type' => 'hidden',
    '#name' => 'IATS_DPM_RecurringOn',
    '#default_value' => ($recurring) ? 'TRUE' : 'FALSE',
  );

  if ($recurring) {

    // Begin date.
    $form['iats_dpm_begin_date'] = array(
      '#type' => 'hidden',
      '#name' => 'IATS_DPM_BeginDate',
      '#default_value' => commerce_iats_direct_post_get_formatted_date(strtotime($order->data['recurring']['begin_date'])),
    );

    // End date.
    $form['iats_dpm_end_date'] = array(
      '#type' => 'hidden',
      '#name' => 'IATS_DPM_EndDate',
      '#default_value' => commerce_iats_direct_post_get_formatted_date(strtotime($order->data['recurring']['end_date'])),
    );

    // Schedule type.
    $form['iats_dpm_schedule_type'] = array(
      '#type' => 'hidden',
      '#name' => 'IATS_DPM_ScheduleType',
      '#default_value' => strtoupper($order->data['recurring']['schedule_type']),
    );

    if ($order->data['recurring']['schedule_type'] == 'Annually') {
      // Schedule month is similar to schedule date, but for annual payments.
      $form['iats_dpm_schedule_month'] = array(
        '#type' => 'hidden',
        '#name' => 'IATS_DPM_ScheduleMonth',
        '#default_value' => $order->data['recurring']['schedule_date'],
      );
    }
    else {
      // Schedule date.
      $form['iats_dpm_schedule_date'] = array(
        '#type' => 'hidden',
        '#name' => 'IATS_DPM_ScheduleDate',
        '#default_value' => $order->data['recurring']['schedule_date'],
      );
    }

  }

  return $form;
}

/**
 * Converts a UNIX timestamp to a date string formatted for the iATS API.
 *
 * @param int $timestamp
 *   The timestamp to convert.
 *
 * @return string
 *   The formatted date string.
 */
function commerce_iats_direct_post_get_formatted_date($timestamp) {
  return date('m/d/Y', $timestamp);
}
