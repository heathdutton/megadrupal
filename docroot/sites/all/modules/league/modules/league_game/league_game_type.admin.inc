<?php

/**
 * @file
 * LeagueGame type editing UI.
 */

/**
 * UI controller.
 */
class LeagueGameTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage league_game entity types, including adding
    and removing fields and the display of fields.';
    return $items;
  }
}

/**
 * Generates the league_game type editing form.
 */
function league_game_type_form($form, &$form_state, $league_game_type, $op = 'edit') {

  if ($op == 'clone') {
    $league_game_type->label .= ' (cloned)';
    $league_game_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $league_game_type->label,
    '#description' => t('The human-readable name of this league_game type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($league_game_type->type) ? $league_game_type->type : '',
    '#maxlength' => 32,
//    '#disabled' => $league_game_type->isLocked() && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'league_game_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this league_game type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save league_game type'),
    '#weight' => 40,
  );

  //Locking not supported yet
  /*if (!$league_game_type->isLocked() && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete league_game type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('league_game_type_form_submit_delete')
    );
  }*/
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function league_game_type_form_submit(&$form, &$form_state) {
  $league_game_type = entity_ui_form_submit_build_entity($form, $form_state);
  $league_game_type->save();
  drupal_alter('league_game_type_form_save', $league_game_type);
  $form_state['redirect'] = 'admin/structure/league_game_types';
}

/**
 * Form API submit callback for the delete button.
 */
function league_game_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/league_game_types/manage/' . $form_state['league_game_type']->type . '/delete';
}
