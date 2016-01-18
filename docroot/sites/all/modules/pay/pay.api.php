<?php

/**
 * @file
 * Hooks provided by the Pay module.
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * This hook is used to inform Pay of available payment method handlers.
 * Administrators will be able to create new instances of payment methods,
 * based on the capabilities and options for your handler(s).
 */
function hook_pay_method_handler_info() {
  return array(
    'pay_method_custom' => array(
      'title' => t('Custom payment'),
      'description' => t('Manual payment entry, for COD payments, pledges, or manually incrementing a total.'),
      'parent' => 'pay_method',
      'module' => 'example',
    ),
    'pay_method_gateway_pieinthesky' => array(
      'title' => 'Pie in the Sky',
      'description' => t('Payment processing using the Pie in the Sky payment gateway'),
      'parent' => 'pay_method_gateway',
      'module' => 'pieinthesky',
      'path' => drupal_get_path('module', 'pieinthesky') .'/includes',
    ),
  );
}

/**
 * This hook is used to inform Pay of available payment form handlers.
 * A payment form is where an actual payment transaction is initiated (e.g.
 * a donation form, checkout page, etc. It minimally includes an 'amount'
 * field, and probably will provide users with the option to complete the
 * transaction by way of one or more payment methods.
 */
function hook_pay_form_handler_info() {
  return array(
    'pay_custom_form' => array(
      'title' => t('Custom pay form'),
      'description' => t('A custom payment form.'),
      'handler' => 'pay_custom_form',
      'path' => drupal_get_path('module', 'pay_custom') .'/includes'
    ),
  );
}

/**
 * This hook is used to grant access to the "Payment settings" tab under 
 * the user edit screen.  The tab will become visible if any module returns
 * TRUE for this hook.
 *
 * Use this function if you are also implementing hook_pay_user_settings_form()
 * and/or you want to add your information to the user's settings page.
 */
function hook_pay_user_settings_access($account) {
  if (user_access('do something interesting with my payment module')) {
    return TRUE;
  }
}

/**
 * Modify or append settings to a user's payment settings page.
 */
function hook_pay_user_settings_form(&$form, &$form_state) {
  $form['mymodule'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for My Module'),
  );
  $form['mymodule']['donation'] = array(
    '#type' => 'checkbox',
    '#title' => t('Donate all profits to the Payment API developers'),
    '#default_value' => 1,
  );
}

/**
 * @} End of "addtogroup hooks".
 */

