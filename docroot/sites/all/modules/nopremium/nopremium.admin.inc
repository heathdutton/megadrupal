<?php
/**
 * @file
 * Written by Henri MEDOT <henri.medot[AT]absyx[DOT]fr>
 * http://www.absyx.fr
 */

function nopremium_settings_form() {
  $form['message'] = array(
    '#type' => 'fieldset',
    '#title' => t('Premium messages'),
    '#description' => t('You may customize the messages displayed to unprivileged users trying to view full premium contents.'),
  );
  $form['message']['nopremium_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Default message'),
    '#description' => t('This message will apply to all content types with blank messages below.'),
    '#default_value' => nopremium_get_message(),
    '#rows' => 3,
    '#required' => TRUE,
  );
  foreach (node_type_get_names() as $type => $name) {
    $form['message']['nopremium_message_'. $type] = array(
      '#type' => 'textarea',
      '#title' => t('Message for %type content type', array('%type' => $name)),
      '#default_value' => variable_get('nopremium_message_'. $type, ''),
      '#rows' => 3,
    );
  }

  return system_settings_form($form);
}
