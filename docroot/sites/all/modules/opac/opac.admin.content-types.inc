<?php
/**
 * @file
 * This file contains configuration forms for opac content types.
 */

/**
 * Form constructor for opac content types overview form.
 *
 * @param object $opac_server
 *   ILS server.
 *
 * @see opac_admin_content_types_overview_form_submit()
 * @ingroup forms
 */
function opac_admin_content_types_overview_form($form, &$form_state, $opac_server) {
  module_load_include('inc', 'opac', "includes/opac.db");

  // Get all content types for $opac_server.
  $types = node_type_get_types();
  $opac_types = opac_get_content_types($opac_server->serv_id);

  // Get connector fields.
  $connector = opac_get_instance($opac_server);
  $connector_fields = $connector->biblioFields();

  foreach ($opac_types as $machine_name => $opac_type) {
    $opac_types[$machine_name]['name'] = $types[$machine_name]->name;
    $opac_types[$machine_name]['field_label'] = $connector_fields[$opac_type['matching_field']]['label'];
  }
  $form['serv_id'] = array(
    '#type' => 'hidden',
    '#value' => $opac_server->serv_id,
  );
  $form['add'] = array(
    '#type' => 'link',
    '#title' => t('+ Add content type', array(), array('context' => 'libraries')),
    '#href' => 'admin/config/opac/servers/manage/' . $opac_server->serv_id . '/content-types/add',
    '#weight' => -15,
  );
  $form['markup'] = array(
    '#markup' => '<br /><br />',
  );

  $form['#tree'] = TRUE;
  foreach ($opac_types as $opac_type) {
    $key = $opac_type['content_type'];
    $form[$key]['content_type'] = array('#markup' => check_plain($opac_type['name']));
    $form[$key]['matching_field'] = array('#markup' => check_plain($opac_type['field_label']));
    $form[$key]['value'] = array('#markup' => check_plain($opac_type['value']));

    $form[$key]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight for @title', array('@title' => $opac_type['content_type'])),
      '#title_display' => 'invisible',
      '#delta' => 10,
      '#default_value' => $opac_type['weight'],
    );
    $form[$key]['edit'] = array(
      '#type' => 'link',
      '#title' => t('Edit'),
      '#href' => 'admin/config/opac/servers/manage/' . $opac_server->serv_id . '/content-types/' . $opac_type['content_type'] . '/edit',
    );
    $form[$key]['mapping'] = array(
      '#type' => 'link',
      '#title' => t('Mapping', array(), array('context' => 'libraries')),
      '#href' => 'admin/config/opac/servers/manage/' . $opac_server->serv_id . '/biblio-mapping/' . $opac_type['content_type'],
    );
    $form[$key]['delete'] = array(
      '#type' => 'link',
      '#title' => t('Delete'),
      '#href' => 'admin/config/opac/servers/manage/' . $opac_server->serv_id . '/content-types/' . $opac_type['content_type'] . '/delete',
    );
  }

  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'));

  return $form;
}

/**
 * Form submission handler for opac_admin_content_types_overview_form().
 */
function opac_admin_content_types_overview_form_submit($form, &$form_state) {
  module_load_include('inc', 'opac', "includes/opac.db");
  $values = $form_state['values'];
  $serv_id = $values['serv_id'];
  // Loop through mapped fields to check their new weight.
  foreach (element_children($values) as $key) {
    if (isset($values[$key]['weight'])) {
      opac_content_type_mod_weight($serv_id, $key, $values[$key]['weight']);
    }
  }
}

/**
 * Form constructor for opac content types edit form.
 *
 * @param object $opac_server
 *   ILS server.
 *
 * @param string $machine_name
 *   Content type machine name.
 *
 * @see opac_content_type_edit_form_submit()
 * @ingroup forms
 */
