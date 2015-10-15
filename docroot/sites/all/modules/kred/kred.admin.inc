<?php

/**
 * @file
 * Kred module admin form.
 *
 * author:
 * Esteban Valerio (estebanvalerioh@gmail.com)
 */

/**
 * Kred configuration form.
 */
function kred_configuration_form() {

  $form['kred'] = array(
    '#type' => 'fieldset',
    '#title' => 'API Configuration',
  );

  $form['kred']['kred_app_id'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => 'Kred app id.',
    '#default_value' => variable_get('kred_app_id', ''),
  );

  $form['kred']['kred_app_key'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#description' => 'Kred app key.',
    '#default_value' => variable_get('kred_app_key', ''),
  );

  $source = array(
    'twitter' => t('Twitter'),
    'facebook' => t('Facebook'),
    'combined' => t('Combined'),
  );

  $form['kred']['kred_source'] = array(
    '#type' => 'radios',
    '#title' => t('Choose a source'),
    '#default_value' => variable_get('kred_source', 'twitter'),
    '#options' => $source,
    '#description' => t("source=twitter will return a user's kred score from Twitter; source=facebook will return kred score for user who has uploaded facebook; source=combined will return the combined kred score for users for both Facebook and Twitter."),
  );

  return system_settings_form($form);
}
