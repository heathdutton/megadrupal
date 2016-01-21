<?php

/**
 * @file
 * Administrative functions for module "Multistep Nodeform".
 */

/**
 * Menu callback; listing of steps for a content type.
 *
 * Allows steps to be reordered and fields to be associated into steps using
 * JS drag-n-drop.
 */
function msnf_step_overview_form(&$form_state, $type_name) {
  // Load necessary functions for form steps.
  module_load_include('inc', 'msnf', 'includes/msnf.steps');

  // When displaying the form, make sure the list of fields is up-to-date.
  if (empty($form_state['post'])) {
    msnf_clear_type_cache();
  }

  // Gather type information.
  $type = msnf_content_types($type_name);
  $field_types = _msnf_content_field_types();

  // Non-CCK fields (such as "title", "body", etc.).
  $extra = $type['extra'];
  // CCK-fields.
  $fields = $type['fields'];
  // Field groups.
  $groups = array();
  if (module_exists('fieldgroup')) {
    $groups = fieldgroup_groups($type['type']);
    $group_types = fieldgroup_types();
  }

  $steps = $step_options = $step_types = array();
  $steps = msnf_steps($type['type']);
  $step_types = msnf_step_types();
  $step_options = _msnf_steps_label($type['type']);
  // Add the ability to group fields under the newly created row.
  $step_options['_add_new_step'] = '_add_new_step';

  // Add group names to step options so we can move fields into groups.
  // $groups is empty if fieldgroup.module is not enabled.
  foreach ($groups as $group_name => $group) {
    $step_options[$group_name] = t($group['label']);
  }

  // Store the default weights as we meet them, to be able to put the
  // 'add new' rows after them.
  $weights = array();

  $form = array(
    '#tree' => TRUE,
    '#type_name' => $type['type'],
    '#fields' => array_keys($fields),
    '#steps' => array_keys($steps),
    '#extra' => array_keys($extra),
    '#groups' => array_keys($groups),
    '#field_rows' => array(),
    '#group_rows' => array(),
    '#step_rows' => array(),
  );

  // Extra-Fields.
  foreach ($extra as $name => $field) {
    $weight = $field['weight'];
    $label = $field['label'];
    $field_name = $name;
    $form[$name] = array(
      'label' => array('#value' => check_plain($label)),
      'field_name' => array('#value' => $field['field_name']),
      'type' => array('#value' => t($field_types[$field['type']]['label'])),
      'weight' => array(
        '#type' => 'textfield',
        '#default_value' => $weight,
        '#size' => 3,
      ),
      'parent' => array(
        '#type' => 'select',
        '#options' => $step_options,
        '#default_value' => '',
      ),
      'prev_parent' => array('#type' => 'hidden', '#value' => ''),
      'hidden_name' => array(
        '#type' => 'hidden',
        '#default_value' => $field_name,
      ),
      '#leaf' => TRUE,
      '#row_type' => 'field',
      'field' => array('#type' => 'value', '#value' => $field),
    );
    $form['#field_rows'][] = $name;
    $weights[] = $weight;
  }

  // Fields (from "fieldable entities").
  foreach ($fields as $name => $field) {
    $weight = $field['widget']['weight'];
    $label = $field['widget']['label'];
    $field_name = $field['field_name'];
    $form[$name] = array(
      'label' => array('#value' => check_plain($label)),
      'field_name' => array('#value' => $field['field_name']),
      'type' => array('#value' => t($field_types[$field['type']]['label'])),
      'weight' => array(
        '#type' => 'textfield',
        '#default_value' => $weight,
        '#size' => 3,
      ),
      'parent' => array(
        '#type' => 'select',
        '#options' =>
        $step_options,
        '#default_value' => '',
      ),
      'prev_parent' => array('#type' => 'hidden', '#value' => ''),
      'hidden_name' => array(
        '#type' => 'hidden',
        '#default_value' => $field_name,
      ),
      '#leaf' => TRUE,
      '#row_type' => 'field',
      'field' => array('#type' => 'value', '#value' => $field),
    );
    $form['#field_rows'][] = $name;
    $weights[] = $weight;
  }

  // Groups.
  foreach ($groups as $name => $group) {
    $weight = $group['weight'];
    $form[$name] = array(
      'label' => array('#value' => check_plain($group['label'])),
      'group_name' => array('#value' => $group['group_name']),
      'group_type' => array('#value' => t($group_types[$group['group_type']])),
      'weight' => array(
        '#type' => 'textfield',
        '#default_value' => $weight,
        '#size' => 3,
      ),
      'parent' => array(
        '#type' => 'select',
        '#options' => $step_options,
        '#default_value' => '',
      ),
      'prev_parent' => array('#type' => 'hidden', '#value' => ''),
      'hidden_name' => array(
        '#type' => 'hidden',
        '#default_value' => $group['group_name'],
      ),
      '#leaf' => FALSE,
      '#row_type' => 'group',
      'group' => array('#type' => 'value', '#value' => $group),
    );
    // Adjust child fields rows.
    foreach ($group['fields'] as $field_name => $field) {
      $form[$field_name]['parent']['#default_value'] = $name;
      $form[$field_name]['prev_parent']['#value'] = $name;
    }
    $form['#group_rows'][] = $name;
    $weights[] = $weight;
  }

  // Steps.
  foreach ($steps as $name => $step) {
    $weight = $step['weight'];
    $form[$name] = array(
      'label' => array('#value' => check_plain($step['label'])),
      'step_name' => array('#value' => $step['step_name']),
      'step_type' => array('#value' => t($step_types[$step['step_type']])),
      'configure' => array(
        '#value' => l(
          t('Configure'),
          "admin/content/node-type/{$type['url_str']}/steps/" .
          $step['step_name']
        ),
      ),
      'remove' => array(
        '#value' => l(
          t('Remove'),
          "admin/content/node-type/{$type['url_str']}/steps/" .
          $step['step_name'] . '/remove'
        ),
      ),
      'weight' => array(
        '#type' => 'textfield',
        '#default_value' => $weight,
        '#size' => 3,
      ),
      'parent' => array('#type' => 'hidden', '#default_value' => ''),
      'hidden_name' => array(
        '#type' => 'hidden',
        '#default_value' => $step['step_name'],
      ),
      '#root' => TRUE,
      '#row_type' => 'step',
      'step' => array('#type' => 'value', '#value' => $step),
    );
    // Adjust child fields rows.
    foreach ($step['fields'] as $field_name => $field) {
      if (strpos($form[$field_name]['parent']['#default_value'], 'group_') > -1) {
        // Field is child of a fieldgroup so do not change its parent!
      }
      else {
        $form[$field_name]['parent']['#default_value'] = $name;
        $form[$field_name]['prev_parent']['#value'] = $name;
      }
    }
    // Adjust child groups rows.
    foreach ($step['groups'] as $group_name => $group) {
      $form[$group_name]['parent']['#default_value'] = $name;
      $form[$group_name]['prev_parent']['#value'] = $name;
    }
    $form['#step_rows'][] = $name;
    $weights[] = $weight;
  }

  $weight = max($weights) + 1;

  // Additional row : add new step.
  if (!empty($step_types)) {
    $weight++;
    $name = '_add_new_step';
    $form[$name] = array(
      'label' => array(
        '#type' => 'textfield',
        '#size' => 15,
        '#description' => t('Label'),
      ),
      'step_name' => array(
        '#type' => 'textfield',
        // This field should stay LTR even for RTL languages.
        '#field_prefix' => '<span dir="ltr">step_',
        '#field_suffix' => '</span>&lrm;', // Left-To-Right-Marker
        '#attributes' => array('dir' => 'ltr'),
        '#size' => 15,
        '#maxlength' => 26,
        '#description' => t('Step name (a-z, 0-9, _)'),
      ),
      'step_option' => array(
        '#type' => 'hidden',
        '#value' => '',
      ),
      'step_type' => array(
        '#type' => 'hidden',
        '#value' => 'standard',
      ),
      'weight' => array(
        '#type' => 'textfield',
        '#default_value' => $weight,
        '#size' => 3,
      ),
      'parent' => array('#type' => 'hidden', '#default_value' => ''),
      'hidden_name' => array('#type' => 'hidden', '#default_value' => $name),
      '#root' => TRUE,
      '#add_new' => TRUE,
      '#row_type' => 'add_new_step',
    );
    if (count($step_types) > 1) {
      $form[$name]['step_type'] = array(
        '#type' => 'select',
        '#description' => t('Type of step.'),
        '#options' => $step_types,
        '#default_value' => 'standard',
      );
    }
    $form['#step_rows'][] = $name;
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}

/**
 * Validation handler for msnf_step_overview_form().
 */
function msnf_step_overview_form_validate($form, &$form_state) {
  // Load necessary functions for form steps.
  module_load_include('inc', 'msnf', 'includes/msnf.steps');

  $form_values = $form_state['values'];
  $step = $form_values['_add_new_step'];

  if (array_filter(array($step['label'], $step['step_name']))) {
    $validation = msnf_step_validate_name($step, $form['#type_name']);
    if (!empty($validation['errors'])) {
      foreach ($validation['errors'] as $type => $messages) {
        foreach ($messages as $message) {
          if ($type == 'label') {
            form_set_error('_add_new_step][label', t('Add new step:') .
              ' ' . $message);
          }
          else {
            form_set_error('_add_new_step][step_name', t('Add new step:') .
              ' ' . $message);
          }
        }
      }
    }
    $step_name = $validation['step_name'];
    form_set_value($form['_add_new_step']['step_name'], $step_name, $form_state);
  }
  else {
    // Fail validation if attempt to nest fields under a new step without the
    // proper information. Not raising an error would cause the nested fields
    // to get weights the user doesn't expect.
    foreach ($form_values as $key => $values) {
      if ($values['parent'] == '_add_new_step') {
        form_set_error('_add_new_step][label', t('Add new step: you need to provide a label.'));
        form_set_error('_add_new_step][step_name', t('Add new step: you need to provide a step name.'));
        break;
      }
    }
  }
}

/**
 * Submit handler for msnf_step_overview_form().
 */
function msnf_step_overview_form_submit($form, &$form_state) {
  // Load necessary functions for form steps.
  module_load_include('inc', 'msnf', 'includes/msnf.steps');

  $form_values = $form_state['values'];
  $type_name = $form['#type_name'];
  $type = msnf_content_types($type_name);

  // Update field weights.
  $extra = array();
  foreach ($form_values as $key => $values) {
    // Steps are handled in later.
    if (in_array($key, $form['#fields']) && module_exists('content')) {
      db_query("UPDATE {" . content_instance_tablename() . "} SET weight = %d WHERE type_name = '%s' AND field_name = '%s'",
        $values['weight'], $type_name, $key);
    }
    elseif (in_array($key, $form['#groups']) && module_exists('fieldgroup')) {
      db_query("UPDATE {" . fieldgroup_tablename() . "} SET weight = %d WHERE type_name = '%s' AND group_name = '%s'",
        $values['weight'], $type_name, $key);
    }
    elseif (in_array($key, $form['#extra'])) {
      $extra[$key] = $values['weight'];
    }
  }

  // We use this variable from CCK to store weights of non-CCK fields.
  if ($extra) {
    variable_set('content_extra_weights_' . $type_name, $extra);
  }
  else {
    variable_del('content_extra_weights_' . $type_name);
  }

  // Create new step if needed.
  if (!empty($form_values['_add_new_step']['label'])) {
    $step = $form_values['_add_new_step'];
    $step['settings'] = msnf_step_default_settings($step['step_type']);
    msnf_save_step($type_name, $step);
    $new_step_name = $step['step_name'];
  }

  // Parse incoming rows.
  foreach ($form_values as $key => $values) {
    // If 'field' row: update field parenting for CCK fields and non-CCK fields.
    if (in_array($key, $form['#fields']) || in_array($key, $form['#extra'])) {
      // If the field was added to the newly created step, replace the
      // '_add_new_step' value with the actual name of the step.
      $parent = ($values['parent'] == '_add_new_step' && isset($new_step_name)) ? $new_step_name : $values['parent'];
      // TODO: check the parent step does exist ?
      msnf_step_update_fields(array('field_name' => $key, 'step' => $parent, 'type_name' => $type_name, 'weight' => $values['weight']));
    }
    elseif (in_array($key, $form['#groups'])) {
      // If the group was added to the newly created step, replace the
      // '_add_new_step' value with the actual name of the step.
      $parent = ($values['parent'] == '_add_new_step' && isset($new_step_name)) ? $new_step_name : $values['parent'];
      // TODO: check the parent step does exist ?
      msnf_step_update_fields(array('field_name' => $key, 'step' => $parent, 'type_name' => $type_name, 'weight' => $values['weight']));
    }
    // If 'step' row:  update steps weights
    // (possible newly created step has already been taken care of).
    elseif (in_array($key, $form['#steps'])) {
      db_query("UPDATE {msnf_step} SET weight = %d WHERE type_name = '%s' AND step_name = '%s'",
        $values['weight'], $type_name, $key);
    }
  }

  cache_clear_all('msnf_step_data:', 'cache', TRUE);
}

/**
 * Menu callback to provide a form for editing a single step.
 */
function msnf_step_edit_form(&$form_state, $type_name, $step_name) {
  // Load necessary functions for form steps.
  module_load_include('inc', 'msnf', 'includes/msnf.steps');

  $content_type = msnf_content_types($type_name);
  $steps = msnf_steps($content_type['type']);

  if (!$step = $steps[$step_name]) {
    drupal_not_found();
    exit;
  }

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $step['label'],
    '#required' => TRUE,
  );

  // Set a default value for step type early in the form so it
  // can be overridden by subsequent form elements added by other modules.
  $step_type = !empty($step['step_type']) ? $step['step_type'] : 'standard';
  $form['step_type'] = array('#type' => 'hidden', '#default_value' => $step_type);

  $form['settings']['#tree'] = TRUE;
  $form['settings']['form'] = array(
    '#type' => 'fieldset',
    '#title' => t('Form settings'),
    '#description' => t('These settings apply to the step in the node editing form.'),
  );
  $form['settings']['form']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Help text'),
    '#default_value' => $step['settings']['form']['description'],
    '#rows' => 5,
    '#description' => t('Information to present to the user on the editing form.'),
    '#required' => FALSE,
  );
  $form['settings']['form']['button_label'] = array(
    '#tree' => TRUE,
    '#type' => 'fieldset',
    '#title' => t('Button labels'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Enter the labels for the buttons of this step.'),
  );
  $form['settings']['form']['button_label']['previous'] = array(
    '#type' => 'textfield',
    '#title' => t('Back'),
    '#default_value' => $step['settings']['form']['button_label']['previous'],
    '#description' => t('Text to use as label for back-button. Leave empty to use the title of the previous step.'),
    '#required' => FALSE,
  );
  $form['settings']['form']['button_label']['next'] = array(
    '#type' => 'textfield',
    '#title' => t('Next'),
    '#default_value' => $step['settings']['form']['button_label']['next'],
    '#description' => t('Text to use as label for next-button. Leave empty to use the title of the next step.'),
    '#required' => FALSE,
  );

  $form['weight'] = array('#type' => 'hidden', '#default_value' => $step['weight']);
  $form['step_name'] = array('#type' => 'hidden', '#default_value' => $step_name);

  $form['#content_type'] = $content_type;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 10,
  );

  return $form;
}

