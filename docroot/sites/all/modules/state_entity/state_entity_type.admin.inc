<?php

/**
 * @file
 * State Entity type editing UI.
 */

/**
 * UI controller.
 */
class state_entityTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage state_entity entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the State Entity type editing form.
 */
function state_entity_type_form($form, &$form_state, $state_entity_type, $op = 'edit') {

  if ($op == 'clone') {
    $state_entity_type->label .= ' (cloned)';
    $state_entity_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $state_entity_type->label,
    '#description' => t('The human-readable name of this State Entity type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($state_entity_type->type) ? $state_entity_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $state_entity_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'state_entity_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this State Entity type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save State Entity type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$state_entity_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete State Entity type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('state_entity_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function state_entity_type_form_submit(&$form, &$form_state) {
  $state_entity_type = entity_ui_form_submit_build_entity($form, $form_state);
  $state_entity_type->save();
  $form_state['redirect'] = 'admin/structure/state_entity_types';
}

/**
 * Form API submit callback for the delete button.
 */
function state_entity_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/state_entity_types/manage/' . $form_state['state_entity_type']->type . '/delete';
}
