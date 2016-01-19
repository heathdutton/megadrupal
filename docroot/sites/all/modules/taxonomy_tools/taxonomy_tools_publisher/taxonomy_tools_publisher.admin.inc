<?php

/**
 * @file
 * Administrative page callbacks for the Taxonomy Publisher module.
 */

/**
 * Build administration form for Taxonomy Publisher.
 *
 * @ingroup forms
 * @see taxonomy_tools_publisher_settings_submit()
 */
function taxonomy_tools_publisher_admin_form($form, &$form_state) {
  $options = array();
  $vocabularies = taxonomy_get_vocabularies();
  foreach ($vocabularies as $vocabulary) {
    $options[$vocabulary->machine_name] = $vocabulary->name;
    // $options[$vocabulary->vid] = $vocabulary->name;
  }
  // List all vocabularies.
  $form['taxonomy_tools_publisher_config'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#title' => t('Vocabularies'),
    '#description' => t('Select vocabularies which will use Taxonomy Publisher'),
    '#default_value' => array_filter(variable_get('taxonomy_tools_publisher_config', array())),
  );
  // Automatic taxonomy term publishing.
  $form['taxonomy_tools_publisher_automatic_publishing'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable automatic publishing of taxonomy terms'),
    '#description' => t('If enabled, taxonomy term will be published when a node using this term is being published.'),
    '#default_value' => variable_get('taxonomy_tools_publisher_automatic_publishing', 0),
  );
  $rendered_form = system_settings_form($form);
  // Add additional submit callback.
  $rendered_form['#submit'][] = 'taxonomy_tools_publisher_settings_submit';
  return $rendered_form;
}

/**
 * Additional submit handler for Taxonomy Publisher administration form.
 *
 * @see taxonomy_tools_publisher_admin_form()
 */
function taxonomy_tools_publisher_settings_submit($form, &$form_state) {
  $config = variable_get('taxonomy_tools_publisher_config', array());
  if (!empty($config)) {
    $instances = taxonomy_tools_publisher_field_instances();
    foreach ($config as $vocabulary => $setting) {
      if ($setting === $vocabulary) {
        // We need to create field instances if they do not exist
        // for this vocabulary.
        foreach ($instances as $instance) {
          $instance['entity_type'] = 'taxonomy_term';
          $instance['bundle'] = $vocabulary;
          $instance_info = field_info_instance($instance['entity_type'], $instance['field_name'], $instance['bundle']);
          if (empty($instance_info)) {
            field_create_instance($instance);
          }
        }
        // Create field group.
        ctools_include('export');
        $group_info = db_query("SELECT * FROM {field_group} WHERE entity_type = 'taxonomy_term' AND bundle = :vmn", array(':vmn' => $vocabulary))->fetchAll();
        if (empty($group_info)) {
          $group = taxonomy_tools_publisher_field_group($vocabulary);
          field_group_group_save($group);
        }
      }
      else {
        // We have to delete field instances if they exist for this vocabulary.
        foreach ($instances as $instance) {
          $instance['entity_type'] = 'taxonomy_term';
          $instance['bundle'] = $vocabulary;
          $instance_info = field_info_instance($instance['entity_type'], $instance['field_name'], $instance['bundle']);
          if (!empty($instance_info)) {
            field_delete_instance($instance);
          }
        }
        // Delete field group.
        db_query("DELETE FROM {field_group} WHERE entity_type = 'taxonomy_term' AND bundle = :vmn", array(':vmn' => $vocabulary));
      }
    }
    // Renew field definitions if last instances were deleted.
    $fields = taxonomy_tools_publisher_fields();
    foreach ($fields as $field) {
      $field_info = field_info_field($field['field_name']);
      if (empty($field_info)) {
        field_create_field($field);
      }
    }
  }
}

/**
 * Create field definitions for Taxonomy Publisher specific fields.
 *
 * @return array
 *   An array of field definitions.
 */