/**
 * Submit handler for msnf_step_edit_form().
 */
function msnf_step_edit_form_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  $content_type = $form['#content_type'];
  msnf_save_step($content_type['type'], $form_values);
  $form_state['redirect'] = 'admin/content/node-type/' . $content_type['url_str'] . '/steps';
}

/**
 * Menu callback to remove a single step.
 */
function msnf_step_remove_step(&$form_state, $type_name, $step_name) {
  // Load necessary functions for form steps.
  module_load_include('inc', 'msnf', 'includes/msnf.steps');

  $content_type = content_types($type_name);
  $steps = msnf_steps($content_type['type']);
  $step = isset($steps[$step_name]) ? $steps[$step_name] : '';

  if (empty($step)) {
    drupal_not_found();
    exit;
  }

  $form['#submit'][] = 'msnf_step_remove_step_submit';
  $form['#content_type'] = $content_type;
  $form['#step_name'] = $step_name;

  $message = t('This action cannot be undone.');

  return confirm_form($form,
    t('Are you sure you want to remove the step %label?',
    array('%label' => t($step['label']))),
    'admin/content/node-type/' . $content_type['url_str'] . '/steps', $message,
    t('Remove'), t('Cancel')
  );
}

/**
 * Submit handler for msnf_step_remove_step_form().
 */
