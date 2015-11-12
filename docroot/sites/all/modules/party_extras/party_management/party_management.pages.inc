<?php
/**
 * @file
 * Pages and forms for the party management system.
 */

/**
 * Manage group members modal form callback.
 *
 * @param $js
 *   Provided by ctools, TRUE if js is enabled.
 * @param $party
 *   A Party object.
 *
 * @return
 *   Array of ctools commands in JSON format.
 */
function party_management_group_members_modal_edit_page($js, $party, $op) {
  if (!$js) {
    if ($op == 'add') {
      return drupal_goto('group/party/' . $party->pid . '/admin/people/add-user');
    }
    else if ($op == 'manage') {
      return drupal_goto('group/party/' . $party->pid . '/admin/people');
    }
  }

  ctools_include('ajax');
  ctools_include('modal');
  ctools_include('og_ui.admin', 'og_ui', '');

  $form_state = array(
    'title' => t('Manage group members'),
    'ajax' => TRUE,
    'build_info' => array(
      'args'  => array(
        'party',
        $party->pid,
      ),
    ),
  );

  if ($op == 'add') {
    $commands = ctools_modal_form_wrapper('og_ui_add_users', $form_state);
  }
  else if ($op == 'manage') {
    $commands = ctools_modal_form_wrapper('og_ui_user_admin_account', $form_state);
  }

  if (!empty($form_state['executed'])) {
    // Overwrite the output if form submission was successfully executed.
    $commands = array();
    $commands[] = ctools_ajax_command_reload();
  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );
}

