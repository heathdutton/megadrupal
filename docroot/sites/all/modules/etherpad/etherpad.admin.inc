<?php
/**
 * @file
 * Administrative settings for Etherpad module.
 */

/**
 * Form builder for administrative settings.
 */
function etherpad_settings_form($form, &$form_state) {
  $form['etherpad_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Etherpad API URL'),
    '#default_value' => variable_get('etherpad_base_url', 'http://localhost:9001'),
  );
  $form['etherpad_api_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Etherpad API Key'),
    '#descrition' => t('Paste contents of APIKEY.TXT file.'),
    '#default_value' => variable_get('etherpad_api_key', ''),
  );
  $form['test'] = array(
    '#type' => 'button',
    '#value' => t('Test'),
    '#ajax' => array(
      'callback' => 'etherpad_lite_test_connection',
      'wrapper' => 'result_div',
    ),
  );
  $form['test_result'] = array(
    '#prefix' => '<div id="result_div">',
    '#suffix' => '</div>',
  );

  return system_settings_form($form);
}

/**
 * AJAX callback for testing connection.
 */
function etherpad_lite_test_connection($form, $form_state) {
  $values = $form_state['values'];
  $path = libraries_get_path('etherpad-lite-client');
  if (empty($path)) {
    drupal_set_message(t('Etherpad light client library not found.'), 'error');
  }
  else {
    try {
      require_once($path . '/etherpad-lite-client.php');
      $client = new EtherpadLiteClient($values['etherpad_api_key'], $values['etherpad_base_url'] . '/api');
      $group = $client->createGroup();
      if ($group->groupID) {
        $client->deleteGroup($group->groupID);
        drupal_set_message(t('Etherpad client works'));
      }
    }
    catch (Exception $e) {
      drupal_set_message(t('Etherpad client error: @msg', array('@msg' => $e->getMessage())), 'error');
    }
  }
  return $form['test_result'];
}
