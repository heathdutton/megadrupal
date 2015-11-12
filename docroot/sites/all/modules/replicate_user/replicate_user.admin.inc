<?php

/**
 * @file
 * Provides administrative interface for replicate user.
 */

/**
 * Form callback for replicate_user settings.
 */
function replicate_user_settings_form() {
  $form = array();

  $form['replicate_user_username_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Username'),
    '#collapsible' => TRUE,
  );

  $form['replicate_user_username_fieldset']['replicate_user_username'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update the username'),
    '#description' => t('Update the username of the replicated user.'),
    '#default_value' => 1,
    '#disabled' => TRUE,
  );

  $form['replicate_user_username_fieldset']['replicate_user_username_method'] = array(
    '#type' => 'select',
    '#title' => t('Method'),
    '#options' => array(
      REPLICATE_USER_PREFIX => t('Add a prefix'),
      REPLICATE_USER_SUFFIX => t('Add a suffix'),
      REPLICATE_USER_RANDOM => t('Random'),
    ),
    '#default_value' => variable_get('replicate_user_username_method', REPLICATE_USER_SUFFIX),
  );

  $form['replicate_user_username_fieldset']['replicate_user_username_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#description' => t('This text will be added before the username. % may be used as a placeholder for an incrementing numeric value (1_username, 2_username).'),
    '#default_value' => variable_get('replicate_user_username_prefix', '%_'),
    '#states' => array(
      'visible' => array(
        ':input[name="replicate_user_username_method"]' => array('value' => REPLICATE_USER_PREFIX),
      ),
      'required' => array(
        ':input[name="replicate_user_username_method"]' => array('value' => REPLICATE_USER_PREFIX),
      ),
    ),
    '#element_validate' => array('replicate_user_element_validate_placeholder'),
  );

  $form['replicate_user_username_fieldset']['replicate_user_username_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Suffix'),
    '#description' => t('This text will be added after the username. % may be used as a placeholder for an incrementing numeric value (username_1, username_2).'),
    '#default_value' => variable_get('replicate_user_username_suffix', '_%'),
    '#states' => array(
      'visible' => array(
        ':input[name="replicate_user_username_method"]' => array('value' => REPLICATE_USER_SUFFIX),
      ),
      'required' => array(
        ':input[name="replicate_user_username_method"]' => array('value' => REPLICATE_USER_SUFFIX),
      ),
    ),
    '#element_validate' => array('replicate_user_element_validate_placeholder'),
  );

  $form['replicate_user_username_fieldset']['replicate_user_username_random_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Length'),
    '#description' => t('Select the length of the generated username.'),
    '#default_value' => variable_get('replicate_user_username_random_length', 7),
    '#size' => 1,
    '#states' => array(
      'visible' => array(
        ':input[name="replicate_user_username_method"]' => array('value' => REPLICATE_USER_RANDOM),
      ),
      'required' => array(
        ':input[name="replicate_user_username_method"]' => array('value' => REPLICATE_USER_RANDOM),
      ),
    ),
    '#element_validate' => array('element_validate_integer_positive'),
  );

  $form['replicate_user_username_fieldset']['replicate_user_username_random_characters'] = array(
    '#type' => 'textfield',
    '#title' => t('Characters'),
    '#description' => t('Fill the list of allowed characters in the random username.'),
    '#default_value' => variable_get('replicate_user_username_random_characters', 'abcdefghijklmnopqrstuvwxyz0123456789'),
    '#states' => array(
      'visible' => array(
        ':input[name="replicate_user_username_method"]' => array('value' => REPLICATE_USER_RANDOM),
      ),
      'required' => array(
        ':input[name="replicate_user_username_method"]' => array('value' => REPLICATE_USER_RANDOM),
      ),
    ),
  );

  $form['replicate_user_password_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Password'),
    '#collapsible' => TRUE,
  );

  $form['replicate_user_password_fieldset']['replicate_user_password'] = array(
    '#type' => 'checkbox',
    '#title' => t('Generate a random password'),
    '#description' => t('Generate a new random password for the replicated user using the Drupal core function.'),
    '#default_value' => variable_get('replicate_user_password', 0),
  );

  $form['replicate_user_mail_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email'),
    '#collapsible' => TRUE,
  );

  $form['replicate_user_mail_fieldset']['replicate_user_mail'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update the email address'),
    '#description' => t('Update the email address of the replicated user.'),
    '#default_value' => 1,
    '#disabled' => TRUE,
  );

  $form['replicate_user_mail_fieldset']['replicate_user_mail_method'] = array(
    '#type' => 'select',
    '#title' => t('Method'),
    '#options' => array(
      REPLICATE_USER_PREFIX => t('Add a prefix'),
      REPLICATE_USER_SUFFIX => t('Add a suffix'),
    ),
    '#default_value' => variable_get('replicate_user_mail_method', REPLICATE_USER_SUFFIX),
    '#states' => array(
      'visible' => array(
        ':input[name="replicate_user_mail"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['replicate_user_mail_fieldset']['replicate_user_mail_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Prefix'),
    '#description' => t('This text will be added before the email address. % may be used as a placeholder for an incrementing numeric value (1_mail@domain.tld, 2_mail@domain.tld).'),
    '#default_value' => variable_get('replicate_user_mail_prefix', '%_'),
    '#states' => array(
      'visible' => array(
        ':input[name="replicate_user_mail"]' => array('checked' => TRUE),
        ':input[name="replicate_user_mail_method"]' => array('value' => REPLICATE_USER_PREFIX),
      ),
      'required' => array(
        ':input[name="replicate_user_mail_method"]' => array('value' => REPLICATE_USER_PREFIX),
      ),
    ),
    '#element_validate' => array('replicate_user_element_validate_placeholder'),
  );

  $form['replicate_user_mail_fieldset']['replicate_user_mail_suffix'] = array(
    '#type' => 'textfield',
    '#title' => t('Suffix'),
    '#description' => t('This text will be added after the email address. % may be used as a placeholder for an incrementing numeric value (email_1@domain.tld, email_2@domain.tld).'),
    '#default_value' => variable_get('replicate_user_mail_suffix', '_%'),
    '#states' => array(
      'visible' => array(
        ':input[name="replicate_user_mail"]' => array('checked' => TRUE),
        ':input[name="replicate_user_mail_method"]' => array('value' => REPLICATE_USER_SUFFIX),
      ),
      'required' => array(
        ':input[name="replicate_user_mail_method"]' => array('value' => REPLICATE_USER_SUFFIX),
      ),
    ),
    '#element_validate' => array('replicate_user_element_validate_placeholder'),
  );

  return system_settings_form($form);
}

/**
 * Validates the % placeholder exists in a form field.
 *
 * @param array $element
 *   Form element to validate.
 */
function replicate_user_element_validate_placeholder($element) {
  if (strpos($element['#value'], '%') === FALSE) {
    form_set_error($element['#name'], t('The placeholder % is required to generate a unique value.'));
  }
}
