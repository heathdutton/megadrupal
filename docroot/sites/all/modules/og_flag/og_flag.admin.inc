<?php
/**
 * @file
 * Admin interfaces for the OG Flag module.
 */

/**
 * Callback function for admin menu.
 *
 * Shows a listing of flags with statuses and actions.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param string $group_type
 *   The group type.
 * @param int $gid
 *   The group id.
 *
 * @return mixed
 *   The admin form.
 */
function og_flag_admin_settings_listing($form, &$form_state, $group_type, $gid) {
  og_flag_set_breadcrumb($group_type, $gid);

  global $user;
  $flags = flag_get_flags('node', NULL, $user, FALSE);
  $form['flag'] = array(
    '#prefix' => '<div id="flags">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#theme' => 'table',
    '#header' => array(t('Flag'), t('Status'), t('Action')),
  );

  $rows = array();
  foreach ($flags as $flag) {
    $status = og_flag_is_flag_overridden($flag->fid, $gid) ? t('Overridden') : t('Default');
    $rows[] = array(
      $flag->name . " ($flag->title)",
      $status,
      l(t('Edit'), "group/$group_type/$gid/admin/og-flag/$flag->name/edit"),
    );
  }
  $form['flag']['#rows'] = $rows;
  return $form;
}

/**
 * Submit handler for revert button on _og_flag_admin_settings_edit form.
 *
 * @param array $form
 *   The form.
 *
 * @param array $form_state
 *   The form state.
 */
function og_flag_revert_default($form, &$form_state) {
  $group_type = $form_state['values']['group_type'];
  $gid = $form_state['values']['gid'];
  $fid = $form_state['values']['fid'];

  // Wipe out previous override settings.
  db_delete('og_flag')
    ->condition('gid', $gid, '=')
    ->condition('fid', $fid, '=')
    ->execute();

  drupal_set_message(t("Flag reverted."), 'status');
  drupal_goto("group/$group_type/$gid/admin/og-flag");
}

/**
 * Edit form for the admin menu.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 * @param string $group_type
 *   The group type.
 * @param int $gid
 *   The group id.
 * @param string $flag_name
 *   The flag machine name.
 *
 * @return mixed
 *   The admin edit form.
 */
function og_flag_admin_settings_edit($form, &$form_state, $group_type, $gid, $flag_name) {
  og_flag_set_breadcrumb($group_type, $gid);

  module_load_all_includes('inc', 'flag');
  module_load_include('inc', 'flag', 'includes/flag.admin');
  // Get the admin form for the specified flag.
  $flag = flag_load($flag_name);
  $flag->setContext();
  $form = drupal_get_form('flag_form', $flag);

  // Repoint the submit handler to our own function.
  // NOTE: We're still using the flag validation from the flag module.
  $form['#submit'] = array('og_flag_admin_settings_edit_submit');

  // Some fields should never be overridable at the group level.  Hide them.
  $form['title']['#disabled'] = TRUE;
  $form['name']['#disabled'] = TRUE;
  $form['global']['#access'] = FALSE;
  $form['access']['types']['#access'] = FALSE;

  // Hide the roles selection, we use the OG Permissions assignments for each flag.
  $form['access']['roles']['#access'] = FALSE;

  $message = t('NOTE: Use the ') .
    l(t('Group Permissions'), "group/node/$gid/admin/permissions") .
    t(' page to control which roles may use this flag.');

  $form['access']['info'] = array(
    '#prefix' => '<div class="og-flag-access-info">',
    '#suffix' => '</div>',
    '#markup' => $message,
    '#weight' => -100,
  );

  // Add in additional values that we'll need when processing the submission.
  $form['gid'] = array(
    '#type' => 'value',
    '#value' => $gid,
  );
  $form['fid'] = array(
    '#type' => 'value',
    '#value' => $flag->fid,
  );
  $form['group_type'] = array(
    '#type' => 'value',
    '#value' => $group_type,
  );
  $form['flag_name'] = array(
    '#type' => 'value',
    '#value' => $flag_name,
  );

  // Add in the revert button.
  if (og_flag_is_flag_overridden($flag->fid, $gid)) {
    $form['actions']['revert'] = array(
      '#type' => 'submit',
      '#value' => t('Revert'),
      '#submit' => array('og_flag_revert_default'),
      '#weight' => 1000,
    );
  }

  return $form;
}

/**
 * Submit function for _og_flag_admin_settings_edit.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function og_flag_admin_settings_edit_submit($form, &$form_state) {
  $group_type = $form_state['values']['group_type'];
  $gid = $form_state['values']['gid'];
  $flag = $form['#flag'];
  // For some reason the 'values' array does not contain the new field values
  // so we're grabbing them from the 'input' array instead.
  $flag->form_input($form_state['input']);
  $flag->gid = $gid;
  $success = og_flag_save($flag);

  if ($success) {
    drupal_set_message(t("Settings saved."), 'status');
  }
  drupal_goto("group/$group_type/$gid/admin/og-flag");
}

/**
 * Sets the OG breadcrumb for the admin pages.
 *
 * @param string $group_type
 *   The OG Group Type.
 *
 * @param int $gid
 *   The OG Group ID.
 */
function og_flag_set_breadcrumb($group_type, $gid) {
  if ($gid) {
    og_set_breadcrumb($group_type, $gid, array(l(t('Group'), "$group_type/$gid/group")), l(t('Flags'), "group/$group_type/$gid/admin/og-flag"));
  }
}
