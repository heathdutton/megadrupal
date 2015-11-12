<?php

/**
 * Entity sky fields form.
 */
function sky_field_sky_field_form($form, &$form_state, $entity_type, $entity) {
  $form_state['wrapper_id'] = 'sky-field-sky-field-form';
  $form_state['entity_type'] = $entity_type;
  $form_state['entity'] = $entity;
  $sky_fields = sky_field_get_sky_fields($entity_type, $entity);
  $form_state['sky_fields'] = $sky_fields;
  $entity_edit_page = l(t('Go to edit page'), sky_field_get_menu_basic_path($entity_type) . '/' . sky_field_get_entity_id($entity_type, $entity) . '/edit');
  $form['description'] = array(
    '#type' => 'markup',
    '#markup' => t('Configure Sky Fields here. Then set values on !link.', array('!link' => $entity_edit_page)),
  );
  // Building node fields form.
  $form['sky_fields'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'id' => $form_state['wrapper_id'],
    ),
    '#tree' => TRUE,
    'fields' => array(
      '#tree' => TRUE,
    ),
  );
  $form['sky_fields']['#theme'] = 'sky_field_sky_field_page_form_fields';
  // Add existing fields form elems.
  _sky_field_sky_field_page_fields_form($form, $form_state, $sky_fields);
  // Add new field form elems.
  _sky_field_sky_field_page_new_field_form($form, $form_state);
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler on "Save" button.
 */
function sky_field_sky_field_form_submit($form, &$form_state) {
  // Update existing fields
  if (isset($form_state['values']['sky_fields']['fields'])) {
    foreach ($form_state['values']['sky_fields']['fields'] as $field) {
      sky_field_update_sky_field($field);
    }
  }
  // Create new field
  $new_sky_field = $form_state['values']['sky_fields']['new'];
  if (!empty($new_sky_field['title'])) {
    // Add default values.
    $new_sky_field += array(
      'entity_id' => sky_field_get_entity_id($form_state['entity_type'], $form_state['entity']),
      'entity_type' => $form_state['entity_type'],
      'show_title' => TRUE,
    );
    if (sky_field_create_sky_field($new_sky_field)) {
      drupal_set_message(t('Field !name successfuly created.', array('!name' => $new_sky_field['title'])));
    }
    else {
      drupal_set_message(t('Field !name wasn\'t created.', array('!name' => $new_sky_field['title'])), 'error');
    }
  }
}

/**
 * Fields form.
 */
function _sky_field_sky_field_page_fields_form(&$form, &$form_state, $sky_fields) {
  if (empty($sky_fields)) {
    return;
  }
  // Building node field form.
  foreach ($sky_fields as $key => $sky_field) {
    $sky_field['key'] = $key;
    $field_form = _sky_field_sky_field_page_field_form($form_state, $sky_field);
    drupal_alter('sky_field_field_form', $field_form, $form_state);
    $form['sky_fields']['fields'][] = $field_form;
  }
}

function _sky_field_sky_field_page_field_form(&$form_state, $sky_field) {
  // Building node field form.
  $form = array(
    '#type' => 'fieldset',
    '#title' => $sky_field['title'],
  );
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $sky_field['id'],
  );
  $form['entity_id'] = array(
    '#type' => 'value',
    '#value' => $sky_field['entity_id'],
    '#access' => FALSE,
  );    
  $form['title'] = array(
    '#markup' => $sky_field['title'],
    '#title' => t('Title'),
    '#title_display' => 'invisible',
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#title_display' => 'invisible',
    '#default_value' => $sky_field['weight'],
    '#delta' => ceil(count($form_state['sky_fields'])/2),
  );
  $form['type'] = array(
    '#markup' => $sky_field['type'],
    '#title' => t('Type'),
    '#title_display' => 'invisible',
  );
  $form['operations'] = array(
    '#type' => 'container',
    '#title' => 'operations',
  );
  $destination = drupal_get_destination();
  $link_attributes = array(
    'query' => array(
      'destination' => $destination['destination'], 
    ),
  );
  $operations = array();
  $operations[] = l(t('edit'), sky_field_get_menu_basic_path($sky_field['entity_type']) . '/' . $sky_field['entity_id'] . '/sky-field/' . $sky_field['id'] . '/edit', $link_attributes);
  $operations[] = l(t('delete'), sky_field_get_menu_basic_path($sky_field['entity_type']) . '/' . $sky_field['entity_id'] . '/sky-field/' . $sky_field['id'] . '/delete', $link_attributes);
  $form['operations']['op'] = array(
    '#markup' => theme('item_list', array('items' => $operations)),
  );
  return $form;
}

