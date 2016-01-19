<?php

require_once 'redmine_sync.constants.inc';

/**
 * redmine_sync_sync_form().
 */
function redmine_sync_sync_form($form, &$form_state) {

  drupal_add_css(REDMINE_SYNC_MODULE_PATH.'/redmine_sync.css');
  $db_init = redmine_sync_init_db_rdm();
  $link_to_db_settings_page = l(t('database settings page'), REDMINE_SYNC_URL_PAGE_SETTINGS_DATABASE);

  // Error message.
  if ($db_init == REDMINE_SYNC_DB_INIT_EMPTY_PARAMETERS) {
    $form['error_message_1'] = array(
      '#markup' => theme('form_instead_message', array('status' => 'warning', 'message' =>
        t('Redmine database parameters is not set!').REDMINE_SYNC_HTML_BR.
        t('Please, set all parameters on !settings_page.', array('!settings_page' => $link_to_db_settings_page))
      )),
    );
  }
  if ($db_init == REDMINE_SYNC_DB_INIT_INVALID_PARAMETERS_OR_SERVER_NOT_AVAILABLE) {
    $form['error_message_2'] = array(
      '#markup' => theme('form_instead_message', array('status' => 'error', 'message' =>
        t('Redmine database parameters is invalid or database server is no available or table %table_name is not exist!', array('%table_name' => 'time_entries')).REDMINE_SYNC_HTML_BR.
        t('Check all parameters on !settings_page or check database server and table %table_name accessibility.', array('!settings_page' => $link_to_db_settings_page, '%table_name' => 'time_entries'))
      )),
    );
  }

  // If no errors.
  if ($db_init == REDMINE_SYNC_DB_INIT_OK) {
    $ids_upd = redmine_sync_db_time_entries_update_exist(true);
    $ids_old = redmine_sync_db_time_entries_remove_old(true);
    $last_id = redmine_sync_db_get_drp_last_id('time_entries');
    $total_imported         = redmine_sync_db_get_drp_total_imported_count('time_entries');
    $total_non_imported     = redmine_sync_db_get_rdm_total_non_imported_count('time_entries', $last_id + 1);
    $last_update_timestamp  = redmine_sync_db_get_drp_last_update_timestamp('time_entries');
    $items_import_per_batch = variable_get('redmine_sync_items_import_per_batch', REDMINE_SYNC_DEF_VAL_ITEMS_IMPORT_PER_BATCH);

    // ENTITY: TIME_ENTRIES
    $form['time_entries'] = array(
      '#type' => 'fieldset',
      '#title' => t('Entity: %name', array('%name' => 'time_entries')),
    );

    // Clear all records.
    $form['time_entries']['clear_all_records'] = array(
      '#type' => 'fieldset',
      '#title' => t('Clear all records'),
    );
    $form['time_entries']['clear_all_records']['button_clear'] = array(
      '#type' => 'submit',
      '#value' => t('Clear all !num records.', array('!num' => $total_imported)),
      '#submit' => array('redmine_sync_sync_form_time_entries_clear_submit'),
      '#disabled' => $total_imported == 0,
    );

    // Import new records.
    $form['time_entries']['import_new_records'] = array(
      '#type' => 'fieldset',
      '#title' => t('Import new records'),
    );
    $form['time_entries']['import_new_records']['report'] = array(
      '#markup' => t('Last imported id = %id', array('%id' => $last_id)).REDMINE_SYNC_HTML_BR.
                   t('Total count of imported records = %count', array('%count' => $total_imported)).REDMINE_SYNC_HTML_BR.
                   t('Total count of record that can be imported = %count', array('%count' => $total_non_imported)).REDMINE_SYNC_HTML_BR,
    );
    $form['time_entries']['import_new_records']['button_import'] = array(
      '#type' => 'submit',
      '#value' => t('Import next !num new records after id = !id', array('!num' => min($total_non_imported, $items_import_per_batch), '!id' => $last_id)),
      '#submit' => array('redmine_sync_sync_form_time_entries_import_submit'),
      '#disabled' => $total_non_imported == 0,
    );

    // Update exist records.
    $form['time_entries']['update_exist_records'] = array(
      '#type' => 'fieldset',
      '#title' => t('Update exist records'),
    );
    $form['time_entries']['update_exist_records']['report'] = array(
      '#markup' => t('Last imported id = %id', array('%id' => $last_id)).REDMINE_SYNC_HTML_BR.
                   t('Last update date = %date (timestamp = %timestamp)', array('%date' => gmdate(REDMINE_SYNC_DEF_DATE_TIME_FORMAT, $last_update_timestamp), '%timestamp' => $last_update_timestamp)).REDMINE_SYNC_HTML_BR.
                   t('Total count of record that can be updated = %count', array('%count' => count($ids_upd))).REDMINE_SYNC_HTML_BR,
    );
    $form['time_entries']['update_exist_records']['button_update'] = array(
      '#type' => 'submit',
      '#value' => t('Update !num records before id = !id and updated_on > !date', array('!num' => count($ids_upd), '!id' => $last_id, '!date' => gmdate(REDMINE_SYNC_DEF_DATE_TIME_FORMAT, $last_update_timestamp))),
      '#submit' => array('redmine_sync_sync_form_time_entries_update_submit'),
      '#disabled' => !count($ids_upd),
    );

    // Remove old records.
    $form['time_entries']['remove_old_records'] = array(
      '#type' => 'fieldset',
      '#title' => t('Remove old records'),
    );
    $form['time_entries']['remove_old_records']['report'] = array(
      '#markup' => t('Last imported id = %id', array('%id' => $last_id)).REDMINE_SYNC_HTML_BR.
                   t('Total count of record that can be removed = %count', array('%count' => count($ids_old))).REDMINE_SYNC_HTML_BR,
    );
    $form['time_entries']['remove_old_records']['button_remove'] = array(
      '#type' => 'submit',
      '#value' => t('Remove !num old records before id = !id', array('!num' => count($ids_old), '!id' => $last_id)),
      '#submit' => array('redmine_sync_sync_form_time_entries_remove_submit'),
      '#disabled' => !count($ids_old),
    );

    // USERS
    $form['users'] = array(
      '#type' => 'fieldset',
      '#title' => t('Users'),
    );
    $form['users']['button_synchronize_users'] = array(
      '#type' => 'submit',
      '#value' => t('Synchronize users.user_id by email'),
      '#submit' => array('redmine_sync_sync_form_synchronize_users_by_email_submit'),
    );

  }

  return $form;
}


