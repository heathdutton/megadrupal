<?php
/**
 * @file
 * Forms and settings for the Elearning entities.
 */

/**
 * Callback for editing and creating Elearning entities.
 */
function elearning_entity_edit_form($form, &$form_state) {

  $args = func_get_args();
  $entity = $args[3];
  $entity_type = $args[5];

  if (!$entity) {
    switch ($entity_type) {
      case 'elearning_course':
      case 'elearning_unit':
        $bundle = $entity_type;
        break;

      case 'elearning_exercise':
      case 'elearning_question':
        $bundle = $args[6];
    }
    $entity = entity_create($entity_type, array('type' => $bundle));
  }
  else {
    $bundle = $entity->bundle();
  }

  $form_state[$entity_type] = $entity;
  $form_state['entity_type'] = $entity_type;

  $entity_info = entity_get_info($entity_type);

  $form['label'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Label'),
    '#description' => t('Label of this @entity_type.', array('@entity_type' => $entity_info['label'])),
    '#default_value' => isset($entity->label) ? $entity->label : '',
    '#maxlength' => 255,
    '#weight' => -5,
  );

  $settings = !empty($entity->settings) ? unserialize($entity->settings) : array();

  $form['settings'] = array();
  $bundle_elements = module_invoke($bundle, 'elearning_entity_settings', $settings, $entity_type, $bundle);

  if ($bundle_elements) {
    $form['settings'] += $bundle_elements;
  }

  $entity_elements = module_invoke($entity_type, 'elearning_entity_settings', $settings, $entity_type, $bundle);

  if ($entity_elements) {
    $form['settings'] += $entity_elements;
  }

  if (!empty($form['settings'])) {
    $form['settings'] += array(
      '#type' => 'fieldset',
      '#title' => t('Settings'),
      '#weight' => 99,
      '#collapsed' => FALSE,
      '#collapsible' => TRUE,
      '#element_validate' => array('elearning_entity_validate_settings'),
    );
  }

  $form['actions'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('form-actions')),
    '#weight' => 400,
  );
  $submit = array();

  if (!empty($form['#submit'])) {
    $submit += $form['#submit'];
  }
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#submit' => $submit + array('elearning_edit_form_submit'),
  );

  if (!empty($entity->label)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => $submit + array('elearning_entity_edit_form_submit_delete'),
      '#weight' => 45,
    );
  }

  if (module_exists('entity_translation') &&
    entity_translation_enabled($entity_type)
  ) {
    $handler = entity_translation_get_handler($entity_type, $entity);
    $langcode = $handler->getFormLanguage();
  }
  else {
    $langcode = LANGUAGE_NONE;
  }
  field_attach_form($entity_type, $entity, $form, $form_state, $langcode);

  return $form;
}

/**
 * Implements ENTITY_TYPE_elearning_entity_settings().
 */
function elearning_course_elearning_entity_settings($settings, $entity_type, $bundle) {
  return _elearning_entity_settings_pass_threshold_element($settings, $entity_type, $bundle);
}

/**
 * Implements ENTITY_TYPE_elearning_entity_settings().
 */
function elearning_unit_elearning_entity_settings($settings, $entity_type, $bundle) {
  return _elearning_entity_settings_pass_threshold_element($settings, $entity_type, $bundle);
}

/**
 * Implements ENTITY_TYPE_elearning_entity_settings().
 */
function elearning_exercise_elearning_entity_settings($settings, $entity_type, $bundle) {
  return _elearning_entity_settings_pass_threshold_element($settings, $entity_type, $bundle);
}

/**
 * Add 'pass_threshold' form element to forms for course, unit and exercises.
 */
function _elearning_entity_settings_pass_threshold_element($settings, $entity_type, $bundle) {

  $default_value = isset($settings['pass_threshold']) ? $settings['pass_threshold'] : variable_get($bundle . '_default_threshold', ELEARNING_DEFAULT_PASSING_THRESHOLD);

  if (variable_get($entity_type . '_threshold_per_' . $bundle, FALSE)) {
    return array(
      'pass_threshold' => array(
        '#type' => 'textfield',
        '#title' => t('Pass threshold'),
        '#element_validate' => array('element_validate_integer_positive'),
        '#description' => t('Percent of questions the user has to answer correctly to pass.'),
        '#size' => 2,
        '#field_suffix' => '%',
        '#default_value' => $default_value,
      ),
    );
  }
}

