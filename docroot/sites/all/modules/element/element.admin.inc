<?php
/**
 * @file
 * Element admin UI.
 */

/**
 * Form constructor for the element type editing form.
 *
 * @param $element_type
 *   An object representing the element type, when editing an existing
 *   element type.
 */
function element_type_form($form, &$form_state, $element_type) {
  $wrapper = entity_metadata_wrapper('element_type', $element_type);
  $bundle = $wrapper->type->value();
  // Make the type object available to implementations of hook_form_alter.
  $form['#element_type'] = $wrapper;
  
  $form['name'] = array(
    '#title' => t('Name'),
    '#type' => 'textfield',
    '#default_value' => $wrapper->name->value(),
    '#description' => t('The human-readable name of this element type. This text will be displayed as part of the list on the <em>Add new element</em> page. It is recommended that this name begin with a capital letter and contain only letters, numbers, and spaces. This name must be unique.'),
    '#required' => TRUE,
    '#size' => 30,
  );

  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => $bundle,
    '#maxlength' => 32,
    '#machine_name' => array(
      'exists' => 'element_type_exists',
    ),
    '#description' => t('A unique machine-readable name for this element type. It must only contain lowercase letters, numbers, and underscores. This name will be used for constructing the URL of the %element-add page, in which underscores will be converted into hyphens.', array(
      '%element-add' => t('Add new element'),
    )),
  );

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textarea',
    '#default_value' => $wrapper->description->value(),
    '#description' => t('Describe this element type. The text will be displayed on the <em>Add new element</em> page.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save element type'),
    '#weight' => 45,
  );

  if (!empty($bundle)) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete element type'),
      '#weight' => 50,
      '#submit' => array('element_type_form_delete_submit'),
    );
  }

  return $form;
}

/**
 * Submit handler for the element type add/edit form.
 */
function element_type_form_submit($form, &$form_state) {
  $element_type = entity_ui_form_submit_build_entity($form, $form_state);
  $element_type->save();
  drupal_set_message(t('The element type: @name has been saved.', array('@name' => $element_type->name)));
  $form_state['redirect'] = 'admin/structure/element-types';
}

/**
 * Form API submit callback for the delete button.
 */
function element_type_form_delete_submit(&$form, &$form_state) {
  $element_type = $form_state['element_type'];
  // We need to unset drupal's destination param because Drupal always honors it
  // over form_state redirect.
  // @see drupal_goto().
  unset($_GET['destination']);
  drupal_static_reset('drupal_get_destination');
  drupal_get_destination();
  $form_state['redirect'] = "admin/structure/element-types/manage/$element_type->type/delete";
}

/**
 * Helper function that checks whether an element type's machine name already
 * exists.
 *
 * @return boolean
 * TRUE if they machine name already exists, FALSE if not.
 */
function element_type_exists($name) {
  $info = element_types();
  return isset($info[$name]);
}

/**
 * Form constructor for adding / editing an element.
 *
 * @param $element_type
 *   An object representing the element.
 */
function element_form($form, &$form_state, $element) {
  $wrapper = entity_metadata_wrapper('element', $element);
  $info = $wrapper->entityInfo();
  $entity_label = !empty($info['bundles']) ? $info['bundles'][$element->type]['label'] : $info['label'];
  $label_prefix = isset($element->is_new) && $element->is_new ? t('Create new') : t('Edit');

  $form['header'] = array(
    '#prefix' => '<h2>',
    '#markup' => "$label_prefix $entity_label",
    '#suffix' => '</h2>',
  );

  $form['title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#default_value' => $wrapper->title->value(),
    '#required' => TRUE,
  );

  field_attach_form('element', $element, $form, $form_state);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit', 
    '#value' => isset($element->id) ? t("Update $entity_label") : t("Save $entity_label"),
    '#weight' => 50,
  );

  if (empty($element->is_new)) {
    $form['log'] = array(
      '#title' => t('Revision Message'),
      '#description' => t('Describe what was edited in this revision. This message will show up in the revision log.'),
      '#type' => 'textarea',
      '#weight' => 20,
    );
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t("Delete $entity_label"),
      '#weight' => 51,
      '#submit' => array('element_form_delete_submit'),
    );
  }

  return $form;
}

/**
 * Submit handler for the element add/edit form.
 */
function element_form_submit($form, &$form_state) {
  $element = entity_ui_form_submit_build_entity($form, $form_state);
  $element->save();
  drupal_set_message(t('The element: @name has been saved.', array('@name' => $element->title)));
  $form_state['redirect'] = $element->uri();
}

/**
 * Form API submit callback for the delete button.
 */
function element_form_delete_submit(&$form, &$form_state) {
  $element = $form_state['element'];
  // We need to unset drupal's destination param because Drupal always honors it
  // over form_state redirect.
  // @see drupal_goto().
  unset($_GET['destination']);
  drupal_static_reset('drupal_get_destination');
  drupal_get_destination();
  $form_state['redirect'] = "element/{$element->id}/delete";
}
