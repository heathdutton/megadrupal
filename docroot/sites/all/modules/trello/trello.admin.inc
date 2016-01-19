<?php

/**
 * @file
 * Administration pages
 *
 * Handle configuration of system API keys and such.
 */

 /**
  * Admin config form.
  */
function trello_config_form($form, &$form_state) {
  $form['trello'] = array(
    '#markup' => t('Get your API key and OAuth secret') . ' ' . l(t('at the Trello web site'), 'https://trello.com/1/appKey/generate'),
  );
  $form['trello_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('API Key'),
    '#default_value' => variable_get('trello_api_key', FALSE),
    '#required' => TRUE,
  );
  $form['trello_api_secret'] = array(
    '#type' => 'textfield',
    '#title' => t('OAuth Secret'),
    '#default_value' => variable_get('trello_api_secret', FALSE),
    '#required' => TRUE,
  );

  $form = system_settings_form($form);

  $form['postscript'] = array(
    '#markup' => '<br />' . t('After adding these values you need to') . ' ' . l(t('get an auth token'), 'trello/auth'),
  );

  return $form;
}