function opac_content_type_edit_form($form, $form_state, $opac_server, $machine_name = NULL) {
  $types = node_type_get_types();
  $types_options = array('' => 'None');
  foreach ($types as $id => $type) {
    $types_options[$id] = $type->name;
  }

  // Get connector fields.
  $connector = opac_get_instance($opac_server);
  $connector_fields = $connector->biblioFields();
  $matching_fields = array('' => 'None');
  foreach ($connector_fields as $name => $field) {
    $matching_fields[$name] = $field['label'];
  }

  $opac_types = array();
  $default_matching_field = '';
  $default_value = '';
  $default_weight = '';
  if ($machine_name) {
    module_load_include('inc', 'opac', "includes/opac.db");
    $opac_types = opac_get_content_types($opac_server->serv_id);
    $default_matching_field = $opac_types[$machine_name]['matching_field'];
    $default_value = $opac_types[$machine_name]['value'];
    $default_weight = $opac_types[$machine_name]['weight'];
    $form['edit'] = array('#type' => 'hidden', '#value' => 1);
  }

  $form['serv_id'] = array('#type' => 'hidden', '#value' => $opac_server->serv_id);
  $form['content_type'] = array(
    '#type' => 'select',
    '#title' => t('Content type'),
    '#options' => $types_options,
    '#default_value' => $machine_name ? $machine_name : '',
    '#description' => t('Select a content type for @opac_server server.', array('@opac_server' => $opac_server->serv_name)),
    '#required' => TRUE,
    '#disabled' => $machine_name ? TRUE : FALSE,
  );

  $form['matching_field'] = array(
    '#type' => 'select',
    '#title' => t("Matching field", array(), array('context' => 'libraries')),
    '#description' => t("The field which will be used to match this content type", array(), array('context' => 'libraries')),
    // Retrieve all fields for opac_node_type value changes and then
    // the form is rebuilt during ajax processing.
    '#options' => $matching_fields,
    '#default_value' => $default_matching_field ? $default_matching_field : '',
    '#required' => TRUE,
    '#prefix' => '<div id="replace_fields_choice_div">',
    '#suffix' => '</div>',
  );
  $form['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Field value(s)', array(), array('context' => 'libraries')),
    '#description' => t("Enter here value(s) that should match the selected field."),
    '#default_value' => $default_value ? $default_value : '',
    '#size' => 60,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $default_weight ? $default_weight : 0,
    '#delta' => 10,
  );
  $form['save'] = array('#type' => 'submit', '#value' => t('Save'));
  return $form;
}

/**
 * Form submission handler for opac_content_type_edit_form().
 */
function opac_content_type_edit_form_submit($form, $form_state) {
  module_load_include('inc', 'opac', "includes/opac.db");
  $values = $form_state['values'];
  $serv_id = $values['serv_id'];

  if (isset($values['edit'])) {
    // Change a content type.
    opac_content_type_edit($values);
  }
  else {
    // Save a new content type.
    opac_content_type_add($values);
  }

  drupal_goto("admin/config/opac/servers/manage/$serv_id/content-types");
}

/**
 * Form confirmation for delete opac content type.
 *
 * @param object $opac_server
 *   ILS server.
 *
 * @param string $machine_name
 *   Content type machine name.
 *
 * @see opac_content_type_delete_confirm_submit()
 * @ingroup forms
 */
function opac_content_type_delete_confirm($form, &$form_state, $opac_server, $machine_name) {
  $form['serv_id'] = array(
    '#type' => 'value',
    '#value' => $opac_server->serv_id,
  );
  $form['content_type'] = array(
    '#type' => 'value',
    '#value' => $machine_name,
  );

  return confirm_form($form,
  t('Are you sure you want to delete this content type?'),
  isset($_GET['destination']) ? $_GET['destination'] : "admin/config/opac/servers/manage/$opac_server->serv_id/content-types",
  t('This action cannot be undone.'),
  t('Delete'),
  t('Cancel'));
}

/**
 * Form submission handler for opac_content_type_delete_confirm().
 */
function opac_content_type_delete_confirm_submit($form, &$form_state) {
  $values = $form_state['values'];
  $serv_id = $values['serv_id'];
  if ($form_state['values']['confirm']) {
    module_load_include('inc', 'opac', "includes/opac.db");
    opac_content_type_delete($values);
    drupal_set_message(t('Content type has been deleted successfully.'));
  }
  drupal_goto("admin/config/opac/servers/manage/$serv_id/content-types");
}
