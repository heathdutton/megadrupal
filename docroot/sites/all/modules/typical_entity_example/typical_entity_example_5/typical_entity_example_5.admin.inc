<?php

/**
 * @file
 * Callbacks for administrating entities.
 */

/**
 * Choose bundle of entity to add.
 * 
 * @return array
 *   Array describing a list of bundles to render.
 */
function typical_entity_example_5_choose_bundle() {
  drupal_set_title(t('Choose type of entity to add.'));

  // Show list of all existing entity bundles.
  $entity_info = entity_get_info('typical_entity_example_5');
  $items = array();
  foreach ($entity_info['bundles'] as $bundle_name => $bundle_data) {
    $items[] = l($bundle_data['label'], 'typical_entity_example_5/add/' . $bundle_name);
  }
  return array(
    'list' => array(
      '#theme' => 'item_list',
      '#items' => $items,
      '#title' => t('Choose type of entity to add.'),
    ),
  );
}

/**
 * Form constructor for the entity add form.
 * 
 * @param string $type
 *   Bundle of the entity to add.
 * 
 * @return array
 *   Entity edit form.
 */
function typical_entity_example_5_add($type) {
  drupal_set_title(t('Create new %type', array('%type' => $type)));

  // Return form for the given entity bundle.
  $new_entity = entity_create('typical_entity_example_5', array('type' => $type));
  $output = entity_ui_get_form('typical_entity_example_5', $new_entity);
  return $output;
}

/**
 * Form constructor for "typical_entity_example_5_bundle_1" bundle.
 * 
 * This function is necessary for entity_ui_get_form() function.
 * 
 * @see entity_ui_get_form()
 * 
 * @param object $entity
 *   Enity to edit.
 * 
 * @return array
 *   Entity edit form.
 */
function typical_entity_example_5_edit_typical_entity_example_5_bundle_1_form($form, &$form_state, $entity) {
  return typical_entity_example_5_form($form, $form_state, $entity);
}

/**
 * Form constructor for "typical_entity_example_5_bundle_2" bundle.
 * 
 * This function is necessary for entity_ui_get_form() function.
 * 
 * @see entity_ui_get_form()
 * 
 * @param object $entity
 *   Enity to edit.
 * 
 * @return array
 *   Entity edit form.
 */
function typical_entity_example_5_edit_typical_entity_example_5_bundle_2_form($form, &$form_state, $entity) {
  return typical_entity_example_5_form($form, $form_state, $entity);
}

/**
 * Form constructor for the entity edit form.
 * 
 * In this form we shall manually define all form elements related to editable
 * properties of the entity.
 * 
 * @param object $entity
 *   Enity to edit.
 * 
 * @return array
 *   Entity edit form.
 */
function typical_entity_example_5_form($form, &$form_state, $entity) {
  // Store the entity in the form.
  $form_state['typical_entity_example_5'] = $entity;

  // Describe all properties of the entity which shall be shown on the form.
  $wrapper = entity_metadata_wrapper('typical_entity_example_5', $entity);
  $form['title'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Title'),
    '#default_value' => $wrapper->title->value(),
    '#weight' => -20,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#default_value' => $wrapper->description->value(),
    '#weight' => -10,
  );

  // Add fields of the entity to the form.
  field_attach_form('typical_entity_example_5', $entity, $form, $form_state);

  // Add some buttons.
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
    '#submit' => array('typical_entity_example_5_form_submit'),
  );
  if (empty($entity->is_new)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#weight' => 15,
      '#submit' => array('typical_entity_example_5_form_submit_delete'),
    );
  }

  return $form;
}

/**
 * Entity edit form "Save" submit handler.
 */
function typical_entity_example_5_form_submit(&$form, &$form_state) {
  // Automatically get edited entity from the form and save it as new revision.
  // @see entity_form_submit_build_entity()
  $entity = $form_state['typical_entity_example_5'];
  entity_form_submit_build_entity('typical_entity_example_5', $entity, $form, $form_state);
  $entity->is_new_revision = TRUE;
  $wrapper = entity_metadata_wrapper('typical_entity_example_5', $entity);
  $wrapper->save();

  // Redirect user to edited entity page.
  $entity_uri = entity_uri('typical_entity_example_5', $entity);
  $form_state['redirect'] = $entity_uri['path'];
}

/**
 * Entity edit form "Delete" submit handler.
 */
function typical_entity_example_5_form_submit_delete($form, &$form_state) {
  // Redirect user to "Delete" URI for this entity.
  $entity = $form_state['typical_entity_example_5'];
  $entity_uri = entity_uri('typical_entity_example_5', $entity);
  $form_state['redirect'] = $entity_uri['path'] . '/delete';
}

/**
 * Form constructor for the entity delete confirmation form.
 * 
 * @param object $entity
 *   Entity to delete.
 * 
 * @return array
 *   Confirmation form
 */
function typical_entity_example_5_form_delete($form, &$form_state, $entity) {
  // Store the entity in the form.
  $form_state['typical_entity_example_5'] = $entity;

  // Show confirm dialog.
  $entity_uri = entity_uri('typical_entity_example_5', $entity);
  $message = t('Are you sure you want to delete typical_entity_example_5 %title?', array('%title' => entity_label('typical_entity_example_5', $entity)));
  return confirm_form(
    $form,
    $message,
    $entity_uri['path'],
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Entity "Confirm delete" form submit handler.
 */
function typical_entity_example_5_form_delete_submit($form, &$form_state) {
  // Delete the entity.
  $entity = $form_state['typical_entity_example_5'];
  $wrapper = entity_metadata_wrapper('typical_entity_example_5', $entity);
  $wrapper->delete();

  // Redirect user.
  drupal_set_message(t('Custom entity v5 %title deleted.', array('%title' => entity_label('typical_entity_example_5', $entity))));
  $form_state['redirect'] = '<front>';
}
