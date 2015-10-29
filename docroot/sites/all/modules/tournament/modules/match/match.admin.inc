<?php

/**
 * @file
 * Admin page callbacks for the match module.
 *
 */

/**
 * Form builder; Configure the match settings.
 *
 * @see system_settings_form()
 */
function match_settings_form($form, &$form_state) {
  $info = entity_get_info();

  $options = array();
  foreach ($info as $type => $entity_info) {
    $options[$type] = $entity_info['label'];
  }

  $form['match_entity_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Entity types'),
    '#description' => t('Select which entity can be match participants.'),
    '#options' => $options,
    '#default_value' => variable_get('match_entity_types', array('user')),
  );
  $form['match_user_challenging'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow challenging between users'),
    '#default_value' => variable_get('match_user_challenging', 1),
    '#states' => array(
      'visible' => array(
        ':input[name="match_entity_types[user]"]' => array('checked' => TRUE),
      ),
    ),
  );
  $form['match_user_picture'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show user pictures on user matches'),
    '#default_value' => variable_get('match_user_picture', 0),
    '#states' => array(
      'visible' => array(
        ':input[name="match_entity_types[user]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['match_mail_user'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to disable match email notifications'),
    '#default_value' => variable_get('match_mail_user', 1),
  );

  return system_settings_form($form);
}
