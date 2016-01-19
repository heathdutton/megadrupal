<?php

/**
 * @file
 * Admin file for the Required Fields.
 */

/**
 * Page callback for admin required fields page.
 *
 * Admin page for managing the content type fields to toggle it's required
 * attribute.
 */
function required_fields_configuration($form, $form_state) {
  // Retrive field instances of existing content types.
  $content_types = field_info_instances('node');
  $count = 0;

  // Generate the table grouping via content type.
  foreach ($content_types as $node_type => $fields) {
    $count++;
    required_fields_prepare_admin_form($form, $node_type, $fields, $count);
  }

  return $form;
}

/**
 * Admin page for managing the field collection fields.
 */
function required_fields_collection_configuration($form, $form_state) {
  // Checks whether field collection module is installed.
  if (!module_exists('field_collection')) {
    drupal_set_message(t('It supports Field collection fields but no !fieldcollection
      module seems to be installed!',
      array(
        '!fieldcollection' => l(t('Field Collection'), 'https://drupal.org/project/field_collection'),
      )
    ), 'status', FALSE);
    return $form;
  }
  // Empty the field info cache to display correct values.
  field_info_cache_clear();
  // Retrive field instances of existing field collection fields.
  $content_types = field_read_fields(array('type' => 'field_collection'));
  $count = 0;

  // Generate the table grouping via field collections.
  foreach ($content_types as $node_type => $collection) {
    $fields = field_info_instances('field_collection_item', $node_type);
    $count++;
    required_fields_prepare_admin_form($form, $node_type, $fields, $count, 'field_collection');
  }
  return $form;
}

/**
 * Generating fields as a row with required/optional toggle button.
 */
function required_fields_prepare_admin_form(&$form, $node_type, $fields, $count, $type = '') {
  $form[$node_type] = array(
    '#type' => 'fieldset',
    '#title' => check_plain($node_type),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array('class' => array('required_fields_configuration')),
  );
  $i = $count;
  foreach ($fields as $key => $field) {
    $form[$node_type]['type_' . $i . '_' . $field['label']] = array(
      '#markup' => check_plain($field['label']),
      '#attributes' => array('class' => array('type')),
      '#prefix' => '<div class="field_title">',
      '#suffix' => '</div>',
    );

    $form[$node_type]['type_' . $i . '_' . $key] = array(
      '#markup' => $field['field_name'],
      '#attributes' => array('class' => array('type')),
      '#prefix' => '<div class="node_type">',
      '#suffix' => '</div>',
    );

    $value = (isset($field['required']) && $field['required'] == 1) ? t('Make Optional') : t('Make Required');
    $form[$node_type]['action_' . $i . '_' . $key] = array(
      '#type' => 'button',
      '#name' => 'action_' . $i . '_' . $key,
      '#limit_validation_errors' => array(),
      '#executes_submit_callback' => TRUE,
      '#value' => $value,
      '#attributes' => array('class' => array('mark')),
      '#prefix' => '<div id="markbutton_' . $i . '_' . $key . '">',
      '#suffix' => '</div>',
      '#ajax' => array(
        'callback' => 'required_fields_ajax_callback',
        'wrapper' => 'markbutton_' . $i . '_' . $key,
        'method' => 'replace',
        'effect' => 'fade',
        'attributes' => array(
          'field' => $key,
          'ctype' => $node_type,
          'parent' => 'action_' . $i . '_' . $key),
      ),
    );
  }
}

/**
 * Ajax callback to toogle required options for the field.
 */
function required_fields_ajax_callback($form, &$form_state) {
  field_info_cache_clear();
  $field = $form_state['triggering_element']['#ajax']['attributes']['field'];
  $bundle = $form_state['triggering_element']['#ajax']['attributes']['ctype'];

  $entity_type = 'node';
  if ($form['#id'] == 'required-fields-collection-configuration') {
    $entity_type = 'field_collection_item';
  }
  $instance_info = field_info_instance($entity_type, $field, $bundle);
  $instance_info['required'] = !$instance_info['required'];
  field_update_instance($instance_info);

  $field = $form_state['triggering_element']['#ajax']['attributes']['parent'];
  $fieldset = $form_state['triggering_element']['#ajax']['attributes']['ctype'];

  if ($instance_info['required'] == 1) {
    $form[$fieldset][$field]['#value'] = t('Make Optional');
  }
  else {
    $form[$fieldset][$field]['#value'] = t('Make Required');
  }

  return $form[$fieldset][$field];
}

/**
 * Form Submit callback.
 */
function required_fields_prepare_admin_form_submit($form, &$form_state) {
  $form_state['rebuild'] = TRUE;
}
