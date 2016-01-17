<?php

/**
 * @file
 * Admin page callback file for the party module.
 */

/**
 * Page callback for managing data sets.
 *
 * @todo: Get links on this page to always redirect back to this page.
 */
function party_data_set_admin() {
  $data_sets = party_get_data_set_info();

  $header = array(
    0 => t('Data set name'),
    1 => array(
      'data' => t('Operations'),
      'colspan' => '6',
    ),
  );

  $rows = array();
  foreach ($data_sets as $key => $data_set) {
    $name = check_plain($data_set['label']);
    $name .= ' <small>' . t('(Machine name: @set-name)', array('@set-name' => $key)) . '</small>';
    $ops = array('edit', 'manage fields', 'manage display', 'clone', 'export', 'delete');

    $row = array($name);
    foreach ($ops as $op) {
      $row[] = array(
        'data' => isset($data_set['admin'][$op]) ? l($op, $data_set['admin'][$op], array('query' => array('destination' => 'admin/community/datasets'))) : '',
      );
    }

    $rows[] = $row;
  }

  $build['dataset_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No data sets available'),
  );

  return $build;
}

/**
 * Settings form for ordering the party pieces.
 */
function party_settings_pieces_order_form($form, &$form_state) {
  // Get the pieces. These come in with the stored weight setting already.
  $pieces = party_get_party_piece_info();

  $form['#tree'] = TRUE;
  foreach ($pieces as $path => $piece) {
    $form['pieces'][$path]['name'] = array('#markup' => check_plain($piece['title']));
    $form['pieces'][$path]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $piece['title'])),
      '#title_display' => 'invisible',
      '#delta' => 10,
      '#default_value' => isset($piece['weight']) ? $piece['weight'] : 0,
    );
  }

  // Include a submit button and weight if more than one piece exists.
  if (count($pieces) > 1) {
    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array('#type' => 'submit', '#value' => t('Save'));
  }
  elseif (isset($piece)) {
    unset($form['pieces'][$path]['weight']);
  }

  return $form;
}

/**
 * Themes the pieces overview form as a sortable list of pieces.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @see party_settings_pieces_order_form()
 * @ingroup themeable
 */
function theme_party_settings_pieces_order_form($variables) {
  $form = $variables['form'];

  $rows = array();

  foreach (element_children($form['pieces']) as $key) {
    $piece = &$form['pieces'][$key];

    $row = array();
    $row[] = drupal_render($piece['name']);
    if (isset($piece['weight'])) {
      $piece['weight']['#attributes']['class'] = array('piece-weight');
      $row[] = drupal_render($piece['weight']);
    }
    $rows[] = array('data' => $row, 'class' => array('draggable'));
  }

  $header = array(t('Piece label'));
  if (isset($form['actions'])) {
    $header[] = t('Weight');
    drupal_add_tabledrag('crm-party-pieces-order', 'order', 'sibling', 'piece-weight');
  }
  return theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => array('id' => 'crm-party-pieces-order'))
  ) . drupal_render_children($form);
}

/**
 * Submit handler for the party pieces order form.
 */
function party_settings_pieces_order_form_submit($form, &$form_state) {
  $settings = array();
  foreach (array_keys($form_state['values']['pieces']) as $path) {
    $settings[$path] = $form_state['values']['pieces'][$path]['weight'];
  }

  variable_set('party_name_pieces_weights', $settings);
  menu_rebuild();
}

/**
 * Menu callback of development information.
 */
function party_devel_info_page() {
  $info = party_get_data_set_info();
  $output = kprint_r($info, TRUE, t('Data set info'));
  $info = party_get_party_piece_info();
  $output .= kprint_r($info, TRUE, t('Party piece info'));
  if (module_exists('party_hat')) {
    $hats = party_hat_get_all_hats();
    $output .= kprint_r($hats, TRUE, t('Hats'));
  }
  return $output;
}

/**
 * Form constructor for setting primary field sources.
 */