function _sky_field_sky_field_page_new_field_form(&$form, $form_state) {
  //Building new field form
  $form['sky_fields']['new'] = array(
    '#tree' => TRUE,
  );
  $form['sky_fields']['new']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Add new field'),
    '#description' => t('Label'),
  );
  $count_field = isset($form['sky_fields']['fields']) ? count($form['sky_fields']['fields']) : 0;
  $weight_delta = ceil(count($count_field)/2);
  $form['sky_fields']['new']['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#title_display' => 'invisible',
    '#default_value' => $weight_delta,
    '#delta' => $weight_delta,
  );
  $form['sky_fields']['new']['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#title_display' => 'invisible',
    '#description' => t('Field type'),
    '#options' => sky_field_sky_field_types_options(),
  );
}

/**
 * Get sky field types options.
 *
 * @return
 *   Associative array of options.
 */
function sky_field_sky_field_types_options() {
  $options = array();
  // Populate sky field types options.
  $sky_field_types = sky_field_types_info();
  foreach ($sky_field_types as $sky_field_type) {
    $options[$sky_field_type['type']] = $sky_field_type['label'];
  }
  return $options;
}

/**
 * Delete sky_field form
 */
function sky_field_sky_field_delete_form($form, &$form_state, $entity_type, $entity, $sky_field) {
  $menu_basic_path = sky_field_get_menu_basic_path($entity_type);
  $destination = $menu_basic_path . '/' . sky_field_get_entity_id($entity_type, $entity) . '/sky-field';
  $question = t('Are you sure you want to delete sky field !title', array('!title' => $sky_field['title']));
  $form = confirm_form($form, $question, $destination);
  $form_state['sky_field'] = $sky_field;
  return $form;
}

function sky_field_sky_field_delete_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    sky_field_delete_sky_field($form_state['sky_field']);
  }
}

/**
* Entity sky field edit form
*/
function sky_field_sky_field_edit_form($form, &$form_state, $entity, $sky_field) {
  $form_state['sky_fields'][] = $sky_field;
  $form_state['entity'] = $entity;
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $sky_field['id'],
  );
  $form['entity_id'] = array(
    '#type' => 'value',
    '#value' => $sky_field['entity_id'],
    '#access' => FALSE,
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Label'),
    '#default_value' => $sky_field['title'],
  );
  $form['show_title'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show title'),
    '#title_display' => 'before',
    '#field_suffix' => t('Yes'),
    '#default_value' => isset($sky_field['show_title']) ? $sky_field['show_title'] : '',
  );
	$form['required'] = array(
		'#type' => 'checkbox',
		'#title' => t('Required'),
		'#title_display' => 'before',
		'#field_suffix' => t('Yes'),
		'#default_value' => isset($sky_field['required']) ? $sky_field['required'] : FALSE,
	);
  $form['settings'] = sky_field_sky_field_settings_form($sky_field);
  $form['settings']['#tree'] = TRUE;
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

/**
 * Submit handler on "Save" button.
 */
function sky_field_sky_field_edit_form_submit($form, &$form_state) {
  // Update field
  $field = $form_state['values'];
  sky_field_update_sky_field($field);
}