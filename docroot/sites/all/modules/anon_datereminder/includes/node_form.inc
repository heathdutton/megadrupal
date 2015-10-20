<?php
/**
 * @file
 * Functions that support node type edit forms.
 */

/**
 * Implements hool_alter_node_form().
 *
 * Add reminder information when a node with reminders is displayed.
 *
 * @param array &$form
 *   The form to be altered.
 * @param array $form_state
 *   Current form state.
 * @param string $form_id
 *   Form id, in case there are more than one.
 *
 * No return value. The form is updated.
 */
function anon_datereminder_alter_node_form(&$form, &$form_state, $form_id) {
  module_load_include('inc', 'anon_datereminder', 'includes/defines');
  // Get type of node that we're editing.
  $type = $form['type']['#value'];
  $enabled = _anon_datereminder_type_enabled($type);
  switch ($enabled) {
    case ANON_DATEREMINDER_TYPE_ON:
      $dflt = ANON_DATEREMINDER_TYPE_ON;
      break;

    case ANON_DATEREMINDER_TYPE_ALLOWED:
      $dflt = ANON_DATEREMINDER_TYPE_RETAIN;
      break;

    default:
      return;
  }

  // Node is not saved but previewed (nid is empty).
  if (isset($form['#node']->build_mode)
          && $form['#node']->build_mode == NODE_BUILD_PREVIEW) {
    $node = $form['#node'];
  }
  // Load the node if it already exists.
  elseif (!empty($form['nid']['#value'])) {
    $node = node_load($form['nid']['#value']);
  }
  else {
    $node = NULL;
  }
  if ($node != NULL && !empty($node->anon_datereminder_enabled)) {
    $dflt = $node->anon_datereminder_enabled;
  }

  $form['reminder'] = array(
    '#type' => 'fieldset',
    '#title' => t('Anonymous Reminder Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#weight' => 40,
  );

  $radios[ANON_DATEREMINDER_TYPE_DISABLED] = t('Disable and discard existing reminders');
  $radios[ANON_DATEREMINDER_TYPE_RETAIN] = t('Disable, but keep existing reminders');
  $radios[ANON_DATEREMINDER_TYPE_ON] = t('Enable');

  $form['reminder']['anon_datereminder_enabled'] = array(
    '#type' => 'radios',
    '#options' => $radios,
    '#default_value' => $dflt,
    '#description' => t('If enabled, visitors can sign up to get reminders before this event'),
  );
  $form['#submit'][] = '_anon_datereminder_form_submit_node';
  $form['#validate'][] = '_anon_datereminder_form_validate_node';
}

/**
 * Implements hook_alter_node_type_form().
 */
function anon_datereminder_alter_node_type_form(&$form, &$form_state, $nodetype) {
  module_load_include('inc', 'anon_datereminder', 'includes/defines');
  module_load_include('inc', 'anon_datereminder', 'includes/date');

  $fields = field_info_fields();
  $datetypes = _anon_datereminder_supported_date_field_types();

  $datefields = array();
  foreach ($fields as $name => $field) {
    if (!isset($field['module'])
        || !isset($field['bundles'])
        || !is_array($field['bundles'])) {
      continue;
    }
    if (($field['module'] == 'date') &&
        array_key_exists($field['type'], $datetypes) &&
        is_array($field['bundles']['node']) &&
        in_array($nodetype, $field['bundles']['node'])) {
      // This node uses this field type.
      $datefields[$name] = $name;
      $lastkey = $name;
    }
  }

  if (count($datefields) == 0) {
    $form['anon_datereminder_enabled'] = array(
      '#type' => 'hidden',
      '#value' => ANON_DATEREMINDER_TYPE_DISABLED,
    );
  }
  else {
    // What is current setting?
    $enabled = _anon_datereminder_type_enabled($nodetype);
    $currentfield = variable_get("anon_datereminder_datefield_$nodetype", $lastkey);
    $fs = array(
      '#type' => 'fieldset',
      '#title' => t('Anonymous Reminder settings'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => 20,
      '#submit' => '_anon_datereminder_node_form_submit',
      '#group' => 'additional_settings',
    );
    $radios[ANON_DATEREMINDER_TYPE_DISABLED] = t('Disabled');
    $radios[ANON_DATEREMINDER_TYPE_RETAIN]
      = t('Disabled, but retain any existing reminders');
    $radios[ANON_DATEREMINDER_TYPE_ALLOWED] = t('Allowed, off by default');
    $radios[ANON_DATEREMINDER_TYPE_ON] = t('Allowed, on by default');
    $fs['anon_datereminder_enabled'] = array(
      '#type' => 'radios',
      '#options' => $radios,
      '#default_value' => $enabled,
    );

    if (count($datefields) > 1) {
      $fs['anon_datereminder_datefield'] = array(
        '#type' => 'select',
        '#options' => $datefields,
        '#default_value' => $currentfield,
        '#description' => t('Select which of several date fields to use for reminders'),
      );
      $lbl = t('Enable allowing visitors to request reminders for an event');
    }
    else {
      $fs['anon_datereminder_datefield'] = array(
        '#type' => 'hidden',
        '#value' => $currentfield,
      );
      $lbl
        = t('Enable allowing visitors to request reminders for an event, based on field %date',
              array('%date' => $datefields[$lastkey]));
    }
    $fs['anon_datereminder_enabled']['#title'] = $lbl;

    $form['reminder'] = $fs;
  }
}

/**
 * Implements hook_node_type_update().
 */
function anon_datereminder_node_type_update($info) {
  module_load_include('inc', 'anon_datereminder', 'includes/defines');

  $en = _anon_datereminder_type_enabled($info->type);
  if ($en == ANON_DATEREMINDER_TYPE_DISABLED) {
    module_load_include('inc', 'anon_datereminder', ANON_DATEREMINDER_DB);
    // If reminders are disabled for this node type, clean out old reminders.
    _anon_datereminder_clean_type_reminders($info->type);
  }
}

/**
 * Implements hook_node_type_delete().
 */
function anon_datereminder_node_type_delete($info) {
  variable_delete('anon_datereminder_enabled_' . $info->type);
}
