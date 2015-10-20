<?php

/**
 * @file
 * Basic admin forms, validators, and submit handlers.
 */

/**
 * Form constructor for the npr push config form.
 *
 * Generates a form to choose which drupal fields will map to which NPR fields.
 * Callback for admin/config/services/npr/push.
 *
 * @see npr_push_menu()
 * @see npr_push_admin_config_validate()
 * @see npr_push_admin_config_submit()
 * @ingroup forms
 */
function npr_push_admin_config() {
  $form = array();
  $available_types = array();
  foreach (node_type_get_types() as $type) {
    $available_types[$type->type] = $type->name;
  }

  $required_fields = array('body');
  $nprml_fields = npr_api_get_nprml_fields();
  $nprml_options = array();
  foreach ($nprml_fields as $key => $nprml_field) {
    $nprml_options[$key] = $key;
  }
  array_unshift($nprml_options, t('None'));

  foreach (node_type_get_types() as $type) {
    $form[$type->type . '_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t($type->name . ' settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );

    $map = variable_get('npr_push_field_map_' . $type->type, array());
    $all_fields = field_info_instances('node', $type->type);
    $fields = array();
    $form[$type->type . '_settings']['npr_push_types_' . $type->type] = array(
      '#type' => 'checkbox',
      '#title' => t('Push ' . $type->name . ' nodes to the NPR API'),
      '#default_value' => variable_get('npr_push_types_' . $type->type, 0),
    );

    $form[$type->type . '_settings'][$type->type . '_help_text'] = array(
      '#markup' => t('Title, teaser, and body will automatically be mapped to the corresponding NPRML elements.'),
    );

    foreach ($all_fields as $key => $all_field) {
      if (!in_array($all_field['field_name'], $required_fields)) {
        $form[$type->type . '_settings']['npr_push_mapping_' . $type->type . $all_field['field_name']] = array(
          '#type' => 'select',
          '#title' => t('Map "' . $all_field['field_name'] . '" to the following NPRML field:'),
          '#options' => $nprml_options,
          '#default_value' => !empty($map[$all_field['field_name']]) ? $map[$all_field['field_name']] : 0,
        );
      }
    }
    $pf = array();
    foreach(array_keys($all_fields) as $field) {
      $pf[$field] = $field;
    }
    array_unshift($pf, t('None'));

    $form[$type->type . '_settings']['flags'] = array(
      '#type' => 'fieldset',
      '#title' => t('Flags'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$type->type . '_settings']['flags']['npr_push_' . $type->type . '_pushflag'] = array(
      '#type' => 'select',
      '#title' => t('Push flag'),
      '#description' => t('Use a (drupal) field to push to the NPR API on a per-node basis.'),
      '#options' => $pf,
      '#default_value' => variable_get('npr_push_' . $type->type . '_pushflag', 0),
    );
    $form[$type->type . '_settings']['flags']['npr_push_' . $type->type . '_nproneflag'] = array(
      '#type' => 'select',
      '#title' => t('NPR One flag'),
      '#description' => t('Use a (drupal) field to push to NPR One on a per-node basis.'),
      '#options' => $pf,
      '#default_value' => variable_get('npr_push_' . $type->type . '_nproneflag', 0),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * Form validation handler for npr_push_admin_config().
 *
 * @see npr_push_admin_config_submit()
 */
function npr_push_admin_config_validate($form, &$form_state) {
  $required_fields = array('body');

  $nprml_fields = npr_api_get_nprml_fields();
  foreach (node_type_get_types() as $type) {
    $is_pushing = $form_state['values']['npr_push_types_' . $type->type];
    variable_set('npr_push_types_' . $type->type, $is_pushing);
    $all_fields = field_info_instances('node', $type->type);
    $taken = array();
    foreach ($all_fields as $key => $all_field) {
      $name = $all_field['field_name'];
      if (!in_array($name, $required_fields)) {
        $value = $form_state['values']['npr_push_mapping_' . $type->type . $name];
        if ($value !== '0') {
          if (!in_array($all_field['widget']['type'], $nprml_fields[$value]['accepted_types'])) {
            $error_msg = "To map to NPRML field $value, the (drupal) field must be one of the following type(s): " . implode(',', $nprml_fields[$value]['accepted_types']);
            form_set_error('npr_push_mapping_' . $all_field['field_name'], t('Incompatible field types.') . ' ' . $error_msg);
          }
          if (in_array($value, $taken)) {
            form_set_error('npr_push_mapping_' . $all_field['field_name'], t('Multiple fields cannot be mapped to the same NPRML field.'));
          }
          $taken[] = $value;
        }
      }
    }
    $required_nprml_fields = array('id');
    foreach ($required_nprml_fields as $nprml_field) {
      if (!in_array($nprml_field, $taken) && !empty($taken)) {
        form_set_error('npr_push', t('A field must map to the NPRML field:') . ' ' . $nprml_field);
      }
    }
  }
}

/**
 * Form submission handler for npr_push_admin_config().
 *
 * @see npr_push_admin_config_validate()
 */
function npr_push_admin_config_submit($form, &$form_state) {

  $required_fields = array('body');

  foreach (node_type_get_types() as $type) {
    variable_set('npr_push_types_' . $type->type, $form_state['values']['npr_push_types_' . $type->type]);

    $all_fields = field_info_instances('node', $type->type);
    $mapping = array();
    foreach ($all_fields as $key => $all_field) {
      if (!in_array($all_field['field_name'], $required_fields)) {
        $mapping[$all_field['field_name']] = $form_state['values']['npr_push_mapping_' . $type->type . $all_field['field_name']];
      }
    }
    variable_set('npr_push_field_map_' . $type->type, $mapping);

    // Insert push flag field, if it exists
    if (isset($form_state['values']['npr_push_' . $type->type . '_pushflag'])) {
      variable_set('npr_push_' . $type->type . '_pushflag', $form_state['values']['npr_push_' . $type->type . '_pushflag']);
    }
    // Insert NPR One flag field, if it exists
    if (isset($form_state['values']['npr_push_' . $type->type . '_nproneflag'])) {
      variable_set('npr_push_' . $type->type . '_nproneflag', $form_state['values']['npr_push_' . $type->type . '_nproneflag']);
    }
  }
}
