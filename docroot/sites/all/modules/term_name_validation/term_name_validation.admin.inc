<?php

/**
 * @file
 * Term name validation admin file.
 */

/**
 * Implements hook_form().
 */
function term_name_validation_admin_form($form, &$form_state) {

  // Get configuration value.
  $term_name_validation_config = variable_get('term_name_validation_config', array());

  // Get all vocabularies.
  $vocabularies = taxonomy_get_vocabularies();

  // Variable to display 1st fieldset collapse open.
  $i = 0;
  // Generate fieldset for each vocabulary along with exclude, min,
  // max and unique form elements.
  foreach ($vocabularies as $type) {

    // Display First fieldset collapsed open.
    if ($i == 0) {
      $form[$type->machine_name] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($type->name),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
    }
    else {
      $form[$type->machine_name] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($type->name),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
    }
    // Increment $i for other fieldsets in collapsed closed.
    $i++;

    $form[$type->machine_name]['exclude-' . $type->machine_name] = array(
      '#type' => 'textarea',
      '#default_value' => isset($term_name_validation_config['exclude-' . $type->machine_name]) ? $term_name_validation_config['exclude-' . $type->machine_name] : '',
      '#title' => t('Blacklist Characters/Words'),
      '#description' => '<p>' . t("Comma separated characters or words to avoided while saving term names. Ex: !,@,#,$,%,^,&,*,(,),1,2,3,4,5,6,7,8,9,0,have,has,were,aren't.") . '</p>' . '<p>' . t('If any of the blacklisted characters/words found in term name,would return validation error on term save.') . '</p>',
    );

    $form[$type->machine_name]['min-' . $type->machine_name] = array(
      '#type' => 'textfield',
      '#title' => t("Minimum length"),
      '#required' => TRUE,
      '#description' => t("Minimum number of characters term name should contain"),
      '#size' => 12,
      '#maxlength' => 3,
      '#default_value' => isset($term_name_validation_config['min-' . $type->machine_name]) ? $term_name_validation_config['min-' . $type->machine_name] : 1,
    );

    $form[$type->machine_name]['max-' . $type->machine_name] = array(
      '#type' => 'textfield',
      '#title' => t("Maximum length"),
      '#required' => TRUE,
      '#description' => t("Maximum number of characters term name should contain"),
      '#size' => 12,
      '#maxlength' => 3,
      '#default_value' => isset($term_name_validation_config['max-' . $type->machine_name]) ? $term_name_validation_config['max-' . $type->machine_name] : 255,
    );

    $form[$type->machine_name]['min-wc-' . $type->machine_name] = array(
      '#type' => 'textfield',
      '#title' => t("Minimum Word Count"),
      '#required' => TRUE,
      '#description' => t("Minimum number of words Term name should contain"),
      '#size' => 12,
      '#maxlength' => 3,
      '#default_value' => isset($term_name_validation_config['min-wc-' . $type->machine_name]) ? $term_name_validation_config['min-wc-' . $type->machine_name] : 1,
    );

    $form[$type->machine_name]['max-wc-' . $type->machine_name] = array(
      '#type' => 'textfield',
      '#title' => t("Maximum Word Count"),
      '#description' => t("Maximum number of words Term name should contain"),
      '#size' => 12,
      '#maxlength' => 3,
      '#default_value' => isset($term_name_validation_config['max-wc-' . $type->machine_name]) ? $term_name_validation_config['max-wc-' . $type->machine_name] : 25,
    );

    $form[$type->machine_name]['unique-' . $type->machine_name] = array(
      '#type' => 'checkbox',
      '#title' => t("Unique term name for @type vocabulary", array('@type' => $type->machine_name)),
      '#default_value' => isset($term_name_validation_config['unique-' . $type->machine_name]) ? $term_name_validation_config['unique-' . $type->machine_name] : 0,
    );
  }

  $form['unique'] = array(
    '#type' => 'checkbox',
    '#title' => t('Unique term name for all vocabularies'),
    '#default_value' => isset($term_name_validation_config['unique']) ? $term_name_validation_config['unique'] : 0,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
  return $form;
}

/**
 * The validation function for term_name_validation_admin_form form.
 */
function term_name_validation_admin_form_validate($form, &$form_state) {

  // Get all vocabularies.
  $vocabularies = taxonomy_get_vocabularies();

  // Loop for each content type & validate min, max values.
  foreach ($vocabularies as $type) {

    $max = $form_state['values']['max-' . $type->machine_name];

    // Validate field is numerical.
    if (!is_numeric($max)) {
      form_set_error('max-' . $type->machine_name, t("These value should be Numerical"));
    }

    // Term name length should be in between 0 and 256.
    if ($max <= 0 || $max > 255) {
      form_set_error('max-' . $type->machine_name, t("Value should be in between 0 and 256"));
    }

    $min = $form_state['values']['min-' . $type->machine_name];

    // Validate field is numerical.
    if (!is_numeric($min)) {
      form_set_error('min-' . $type->machine_name, t("These value should be Numerical"));
    }

    // Validate field is greater than 1.
    if ($min < 1) {
      form_set_error('min-' . $type->machine_name, t("These value should be more than 1"));
    }

    // Validate min is less than max value.
    if ($min > $max) {
      form_set_error('min-' . $type->machine_name, t("Minimum length should not be more than Max length"));
    }

    // Validate field is numerical.
    $min_wc = $form_state['values']['min-wc-' . $type->machine_name];
    if (!empty($min_wc) && !is_numeric($min_wc)) {
      form_set_error('min-wc-' . $type->machine_name, t("Value should be Numerical"));
    }
    $max_wc = $form_state['values']['max-wc-' . $type->machine_name];
    if (!empty($max_wc) && !is_numeric($max_wc)) {
      form_set_error('max-wc-' . $type->machine_name, t("Value should be Numerical"));
    }

    // Validate min is less than max value.
    if (!empty($min_wc) && !empty($max_wc) && $min_wc > $max_wc) {
      form_set_error('max-wc-' . $type->machine_name, t("Minimum word count of Term Name should not be more than Maximum word count"));
    }
  }
}

/**
 * Implements hook_form_submit().
 */
function term_name_validation_admin_form_submit($form, &$form_state) {

  $values = array();
  // Get all vocabularies.
  $vocabularies = taxonomy_get_vocabularies();

  // Store Form values in term_name_validation_config variable.
  foreach ($vocabularies as $type) {
    $values['exclude-' . $type->machine_name] = $form_state['values']['exclude-' . $type->machine_name];
    $values['min-' . $type->machine_name] = $form_state['values']['min-' . $type->machine_name];
    $values['max-' . $type->machine_name] = $form_state['values']['max-' . $type->machine_name];
    $values['min-wc-' . $type->machine_name] = $form_state['values']['min-wc-' . $type->machine_name];
    $values['max-wc-' . $type->machine_name] = $form_state['values']['max-wc-' . $type->machine_name];
    $values['unique-' . $type->machine_name] = $form_state['values']['unique-' . $type->machine_name];
  }
  $values['unique'] = $form_state['values']['unique'];

  // Set term_name_validation_config variable.
  variable_set('term_name_validation_config', $values);

  drupal_set_message(t('Configurations saved successfully!'));
}
