<?php

function pow_captcha_settings_form() {
  $items = array();

  $items['pow_captcha_complexity'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Proof-of-Work Complexity'),
    '#default_value' => variable_get('pow_captcha_complexity', 0.01),
    '#description'   => t('Higher this value - longer user have to wait before submitting form. Good base line value would be 0.01'),
    '#required'      => TRUE,
  );

  $items['pow_captcha_public_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Hashcash.io Public Key'),
    '#default_value' => variable_get('pow_captcha_public_key'),
    '#description'   => t('Public API Key - obtain it at https://hashcash.io/'),
    '#required'      => TRUE,
  );

  $items['pow_captcha_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Secret Key'),
    '#default_value' => variable_get('pow_captcha_key'),
    '#description'   => t('Keep this secret! Obtain it at https://hashcash.io/'),
    '#required'      => TRUE,
  );

  return system_settings_form($items);
}
