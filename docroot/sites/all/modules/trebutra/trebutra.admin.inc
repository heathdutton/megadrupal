<?php

/**
 * @file
 * Administration functions for the Trebutra module.
 */

/**
 * Form constructor for the admin settings form.
 */
function trebutra_settings_form($form, &$form_state) {
  $form['trebutra_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Trello API Key'),
    '#default_value' => variable_get('trebutra_key', ''),
    '#description' => t('Obtained via the Trello API page'),
    '#required' => TRUE,
  );
  $form['trebutra_apiend'] = array(
    '#type' => 'textfield',
    '#title' => t('Trello API endpoint'),
    '#default_value' => variable_get('trebutra_apiend', 'https://api.trello.com/1'),
    '#description' => t('Usually https://api.trello.com/1'),
    '#required' => TRUE,
  );
  $form['trebutra_listid'] = array(
    '#type' => 'textfield',
    '#title' => t('Trello List ID'),
    '#default_value' => variable_get('trebutra_listid', ''),
    '#description' => t('The list ID you want cards to go to on Trello. Go to Export on Trello and click JSON and look for idList and enter that value here.'),
    '#required' => TRUE,
  );
  $form['trebutra_token'] = array(
    '#type' => 'textfield',
    '#title' => t('Trello API token'),
    '#default_value' => variable_get('trebutra_token', ''),
    '#description' => t('Get a token by substituting your API key and a made-up app name into this URL (https://trello.com/1/connect?key=apikeygoeshere&name=appnamegoeshere&response_type=token&scope=read,write&expiration=never) and visiting it in your browser.'),
    '#required' => TRUE,
  );
  $form['trebutra_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Report recipient email'),
    '#default_value' => variable_get('trebutra_email', ''),
    '#description' => t('If you would like reports, enter an email here and tick the box below'),
  );
  $form['trebutra_emailsend'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable report emails'),
    '#default_value' => variable_get('trebutra_emailsend', 1),
  );
  return system_settings_form($form);
}
