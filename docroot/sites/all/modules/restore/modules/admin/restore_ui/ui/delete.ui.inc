<?php
/**
 * @file
 * The delete ui form.
 */

/**
 * The delete/revert form callback.
 */
function restore_ui_script_delete($form, &$form_state, $script) {
  $form = array();

  return confirm_form(
    $form,
    t('Are you sure you want to @state this restore script', array(
      '@state' => $script->module() ? t('revert') : t('delete'),
    )),
    RESTORE_MENU_PATH,
    t('This action can not be undone.'),
    $script->module() ? t('Revert') : t('Delete'),
    t('Cancel')
  );
}

/**
 * The form submit callback.
 */
function restore_ui_script_delete_submit($form, &$form_state) {
  $script = $form_state['build_info']['args'][0];

  $num_deleted = db_delete('restore_scripts')
    ->condition('name', $script->name())
    ->execute();

  if ($num_deleted) {
    drupal_set_message(t('The restore script @name has been @state.', array(
      '@name' => $script->title(),
      '@state' => $script->module() ? t('reverted') : t('deleted'),
    )));

    $form_state['redirect'] = $script->module() ? RESTORE_MENU_PATH . "/script/{$script->name()}/view" : RESTORE_MENU_PATH;
  }
  else {
    drupal_set_message(t('There was a problem @state the restore script.', array(
      '@state' => $script->module() ? t('reverting') : t('deleting'),
    )), 'error');
  }
}
