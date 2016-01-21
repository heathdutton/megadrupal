<?php

/**
 *  Admin settings form builder.
 */
function ctools_automodal_admin_admin_settings_form($form, &$form_state) {
  $form['content_types_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Modal content types'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $modal_content_types = variable_get('ctools_automodal_admin_modal_content_types', array());
  $node_types = node_type_get_types();
  $node_types_options = array();
  foreach ($node_types as $type) {
    $node_types_options[$type->type] = $type->name;
    if (isset($modal_content_types[$type->type])) {
      $modal_content_types[$type->type] = $type->type;
    }
  }
  $form['content_types_fieldset']['modal_content_types'] = array(
    '#title' => t('Modal content types'),
    '#type' => 'checkboxes',
    '#options' => $node_types_options,
    '#default_value' => $modal_content_types,
    '#description' => t('Choose which content types add forms should be modal.'),
  );
  $extra_modal_links = variable_get('ctools_automodal_admin_extra_paths', array());
  $form['extra_modal_links'] = array(
    '#type' => 'textarea',
    '#title' => t('Extra modal links'),
    '#default_value' => implode($extra_modal_links, "\n"),
    '#description' => t('Enter hook_menu paths here. Each path should take one line.'),
    '#rows' => 20
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Admin settings form submit.
 */
function ctools_automodal_admin_admin_settings_form_submit(&$form, $form_state) {
  $values = $form_state['values'];
  // Save modal content types.
  $modal_content_types = array();
  $values_modal_content_types = $form_state['values']['modal_content_types'];
  foreach ($values_modal_content_types as $key => $item) {
    if ($item) {
      $type_url_str = 'node/add/' . str_replace('_', '-', $key);
      $modal_content_types[$key] = $type_url_str;
    }
  }
  variable_set('ctools_automodal_admin_modal_content_types', $modal_content_types);

  // Save extra modal paths.
  $extra_modal_paths = array();
  $values_extra_modal_paths = preg_split('/\n\r|\r\n|\n|\r/', $values['extra_modal_links']);
  foreach ($values_extra_modal_paths as $item) {
    $item = trim($item);
    if (($item !== '') && !in_array($item, $extra_modal_paths) && !in_array($item, $modal_content_types)) {
      $extra_modal_paths[] = $item;
    }
  }
  variable_set('ctools_automodal_admin_extra_paths', $extra_modal_paths);

  // Flush cache to update menu hooks.
  drupal_flush_all_caches();
}
