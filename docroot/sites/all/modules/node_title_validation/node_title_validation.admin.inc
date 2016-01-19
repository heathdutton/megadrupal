<?php

/**
 * @file
 * Node title validation admin file.
 */

/**
 * Implements hook_form().
 */
function node_title_validation_admin_form($form, &$form_state) {

  // Get configuration value.
  $node_title_validation_config = variable_get('node_title_validation_config', array());

  // Get available content types.
  $node_types = node_type_get_types();

  // Variable to display 1st fieldset collapse open.
  $i = 0;
  // Generate fieldset for each content type along with exclude, min,
  // max and unique form elements.
  foreach ($node_types as $type) {

    // Display First fieldset collapsed open.
    if ($i == 0) {
      $form[$type->type] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($type->name),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
    }
    else {
      $form[$type->type] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($type->name),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );
    }
    // Increment $i for other fieldsets in collapsed closed.
    $i++;

    $form[$type->type]['exclude-' . $type->type] = array(
      '#type' => 'textarea',
      '#default_value' => isset($node_title_validation_config['exclude-' . $type->type]) ? $node_title_validation_config['exclude-' . $type->type] : '',
      '#title' => t('Blacklist Characters/Words'),
      '#description' => '<p>' . t("Comma separated characters or words to avoided while saving node title. Ex: !,@,#,$,%,^,&,*,(,),1,2,3,4,5,6,7,8,9,0,have,has,were,aren't.") . '</p>' . '<p>' . t('If any of the blacklisted characters/words found in node title,would return validation error on node save.') . '</p>',
    );

    $form[$type->type]['min-' . $type->type] = array(
      '#type' => 'textfield',
      '#title' => t("Minimum characters"),
      '#required' => TRUE,
      '#description' => t("Minimum number of characters node title should contain"),
      '#size' => 12,
      '#maxlength' => 3,
      '#default_value' => isset($node_title_validation_config['min-' . $type->type]) ? $node_title_validation_config['min-' . $type->type] : 1,
    );

    $form[$type->type]['max-' . $type->type] = array(
      '#type' => 'textfield',
      '#title' => t("Maximum characters"),
      '#required' => TRUE,
      '#description' => t("Maximum number of characters node title should contain"),
      '#size' => 12,
      '#maxlength' => 3,
      '#default_value' => isset($node_title_validation_config['max-' . $type->type]) ? $node_title_validation_config['max-' . $type->type] : 255,
    );

    $form[$type->type]['min-wc-' . $type->type] = array(
      '#type' => 'textfield',
      '#title' => t("Minimum Word Count"),
      '#required' => TRUE,
      '#description' => t("Minimum number of words node title should contain"),
      '#size' => 12,
      '#maxlength' => 3,
      '#default_value' => isset($node_title_validation_config['min-wc-' . $type->type]) ? $node_title_validation_config['min-wc-' . $type->type] : 1,
    );

    $form[$type->type]['max-wc-' . $type->type] = array(
      '#type' => 'textfield',
      '#title' => t("Maximum Word Count"),
      '#description' => t("Maximum number of words node title should contain"),
      '#size' => 12,
      '#maxlength' => 3,
      '#default_value' => isset($node_title_validation_config['max-wc-' . $type->type]) ? $node_title_validation_config['max-wc-' . $type->type] : 25,
    );

    $form[$type->type]['unique-' . $type->type] = array(
      '#type' => 'checkbox',
      '#title' => t("Unique node title for @type content type", array('@type' => $type->type)),
      '#default_value' => isset($node_title_validation_config['unique-' . $type->type]) ? $node_title_validation_config['unique-' . $type->type] : 0,
    );
  }

  $form['unique'] = array(
    '#type' => 'checkbox',
    '#title' => t('Unique node title for all content types'),
    '#default_value' => isset($node_title_validation_config['unique']) ? $node_title_validation_config['unique'] : 0,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
  return $form;
}

/**
 * The validation function for node_title_validation_admin_form form.
 */
function node_title_validation_admin_form_validate($form, &$form_state) {

  // Get available content types.
  $node_types = node_type_get_types();
  // Loop for each content type & validate min, max values.
  foreach ($node_types as $type) {

    $max = $form_state['values']['max-' . $type->type];

    // Validate field is numerical.
    if (!is_numeric($max)) {
      form_set_error('max-' . $type->type, t("These value should be Numerical"));
    }

    // Validate field should be between 0 and 256.
    if ($max <= 0 || $max > 255) {
      form_set_error('max-' . $type->type, t("These value should be between 0 and 256"));
    }

    $min = $form_state['values']['min-' . $type->type];

    // Validate field is numerical.
    if (!is_numeric($min)) {
      form_set_error('min-' . $type->type, t("These value should be Numerical"));
    }

    // Validate field is greater than 1.
    if ($min < 1) {
      form_set_error('min-' . $type->type, t("These value should be more than 1"));
    }

    // Validate min is less than max value.
    if ($min > $max) {
      form_set_error('min-' . $type->type, t("Minimum length should not be more than Max length"));
    }

    // Validate field is numerical.
    $min_wc = $form_state['values']['min-wc-' . $type->type];
    if (!empty($min_wc) && !is_numeric($min_wc)) {
      form_set_error('min-wc-' . $type->type, t("Value should be Numerical"));
    }
    $max_wc = $form_state['values']['max-wc-' . $type->type];
    if (!empty($max_wc) && !is_numeric($max_wc)) {
      form_set_error('max-wc-' . $type->type, t("Value should be Numerical"));
    }

    // Validate min is less than max value.
    if (!empty($min_wc) && !empty($max_wc) && $min_wc > $max_wc) {
      form_set_error('max-wc-' . $type->type, t("Minimum word count of title should not be more than Maximum word count"));
    }
  }
}

/**
 * Implements hook_form_submit().
 */
function node_title_validation_admin_form_submit($form, &$form_state) {

  $values = array();
  // Get available content types.
  $node_types = node_type_get_types();

  // Store Form values in node_title_validation_config variable.
  foreach ($node_types as $type) {
    $values['exclude-' . $type->type] = $form_state['values']['exclude-' . $type->type];
    $values['min-' . $type->type] = $form_state['values']['min-' . $type->type];
    $values['max-' . $type->type] = $form_state['values']['max-' . $type->type];
    $values['min-wc-' . $type->type] = $form_state['values']['min-wc-' . $type->type];
    $values['max-wc-' . $type->type] = $form_state['values']['max-wc-' . $type->type];
    $values['unique-' . $type->type] = $form_state['values']['unique-' . $type->type];
  }
  $values['unique'] = $form_state['values']['unique'];

  // Set node_title_validation_config variable.
  variable_set('node_title_validation_config', $values);

  drupal_set_message(t('Configurations saved successfully!'));
}
