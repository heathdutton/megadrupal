<?php

/**
 * @file
 * Administration file for the content nodes module, containing the
 * config methods for the module.
 */

/**
 * Form builder for the settings form.
 */
function content_nodes_settings_form($form, &$form_state) {
  $form['content_nodes']['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('UI Integration'),
  );
  // Settings for alter admin/content form.
  $form['content_nodes']['settings']['content_nodes_alter_content_admin_form'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('content_nodes_alter_content_admin_form', TRUE),
    '#title' => t('Alter admin content form.'),
    '#description' => t('Add "edit content" link to page !link. Also edit link will renamed to "edit base".', array('!link' => l(t('admin/content'), 'admin/content'))),
  );
  // Settings for alter default edit link title.
  $form['content_nodes']['settings']['content_nodes_alter_edit_link_title'] = array(
    '#type' => 'checkbox',
    '#default_value' => variable_get('content_nodes_alter_edit_link_title', TRUE),
    '#title' => t('Alter default edit node title.'),
    '#description' => t('The default edit node title will is to "edit base". Clear cache is needed after change.'),
  );

  return system_settings_form($form);
}

/**
 * Form builder for the nodes configuration form.
 */
function content_nodes_nodes_form($form, &$form_state) {

  $node_types = node_type_get_types();
  uasort($node_types, '_content_nodes_sort_nodes_by_name');

  $table_rows = array();

  // Build the table with existing content node fields.
  foreach ($node_types as $node_type) {
    $fields = field_info_instances("node", $node_type->type);
    // Search fields with prefix 'content_nodes_'.
    foreach ($fields as $key => $field) {
      if (strpos($key, 'content_nodes_') === 0) {
        $table_rows[] = array(
          l($node_type->name, sprintf('admin/structure/types/manage/%s/fields', $node_type->type)),
          $field['label'] . ' (' . $key . ')',
          l(
            t('Configure'),
            sprintf('admin/structure/types/manage/%s/fields/%s', $node_type->type, $key),
            array('query' => array('destination' => 'admin/config/content/content_nodes/nodes'))
          ),
          l(
            t('Delete field'),
            sprintf('admin/structure/types/manage/%s/fields/%s/delete', $node_type->type, $key),
            array('query' => array('destination' => 'admin/config/content/content_nodes/nodes'))
          ),
        );
      }
    }
  }

  if (count($table_rows) > 0) {
    $form['enabled'] = array(
      '#type' => 'fieldset',
      '#title' => t('Content Nodes fields'),
    );
    $header = array(
      t('Content Type'),
      t('Fieldname'),
      array('data' => t('Operations'), 'colspan' => 2),
    );
    $form['enabled'][] = array(
      '#markup' => theme('table', array('header' => $header, 'rows' => $table_rows)),
    );
  }

  if (count($node_types) > 0) {
    $form['create'] = array(
      '#type' => 'fieldset',
      '#title' => t('Create new Content Nodes field'),
    );

    foreach ($node_types as $node_type) {
      $options[$node_type->type] = $node_type->name;
    }
    $form['create']['node_type'] = array(
      '#type' => 'select',
      '#options' => $options,
      '#title' => t('Select node types, to attach a new "Content Nodes" field'),
      '#description' => t('A new "Content Nodes" will add to the selected content type. After this, go to the field configuration and enable the content types you want to allow to add in node edit form.'),
      '#required' => TRUE,
    );
    $form['create']['field_label'] = array(
      '#type' => 'textfield',
      '#title' => t('Label'),
      '#required' => TRUE,
    );
    $form['create']['field_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Machine name'),
      '#required' => TRUE,
      '#maxlength' => 25,
    );
  }

  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Enable content nodes'),
  );

  return $form;
}

/**
 * Validation of the admin settings form.
 *
 * @see content_nodes_nodes_form()
 */
function content_nodes_nodes_form_validate($form, &$form_state) {
  if (!preg_match('~^[a-z0-9_]+$~', $form_state['values']['field_name'])) {
    form_set_error('field_name', t('The machine-readable name must contain only lowercase letters, numbers, and underscores.'));
  }
}

/**
 * Submit callback for settings form.
 *
 * @see content_nodes_settings_form()
 */
function content_nodes_nodes_form_submit($form, &$form_state) {
  // Save a new shortcut set with links copied from the user's default set.
  $values = $form_state['values'];
  $field_name = 'content_nodes_' . $values['field_name'];

  if (!empty($values['node_type'])) {
    $fields = field_info_instances("node", $values['node_type']);
    if (!isset($fields[$field_name])) {
      _content_nodes_install_field($field_name);
      $instance = array(
        'field_name' => $field_name,
        'bundle' => $values['node_type'],
        'entity_type' => 'node',
        'label' => $values['field_label'],
        'description' => t('Content nodes elements'),
        'required' => FALSE,
      );
      field_create_instance($instance);
      drupal_set_message(t('Content node field has been added for :node_type.', array(':node_type' => $values['node_type'])));
    }
    else {
      drupal_set_message(t('Content node field is already added for :node_type', array(':node_type' => $values['node_type'])), 'error');
    }
  }
  else {
    drupal_set_message(t('Please select a content type!'), 'error');
  }
  $form_state['redirect'] = 'admin/config/content/content_nodes/nodes';
}

/**
 * Create a new field for content nodes.
 *
 * @see content_nodes_settings_form_submit()
 */
function _content_nodes_install_field($name) {
  $field = _content_nodes_get_field_base($name);
  if (!field_info_field($field['field_name'])) {
    field_create_field($field);
  }
}

/**
 * Get the base field configuration for a content node field.
 */
function _content_nodes_get_field_base($name) {
  return array(
    'active' => 1,
    'cardinality' => -1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => $name,
    'foreign keys' => array(
      'node' => array(
        'columns' => array(
          'target_id' => 'nid',
        ),
        'table' => 'node',
      ),
    ),
    'indexes' => array(
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(),
      'target_type' => 'node',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );
}
