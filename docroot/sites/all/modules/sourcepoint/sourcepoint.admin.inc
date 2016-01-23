<?php
/**
 * @file
 * Administrative page callbacks for the Sourcepoint module.
 */

/**
 * Implements hook_admin_settings() for module settings configuration.
 */
function sourcepoint_admin_settings_form($form_state) {

  $form['sourcepoint_api_key'] = array(
    '#title' => t('Sourcepoint Api Key'),
    '#type' => 'textfield',
    '#default_value' => variable_get('sourcepoint_api_key', ''),
    '#size' => 70,
    '#required' => TRUE,
  );

  $form['sourcepoint_get_method'] = array(
    '#type' => 'select',
    '#title' => t('Delivery Method'),
    '#options' => array(
      'script' => t('As a tag'),
      // @TODO: Inline script should not be outputted directly onto the page
      // Ideally it should be stored on a local file or a menu item that will
      // return the script added to the header.
      // 'inline' => t('Inline Script'),
    ),
    '#default_value' => variable_get('sourcepoint_get_method', ''),
    '#required' => TRUE,
    '#description' => t('Independent on option - The script will have the async param.'),
  );

  // Hold encoded script.
  $form['sourcepoint_encoded_script'] = array(
    '#title' => t('Sourcepoint Api Key'),
    '#type' => 'hidden',
  );

  return system_settings_form($form);
}

/**
 * Implements hook_form_validate().
 */
function sourcepoint_admin_settings_form_validate($form, &$form_state) {
  $api_key = $form_state['input']['sourcepoint_api_key'];
  $method = $form_state['input']['sourcepoint_get_method'];

  try {
    $data = sourcepoint_request_sourcepoint_script($api_key, $method);
    form_set_value($form['sourcepoint_encoded_script'], $data->encodedData, $form_state);
  } catch (Exception $e) {
    form_set_error('sourcepoint_api_key', t('Invalid sourcepoint data.'));
  }
}



