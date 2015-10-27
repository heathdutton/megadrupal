<?php

/**
 * @file
 * Functions and pages needed for the admin UI of regcode module.
 */

/**
 * Default admin list page, which should be overriden by Views.
 *
 * If this is appearing, then our views_default.inc has not been loaded
 * properly.
 */
function regcode_admin_list() {
  return t('This page should be replaced by Views. If you are seeing this page, please check your Views configuration.');
}


/**
 * Manage form.
 */
function regcode_admin_manage() {
  $form = array();

  $operations = array(
    REGCODE_CLEAN_TRUNCATE => t('Delete all registration codes'),
    REGCODE_CLEAN_EXPIRED  => t('Delete all expired codes'),
    REGCODE_CLEAN_INACTIVE => t('Delete all inactive codes'),
  );

  $form['regcode_operations'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Operations'),
    '#options'       => $operations,
  );

  $form['regcode_submit'] = array(
    '#type'          => 'submit',
    '#value'         => t('Perform operations'),
  );

  return $form;
}


/**
 * Manage action handler.
 */
function regcode_admin_manage_submit($form, $form_state) {
  $operations = $form_state['values']['regcode_operations'];

  foreach ($operations as $operation) {
    switch ($operation) {
      case REGCODE_CLEAN_TRUNCATE:
        regcode_clean(REGCODE_CLEAN_TRUNCATE);
        drupal_set_message(t('All registration codes were deleted.'));
        break;

      case REGCODE_CLEAN_EXPIRED:
        regcode_clean(REGCODE_CLEAN_EXPIRED);
        drupal_set_message(t('All expired registration codes were deleted.'));
        break;

      case REGCODE_CLEAN_INACTIVE:
        regcode_clean(REGCODE_CLEAN_INACTIVE);
        drupal_set_message(t('All inactive registration codes were deleted.'));
        break;
    }
  }
}


/**
 * Settings page form.
 */
function regcode_admin_settings() {
  $form = array();

  $form['regcode_fieldset_title'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Field set title'),
    '#description'   => t('The title of the registration code fieldset'),
    '#required'      => TRUE,
    '#default_value' => variable_get('regcode_fieldset_title', t('Registration Code')),
  );

  $form['regcode_field_title'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Field label'),
    '#description'   => t('The label of the registration code textfield'),
    '#required'      => TRUE,
    '#default_value' => variable_get('regcode_field_title', t('Registration Code')),
  );

  $form['regcode_field_description'] = array(
    '#type'          => 'textarea',
    '#title'         => t('Field description'),
    '#description'   => t('The description under the registration code textfield'),
    '#rows'          => 2,
    '#default_value' => variable_get('regcode_field_description', t('Please enter your registration code.')),
  );

  $form['regcode_vocabulary'] = array(
    '#type'          => 'select',
    '#title'         => t('Vocabulary'),
    '#options'       => _regcode_get_taxonomy_vocabularies(),
    '#required'      => TRUE,
    '#default_value' => variable_get('regcode_vocabulary', 1),
    '#description'   => t('The vocabulary used to classify registration codes.'),
  );

  $form['regcode_optional'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Make registration code optional'),
    '#default_value' => variable_get('regcode_optional', 0),
    '#description'   => t('If checked, users can register without a registration code.'),
  );

  return system_settings_form($form);
}


/**
 * Create registration codes form.
 */
