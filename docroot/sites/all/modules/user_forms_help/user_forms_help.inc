<?php

/**
 * Form constructor for the user_forms_help form.
 */
function user_forms_help_form($form, &$form_state) {

  // Break out translatable strings per documentation found,
  // here: https://www.drupal.org/node/322774.
  $description = '<p>'
    . t('Enter the message to display to users on password reset message page.')
    . '</p>';
  $description .= '<p>'
    . t('Html is allowed.')
    . '</p>';
  $description .= '<p>'
    . t('You may also enter tokens from the list provided.')
    . '</p>';

  $form['user_forms_help_user'] = array(
    '#type' => 'textfield',
    '#title' => t('Site Admin Representative'),
    '#default_value' => variable_get('user_forms_help_user'),
    '#description' => 'The User to make available for picture token on password reset form.',
    '#required' => TRUE,
    '#autocomplete_path' => 'users/autocomplete',
  );

  $form['user_forms_help_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => variable_get('user_forms_help_message'),
    '#description' => $description,
    '#required' => TRUE,
  );

  $form['tokens'] = array(
    '#theme' => 'token_tree_link',
    '#token_types' => array('node'),
    '#global_types' => TRUE,
    '#click_insert' => TRUE,
  );

  return system_settings_form($form);
}
