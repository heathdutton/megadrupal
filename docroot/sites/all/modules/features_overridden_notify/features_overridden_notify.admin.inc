<?php
/**
 * @file
 * Forms for Features admin screens
 */


/**
 * Settings form for features_overridden_notify.
 */
function features_overridden_notify_admin_form($form, $form_state) {
  $form = array();

  $form['features_overridden_notify'] = array(
    '#type' => 'fieldset',
    '#title' => t('Features Overridden Notify'),
    '#description' => t('Set notifications when there are overridden Features (checked on cron).'),
  );

  if (module_exists('rules')) {
    $form['features_overridden_notify']['#description'] .= '<p>' .
    t('You can also send notifications through Rules.  Create a new Rule using the "Features are overridden" event.') .
    '</p>';
  }

  $form['features_overridden_notify']['features_overridden_notify_status_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show status messages.'),
    '#description' => t('Show a status message on every page when there are overridden features.'),
    '#default_value' => variable_get('features_overridden_notify_status_message', TRUE),
  );

  return system_settings_form($form);
}
