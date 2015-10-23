<?php

/**
 * @file
 * Flo2Cash Checkout administration menu items.
 */

/**
 * Form builder for administrative settings form.
 *
 * @ingroup forms
 */
function uc_flo2cash_settings($form, &$form_state) {
  $form['flo2cash']['uc_flo2cash_web2pay_url'] = array(
    '#type' => 'textfield',
    '#title' => t('web2pay URL'),
    '#default_value' => variable_get('uc_flo2cash_web2pay_url', 'https://secure.flo2cash.co.nz/web2pay/default.aspx'),
  );
  $form['flo2cash']['uc_flo2cash_web2pay_verify_url'] = array(
    '#type' => 'textfield',
    '#title' => t('web2pay verification URL'),
    '#default_value' => variable_get('uc_flo2cash_web2pay_verify_url', 'https://secure.flo2cash.co.nz/web2pay/MNSHandler.aspx'),
  );
  $form['flo2cash']['uc_flo2cash_account_id'] = array(
    '#type' => 'textfield',
    '#title' => t('flo2cash account ID'),
    '#default_value' => variable_get('uc_flo2cash_account_id', ''),
  );
  return system_settings_form($form);
}
