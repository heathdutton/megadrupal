<?php
/**
 * @file
 * The XC ILS is a small mmodule which helps to use ILS-specific settings in the Drupal Toolkit.
 *
 * We currently use only one of the possible settings: mapping the bibliographic
 * record identifier to an NCIP server. When the DT display an XC record, the
 * XC ILS module give the name of the identifier field, and the content of that
 * field will be used by the NCIP protocol.
 *
 * The module use the xc_ils_settings table, which have the following fields:
 * - <code>settings_id</code> The identifier of the record
 * - <code>ncip_provider_id</code> The identifier of the NCIP server record in
 *   database
 * - <code>book_id_field<code> The name of the bibliographical identifier field
 *
 * @copyright (c) 2010-2011 eXtensible Catalog Organization
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_ils_delete_form($form, &$form_state, $record) {
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $record->settings_id,
  );
  return confirm_form($form,
    t('Are you sure you want to remove this IP address range?'),
    'admin/xc/ils/list', '', t('Remove'), t('Cancel'));
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_ils_delete_form_submit($form, &$form_state) {
  $id = $form_state['values']['id'];
  $result = db_delete('xc_ils_settings')
            ->condition('settings_id', $id)
            ->execute();
  if ($result == 1) {
    drupal_set_message(t('ISL settings removed'));
    $form_state['redirect'] = 'admin/xc/ils/list';
  }
  else {
    drupal_set_message(t('Unexpected error. Deletion removed %count ILS setting(s).',
      array('%count' => $result)));
  }
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_ils_disable_form($form, &$form_state, $record) {
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $record->settings_id,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to disable this ILS mapping?'),
    'admin/xc/ils/list', '', t('Disable'), t('Cancel')
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_ils_disable_form_submit($form, &$form_state) {
  $id = $form_state['values']['id'];
  $sql = 'UPDATE {xc_ils_settings} SET is_enabled = 0 WHERE settings_id = %d';
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query($sql, $id) */
  $result = db_update('xc_ils_settings')
              ->fields(array(
                'is_enabled' => 0,
              ))
              ->condition('settings_id', $id)
              ->execute();
  if ($result) {
    drupal_set_message(t('ISL settings disabled'));
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to disabled ILS settings.'));
  }
  $form_state['redirect'] = 'admin/xc/ils/list';
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_ils_enable_form($form, &$form_state, $record) {
  $form['id'] = array(
    '#type' => 'value',
    '#value' => $record->settings_id,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to enable this ILS mapping?'),
    'admin/xc/ils/list', '', t('Enable'), t('Cancel')
  );
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function xc_ils_enable_form_submit($form, &$form_state) {
  $id = $form_state['values']['id'];
  $sql = 'UPDATE {xc_ils_settings} SET is_enabled = 1 WHERE settings_id = %d';
  // TODO Please review the conversion of this statement to the D7 database API syntax.
  /* db_query($sql, $id) */
  $result = db_update('xc_ils_settings')
              ->fields(array(
                'is_enabled' => 1,
              ))
              ->condition('settings_id', $id)
              ->execute();
  if ($result) {
    drupal_set_message(t('ISL settings enabled'));
  }
  else {
    drupal_set_message(t('Unexpected error. Failed to enabled ILS settings.'));
  }
  $form_state['redirect'] = 'admin/xc/ils/list';
}

