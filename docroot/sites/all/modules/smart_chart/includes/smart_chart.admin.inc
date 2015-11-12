<?php

/**
 * @file
 * Admin forms for Smart Chart.
 */

/**
 * Implements hook_form().
 */
function smart_chart_settings_form($form, &$form_state) {

  // Get content types that don't and do have a connection
  // entity reference.
  $file_path = module_load_include('inc', 'smart_chart',
      'includes/smart_chart.functions');
  $types_available = sc_get_chart_item_types(TRUE);
  $types_selected = sc_get_chart_item_types();

  $form['header_add'] = array(
    '#type' => 'item',
    '#markup' => t('Add content types'),
    '#attributes' => array('class' => array('header')),
  );

  if (count($types_available)) {
    // Select new content type to add connection to.
    $form['types_available'] = array(
      '#type' => 'select',
      '#title' => t('Content types'),
      '#options' => $types_available,
      '#description' => t('Select content type to add connection entity reference to.'),
    );

    // Add type.
    $form['add_type'] = array(
      '#type' => 'submit',
      '#value' => t('Add'),
      '#submit' => array('_smart_chart_add_type'),
    );
  }
  else {
    $form['no_types_available'] = array(
      '#type' => 'item',
      '#markup' => t('No content types can be added.'),
      '#attributes' => array('class' => array('header')),
    );
  }

  $form['header_remove'] = array(
    '#type' => 'item',
    '#markup' => t('Remove content types'),
    '#attributes' => array('class' => array('header')),
  );

  // Selected types.
  $form['types_selected_container'] = array(
    '#type' => 'container',
    '#attributes' => array('class' => array('chart-types-list')),
  );

  foreach ($types_selected as $key => $type) {
    // Title.
    $form['types_selected_container']['_title_' . $key] = array(
      '#type' => 'markup',
      '#markup' => $type,
      '#attributes' => array('class' => array('chart-types-title')),
    );

    // Remove.
    $form['types_selected_container']['_remove_' . $key] = array(
      '#type' => 'submit',
      '#value' => t('Remove'),
      '#submit' => array('_smart_chart_remove_type'),
      '#attributes' => array('class' => array('chart-types-button')),
    );
  }

  return $form;
}

/**
 * Add content type to chart.
 */
function _smart_chart_add_type($form, &$form_state) {
  // Get selected content type.
  $type = $form_state['input']['types_available'];

  $file_path = module_load_include('inc', 'smart_chart', 'includes/smart_chart.functions');
  sc_add_type($type);
}

/**
 * Remove content type from chart.
 */
function _smart_chart_remove_type($form, &$form_state) {
  // Get Content Type to be removed.
  $type = str_replace('-', '_', str_replace('edit-remove-', '', $form_state['triggering_element']['#id']));

  $file_path = module_load_include('inc', 'smart_chart', 'includes/smart_chart.functions');
  sc_remove_type($type);

  drupal_set_message(t('Deleted Smart Connections of removed bundle.'));
}
