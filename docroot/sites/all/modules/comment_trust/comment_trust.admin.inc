<?php

/**
 * Settings form.
 */
function comment_trust_settings() {
  $form['comment_trust_threshold'] = array(
    '#type' => 'select',
    '#title' => t('Threshold'),
    '#description' => t('After an email address has this many published comments, all subsequent comments will be published automatically.'),
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10)),
    '#default_value' => variable_get('comment_trust_threshold', 1),
  );
  $form['comment_trust_exclude'] = array(
    '#type' => 'textarea',
    '#title' => t('Email addresses to never auto-approve'),
    '#description' => t("Enter one email address per line. The '*' character is a wildcard. Examples are <em>nonsense@nonsense.com</em> and <em>*@nonsense.com</em>."),
    '#default_value' => variable_get('comment_trust_exclude', ''),
  );
  return system_settings_form($form);
}
