<?php

/**
 * @file
 * LeaguePerson type editing UI.
 */

/**
 * UI controller.
 */
class LeaguePersonTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage league_person entity types, including adding
    and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the league_person type editing form.
 */
function league_person_type_form($form, &$form_state, $league_person_type, $op = 'edit') {

  if ($op == 'clone') {
    $league_person_type->label .= ' (cloned)';
    $league_person_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $league_person_type->label,
    '#description' => t('The human-readable name of this league_person type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($league_person_type->type) ? $league_person_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $league_person_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'league_person_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this league_person type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save League Person type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$league_person_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete league_person type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('league_person_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function league_person_type_form_submit(&$form, &$form_state) {
  $league_person_type = entity_ui_form_submit_build_entity($form, $form_state);
  $league_person_type->save();
  drupal_alter('league_person_type_form_save', $league_person_type);
  $form_state['redirect'] = 'admin/structure/league_person_types';
}

/**
 * Form API submit callback for the delete button.
 */
function league_person_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/league_person_types/manage/' . $form_state['league_person_type']->type . '/delete';
}
