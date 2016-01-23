<?php

/**
 * @file
 * Admin settings file for update_external_links module.
 */

/**
 * Form constructor for update_external_links_input_form.
 */
function update_external_links_input_form($form, &$form_state) {
  $content_types = node_type_get_names();
  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => '<p><strong>' . t('Please select content type you want to update for external links to be opened in new tab.') . '</strong></p>',
  );
  foreach ($content_types as $key => $value) {
    $form[$key] = array(
      '#type' => 'checkbox',
      '#title' => $value,
      '#options' => $value,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update External Links'),
  );

  return $form;
}

/**
 * Form validate for update_external_links_input_form.
 */
function update_external_links_input_form_validate($form, &$form_state) {
  foreach ($form_state['values'] as $key => $values) {
    if ($values == 1) {
      $content_types[] = $key;
    }
  }
  if (empty($content_types)) {
    form_set_error('select_content_types', t('Seems you have not selected any content type. Please select atleast one content type you want to update first.'));
  }
}

/**
 * Submission handler for update_external_links_input_form.
 */
function update_external_links_input_form_submit($form, &$form_state) {
  $selected_content_types = array();
  foreach ($form_state['values'] as $key => $values) {
    if ($values == 1) {
      $selected_content_types[] = $key;
    }
  }
  $node_ids = update_external_links_selected_nid($selected_content_types);
  if (empty($node_ids)) {
    drupal_set_message(t("Seems You don't currently have any nodes with the selected content types."));
  }
  // Execute the function named update_external_links_batch_start.
  else {
    $batch = update_external_links_batch_start($node_ids);
    batch_set($batch);
  }
}

/**
 * Utility function - simply queries and loads all nid of selected content type.
 */
function update_external_links_selected_nid($content_types_name) {
  $query = db_select('node', 'n');
  $query->innerjoin('field_data_body', 'b', 'n.nid = b.entity_id');
  $query->fields('n', array('nid'))->condition('b.body_value', '', '!=')->condition('type', array($content_types_name))->orderBy('n.nid', 'ASC');
  $nid = $query->execute()->fetchCol();
  return $nid;
}
