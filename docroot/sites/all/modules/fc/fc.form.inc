<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity - admin.
 */

/**
 * Implements hook_field_widget_settings_form().
 *
 * This function adds the checkbox to every field to determine whether it
 * should be used in the completeness calculation. It is set differently
 * for every field instance.
 */
function _fc_form_field_ui_field_edit_form_alter(&$form, &$form_state) {
  $settings = $form['#instance']['settings'];

  $form['instance']['required'] += array(
    '#states' => array(
      // Hide the "required" field when the 'Field Complete' checkbox is enabled.
      'invisible' => array(
        ':input[name="instance[settings][fc][fc_include]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['instance']['settings']['fc'] = array(
    '#type' => 'fieldset',
    '#title' => t('Field complete'),
    '#description' => t('Select whether this field instance should be included in completeness for its parent entity, plus any additional settings for this field type.'),
  );

  $form['instance']['settings']['fc']['fc_include'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include in field completeness'),
    '#description' => t('If checked, this field instance will be included in the completeness calculation for its parent entity.'),
    '#default_value' => !empty($settings['fc']['fc_include']),
    '#states' => array(
      // Only show this field when the 'fc_include' checkbox is enabled.
      'invisible' => array(
        ':input[name="instance[required]"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['instance']['settings']['fc']['fc_dummy'] = array(
    '#type' => 'item',
    '#title' => t('Field Complete cannot be selected when this field is set to be "Required".'),
    '#states' => array(
      // Only show this field when the 'fc_include' checkbox is enabled.
      'visible' => array(
        ':input[name="instance[required]"]' => array('checked' => TRUE),
      ),
    ),
  );

  // Choose the right plugin for the field type.
  $plugin = fc_get_plugin($form['#field']['type']);

  if (!empty($plugin['zero can be empty'])) {
    $form['instance']['settings']['fc']['fc_check_zero'] = array(
      '#type' => 'checkbox',
      '#title' => t('Treat a string zero as empty'),
      '#description' => t('In some instances you may want a "0" to be the equivalent of empty. This only works for fields which uses the "value" field.'),
      '#default_value' => !empty($settings['fc']['fc_check_zero']),
      '#states' => array(
        // Only show this field when the 'fc_include' checkbox is enabled.
        'visible' => array(
          ':input[name="instance[settings][fc][fc_include]"]' => array('checked' => TRUE),
        ),
      ),
    );
  }

  if (!empty($plugin['can be empty'])) {
    $form['instance']['settings']['fc']['fc_allow_empty'] = array(
      '#type' => 'checkbox',
      '#title' => t('Mark as complete if empty'),
      '#description' => t('Permit complex fields (like entityreference and field_collection) to be marked as complete even if they are empty. So only check completeness if there is something to check.'),
      '#default_value' => !empty($settings['fc']['fc_allow_empty']),
      '#states' => array(
        // Only show this field when the 'fc_include' checkbox is enabled.
        'visible' => array(
          ':input[name="instance[settings][fc][fc_include]"]' => array('checked' => TRUE),
        ),
      ),
    );
  }
  else {
    $form['instance']['settings']['fc']['fc_allow_empty'] = array(
      '#type' => 'value',
      '#value' => FALSE,
    );
  }

  // Add any special fields
  foreach ($plugin['field form'] as $setting => $element) {
    $form['instance']['settings']['fc'][$setting] = $element;
    $form['instance']['settings']['fc'][$setting] += array(
      '#default_value' => !empty($settings['fc'][$setting]) ? $settings['fc'][$setting] : NULL,
      '#states' => array(
        // Only show this field when the 'fc_include' checkbox is enabled.
        'visible' => array(
          ':input[name="instance[settings][fc][fc_include]"]' => array('checked' => TRUE),
        ),
      ),
    );
  }
}

/**
 * Implements hook_field_attach_form().
 */
function _fc_field_attach_form($entity_type, $entity, &$form, &$form_state, $langcode) {
  // Must check that there are fields first: https://drupal.org/node/2049945
  if (empty($form_state['field'])) {
    // No fields so let's just get out of here.
    return;
  }

  foreach ($form_state['field'] as $field_name => $field_data) {
    if (empty($field_data[$langcode])) {
      // No field data for the current language
      continue;
    }

    $instance = $field_data[$langcode]['instance'];
    if (!empty($instance['settings']['fc']['fc_include']) && !$instance['deleted'] && !empty($form[$field_name])) {
      // Add my after build function
      $form[$field_name][$langcode]['#after_build'][] = 'fc_field_attach_after_build';
    }
  }

  $form['#pre_render'][] = 'fc_form_pre_render';
}

function _fc_field_attach_after_build($element, &$form_state) {
  _fc_field_attach_ids($element);
  return $element;
}

function _fc_field_attach_ids(&$element) {
  $keys = element_children($element);
  if (!empty($keys)) {
    foreach ($keys as $key) {
      _fc_field_attach_ids($element[$key]);
    }
    $element['#field_complete'] = TRUE;
    if (!empty($element['#type']) && $element['#type']=='radios') {
      unset($element['_none']);
    }
  }
  else {
    fc_add_id($element['#id']);
  }
}

function _fc_form_pre_render($form) {
  $ids = fc_get_ids();
  // Allow other modules to change the IDs based on the form
  drupal_alter('fc_field_ids', $ids, $form);

  if (!empty($ids)) {
    $form['#field_complete'] = TRUE;
    $form['#attached']['css'][] = drupal_get_path('module', 'fc') . '/fc.css';

    if (variable_get('fc_js_ids', TRUE)) {
      $form['#attached']['js'][] = array(
        'data' => array('fc' => array('ids' => $ids)),
        'type' => 'setting',
      );
    }

    // Allow other modules to change the form based on the IDs
    drupal_alter('fc_form_pre_render', $form, $ids);
  }
  return $form;
}

function fc_add_id($id = NULL) {
  $fc_ids =& drupal_static(__FUNCTION__, array());
  if ($id !== NULL) {
    $fc_ids[$id] = NULL;
  }
  return $fc_ids;
}

function fc_get_ids() {
  return fc_add_id();
}

//===================================================================================

/**
 * Implements hook_form_profile2_form_alter().
 *
 * Profile2 behaves very non-standardly with its edit-form handling
 * so we have to try to see if we can't get round its nonsense.
 */
function _fc_form_profile2_form_alter(&$form, &$form_state) {
  // Must check that there are fields first: https://drupal.org/node/2049945
  // and further developments on that issue. It's feasible that this may be
  // called during registration, in which case we want to do nothing.
  if (empty($form_state['field']) || empty($form_state['entity_type']) || $form_state['entity_type'] != 'profile2') {
    // No fields so let's just get out of here.
    return;
  }

  $entity_type = $form_state['entity_type'];
  $entity =  $form_state[$form_state['entity_type']];

  $langcode = entity_language($entity_type, $entity);
  if (empty($langcode)) {
    $langcode = LANGUAGE_NONE;
  }

  foreach ($form_state['field']['#parents'] as $profile_name => $profile_fields) {
    foreach ($profile_fields['#fields'] as $field_name => $field_data) {
      if (empty($field_data[$langcode])) {
        // No field data for the current language
        continue;
      }

      $instance = $field_data[$langcode]['instance'];
      if (!empty($instance['settings']['fc']['fc_include']) && !$instance['deleted'] && !empty($form[$profile_name][$field_name])) {
        // Add my after build function
        $form[$profile_name][$field_name][$langcode]['#after_build'][] = 'fc_field_attach_after_build';
      }
    }
  }

  $form['#pre_render'][] = 'fc_form_pre_render';
}