/**
 * Title callback for Elearning entity Edit form.
 */
function elearning_entity_edit_title($entity_type, $entity) {
  // The grant and deny access tabs on courses are views. You can't use the
  // entities as path objects in views, so the breadcrumb wouldn't work for
  // views that work like tabs on an entity. That's why we check if the passed
  // entity is actually an entity. If not we load it again.
  return t('Edit @label', array('@label' => entity_label($entity_type, $entity)));
}

/**
 * Elearning entity submit callback.
 */
function elearning_edit_form_submit($form, &$form_state) {
  $entity_type = $form_state['entity_type'];
  $entity = $form_state[$entity_type];
  $entity_info = entity_get_info($entity_type);
  entity_form_submit_build_entity($entity_type, $entity, $form, $form_state);
  if ($entity->is_new = isset($entity->is_new) ? $entity->is_new : 0) {
    $entity->created = time();
  }
  $entity->changed = time();
  entity_save($entity_type, $entity);
  $form_state['redirect'] = 'admin/elearning/' . str_replace('elearning_', '', $entity_type);
  drupal_set_message(t('The @entity_type <em>%label</em> saved succesfully', array('@entity_type' => $entity_info['label'], '%label' => $entity->label)));
}

/**
 * Elearning entity delete button callback.
 */
function elearning_entity_edit_form_submit_delete($form, &$form_state) {
  $entity_type = $form_state['entity_type'];
  $entity = $form_state[$entity_type];
  $entity_ids = entity_extract_ids($entity_type, $entity);
  $entity_path_element = str_replace('elearning_', '', $entity_type);
  unset($_GET['destination']);
  $form_state['redirect'] = 'admin/elearning/' . $entity_path_element . '/' . $entity_ids[0] . '/delete';
}

/**
 * Delete confirmation form.
 */
function elearning_entity_delete_form($form, &$form_state, $entity, $entity_type) {
  $form_state['entity'] = $entity;
  $form_state['entity_type'] = $entity_type;
  $cancel_uri = 'admin/elearning/' . str_replace('elearning_', '', $entity_type);
  $entity_path_element = str_replace('elearning_', '', $entity_type);
  $return = confirm_form($form, t('Are you sure you want to delete %label?', array('%label' => entity_label($entity_type, $entity))), $cancel_uri, t('This action cannot be undone.'), t('Delete'), t('Cancel'));

  return $return;
}

/**
 * Delete form submit handler().
 */
function elearning_entity_delete_form_submit($form, &$form_state) {
  $entity = $form_state['entity'];
  $entity_type = $form_state['entity_type'];
  $entity_ids = entity_extract_ids($entity_type, $entity);
  $entity_info = entity_get_info($entity_type);

  entity_delete($entity_type, $entity_ids[0]);
  drupal_set_message(t('The @entity_type <em>%label</em> was succesfully deleted', array(
    '@entity_type' => $entity_info['label'],
    '%label' => entity_label($entity_type, $entity),
    )));
  $entity_path_element = str_replace('elearning_', '', $entity_type);
  $form_state['redirect'] = 'admin/elearning/' . $entity_path_element;
}

/**
 * Callback that adds the settings to the entity in a serialized array.
 */
function elearning_entity_validate_settings($element, &$form_state, $form) {
  foreach (element_children($element) as $setting) {
    $settings[$setting] = $element[$setting]['#value'];
  }
  form_set_value($element, serialize($settings), $form_state);
}

/**
 * Menu callback for settings form.
 */
function elearning_settings_wrapper($form, &$form_state, $entity_type, $bundle) {

  $settings_form = array();

  // Settings per bundle.
  $elements = module_invoke($bundle, 'elearning_settings', $entity_type, $bundle);
  if ($elements) {
    $settings_form += $elements;
  }

  // Settings per entity type.
  $elements = module_invoke($entity_type, 'elearning_settings', $entity_type, $bundle);
  if ($elements) {
    $settings_form += $elements;
  }

  if (empty($settings_form)) {
    return array('message' => array('#markup' => t('No settings avaiable')));
  }
  else {
    return system_settings_form($settings_form);
  }
}