function regcode_admin_create() {

  $form = array();

  // Basics.
  $form['regcode_create'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Create code'),
  );

  $form['regcode_create']['regcode_create_code'] = array(
    '#type'          => 'textfield',
    '#title'         => t("Registration code"),
    '#description'   => t('Leave blank to have code generated. Used as prefix when <em>Number of codes</em> is greater than 1.'),
  );

  $form['regcode_create']['regcode_create_maxuses'] = array(
    '#type'          => 'textfield',
    '#title'         => t("Maximum uses"),
    '#default_value' => 1,
    '#size'          => 10,
    '#required'      => TRUE,
    '#description'   => t('How many times this code can be used to register (enter 0 for unlimited).'),
  );

  $form['regcode_create']['regcode_create_begins'] = array(
    '#type'          => 'date',
    '#title'         => t("Active from"),
    '#description'   => t('When this code should activate (leave blank to activate immediately). Accepts any date format that strtotime can handle.'),
    '#default_value' => array('day' => 0, 'month' => 0, 'year' => 0),
    '#after_build'   => array('_regcode_date_optional'),
    '#element_validate' => array('_regcode_date_validate'),
  );

  $form['regcode_create']['regcode_create_expires'] = array(
    '#type'          => 'date',
    '#title'         => t("Expires on"),
    '#description'   => t('When this code should expire (leave blank for no expiry). Accepts any date format that strtotime can handle.'),
    '#default_value' => array('day' => 0, 'month' => 0, 'year' => 0),
    '#after_build'   => array('_regcode_date_optional'),
    '#element_validate' => array('_regcode_date_validate'),
  );

  $params = array('!url' => url('admin/structure/taxonomy/regcode'));
  $form['regcode_create']['regcode_create_tags'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t("Tags"),
    '#options'       => regcode_get_vocab_terms(),
    '#description'   => t('You may assign tags to help manage your codes (or <a href="!url">create some tags</a>).', $params),
  );

  // Bulk.
  $form['regcode_create_bulk'] = array(
    '#type'          => 'fieldset',
    '#title'         => t('Bulk creation'),
    '#description'   => t('Multiple codes can be created at once, use these settings to configure the code generation.'),
  );

  $form['regcode_create_bulk']['regcode_create_number'] = array(
    '#type'          => 'textfield',
    '#title'         => t("Number of codes to generate"),
    '#size'          => 10,
    '#default_value' => 1,
  );

  $form['regcode_create_bulk']['regcode_create_length'] = array(
    '#type'          => 'textfield',
    '#title'         => t("Code size"),
    '#size'          => 10,
    '#default_value' => 10,
  );

  $form['regcode_create_bulk']['regcode_create_format'] = array(
    '#type' => 'select',
    '#default_value' => variable_get('regcode_generate_format', 'alpha'),
    '#title' => t('Format of the generated codes'),
    '#options' => array(
      'alpha' => t('Letters'),
      'numeric' => t('Numbers'),
      'alphanum' => t('Numbers & Letters'),
      'hexadec' => t('Hexadecimal'),
    ),
  );

  $form['regcode_create_bulk']['regcode_create_case'] = array(
    '#type'          => 'checkbox',
    '#title'         => t('Uppercase generated codes'),
    '#default_value' => variable_get('regcode_generate_case', ''),
  );

  $form['regcode_create_bulk_submit'] = array(
    '#type'          => 'submit',
    '#value'         => t("Create codes"),
  );

  return $form;
}


/**
 * Validate create form.
 */
function regcode_admin_create_validate($form, &$form_state) {
  if (!is_numeric($form_state['values']['regcode_create_maxuses']) ||
      $form_state['values']['regcode_create_maxuses'] < 0) {
    form_set_error('regcode_create_maxuses', t('Invalid maxuses, specify a positive integer or enter "0" for unlimited'));
  }
}


/**
 * Process creation form.
 */
function regcode_admin_create_submit($form, &$form_state) {
  $code = new stdClass();

  // Convert dates into timestamps.
  foreach (array('begins', 'expires') as $field) {
    $value = $form_state['values']['regcode_create_' . $field];
    $code->$field = NULL;
    if ($value['year'] != 0) {
      $code->$field = mktime(0, 0, 0, $value['month'], $value['day'], $value['year']);
    }
  }

  // Grab form values.
  $code->is_active = 1;
  $code->maxuses   = $form_state['values']['regcode_create_maxuses'];
  $terms           = $form_state['values']['regcode_create_tags'];

  // Start creating codes.
  for ($i = 0; $i < (int) $form_state['values']['regcode_create_number']; $i++) {
    $code->code = $form_state['values']['regcode_create_code'];

    // Generate a code.
    if (empty($code->code) || $form_state['values']['regcode_create_number'] > 1) {
      $gen = regcode_generate($form_state['values']['regcode_create_length'],
        $form_state['values']['regcode_create_format'],
        $form_state['values']['regcode_create_case']);
      $code->code .= $gen;
    }

    // Save code.
    if (regcode_save($code, $terms, REGCODE_MODE_SKIP)) {
      drupal_set_message(t('Created registration code (%code)', array('%code' => $code->code)));
    }
    else {
      drupal_set_message(t('Unable to create code (%code) as code already exists', array('%code' => $code->code)), 'warning');
    }
  }
}


/**
 * Return a list of vocabularies.
 */
function _regcode_get_taxonomy_vocabularies() {
  $vocabularies = taxonomy_get_vocabularies();
  $options      = array();
  foreach ($vocabularies as $voc) {
    $options[$voc->vid] = check_plain($voc->name);
  }
  return $options;
}


/**
 * Make a date field optional.
 */
function _regcode_date_optional($element, &$form_state) {
  foreach (array('year', 'month', 'day') as $field) {
    $element[$field]['#options'][0] = '--';
    ksort($element[$field]['#options']);
  }

  return $element;
}


/**
 * Ensure a blank date validates.
 */
function _regcode_date_validate($element) {
  $blank = TRUE;
  foreach (array('year', 'month', 'day') as $field) {
    $blank = $blank && $element[$field]['#value'][0] === '0';
  }
  if ($blank) {
    return TRUE;
  }
  return date_validate($element);
}
