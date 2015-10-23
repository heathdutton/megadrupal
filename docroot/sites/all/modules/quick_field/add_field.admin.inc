<?php

/**
 * @file
 * This file holds functions for creating a new field in content types.
 */

/**
 * Form builder for the adding new field to a content type.
 *
 * @param string $entity_type
 *   The machine name of the entity.
 * @param string $bundle_name
 *   The machine name of the bundle, or a bundle object if the particular
 *   entity type has a menu loader for bundles.
 */
function quick_field_add_new_field_form($form, &$form_state, $entity_type, $bundle_name) {
  $bundle = field_extract_bundle($entity_type, $bundle_name);
  // When displaying the form, make sure the list of fields is up-to-date.
  if (empty($form_state['post'])) {
    field_info_cache_clear();
  }

  // Gather bundle information.
  $instances = field_info_instances($entity_type, $bundle);
  $extra_fields = field_info_extra_fields($entity_type, $bundle, 'form');

  $form += array(
    '#entity_type' => $entity_type,
    '#bundle' => $bundle,
    '#fields' => array_keys($instances),
    '#extra' => array_keys($extra_fields),
  );

  module_load_include('inc', 'field_ui', 'field_ui.admin');
  $max_weight = field_info_max_weight($entity_type, $bundle, 'form');
  $field_type_options = field_ui_field_type_options();
  $widget_type_options = field_ui_widget_type_options(NULL, TRUE);
  if ($field_type_options && $widget_type_options) {
    $form['label'] = array(
      '#type' => 'textfield',
      '#title' => t('New field label'),
      '#size' => 15,
      '#title_display' => 'invisible',
      '#description' => t('Enter Label of the field'),
      '#prefix' => '<div class="label-input"><div class="add-new-placeholder">' . t('Add new field') . '</div>',
      '#suffix' => '</div>',
      '#required' => TRUE,
    );

    $form['weight'] = array(
      '#type' => 'hidden',
      '#default_value' => $max_weight + 1,
      '#size' => 3,
      '#title_display' => 'invisible',
      '#title' => t('Weight for new field'),
      '#attributes' => array('class' => array('field-weight')),
      '#required' => TRUE,
    );

    $form['field_name'] = array(
      '#type' => 'machine_name',
      '#title' => t('New field name'),
      '#title_display' => 'invisible',
      // This field should stay LTR even for RTL languages.
      '#field_prefix' => '<span dir="ltr">field_',
      '#field_suffix' => '</span>&lrm;',
      '#size' => 15,
      '#description' => t('A unique machine-readable name containing letters, numbers, and underscores.'),
      // 32 characters minus the 'field_' prefix.
      '#maxlength' => 26,
      '#prefix' => '<div class="add-new-placeholder">&nbsp;</div>',
      '#machine_name' => array(
        'source' => array('label'),
        'exists' => '_quick_field_field_name_exists',
        'standalone' => TRUE,
        'label' => '',
      ),
      '#required' => TRUE,
    );

    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Type of new field'),
      '#options' => $field_type_options,
      '#title_display' => 'invisible',
      '#empty_option' => t('- Select a field type -'),
      '#description' => t('Type of data to store.'),
      '#attributes' => array('class' => array('field-type-select')),
      '#prefix' => '<div class="add-new-placeholder">&nbsp;</div>',
      '#required' => TRUE,
    );

    $form['widget_type'] = array(
      '#type' => 'select',
      '#title' => t('Widget for new field'),
      '#title_display' => 'invisible',
      '#options' => $widget_type_options,
      '#empty_option' => t('- Select a widget -'),
      '#description' => t('Form element to edit the data.'),
      '#attributes' => array('class' => array('widget-type-select')),
      '#cell_attributes' => array('colspan' => 3),
      '#prefix' => '<div class="add-new-placeholder">&nbsp;</div>',
      '#ajax' => array(
        'callback' => 'quick_field_update_field_form',
        'wrapper' => 'dropdown_second_replace',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#required' => TRUE,
    );
    $form['#tree'] = TRUE;
    $form['settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Setting Fields'),
      // Set up the wrapper so that AJAX will be able to replace the fieldset.
      '#prefix' => '<div id="dropdown_second_replace">',
      '#suffix' => '</div>',
      '#collapsible' => TRUE,
    );
    if (!empty($form_state['values'])) {
      $instance_settings = quick_field_create_field($form, $form_state, $entity_type);
      if (
        (!isset($instance_settings['#commands']['error']) &&
        (!isset($instance_settings['message']))
        )
      ) {
        foreach ($instance_settings as $key => $value) {
          $form['settings'][$key] = $value;
        }
      }
      elseif (isset($instance_settings['#error']) && $instance_settings['#error']) {
        drupal_set_message(t('There was a problem creating field %label: @message'),
          array(
            '%label' => $form_state['values']['label'],
            '@message' => $instance_settings['error'], 'error')
          );
      }
      else {
        drupal_set_message(t('Created field %label without any field settings.'), array('%label' => $form_state['values']['label']));
      }
      unset($instance_settings);
    }
  }

  $form['#attached']['css'][] = drupal_get_path('module', 'field_ui') . '/field_ui.css';
  $form['#attached']['js'][] = drupal_get_path('module', 'quick_field') . '/quick_field.js';

  $form['#theme'] = 'quick_field_add_new_field_form';
  $form['#attached']['js'][] = array(
    'type' => 'setting',
    'data' => array('fieldWidgetTypes' => field_ui_widget_type_options()),
  );

  return $form;
}

