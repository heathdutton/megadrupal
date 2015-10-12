<?php

/**
 * @file
 * Provides API key for sentiment analysis.
 */

/**
 * Implements hook_form().
 */
function sentiment_analysis_form($form, $form_state) {
  $api_key = sentiment_analysis_get_api();
  $form['get_key'] = array(
    '#markup' => '<a href="https://www.idolondemand.com/login.html">Click Here to get API key.</a>',
  );
  $form['api_key'] = array(
    '#type' => 'textfield',
    '#title' => 'API Key',
    '#default_value' => isset($api_key) ? $api_key : '',
    '#description' => t('API key for sentiment analysis'),
    '#required' => 'TRUE',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Implements hook_form_submit().
 */
function sentiment_analysis_form_submit($form, &$form_state) {
  // To save API key.
  $api_key = $form_state['values']['api_key'];
  db_merge('sentiment_analysis_api')
      ->fields(array(
        'api' => $api_key,
      ))
      ->condition('api_set', 1)
      ->execute();
  drupal_set_message(t('API key successfully updated'));
}
