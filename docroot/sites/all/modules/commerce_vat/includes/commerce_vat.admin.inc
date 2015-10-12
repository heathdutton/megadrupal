<?php

/**
 * @file
 * Contains admin menu callbacks for the Commerce VAT module.
 */

/**
 * Settings form callback.
 */
function commerce_vat_settings_form($form, &$form_state) {
  $help = t('Select the calculation direction for VAT') . '<br />';
  $help .= t('Forward calculation is the default and means you enter prices excluding VAT. Revese calculation meens you enter the price the customer pays, this may or may not include VAT.');
  $form['help'] = array(
    '#markup' => $help,
  );

  $options = array(
  	'forward' => 'Forward',
    'reverse' => 'Reverse',
  );
  $form['commerce_vat_direction'] = array(
    '#title' => t('Calculation Direction'),
    '#type' => 'select',
    '#default_value' => variable_get('commerce_vat_direction', 'forward'),
    '#options' => $options,
  );

  $form = system_settings_form($form);
  return $form;
}
