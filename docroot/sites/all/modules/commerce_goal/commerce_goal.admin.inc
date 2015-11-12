<?php

/**
 * @file
 * Admin form for Commerce Goal module.
 */

/**
 * Form definition for admin form.
 */
function commerce_goal_admin_form($form, $form_state) {
  $order_statuses = commerce_order_statuses();
  $statuses = array();
  foreach ($order_statuses as $status => $info) {
    $statuses[$status] = $info['title'];
  }

  $form['commerce_goal_order_statuses'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Order statuses that count toward goal totals'),
    '#options' =>$statuses,
    '#default_value' => variable_get('commerce_goal_order_statuses', commerce_goal_get_default_order_statuses()),
  );
  return system_settings_form($form);
}
