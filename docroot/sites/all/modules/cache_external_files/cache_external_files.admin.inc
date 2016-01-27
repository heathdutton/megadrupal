<?php
/**
 * Administration pages for the Cache External Files module.
 */

/**
 * Form constructor to build the page list form.
 *
 * @ingroup forms
 * @see cache_external_files_admin_validate()
 * @see cache_external_files_admin_submit()
 */
function cache_external_files_admin() {
  $timeouts = drupal_map_assoc(
    array(
      1800,
      3600,
      7200,
      14400,
      28800,
      43200,
      86400,
      172800,
      345600,
      604800,
      1209600,
      2419200,
    ), 'format_interval');
  $form = array();

  $result = db_query('SELECT * FROM {cache_external_files} ORDER BY fid ASC');
  foreach ($result as $row) {
    $form['table'][$row->fid]['enabled_' . $row->fid] = array(
      '#type' => 'checkbox',
      '#default_value' => $row->enabled,
    );
    $form['table'][$row->fid]['url_' . $row->fid] = array(
      '#type' => 'textfield',
      '#default_value' => $row->url,
      '#size' => 40,
      '#maxlength' => 255,
    );
    $form['table'][$row->fid]['path_' . $row->fid] = array(
      '#type' => 'item',
      '#markup' => _cache_external_files_url_to_path($row->url),
    );
    $form['table'][$row->fid]['cached_' . $row->fid] = array(
      '#type' => 'item',
      '#markup' => format_date($row->cached, 'small'),
    );
    $form['table'][$row->fid]['timeout_' . $row->fid] = array(
      '#type' => 'select',
      '#options' => $timeouts,
      '#default_value' => $row->timeout,
    );
    $form['table'][$row->fid]['refresh_' . $row->fid] = array(
      '#type' => 'checkbox',
      '#default_value' => 0,
    );
    $form['table'][$row->fid]['delete_' . $row->fid] = array(
      '#type' => 'checkbox',
      '#default_value' => 0,
    );
  }

  $form['add_new_enabled'] = array(
    '#type' => 'checkbox',
    '#default_value' => 0,
  );
  $form['add_new_url'] = array(
    '#type' => 'textfield',
    '#size' => 40,
    '#maxlength' => 255,
  );
  $form['add_new_timeout'] = array(
    '#type' => 'select',
    '#options' => $timeouts,
    '#default_value' => 14400,
  );
  $form['add_new_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  $form['#theme'] = 'cache_external_files_admin';

  return $form;
}

/**
 * Form validation handler for cache_external_files_admin().
 *
 * @see cache_external_files_admin_submit()
 */
function cache_external_files_admin_validate($form, &$form_state) {
  if (isset($form['table']) && $form['table']) {
    foreach (element_children($form['table']) as $fid) {
      if (!((substr($form_state['values']['url_' . $fid], 0, 7) == 'http://') || (substr($form_state['values']['url_' . $fid], 0, 8) == 'https://'))) {
        form_set_error('url_' . $fid, 'URL should begin with http:// or https://');
      }
    }
  }
  if ($form_state['values']['add_new_url'] && !((substr($form_state['values']['add_new_url'], 0, 7) == 'http://') || (substr($form_state['values']['add_new_url'], 0, 8) == 'https://'))) {
    form_set_error('add_new_url', 'URL should begin with http:// or https://');
  }
}

/**
 * Form submission handler for cache_external_files_admin().
 *
 * @see cache_external_files_admin_validate()
 */
function cache_external_files_admin_submit($form, &$form_state) {
  if (isset($form['table']) && $form['table']) {
    foreach (element_children($form['table']) as $fid) {
      // Delete file.
      if ($form_state['values']['delete_' . $fid]) {
        db_query('DELETE FROM {cache_external_files} WHERE fid = ' . $fid);
        drupal_set_message('Deleted file ' . $form_state['values']['url_' . $fid]);
        continue;
      }
      // Update file configuration.
      if ($form_state['values']['url_' . $fid] != $form['table'][$fid]['url_' . $fid]['#default_value']
        || $form_state['values']['timeout_' . $fid] != $form['table'][$fid]['timeout_' . $fid]['#default_value']
        || $form_state['values']['enabled_' . $fid] != $form['table'][$fid]['enabled_' . $fid]['#default_value']) {
        $file = array(
          'enabled' => $form_state['values']['enabled_' . $fid],
          'fid' => $fid,
          'url' => $form_state['values']['url_' . $fid],
          'timeout' => $form_state['values']['timeout_' . $fid],
        );
        if (drupal_write_record('cache_external_files', $file, 'fid') == SAVED_UPDATED) {
          drupal_set_message('Updated file ' . $form_state['values']['url_' . $fid]);
        }
        else {
          drupal_set_message('Failed updating file ' . $form_state['values']['url_' . $fid]);
        }
      }
      // Refresh file.
      if ($form_state['values']['refresh_' . $fid]) {
        drupal_set_message('Refreshing file ' . $form_state['values']['url_' . $fid]);
        _cache_external_files_update_file($fid, $form_state['values']['url_' . $fid]);
      }
    }
  }
  // Add new file.
  if ($form_state['values']['add_new_url']) {
    $file = array(
      'enabled' => $form_state['values']['add_new_enabled'],
      'url' => $form_state['values']['add_new_url'],
      'timeout' => $form_state['values']['add_new_timeout'],
    );
    if (drupal_write_record('cache_external_files', $file) == SAVED_NEW) {
      drupal_set_message('Added new file ' . $form_state['values']['add_new_url']);
    }
    else {
      drupal_set_message('Failed adding new file ' . $form_state['values']['add_new_url']);
    }
  }
}
