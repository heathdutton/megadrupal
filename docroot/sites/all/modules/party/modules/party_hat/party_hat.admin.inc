<?php
/**
 * @file
 * Contains admin page callbacks for the party hats module.
 */

/**
 * Entity form for party hats.
 *
 * @param $hat
 *  A hat object.
 */
function party_hat_form($form, &$form_state, $hat, $op = 'edit') {
  $form['#hat'] = $hat;

  if (isset($hat->hid)) {
    $form['hid'] = array(
      '#type' => 'hidden',
      '#value' => $hat->hid,
    );
    drupal_set_title(t('Edit hat: @label', array(
      '@label' => $hat->label,
    )));
  }

  $form['label'] = array(
    '#type' => 'textfield',
    '#title' => t('Hat name'),
    '#required' => TRUE,
    '#default_value' => isset($hat->label) ? $hat->label : '',
  );

  $form['name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Hat machine name'),
    '#default_value' => isset($hat->name) ? $hat->name : '',
    '#description' => t('A unique, machine readable name for the hat'),
    '#machine_name' => array(
      'exists' => 'party_hat_hat_machine_name_exists',
      'source' => array('label'),
    ),
    '#disabled' => isset($hat->name),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => isset($hat->description) ? $hat->description : '',
  );

  $parents = party_hat_get_tree();
  $descendants = isset($hat->name) ? party_hat_get_tree($hat->name) : array();
  $options = array(
    '' => t('--No Parent--'),
  );
  foreach ($parents as $parent) {
    // Don't include this hat in the options.
    if (isset($hat->name) && $parent->name == $hat->name) {
      continue;
    }

    // Don't include hats that are descendents of this hat in the options.
    if (isset($descendants[$parent->name])) {
      continue;
    }

    $options[$parent->name] = str_repeat('-', $parent->depth) . $parent->label;
  }

  $form['parent'] = array(
    '#type' => 'select',
    '#title' => t('Parent'),
    '#options' => $options,
    '#default_value' => isset($hat->parent) ? $hat->parent : NULL,
    '#description' => t('The parent hat for this hat.'),
  );

  field_attach_form('party_hat', $hat, $form, $form_state);

  $form['required'] = array(
    '#type' => 'checkbox',
    '#title' => t('Required'),
    '#default_value' => (isset($hat->required)) ? $hat->required : 0,
    '#description' => 'If enabled, all parties require this hat.',
    '#weight' => 1,
  );

  $form['data_set_rules'] = array(
    '#type' => 'fieldset',
    '#title' => t('Data rules'),
    '#description' => 'Set rules for which data set these hats give.',
    '#weight' => 3,
    '#tree' => TRUE,
    '#theme' => 'crm_hat_data_set_rules_form',
  );

  foreach (party_get_data_set_info() as $data_set_name => $data_set) {
    // Get the rule.
    if (isset($hat->name)) {
      $default = party_hat_get_data_set_rule($hat, $data_set_name);
      foreach ($default as $key => $var) {
        if ($var == 1) {
          $default[$key] = $key;
        }
      }
    }
    else {
      $default = array('has' => 0);
    }

    $form['data_set_rules'][$data_set_name] = array(
      '#type' => 'checkboxes',
      '#title' => check_plain($data_set['label']),
      '#default_value' => $default,
      '#options' => array(
        // We have to use different keys to what the access hook expects for
        // FormAPI radio values to work.
        'has'   => t('Allowed'),
      ),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 99,
  );

  return $form;
}

/**
 * Theme function for the data set rules form fieldset.
 */
function theme_crm_hat_data_set_rules_form($variables) {
  $form = $variables['form'];

  $output = '';
  $rows = array();
  foreach (element_children($form) as $data_set) {
    $row = array();
    $row[] = $form[$data_set]['#title'];
    foreach (array_keys($form[$data_set]['#options']) as $radio_key) {
      // Remove the checkbox label; it's shown in the table header.
      unset($form[$data_set][$radio_key]['#title']);
      $row[] = drupal_render($form[$data_set][$radio_key]);
    }

    $rows[] = $row;
  }

  $header = array_merge(array(t('Data set')), $form[$data_set]['#options']);
  $output .= theme('table', array('rows' => $rows, 'header' => $header));

  return $output;
}

/**
 * Form validation for the hat edit form.
 */
function party_hat_form_validate($form, &$form_state) {
  $pseudo_entity = (object) $form_state['values'];
  field_attach_form_validate('party_hat', $pseudo_entity, $form, $form_state);
}

/**
 * Form submission for the hat edit form.
 */
function party_hat_form_submit($form, &$form_state) {
  // Build the entity and save it.
  $hat = entity_ui_form_submit_build_entity($form, $form_state);

  // Set the data set rules in the hat.
  party_hat_set_data_set_rules($hat, $form_state['values']['data_set_rules']);

  party_hat_save($hat);

  field_attach_submit('party_hat', $hat, $form, $form_state);

  drupal_set_message(t("Party hat has been saved."));

  // If editing an existing hat, redirect to the hat admin page.
  if ($form_state['build_info']['args'][1] == 'edit') {
    $form_state['redirect'] = 'admin/community/hats';
  }
}

/**
 * Helper to check whether a machine name already exists.
 */
function party_hat_hat_machine_name_exists($value) {
  $exists = db_query_range('SELECT 1 FROM {party_hat} WHERE `name` = :name', 0, 1, array(':name' => $value))->fetchField();
  return $exists;
}
