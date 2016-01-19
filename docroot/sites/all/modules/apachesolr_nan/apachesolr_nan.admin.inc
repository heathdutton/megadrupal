<?php

/**
 * @file
 *
 * Administrative functions and form builders for Apache Solr NAN Search.
 */

/**
 * Builds the administration form for Apache Solr NAN search.
 */
function apachesolr_nan_nan_settings_form($form, &$form_state) {
  // We will have many fields with the same name, so we need to be able to
  // access the form hierarchically.
  $form['#tree'] = TRUE;

  $environments = apachesolr_load_all_environments();
  $env_options = array();
  foreach ($environments as $machine_name => $env) {
    $env_options[$machine_name] = $env['name'];
  }
  $form['environment'] = array(
    '#type' => 'select',
    '#options' => $env_options,
    '#description' => t('Choose the solr server with which to index the content.'),
    '#default_value' => apachesolr_default_environment(),
    '#required' => TRUE,
  );

  $form['description'] = array(
    '#type' => 'item',
    '#title' => t('Define the paths you would like indexed along with names and descriptions for them.'),
  );

  if (empty($form_state['items'])) {
    $results = db_select('apachesolr_nan_index_paths', 'p')->fields('p')->execute();
    $items = array();
    while ($item = $results->fetchAssoc()) {
      $items[] = $item;
    }
    $form_state['items'] = $items;
  }
  if (!empty($form_state['all_removed']) || empty($form_state['items'])) {
    $form_state['items'] = NULL;//array(0 => array());
  }

  if (!empty($form_state['items'])) {
    // Build a fieldset for each item in the items array.
    $c = 1;
    foreach ($form_state['items'] as $i => $data) {

      $form['item'][$i] = array(
        '#type' => 'fieldset',
        '#title' => t('Item #@num', array('@num' => $c)),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );

      $form['item'][$i]['path'] = array(
        '#type' => 'textfield',
        '#title' => t('Path to Index'),
        '#description' => t('Enter a path to index.'),
        '#required' => TRUE,
        '#default_value' => !empty($data['path']) ? $data['path'] : '',
      );
      $form['item'][$i]['title'] = array(
        '#type' => 'textfield',
        '#title' => t('Indexed Title'),
        '#description' => t('A title to show on the search results page. This can be the name of the page or something else but users will see this in search results and the text will be used to boost result order.'),
        '#default_value' => !empty($data['title']) ? $data['title'] : '',
      );
      $form['item'][$i]['description'] = array(
        '#type' => 'textarea',
        '#title' => t('Page description'),
        '#description' => t('The description will show in search results only. It can be used to improve search results for certain pages.'),
        '#default_value' => !empty($data['description']) ? $data['description'] : '',
      );
      $form['item'][$i]['frequency'] = array(
        '#type' => 'select',
        '#title' => t('Time between indexes'),
        '#options' => array(
          3600    => 'hourly',
          86400   => 'daily',
          604800  => 'weekly',
          2419200 => 'monthly',
        ),
        '#description' => t('The page will be reindexed at the set interval.'),
        '#default_value' => isset($data['frequency']) ? $data['frequency'] : 86400,
        '#required' => TRUE,
      );
      $form['item'][$i]['delete'] = array(
        '#type' => 'submit',
        '#name' => $i,
        '#value' => 'Delete',
        '#submit' => array('apachesolr_nan_remove_item'),
        '#limit_validation_errors' => array(),
      );
      $c++;
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  // Adds "Add another name" button
  $form['add_item'] = array(
    '#type' => 'submit',
    '#value' => t('Add another item'),
    '#submit' => array('apachesolr_nan_add_item'),
  );

  return $form;
}

/**
 * Handles adding an additional item to the admin form.
 *
 * @see apachesolr_nan_nan_settings_form().
 */
function apachesolr_nan_add_item($form, &$form_state) {
  // Add an element to the end of the array with the next available number.
  if (empty($form_state['items'])) {
    $form_state['items'][] = array();
  }
  else {
    $keys = array_keys($form_state['items']);
    $end = end($keys);
    $end++;
    $form_state['items'][$end] = array();
  }
  // Rebulid the form.
  $form_state['rebuild'] = TRUE;
  drupal_set_message(t('Your changes will not be saved until you press the submit button.'), 'warning');
}

/**
 * Handles item removal from the admin page.
 *
 * @see apachesolr_nan_nan_settings_form().
 */
function apachesolr_nan_remove_item($form, &$form_state) {
  // Remove the item from the array that corresponds to the pressed delete
  // button.
  $keys = array_keys($form_state['values']['item']);
  $first = reset($keys);
  unset($form_state['items'][$first]);
  $form_state['rebuild'] = TRUE;
  if (empty($form_state['items'])) {
    $form_state['all_removed'] = TRUE;
  }
  drupal_set_message(t('If you are making changes to existing data, your changes will not be saved until you press the submit button.'), 'warning');
}

/**
 * Validation for the admin page.
 *
 * @see apachesolr_nan_nan_settings_form().
 * @see apachesolr_nan_nan_settings_form_submit().
 */
function apachesolr_nan_nan_settings_form_validate($form, &$form_state) {
  // Validate the paths.
  if (!empty($form_state['items'])) {
    foreach ($form_state['items'] as $i => $data) {
      $path = $form_state['values']['item'][$i]['path'];
      if (!drupal_valid_path($path)) {
        if (empty($form_state['all_removed']) && $path != '') {
          form_set_error("item][$i][path", t('The path is not valid or you do not have access to it.'));
        }
      }
    }
  }
}

/**
 * Submit form for admin page.
 *
 * @see apachesolr_nan_nan_settings_form().
 * @see apachesolr_nan_nan_settings_form_validate().
 */
function apachesolr_nan_nan_settings_form_submit($form, &$form_state) {
  $old_env = variable_get('apachesolr_nan_nan_env', apachesolr_default_environment());
  if ($form_state['values']['environment'] != $old_env) {
    // We switched environments so clean everything out of the other one.
    $ids = db_select('apachesolr_nan_index_paths', 'p')->fields('p', array('id'))->execute()->fetchCol();
    if (!empty($ids)) {
      foreach ($ids as $id) {
        apachesolr_index_delete_entity_from_index($old_env, 'node', $id);
      }
    }
  }
  // Set the new environment.
  $env = $form_state['values']['environment'];
  variable_set('apachesolr_nan_nan_env', $env);
  if (!empty($form_state['items'])) {
    foreach ($form_state['values']['item'] as $key => $item) {
      unset($form_state['values']['item'][$key]['delete']);
    }
    $items = array_values($form_state['values']['item']);
  }
  else {
    $items = array();
  }
  // Set the items into the database;
  $paths = db_select('apachesolr_nan_index_paths', 'p')->fields('p', array('id', 'path'))->execute()->fetchAllKeyed();
  $new_paths = array();
  $saved = FALSE;
  foreach ($items as $item) {
    $new_paths[] = $item['path'];
    if ($key = array_search($item['path'], $paths)) {
      $record = $item + array('id' => $key);
      drupal_write_record('apachesolr_nan_index_paths', $record, array('id'));
      $saved = TRUE;
    }
    else {
      drupal_write_record('apachesolr_nan_index_paths', $item);
      $saved = TRUE;
    }
  }
  if ($saved) {
    drupal_set_message(t('Your paths have beed updated or added.'));
  }
  // Find the removed elements and delete them.
  $delete = array_diff($paths, $new_paths);
  $delete_keys = array_keys($delete);
  if (!empty($delete_keys)) {
    module_load_include('inc', 'apachesolr', 'apachesolr.index');
    foreach ($delete_keys as $key) {
      $id = (int) $key * -1;
      if (apachesolr_index_delete_entity_from_index($env, 'node', $id)) {
        watchdog('ApacheSolr NAN search', 'The apache solr non-node item %id was deleted', array('%id' => $id));
      }
      else {
        drupal_set_message(t('There was a problem removing the page from the index.'), 'error');
        watchdog('ApacheSolr NAN search', 'The apache solr non-node item %id could not be deleted', array('%id' => $id), WATCHDOG_ERROR);
      }
    }
  }
  if (!empty($delete_keys)) {
    $deleted = db_delete('apachesolr_nan_index_paths')->condition('id', $delete_keys, 'IN')->execute();
    if ($deleted > 0) {
      drupal_set_message(t('The following paths were deleted:'));
      foreach ($delete as $removed_path) {
        drupal_set_message($removed_path);
      }
    }
  }
}
