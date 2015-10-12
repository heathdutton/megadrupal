<?php

/**
 * @file
 * Pollim type editing UI.
 */

/**
 * UI controller.
 */
class PollimTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
		$items[$this->path]['description'] = 'Manage pollim entity types, including adding
		and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the pollim type editing form.
 */
function pollim_type_form($form, &$form_state, $pollim_type, $op = 'edit') {

  if ($op == 'clone') {
    $pollim_type->label .= ' (cloned)';
    $pollim_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $pollim_type->label,
    '#description' => t('The human-readable name of this pollim type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($pollim_type->type) ? $pollim_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $pollim_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'pollim_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this pollim type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save pollim type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$pollim_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete pollim type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('pollim_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function pollim_type_form_submit(&$form, &$form_state) {
  $pollim_type = entity_ui_form_submit_build_entity($form, $form_state);
  $pollim_type->save();
  $form_state['redirect'] = 'admin/structure/pollim_types';
}

/**
 * Form API submit callback for the delete button.
 */
function pollim_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/pollim_types/manage/' . $form_state['pollim_type']->type . '/delete';
}
