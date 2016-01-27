<?php

/**
 * @file
 * Configuration settings page for Skype status module.
 */

/**
 * Configuration form.
 */
function skype_status_admin_settings($form, &$form_state) {
  $form = array();

  $fields = skype_status_get_fields();
  if (empty($fields)) {
    $form['message'] = array(
      '#markup' => t('You need to create at least one textfield for storing the Skype ID!'),
    );
    return $form;
  }
  else {
    $settings = skype_status_get_profile_settings();

    $form['profile'] = array(
      '#type' => 'fieldset',
      '#title' => t('User profiles'),
    );
    $form['profile']['field'] = array(
      '#type' => 'radios',
      '#title' => t('Profile field for Skype ID'),
      '#options' => $fields,
      '#default_value' => $settings['field'],
      '#required' => TRUE,
      '#description' => t("Select the custom profile field that contains user's Skype IDs. Note that users who have not filled out this profile field will be ignored when updating status information from Skype's web presence service API."),
    );
    $form['profile']['title'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show the title for this field'),
      '#description' => t('Uncheck this box to hide the title for the chosen field.'),
      '#default_value' => $settings['title'],
    );

    $form['theming'] = array(
      '#type' => 'fieldset',
      '#title' => t('Skype Theming'),
      '#description' => t('Choose the images and other settings you would like to use for Skype status.'),
    );
    $form['theming']['action'] = array(
      '#type' => 'radios',
      '#title' => t('Action'),
      '#description' => t('What to do when clicking on the Skype status button.'),
      '#options' => array(
        'call' => t('Call'),
        'chat' => t('Chat'),
      ),
      '#default_value' => $settings['action'],
    );
    $form['theming']['style'] = array(
      '#type' => 'radios',
      '#title' => t('Button style preference'),
      '#options' => skype_status_get_styles(),
      '#default_value' => $settings['style'],
    );

    $form['#attached']['js'] = array(
      drupal_get_path('module', 'skype_status') . '/includes/skype_status.admin.js',
    );

    $form['#submit'][] = 'skype_status_admin_settings_submit';

    return system_settings_form($form);
  }
}

function skype_status_admin_settings_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $form_state['values'] = array('skype_status_profile' => $form_state['values']);
}