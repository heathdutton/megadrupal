<?php
/**
 * @file
 * Administrative page callbacks for the cmp module.
 */

/**
 * Implements hook_form().
 */
function cmp_settings_form($form, &$form_state) {
  $desc = array('!link' => l('CaptureMyPage', 'http://capturemypage.com/'));

  $form['cmp_service_key'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Service Key'),
    '#description'   => t('You can get your Service Key by signing up at: !link', $desc),
    '#default_value' => variable_get('cmp_service_key', ''),
    '#required'      => FALSE,
  );

  return system_settings_form($form);
}
