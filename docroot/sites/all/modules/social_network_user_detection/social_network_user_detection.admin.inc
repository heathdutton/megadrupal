<?php
/**
 * @file
 * The admin form.
 */

/**
 * The administration form.
 * @return Array
 *   of the admin settings form.
 */
function social_network_user_detection_settings() {
  $form['social_network_user_detection'] = array(
    '#type' => 'fieldset',
    '#title' => t('Social network settings'),
  );
  $form['social_network_user_detection']['social_network_user_detection_fb_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('The Facebook APP ID for the social network detection.'),
    '#description' => t('FB App ID for social network detection.'),
    '#required' => FALSE,
    '#default_value' => variable_get('social_network_user_detection_fb_app_id', ''),
  );
  $form['social_network_user_detection']['social_network_user_detection_do_not_use_fb_init'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check this if this module does not need to add the FB init because this has already been done by another module.'),
    '#description' => t('This can be added by "social connect" for example or the FB module.'),
    '#default_value' => variable_get('social_network_user_detection_do_not_use_fb_init', ''),
  );
  $form['social_network_user_detection']['social_network_user_detection_hyves_app_id'] = array(
    '#type' => 'textfield',
    '#title' => t('The Hyves APP ID for the social network detection.'),
    '#description' => t('Hyves App ID for social network detection.'),
    '#required' => FALSE,
    '#default_value' => variable_get('social_network_user_detection_hyves_app_id', ''),
  );
  $form['social_network_user_detection']['social_network_user_detection_do_not_use_hyves_init'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check this if this module does not need to add the Hyves init because this has already been done by another module.'),
    '#description' => t('This can be added by "social connect" for example or the FB module.'),
    '#default_value' => variable_get('social_network_user_detection_do_not_use_hyves_init', ''),
  );
  $form['social_network_user_detection']['social_network_user_detection_mode'] = array(
    '#type' => 'select',
    '#title' => t('Social network detection mode'),
    '#options' => array(
      'async' => t('Asynchronous'),
      'old' => t('Old version'),
    ),
    '#default_value' => variable_get('social_network_user_detection_mode', 'async'),
    '#description' => t('Set this to the desired network detection mode.'),
  );

  return system_settings_form($form);
}