function taxonomy_tools_publisher_fields() {
  $fields = array(
    'field_taxonomy_term_status' => array(
      'field_name' => 'field_taxonomy_term_status',
      'type' => 'list_boolean',
      'settings' => array(
        'allowed_values' => array(
          0 => '',
          1 => '',
        ),
      ),
    ),
    'field_taxonomy_term_publish_on' => array(
      'field_name' => 'field_taxonomy_term_publish_on',
      'type' => 'datestamp',
      'settings' => array(
        'granularity' => array(
          'year' => 'year',
          'month' => 'month',
          'day' => 'day',
          'hour' => 'hour',
          'minute' => 'minute',
          'second' => 'second',
        ),
        'tz_handling' => 'none',
      ),
    ),
    'field_taxonomy_term_unpublish_on' => array(
      'field_name' => 'field_taxonomy_term_unpublish_on',
      'type' => 'datestamp',
      'settings' => array(
        'granularity' => array(
          'year' => 'year',
          'month' => 'month',
          'day' => 'day',
          'hour' => 'hour',
          'minute' => 'minute',
          'second' => 'second',
        ),
        'tz_handling' => 'none',
      ),
    ),
  );
  return $fields;
}

/**
 * Create field instance definitions for Taxonomy Publisher specific fields.
 *
 * @return array
 *   An array of field instances definitions.
 */
function taxonomy_tools_publisher_field_instances() {
  $instances = array(
    'field_taxonomy_term_status' => array(
      'label' => 'Published',
      'widget' => array(
        'weight' => 20.1,
        'type' => 'options_onoff',
        'settings' => array(
          'display_label' => 1,
        ),
      ),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'type' => 'hidden',
        ),
      ),
      'default_value' => array(
        0 => array(
          'value' => 1,
        ),
      ),
      'field_name' => 'field_taxonomy_term_status',
    ),
    'field_taxonomy_term_publish_on' => array(
      'label' => 'Publish on:',
      'widget' => array(
        'weight' => 20.2,
        'type' => 'date_popup',
        'settings' => array(
          'input_format' => 'd.m.Y - H:i:s',
          'increment' => 1,
        ),
      ),
      'settings' => array(
        'default_value' => 'blank',
      ),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'type' => 'hidden',
        ),
      ),
      'field_name' => 'field_taxonomy_term_publish_on',
    ),
    'field_taxonomy_term_unpublish_on' => array(
      'label' => 'Unpublish on:',
      'widget' => array(
        'weight' => 20.3,
        'type' => 'date_popup',
        'settings' => array(
          'input_format' => 'd.m.Y - H:i:s',
          'increment' => 1,
        ),
      ),
      'settings' => array(
        'default_value' => 'blank',
      ),
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'type' => 'hidden',
        ),
      ),
      'field_name' => 'field_taxonomy_term_unpublish_on',
    ),
  );
  return $instances;
}

/**
 * Create field group definition for Taxonomy Publisher fields.
 *
 * @param string $bundle
 *   A string containing bundle name for which the field group is created.
 *
 * @return stdClass
 *   A field group object.
 */
function taxonomy_tools_publisher_field_group($bundle) {
  $group = new stdClass();
  $group->label = t('Publishing settings');
  $group->weight = 20;
  $group->format_type = 'fieldset';
  $group->group_name = 'group_taxonomy_term_publishing';
  $group->entity_type = 'taxonomy_term';
  $group->bundle = $bundle;
  $group->mode = 'form';
  $group->identifier = 'group_taxonomy_term_publishing|taxonomy_term|' . $bundle . '|form';
  $group->children = array(
    0 => 'field_taxonomy_term_status',
    1 => 'field_taxonomy_term_publish_on',
    2 => 'field_taxonomy_term_unpublish_on',
  );
  $group->format_settings = array(
    'formatter' => 'collapsed',
    'label' => t('Publishing settings'),
    'instance_settings' => array(
      'required_fields' => 0,
      'classes' => '',
      'description' => '',
    ),
  );
  return $group;
}
