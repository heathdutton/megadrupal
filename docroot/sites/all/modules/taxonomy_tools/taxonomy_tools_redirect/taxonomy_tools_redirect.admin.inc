<?php

/**
 * @file
 * Administrative page callbacks for the Taxonomy Redirect module.
 */

/**
 * Build administration form for Taxonomy Redirect.
 *
 * @ingroup forms
 * @see taxonomy_tools_redirect_settings_submit()
 */
function taxonomy_tools_redirect_admin_form($form, &$form_state) {
  $options = array();
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vocabulary) {
    $options[$vocabulary->vid] = $vocabulary->name;
  }
  // List all vocabularies.
  $form['taxonomy_tools_redirect_config'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t('Vocabularies'),
    '#description' => t('Select vocabularies which will use Taxonomy Redirect'),
    '#default_value' => variable_get('taxonomy_tools_redirect_config', array()),
  );
  $rendered_form = system_settings_form($form);
  // Add additional submit callback.
  $rendered_form['#submit'][] = 'taxonomy_tools_redirect_settings_submit';
  return $rendered_form;
}

/**
 * Additional submit handler for Taxonomy Publisher administration form.
 *
 * @see taxonomy_tools_redirect_admin_form()
 */
function taxonomy_tools_redirect_settings_submit($form, &$form_state) {
  $config = variable_get('taxonomy_tools_redirect_config', array());
  if (!empty($config)) {
    $instances = taxonomy_tools_redirect_field_instances();
    foreach ($config as $vid => $setting) {
      $vmn = db_query("SELECT machine_name FROM {taxonomy_vocabulary} WHERE vid = :vid", array(':vid' => $vid))->fetchField();
      if ($setting > 0) {
        // We need to create field instances if they do not exist
        // for this vocabulary.
        foreach ($instances as $instance) {
          $instance['entity_type'] = 'taxonomy_term';
          $instance['bundle'] = $vmn;
          $instance_info = field_info_instance($instance['entity_type'], $instance['field_name'], $instance['bundle']);
          if (empty($instance_info)) {
            field_create_instance($instance);
          }
        }
      }
      else {
        // We have to delete field instances if they exist for this vocabulary.
        foreach ($instances as $instance) {
          $instance['entity_type'] = 'taxonomy_term';
          $instance['bundle'] = $vmn;
          $instance_info = field_info_instance($instance['entity_type'], $instance['field_name'], $instance['bundle']);
          if (!empty($instance_info)) {
            field_delete_instance($instance);
          }
        }
      }
    }
    // Renew field definitions if last instances were deleted.
    $fields = taxonomy_tools_redirect_fields();
    foreach ($fields as $field) {
      $field_info = field_info_field($field['field_name']);
      if (empty($field_info)) {
        field_create_field($field);
      }
    }
  }
}

/**
 * Create field definitions for Taxonomy Redirect specific fields.
 *
 * @return array
 *   An array of field definitions.
 */
function taxonomy_tools_redirect_fields() {
  $fields = array(
    'field_taxonomy_term_redirect' => array(
      'field_name' => 'field_taxonomy_term_redirect',
      'type' => 'text',
      'settings' => array(
        'max_length' => '255',
      ),
    ),
  );
  return $fields;
}

/**
 * Create field instance definitions for Taxonomy Redirect specific fields.
 *
 * @return array
 *   An array of field instances definitions.
 */
function taxonomy_tools_redirect_field_instances() {
  $instances = array(
    'field_taxonomy_term_redirect' => array(
      'label' => t('Taxonomy term redirect link'),
      'description' => t('Enter URL this taxonomy term redirects to. You can use internal paths, e.g., %term, %node, (URL aliases will not work) or external addresses, e.g., %example. Enter %front to redirect to front page.', array(
        '%term' => 'taxonomy/term/XXX',
        '%node' => 'node/XXX',
        '%example' => 'http://www.example.com',
        '%front' => '<front>')
      ),
      'widget' => array(
        'weight' => 21,
        'type' => 'text_textfield',
        'settings' => array(
          'size' => 60,
        ),
      ),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'type' => 'hidden',
        ),
      ),
      'field_name' => 'field_taxonomy_term_redirect',
    ),
  );
  return $instances;
}
