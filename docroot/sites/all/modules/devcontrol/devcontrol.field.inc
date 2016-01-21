<?php

/**
 * @file
 * Field creation wizzard.
 */

/**
 * Field wizzard form.
 */
function devcontrol_admin_field_wizzard($form, &$form_state) {

  if (!isset($form_state['step'])) {
    $form_state['step'] = 1;
  }

  $form['#tree'] = true;

  switch ($form_state['step']) {

    // Field type and identification.
    case 1:
      $form['field'] = array(
        '#type' => 'fieldset',
        '#title' => t("Field identity and nature"),
      );
      $form['field']['label'] = array(
        '#type' => 'textfield',
        '#title' => t("Label"),
        '#required' => true,
        '#description' => t("This will be used for later instances only."),
        '#size' => 20,
      );
      $form['field']['field_name'] = array(
        '#type' => 'machine_name',
        '#title' => t("Field name"),
        '#machine_name' => array(
          'source' => array('field', 'label'),
          'replace_pattern' => '[^a-z0-9_]+',
          'replace' => '_',
          'exists' => 'devcontrol_admin_field_wizzard_false',  
        ),
        '#required' => true,
        '#description' => null,
      );
      $typelist = array();
      $modulelist = system_list('module_enabled');
      foreach (field_info_field_types() as $name => $info) {
        $typelist[$modulelist[$info['module']]->info['name']][$name] = $info['label'];
      }
      $form['field']['type'] = array(
        '#type' => 'select',
        '#title' => t("Field type"),
        '#options' => $typelist,
        '#required' => true,
      );
      // FIXME: Missing element validate here.
      $form['field']['cardinality'] = array(
        '#type' => 'textfield',
        '#title' => t("Cardinality"),
        '#description' => t("Write 0 or leave empty for unlimited."),
        '#default_value' => 1,
        '#size' => 20,
      );
      $form['field']['translatable'] = array(
        '#type' => 'checkbox',
        '#title' => t("Translatable"),
        '#default_value' => 0,
      );
      $form['field']['locked'] = array(
        '#type' => 'checkbox',
        '#title' => t("Locked"),
        '#default_value' => 0,
      );
      break;

    // Field type settings.
    case 2:
      $field    = $form_state['storage']['field_full'];
      $instance = $form_state['storage']['fake_instance'];
      $has_opts = false;

      $form['field'] = array(
        '#type' => 'fieldset',
        '#title' => t("Field identity and nature"),
      );

      $additions = module_invoke($field['module'], 'field_settings_form', $field, $instance, false);
      if (is_array($additions)) {
        $form['field']['settings'] = $additions;
        $has_opts = true;
      }

      if (!$has_opts) {
        $form['field']['settings']['text']['#markup'] = t("This field type has no global settings.");
      }
      break;

    // Widget and formatter selection.
    case 3:
      field_info_widget_types();
      break;

    // Widget and formatter settings.
    case 4:
      break;

    // Synthesis, generated code for field, and questions:
    //   1 - Save it into the current site?
    //   2 - Create an instance?
    case 5:
      break;

    // Instance entity, bundle, and settings selection.
    // Possibility of changing the formatter and widget.
    case 6:
      break;

    // In case of wiget and/or formatter change, selection.
    case 7:
      break;

    // Instance widget and formatter settings.
    case 8:
      break;

    // Synthesis, generated code for instance, and questions:
    //   1 - Save it into the current site?
    //   2 - Create another instance?
    case 9:
      break;

    // The end, display all generated code.
    case 10:
      $field          = &$form_state['storage']['field'];
      $field_complete = &$form_state['storage']['field_full'];

      $form['normal'] = array(
        '#type' => 'fieldset',
        '#title' => t("Array structure to copy/paste for export"),
        '#collapsible' => true,
        '#collapsed' => false,
      );
      $form['normal']['export'] = array(
        '#prefix' => '<pre>',
        '#suffix' => '</pre>',
        '#markup' => var_export($field, true),
      );

      $form['complete'] = array(
        '#type' => 'fieldset',
        '#title' => t("Complete field structure"),
        '#collapsible' => true,
        '#collapsed' => true,
      );
      $form['complete']['export'] = array(
        '#prefix' => '<pre>',
        '#suffix' => '</pre>',
        '#markup' => var_export($field_complete, true),
      );

      break;

    // Oups.
    default:
      return MENU_NOT_FOUND;
  }

  $handlers = array();

  foreach (array('submit', 'validate') as $handler) {
    $function = __FUNCTION__ . '_' . $form_state['step'] . '_' . $handler;

    if (function_exists($function)) {
      $handlers['#' . $handler][] = $function;
    } else {
      // Validate and submit are in order. If no submit handler is present,
      // the validate one can be safely dropped.
      break;
    }
  }

  if (!empty($handlers)) {
    $handlers['#submit'][] = 'devcontrol_admin_field_wizzard_rebuild_submit';

    $form['actions']['#type'] = 'actions';

    $form['actions']['continue'] = array(
      '#type' => 'submit',
      '#value' => t("Continue"),
    ) + $handlers;
  }

  return $form;
}

