<?php

function addressfield_nl_postcode_settings($form, &$form_state) {
  $form['addressfield_nl_postcode_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Postcode.nl Key'),
    '#default_value' => variable_get('addressfield_nl_postcode_key', ''),
    '#description' => t('The Postcode.nl key for your account.'),
  );
  $form['addressfield_nl_postcode_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('Postcode.nl Secret'),
    '#default_value' => variable_get('addressfield_nl_postcode_secret', ''),
    '#description' => t('The Postcode.nl secret for your account.'),
  );

  return system_settings_form($form);
}
