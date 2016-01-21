<?php
/**
 * Admin forms for the notifications CCK signup module.
 */
function _notifications_cck_signup_form_node_type_form_alter(&$form, $form_state) {
  $type = $form['old_type']['#value'];
  if ($type) {
    $form['cck_signup']['notification_cck_signup_interval'] = array(
      '#type' => 'textfield',
      '#size' => 3,
      '#title' => t('Days prior to event to send e-mail notifications'),
      '#description' => t('Set to 0 to disable e-mail notifications.'),
      '#default_value' => variable_get('notification_cck_signup_interval_' . $type, 2),
    );
  }
}