/**
 * Function to create a new field and its instance.
 */
function quick_field_create_field($form, &$form_state, $entity_type) {
  $field = array();
  $additions = array();
  if (!empty($form_state['values']['field_name'])) {
    $values = $form_state['values'];
    $field = array(
      'field_name' => $values['field_name'],
      'type' => $values['type'],
      'translatable' => isset($values['translatable']) ? $values['translatable'] : '',
    );
    $bundle = $form['#bundle'];
    $instance = array(
      'field_name' => $field['field_name'],
      'entity_type' => $entity_type,
      'bundle' => $bundle,
      'label' => $values['label'],
      'widget' => array(
        'type' => $values['widget_type'],
        'weight' => $values['weight'],
      ),
    );

    // Create the field and instance.
    $new_field_array = explode('field_', $field['field_name']);
    $new_field_name_value = end($new_field_array);
    if (!_field_ui_field_name_exists($new_field_name_value)) {
      try {
        field_create_field($field);
        field_create_instance($instance);
        $field = field_info_field($instance['field_name']);
        $additions = quick_field_get_field_settings($field, $instance);
        if (isset($additions['message']) && $additions['message']) {
          unset($_SESSION['messages']);
          return quick_field_reset_form_fields($additions['message'], 'status');
        }
      }
      catch (Exception $e) {
        return quick_field_reset_form_fields($e->getMessage);
      }
    }
    else {
      $message = t('Attempt to create field name %name which already exists and is active.', array('%name' => $field['field_name']));
      return quick_field_reset_form_fields($message);
    }
  }
  return $additions;
}

/**
 * Helper function to invoke javascript in case of any error / exception.
 */
function quick_field_reset_form_fields($message, $message_type = 'error') {
  $commands = array();
  $commands[] = array(
    // The command will be used in our JavaScript file (see next section).
    'command' => 'resetTypeWidgetFields',
    // We pass the value that the user selected in the select element to our
    // JavaScript function:
    'selectedValue' => $message,
    'messageType' => $message_type,
    'error' => 1,
  );
  // And finally, we will return all of our commands to the system
  // to be executed:
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Function to get field settings corresponding to field type.
 *
 * @return array
 *   Field settings array
 */
function quick_field_get_field_settings($field, $instance) {
  $additions = module_invoke($field['module'], 'field_settings_form', $field, $instance, '');
  if (empty($additions)) {
    $additions = array('message' => t('Created field %label without any field settings.', array('%label' => $field['field_name'])));
  }
  else {
    $additions['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  }
  return $additions;
}

/**
 * Form submission handler for quick_field_add_new_field_form().
 */
function quick_field_add_new_field_form_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  $field_values = $form_values;
  // Merge incoming form values into the existing field.
  $field = field_info_field($field_values['field_name']);
  if ($field['field_name']) {
    $entity_type = $form['#entity_type'];
    $bundle = $form['#bundle'];
    $instance = field_info_instance($entity_type, $field['field_name'], $bundle);

    // Update the field.
    $field = array_merge($field, $field_values);

    try {
      field_update_field($field);
      drupal_set_message(t('Created field %label field settings.', array('%label' => $instance['label'])));
    }
    catch (Exception $e) {
      drupal_set_message(t('Attempt to update field %label failed: %message.', array('%label' => $instance['label'], '%message' => $e->getMessage())), 'error');
    }
  }
  else {
    drupal_set_message(t('Trying to update a non-existing field'), 'error');
  }
}

/**
 * Form validation handler for quick_field_add_new_field_form().
 */
function quick_field_add_new_field_form_validate($form, &$form_state) {
  $field = $form_state['values'];
  // Validate if any information was provided in the 'add new field' row.
  // Missing label.
  if (!$field['label']) {
    form_set_error('label', t('Add new field: you need to provide a label.'));
  }

  // Missing field name.
  if (!$field['field_name']) {
    form_set_error('field_name', t('Add new field: you need to provide a field name.'));
  }
  // Field name validation.
  else {
    $field_name = $field['field_name'];

    // Add the 'field_' prefix.
    $field_name = 'field_' . $field_name;
    form_set_value($form['field_name'], $field_name, $form_state);
  }

  // Missing field type.
  if (!$field['type']) {
    form_set_error('type', t('Add new field: you need to select a field type.'));
  }

  // Missing widget type.
  if (!$field['widget_type']) {
    form_set_error('widget_type', t('Add new field: you need to select a widget.'));
  }
}

/**
 * Ajax handler for updating form for creating fields.
 */
function quick_field_update_field_form($form, $form_state) {
  return $form['settings'];
}
