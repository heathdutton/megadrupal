<?php

/**
 * @file
 * White label entity type editing UI.
 */

/**
 * UI controller.
 */
class WhileTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = while_get_string('admin menu description');
    return $items;
  }
}

/**
 * Generates the while type editing form.
 */
function while_type_form($form, &$form_state, Entity $while_type, $op = 'edit') {

  if ($op == 'clone') {
    $while_type->label .= ' (cloned)';
    $while_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $while_type->label,
    '#description' => t('The human-readable name of this @while-type.', array('@while-type' => while_get_string('type label'))),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($while_type->name) ? $while_type->name : '',
    '#maxlength' => 32,
    '#disabled' => $while_type->hasStatus(ENTITY_IN_CODE) && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'while_type_load',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this @while-type. It must only contain lowercase letters, numbers, and underscores.', array('@while-type' => while_get_string('type label'))),
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $while_type->weight,
    '#description' => t('When showing @whiles, those with lighter (smaller) weights get listed before @whiles with heavier (larger) weights.', array('@whiles' => while_get_string('entity plural label'))),
    '#weight' => 10,
  );

  $form['data']['#tree'] = TRUE;
  $form['data']['supports_revisions'] = array('#type' => 'checkbox',
    '#title' => t('Support revisions'),
    '#default_value' => !empty($while_type->data['supports_revisions']),
    '#description' => t('Enable revision support for this @while-type.', array('@while-type' => while_get_string('type label'))),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save @while-type', array('@while-type' => while_get_string('type label'))),
    '#weight' => 40,
  );
  $form['actions']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete @while-type', array('@while-type' => while_get_string('type label'))),
    '#weight' => 45,
    '#limit_validation_errors' => array(),
    '#submit' => array('while_type_form_submit_delete'),
    '#access' => !$while_type->hasStatus(ENTITY_IN_CODE) && $op != 'add' && $op != 'clone'
  );
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function while_type_form_submit(&$form, &$form_state) {
  $while_type = entity_ui_form_submit_build_entity($form, $form_state);
  // Save and go back.
  $while_type->save();
  $form_state['redirect'] = while_get_string('admin menu path');
}

/**
 * Form API submit callback for the delete button.
 */
function while_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = while_get_string('admin menu path') . '/' . $form_state['while_type']->name . '/delete';
}