function redmine_sync_sync_form_time_entries_clear_submit($form, &$form_state) {
  drupal_goto(REDMINE_SYNC_URL_CONFIRM_CLEAR_ALL_RECORDS);
}


function redmine_sync_sync_form_time_entries_clear_confirm($form, &$form_state) {
  return confirm_form($form,
    t('Are you sure you want to delete all records?'), REDMINE_SYNC_URL_PAGE_SYNC,
    t('This action cannot be undone.'), t('Clear'), t('Cancel'), 'redmine_sync_sync_form_clear_submit');
}


function redmine_sync_sync_form_time_entries_clear_confirm_submit($form, &$form_state) {
  $clear_result = redmine_sync_db_time_entries_clear_all();
  if ($clear_result) {
    drupal_set_message('Cleaning was done.');
    drupal_goto(REDMINE_SYNC_URL_PAGE_SYNC);
  }
}


function redmine_sync_sync_form_time_entries_import_submit($form, &$form_state) {
  $ids_new = redmine_sync_db_time_entries_import_new();
  if (count($ids_new)) {
    drupal_set_message(t('Imported !count records.', array('!count' => count($ids_new))));
    watchdog('redmine_sync', 'Imported !count records. The next id\'s was imported: !ids', array('!count' => count($ids_new), '!ids' => implode(', ', $ids_new)));
  }
}


function redmine_sync_sync_form_time_entries_update_submit($form, &$form_state) {
  $ids_upd = redmine_sync_db_time_entries_update_exist();
  if (count($ids_upd)) {
    drupal_set_message(t('Updated !count records.', array('!count' => count($ids_upd))));
    watchdog('redmine_sync', 'Updated !count records. The next id\'s was updated: !ids', array('!count' => count($ids_upd), '!ids' => implode(', ', $ids_upd)));
  }
}


function redmine_sync_sync_form_time_entries_remove_submit($form, &$form_state) {
  $ids_old = redmine_sync_db_time_entries_remove_old();
  if (count($ids_old)) {
    drupal_set_message(t('Removed !count records.', array('!count' => count($ids_old))));
    watchdog('redmine_sync', 'Removed !count records. The next id\'s was removed: !ids', array('!count' => count($ids_old), '!ids' => implode(', ', $ids_old)));
  }
}


function redmine_sync_sync_form_synchronize_users_by_email_submit() {
  redmine_sync_db_user_sync_by_email();
}

