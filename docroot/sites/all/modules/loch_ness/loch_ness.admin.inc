<?php

function loch_ness_admin_settings($form, $form_state) {
  $options = drupal_map_assoc(array(1, 2, 3, 5, 8, 13, 21, 34, 55));
  $form['loch_ness_timeout'] = array(
    '#type' => 'select',
    '#title' => t('Lock Lifespan'),
    '#description' => t('This is how many mintues until the system will break the lock.'),
    '#default_value' => variable_get('loch_ness_timeout', 21),
    '#options' => $options,
  );

  return system_settings_form($form);
}