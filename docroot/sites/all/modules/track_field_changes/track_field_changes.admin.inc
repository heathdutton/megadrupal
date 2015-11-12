<?php

/**
 * @file
 * The Track Field Changes Admin.
 */

/**
 * Admin track field changes.
 */
function track_field_changes_admin_settings() {

  // When displaying the page, make sure the list of fields is up-to-date.
  field_info_cache_clear();

  $form = array();

  $options = NULL;
  $options = array(
    'checked' => t('Yes'),
  );

  $form['track_field_changes_new']['track_field_changes_disable_multiple'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Disable multiple records per node?'),
    '#description' => t('Only store one record per node in the database. Useful to prevent views results duplication.<br>Note that if were already tracking field changes before checking this box, you will need to remove the duplicate rows manually from the database.'),
    '#default_value' => variable_get('track_field_changes_disable_multiple', array()),
    '#options' => $options,
  );

  $form['settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['track_field_changes'] = array(
    '#type' => 'fieldset',
    '#title'       => t('Content Type'),
    '#group'       => 'settings',
  );

  // Gather node type information.
  $node_types = node_type_get_names();

  $form['track_field_changes']['track_field_changes_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Enable field audit on content types.'),
    '#description' => t('Enable field audit on content types.'),
    '#default_value' => variable_get('track_field_changes_node_types', array()),
    '#options' => $node_types,
  );

  $types = variable_get('track_field_changes_node_types', array());

  foreach ($types as $key => $type) {

    if ($type) {
      $form['track_field_changes_' . $key] = array(
        '#type' => 'fieldset',
        '#title' => $node_types[$key],
        '#group'       => 'settings',
      );

      $fields = field_info_instances('node', $key);

      $selected_fields = track_field_changes_get_selected_field($key);

      $options = NULL;
      $options['title'] = t('Title');

      foreach ($fields as $field) {
        $options[$field['field_name']] = $field['label'];
      }

      $form['track_field_changes_' . $key]['track_field_changes_fields']['track_field_changes_enable_log_' . $key] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable Log'),
        '#description' => t('Enable log message.'),
        '#default_value' => variable_get('track_field_changes_enable_log_' . $key, FALSE),
      );

      $form['track_field_changes_' . $key]['track_field_changes_fields']['track_field_changes_basic_new_' . $key] = array(
        '#type' => 'checkbox',
        '#title' => t('Basic audit for created node'),
        '#description' => t('Record basic informations (timestamp, user and log) when a node is created.'),
        '#default_value' => variable_get('track_field_changes_basic_new_' . $key, FALSE),
      );

      $form['track_field_changes_' . $key]['track_field_changes_fields']['track_field_changes_basic_revision_' . $key] = array(
        '#type' => 'checkbox',
        '#title' => t('Basic audit for updated node'),
        '#description' => t('Record basic informations (timestamp, user and log) when a node is updated.'),
        '#default_value' => variable_get('track_field_changes_basic_revision_' . $key, FALSE),
      );

      $form['track_field_changes_' . $key]['track_field_changes_fields']['track_field_changes_track_revision_fields_' . $key] = array(
        '#type' => 'checkbox',
        '#title' => t('Track field changes for updated node'),
        '#description' => t('Enable fields audit on updated nodes. Each selected and amended field will be recorded.'),
        '#default_value' => variable_get('track_field_changes_track_revision_fields_' . $key, FALSE),
      );

      $form['track_field_changes_' . $key]['track_field_changes_fields'][$key] = array(
        '#type' => 'checkboxes',
        '#title' => t('Enable field audit'),
        '#description' => t('Select which fields need to be audited.'),
        '#default_value' => $selected_fields,
        '#options' => $options,
        '#states' => array(
          'invisible' => array(
            ':input[name="track_field_changes_track_revision_fields_' . $key . '"]' => array('checked' => FALSE),
          ),
        ),
      );

    };
  }

  $form['#submit'][] = 'track_field_changes_admin_settings_submit';

  return system_settings_form($form);

}

/**
 * Save track field settings.
 */
function track_field_changes_admin_settings_submit($form, &$form_state) {

  // Delete old values.
  db_delete('track_field_changes')->execute();

  $node_types = $form_state['values']['track_field_changes_node_types'];

  foreach ($node_types as $nt_key => $nt_value) {
    if ($nt_value) {

      if (isset($form_state['values'][$nt_key])) {
        $fields = $form_state['values'][$nt_key];
      }
      else {
        $fields = array();
      }

      foreach ($fields as $f_key => $f_value) {
        if ($f_value) {
          db_insert('track_field_changes')
            ->fields(array(
            'field_name' => $f_key,
            'entity_type' => 'node',
            'bundle' => $nt_key,
          ))
          ->execute();
        }
      }
    }
  }

  drupal_theme_rebuild();
}