function party_primary_fields_edit_field($form, &$form_state, $target) {
  // Get some information about this primary field.
  $party = entity_create_stub_entity('party', array(
    0 => NULL,
    2 => 'party',
  ));
  $wrapper = entity_metadata_wrapper('party', $party);
  $form['#submit'][] = 'party_primary_fields_edit_field_submit';
  $form['#target'] = $target;
  $form['#info'] = $wrapper->getPropertyInfo($target);
  $sources = PartyPrimaryFields::getFields($target);

  // Set the page title.
  drupal_set_title($form['#info']['label']);

  $form['sources'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sources'),
    '#target' => $target,
    '#parents' => array('sources'),
    '#default_value' => $sources,
    '#weight' => -1,
  );
  form_load_include($form_state, 'inc', 'party', 'party.primary_fields');
  PartyPrimaryFields::sourceForm($form['sources'], $form_state, $form);

  if ($target == 'email') {
    $form['options'] = array(
      '#type' => 'fieldset',
      '#title' => t('Settings'),
    );

    $form['options']['party_ensure_no_dup_emails'] = array(
      '#title' => t('Ensure that Primary Emails are not duplicated'),
      '#description' => t('When a Party is about to be saved with a primary email that already exists the email will be unset and a message shown to the user.'),
      '#type' => 'checkbox',
      '#default_value' => variable_get('party_ensure_no_dup_emails', FALSE),
    );
  }

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  return $form;
}

/**
 * Form submission handler for party_primary_fields_edit_field().
 */
function party_primary_fields_edit_field_submit(&$form, &$form_state) {
  // Get hold of our variable.
  $primary_fields = variable_get('party_primary_fields', array());

  // Merge in our new values and save.
  $primary_fields[$form['#target']] = $form_state['values']['sources'];
  variable_set('party_primary_fields', $primary_fields);

  // Deal with email specific options.
  if ($form['#target'] == 'email') {
    variable_set('party_ensure_no_dup_emails', (bool) $form_state['values']['party_ensure_no_dup_emails']);
  }

  drupal_set_message(t('Settings for primary field %target saved.', array(
    '%target' => $form['#info']['label'],
  )));

  $form_state['redirect'] = 'admin/community/party';
}

/**
 * Pre render callback for the party_primary_fields_source_table element.
 */
function party_primary_fields_source_table_pre_render($element) {
  // Build our table rows.
  foreach (element_children($element) as $key) {
    $element['#rows'][$key] = array(
      'class' => array('draggable'),
    );

    foreach (array_keys($element['#header']) as $field) {
      $element['#rows'][$key]['data'][$field] = drupal_render($element[$key][$field]);
    }
  }

  return $element;
}

/**
 * Submission handler for party_primary_fields_source_table().
 */
function party_primary_fields_source_table_source_cleanup($element, &$form_state, $form) {
  $values = &drupal_array_get_nested_value($form_state['values'], $element['#parents']);
  unset($values['remove']);
}

/**
 * Validation handler for the add source button.
 */
function party_primary_fields_source_table_validate(&$form, &$form_state) {
  // Get our element.
  $parents = array_slice($form_state['triggering_element']['#array_parents'], 0, -2);
  $element = drupal_array_get_nested_value($form, $parents);

  // Check we had a source selected..
  if (empty($form_state['values']['add_source_property'])) {
    form_error($element['add_source']['add_source_property'], t('You must select a property to add.'));
  }
}

/**
 * Submission handler for the add/remove source buttons.
 */
function party_primary_fields_source_table_submit(&$form, &$form_state) {
  // Get our element.
  $parents = array_slice($form_state['triggering_element']['#array_parents'], 0, -2);
  $element = drupal_array_get_nested_value($form, $parents);

  // Get our values
  $values = drupal_array_get_nested_value($form_state['values'], $element['#parents']);

  // If removing, clear the source item.
  if (end($form_state['triggering_element']['#parents']) == 'remove') {
    $key = $form_state['triggering_element']['#parents'][count($form_state['triggering_element']['#parents']) - 2];
    unset($values[$key]);
  }
  elseif ($form_state['triggering_element']['#parents'][0] == 'add_source_submit') {
    // Get hold of our new source.
    $key = $form_state['values']['add_source_property'];
    $source = array('weight' => 0);
    list($source['data_set'], $source['property'], $source['value']) = explode(':', $key . ':');

    // Insert the source
    $values[$key] = $source;
  }

  // Store our updated value.
  drupal_array_set_nested_value($form_state['values'], $element['#parents'], $values);
  $form_state['rebuild'] = TRUE;
}

/**
 * Submission handler to clear primary field caches.
 */
function party_primary_fields_clear_caches() {
  PartyPrimaryFields::clearCaches();
}