function msnf_step_remove_step_submit($form, &$form_state) {
  $content_type = $form['#content_type'];
  $step_name = $form['#step_name'];
  msnf_step_delete($content_type['type'], $step_name);
  drupal_set_message(t('The step %step_name has been removed.', array('%step_name' => $step_name)));
  $form_state['redirect'] = 'admin/content/node-type/' . $content_type['url_str'] . '/steps';
}

/**
 * Helper function to order fields and steps when theming overview forms.
 *
 * The $form is passed by reference because we assign depths as parenting
 * relationships are sorted out.
 */
function _msnf_overview_order(&$form, $field_rows, $group_rows, $step_rows) {
  // Put weight and parenting values into a $dummy render structure
  // and let drupal_render figure out the corresponding row order.
  $dummy = array();

  // Step rows: account for weight.
  foreach ($step_rows as $name) {
    $dummy[$name] = array(
      '#weight' => $form[$name]['weight']['#value'],
      '#value' => $name . ' ',
    );
  }

  // Group rows: account for weight and parenting.
  if (module_exists('fieldgroup')) {
    foreach ($group_rows as $name) {
      $dummy[$name] = array(
        '#weight' => $form[$name]['weight']['#value'],
        '#value' => $name . ' ',
      );
      if ($parent = $form[$name]['parent']['#value']) {
        $form[$name]['#depth'] = 1;
        $dummy[$parent][$name] = $dummy[$name];
        unset($dummy[$name]);
      }
    }
  }

  // Field rows : account for weight and parenting.
  foreach ($field_rows as $name) {
    $dummy[$name] = array(
      '#weight' => $form[$name]['weight']['#value'],
      '#value' => $name . ' ',
    );
    if ($parent = $form[$name]['parent']['#value']) {
      $form[$name]['#depth'] = isset($form[$parent]['#depth']) ? $form[$parent]['#depth'] + 1 : 1;
      if (($grand_parent = $form[$parent]['parent']['#value'])
        && $form[$parent]['parent']['#type'] != 'hidden') {
        // Parent element is not a step.
        $dummy[$grand_parent][$parent][$name] = $dummy[$name];
      }
      else {
        $dummy[$parent][$name] = $dummy[$name];
      }
      unset($dummy[$name]);
    }
  }
  return $dummy ? explode(' ', trim(drupal_render($dummy))) : array();
}