/**
 * Implements ENTITY_TYPE_elearning_settings().
 */
function elearning_course_elearning_settings($entity_type, $bundle) {
  $form['elearning_course_default_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Default passing threshold for Units'),
    '#size' => 2,
    '#default_value' => variable_get('elearning_course_default_threshold', ELEARNING_DEFAULT_PASSING_THRESHOLD),
    '#element_validate' => array('element_validate_integer_positive'),
    '#field_suffix' => '%',
  );
  $form['elearning_course_threshold_per_elearning_course'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set a passing threshold for each individual unit.'),
    '#default_value' => variable_get('elearning_course_threshold_per_elearning_course', FALSE),
  );

  $form['elearning_course_allow_redo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to redo the course after finishing.'),
    '#default_value' => variable_get('elearning_course_allow_redo', FALSE),
  );

  return $form;
}

/**
 * Implements ENTITY_TYPE_elearning_settings().
 */
function elearning_unit_elearning_settings($entity_type, $bundle) {
  $exercise_type_info = entity_get_info('elearning_exercise');
  $exercise_types = array_keys($exercise_type_info['bundles']);

  $form['elearning_exercise_types_for_unit_evaluation'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Exercise types to include for unit results'),
    '#options' => array(),
    '#description' => t('Based on the selected exercise types the unit
      progress, result and status will be calculated.'),
    '#default_value' => variable_get('elearning_exercise_types_for_unit_evaluation', array()),
  );

  foreach ($exercise_types as $exercise_type) {
    $form['elearning_exercise_types_for_unit_evaluation']['#options'][$exercise_type] = $exercise_type_info['bundles'][$exercise_type]['label'];
    $form['elearning_exercise_types_for_unit_evaluation']['#options'][$exercise_type] = $exercise_type_info['bundles'][$exercise_type]['label'];
  }

  $form['elearning_unit_default_threshold'] = array(
    '#type' => 'textfield',
    '#title' => t('Default passing threshold for Units'),
    '#size' => 2,
    '#default_value' => variable_get('elearning_unit_default_threshold', ELEARNING_DEFAULT_PASSING_THRESHOLD),
    '#element_validate' => array('element_validate_integer_positive'),
    '#field_suffix' => '%',
  );
  $form['elearning_unit_threshold_per_elearning_unit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Set a passing threshold for each individual unit.'),
    '#default_value' => variable_get('elearning_unit_threshold_per_elearning_unit', FALSE),
  );

  $form['elearning_unit_allow_redo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow users to redo the unit after finishing.'),
    '#default_value' => variable_get('elearning_unit_allow_redo', FALSE),
  );

  return $form;
}

/**
 * Implements ENTITY_TYPE_elearning_settings().
 */
function elearning_exercise_elearning_settings($entity_type, $bundle) {

  $exercise_type_info = entity_get_info('elearning_exercise');

  $form = array(
    $bundle . '_default_threshold' => array(
      '#type' => 'textfield',
      '#title' => t('Default passing threshold for @label', array('@label' => $exercise_type_info['bundles'][$bundle]['label'])),
      '#size' => 2,
      '#default_value' => variable_get($bundle . '_default_threshold', ELEARNING_DEFAULT_PASSING_THRESHOLD),
      '#element_validate' => array('element_validate_integer_positive'),
      '#field_suffix' => '%',
    ),
    $entity_type . '_threshold_per_' . $bundle => array(
      '#type' => 'checkbox',
      '#title' => t('Set a passing threshold for each individual @label', array('@label' => $exercise_type_info['bundles'][$bundle]['label'])),
      '#default_value' => variable_get($entity_type . '_threshold_per_' . $bundle, FALSE),
    ),
    $bundle . '_allow_retry' => array(
      '#type' => 'checkbox',
      '#title' => t('Allow retry'),
      '#default_value' => variable_get($bundle . '_allow_retry', FALSE),
      '#description' => t('Allow user to retry the @label without restarting the unit.', array('@label' => $exercise_type_info['bundles'][$bundle]['label'])),
    ),
  );
  return $form;
}