/**
 * Submit handler that allow to proceed to next step, if specified by one of
 * the previous submit handler that have been called.
 */
function devcontrol_admin_field_wizzard_rebuild_submit($form, &$form_state) {
  if (isset($form_state['next_step'])) {
    $form_state['rebuild'] = true;
    $form_state['step'] = $form_state['next_step'];
    unset($form_state['next_step']);
  }
}

/**
 * Every validation we could dream of are handled by element_validate handlers.
 */
function devcontrol_admin_field_wizzard_1_submit($form, &$form_state) {

  $field = $field_complete = $form_state['values']['field'];

  $field_type = field_info_field_types($field['type']);

  // Complete the field fake instance, for posterity. Code borrowed from the
  // field_create_field() function.
  $field_complete += array(
    'entity_types' => array(),
    'cardinality' => 1,
    'translatable' => false,
    'locked' => false,
    'settings' => array(),
    'storage' => array(),
    'deleted' => 0,
  );
  $field_complete['settings'] += field_info_field_settings($field['type']);
  $field_complete['module'] = $field_type['module'];
  $field_complete['active'] = 1;
  $field_complete['storage'] += array(
    'type' => variable_get('field_storage_default', 'field_sql_storage'),
    'settings' => array(),
  );

  // Build a fake instance for various other operations.
  $form_state['storage']['fake_instance'] = array('entity_type' => 'foo', 'bundle' => 'foo');
  $form_state['storage']['field_full']    = $field_complete;
  $form_state['storage']['field']         = $field;
  $form_state['next_step']                = 2;
}

/**
 * The field settings form should have its own validators. We don't need a
 * validate handler for this step.
 */
function devcontrol_admin_field_wizzard_2_submit($form, &$form_state) {
  $field          = &$form_state['storage']['field'];
  $field_complete = &$form_state['storage']['field_full'];

  $settings = $form_state['values']['field']['settings'];
  $defaults = $field_complete['settings'];

  // In any case, we need to overwrite the complete field settings.
  $field_complete['settings'] = $settings;

  // Check differences between defaults and the POSTed values, allowing us
  // to filter settings to the strict minima.
  // FIXME: Some default values gets in the way.
  devcontrol_array_diff_deep($settings, $defaults);

  if (!empty($settings)) {
    $field['settings'] = $settings;
  }

  $form_state['next_step'] = 10; // 3 
}

function devcontrol_admin_field_wizzard_3_validate($form, &$form_state) {}

function devcontrol_admin_field_wizzard_3_submit($form, &$form_state) {
  $form_state['next_step'] = 4;
}

function devcontrol_admin_field_wizzard_4_validate($form, &$form_state) {}

function devcontrol_admin_field_wizzard_4_submit($form, &$form_state) {
  $form_state['next_step'] = 5;
}

function devcontrol_admin_field_wizzard_5_validate($form, &$form_state) {}

function devcontrol_admin_field_wizzard_5_submit($form, &$form_state) {
  $form_state['next_step'] = 6;
}

function devcontrol_admin_field_wizzard_6_validate($form, &$form_state) {}

function devcontrol_admin_field_wizzard_6_submit($form, &$form_state) {
  $form_state['next_step'] = 7;
}

function devcontrol_admin_field_wizzard_7_validate($form, &$form_state) {}

function devcontrol_admin_field_wizzard_7_submit($form, &$form_state) {
  $form_state['next_step'] = 8;
}

function devcontrol_admin_field_wizzard_8_validate($form, &$form_state) {}

function devcontrol_admin_field_wizzard_8_submit($form, &$form_state) {
  $form_state['next_step'] = 9;
}

function devcontrol_admin_field_wizzard_9_validate($form, &$form_state) {}

function devcontrol_admin_field_wizzard_9_submit($form, &$form_state) {
  $form_state['next_step'] = 10;
}

/**
 * Returns false.
 *
 * @return boolean False
 */
function devcontrol_admin_field_wizzard_false() {
  return false;
}

/**
 * Clean up the source array by keeping only values that changes in the
 * reference array. This function proceed deeply into arrays.
 *
 * Consider this function unsafe, and use it only if the arrays carry only
 * scalar values and no circular references. 
 *
 * @param array &$source
 * @param array $reference
 */
function devcontrol_array_diff_deep(array &$source, array $reference) {

  foreach ($source as $key => $value) {

    // Keep stuff that don't exists.
    if (!array_key_exists($key, $reference)) {
      continue;
    }

    // Proceed deeply into arrays.
    if (is_array($source[$key])) {
      devcontrol_array_diff_deep($source[$key], $reference[$key]);

      // In case the array have been emptied, we can remove it.
      if (empty($source[$key])) {
        unset($source[$key]);
      }

      continue;
    }

    // Else, ensure both value and type. This is efficient only for null and
    // scalar values.
    if ($source[$key] === $reference[$key]) {
      unset($source[$key]);
    }
  }
}
