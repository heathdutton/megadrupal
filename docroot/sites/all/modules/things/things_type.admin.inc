<?php

/**
 * @file
 * Things type editing UI.
 */

/**
 * UI controller.
 */
class ThingsTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage things entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the things type editing form.
 */
function things_type_form($form, &$form_state, $things_type, $op = 'edit') {

  if ($op == 'clone') {
    $things_type->label .= ' (cloned)';
    $things_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $things_type->label,
    '#description' => t('The human-readable name of this things type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($things_type->type) ? $things_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $things_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'things_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this things type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save things type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$things_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete things type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('things_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function things_type_form_submit(&$form, &$form_state) {
  $things_type = entity_ui_form_submit_build_entity($form, $form_state);
  $things_type->save();
  $form_state['redirect'] = 'admin/structure/things_types';
}

/**
 * Form API submit callback for the delete button.
 */
function things_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/things_types/manage/' . $form_state['things_type']->type . '/delete';
}
