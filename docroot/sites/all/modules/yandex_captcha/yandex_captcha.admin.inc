<?php

/**
 * @file
 * Provides the Yandex.Captcha administration settings.
 */

/**
 * Form callback; administrative settings for Yandex.Captcha.
 */
function yandex_captcha_admin_settings() {
  $form = array();

  $form['yandex_captcha_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => variable_get('yandex_captcha_key', NULL),
    '#maxlength' => 200,
    '#description' => t('The key given to you when you register on Yandex services and get it in !url.', array(
      '!url' => l(t('API documentation area'), 'http://api.yandex.ru/key/form.xml?service=cw', array('attributes' => array('target' => '_blank'))),
    )),
    '#required' => TRUE,
  );

  $form['yandex_captcha_type'] = array(
    '#type' => 'select',
    '#title' => t('Type of Yandex.Captcha'),
    '#default_value' => variable_get('yandex_captcha_type', 'std'),
    '#description' => t('Select type of Yandex.CleanWeb captcha.'),
    '#options' => array(
      'std'   => t('Numeric, russian logo'),
      'estd'  => t('Numeric, english logo'),
      'lite'  => t('Easy-to-read, numeric, russian logo'),
      'elite' => t('Easy-to-read, numeric, english logo'),
      'rus'   => t('Russian letters, russian logo'),
      'latl'  => t('Latin lowercase letters, russian logo'),
      'elatl' => t('Latin lowercase letters, english logo'),
      'latu'  => t('Latin capital letters, russian logo'),
      'elatu' => t('Latin capital letters, english logo'),
      'latm'  => t('Latin mixed case letters, russian logo'),
      'elatm' => t('Latin mixed case letters, english logo'),
    ),
  );

  $form['yandex_captcha_refresh_image'] = array(
    '#type' => 'checkbox',
    '#title' => t('Refresh link'),
    '#default_value' => variable_get('yandex_captcha_refresh_image', FALSE),
    '#description' => t('Add refrech captcha link.'),
  );

  return system_settings_form($form);
}
