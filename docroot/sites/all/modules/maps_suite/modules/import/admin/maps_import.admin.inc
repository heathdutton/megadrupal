<?php

/**
 * @file
 * Administrative functions for MaPS Import module.
 */

use Drupal\maps_import\Profile\Profile;

/**
 * Form builder; manual import form.
 *
 * @see maps_import_import_form_validate()
 * @see maps_import_import_form_submit()
 * @ingroup forms
 */
function maps_import_import_form($form, &$form_state, Profile $profile) {
  drupal_set_title(t('Manual import for profile @profile', array('@profile' => $profile->getTitle())));

  if (variable_get('maps_import_lock') == 1) {
    drupal_set_message(t('There is already an operation under processing. Would you like to <a href="@url"> break the lock</a> ?', array('@url' => '/admin/maps-suite/lock/break')), 'warning');
  }

  $form['#profile'] = $profile;
  $form['info'] = array(
    '#markup' => t('All enabled profiles are usually processed through Drush scripts. However you may want to process some import operation manually, for either enabled or disabled profiles.<br/>'),
  );

  $form['operations'] = array(
    '#type' => 'fieldset',
    '#title' => t('Operations'),
    '#description' => t('This will process the selected operations.'),
  );

  if ($profile->getOptionsItem('differential')) {
    $form['operations']['differential'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use differential import.'),
      '#default_value' => 1,
    );
  }

  $operations = maps_import_import_get_import_operations($profile);
  $options = maps_suite_reduce_array($operations, 'title');

  if (!$profile->getConfiguration()) {
    $options = array_intersect_key($options, array('configuration_fetch' => TRUE));
  }

  $form['operations']['list'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Operations to process'),
    '#options' => $options,
    '#required' => TRUE,
  );

  $form['operations']['update'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
  );

  return $form;
}

/**
 * Form validation handler; .
 *
 * @see maps_import_import_form()
 * @see maps_import_import_form_submit()
 */
function maps_import_import_form_validate($form, &$form_state) {
  if (variable_get('maps_import_lock')) {
    form_set_error('', t('There is already operations under processing'));
  }
}

/**
 * Form submission handler; manual import form.
 *
 * @see maps_import_import_form()
 * @see maps_import_import_form_validate()
 */
function maps_import_import_form_submit($form, &$form_state) {
  // Do not go further if running tests.
  if (!empty($GLOBALS['drupal_test_info']['test_run_id'])) {
    return;
  }

  variable_set('maps_import_differential', !empty($form_state['values']['differential']));
  $operations = array_filter($form_state['values']['list']);

  $batch_operations = array();
  foreach ($operations as $name) {
    if ($operation = maps_import_get_import_operations($form['#profile'], $name)) {
      $op = new $operation['class']($form['#profile']);
      $batch_operations = array_merge($batch_operations, $op->batchOperations());
    }
  }

  $batch = array(
    'operations' => $batch_operations,
    'title' => t('Update for the profile %title', array('%title' => $form['#profile']->getTitle())),
    'file' => drupal_get_path('module', 'maps_import') . '/admin/maps_import.admin.inc',
    'finished' => 'maps_import_import_finished',
  );

  if (!empty($batch['operations'])) {
    variable_set('maps_import_lock', 1);
    batch_set($batch);
  }
  else {
    drupal_set_message(t('There is nothing to do.'));
  }
}

/**
 * Batch "finished" callback; manual import.
 */
function maps_import_import_finished($success, $results, $operations) {
  // Remove the lock.
  variable_set('maps_import_lock', 0);
  variable_set('maps_import_differential', 0);

  $message = array();
  $message_type = 'status';

  if (!empty($results['logs'])) {
    foreach ($results['logs'] as $operation => $log) {
      if (file_exists($log)) {
        $message[] = t('The log file for operation !operation is available at !log_url.', array('!operation' => $operation, '!log_url' => l($log, $log)));
      }

      module_invoke_all('maps_import_operation_finished', $operation);
    }
  }

  if ($success) {
    // Check if something went wrong.
    if (isset($results['process']) && FALSE === $results['process']) {
      array_unshift($message, t('The operation %name failed.', array('%name' => $results['operation'])));
      $message_type = 'error';
    }
    else {
      // Total operations.
      if (!empty($results['total'])) {
        $message[] = t('%count operations:', array('%count' => $results['total']));
      }

      // Details for updated / imported elements.
      if (!empty($results['indexes'])) {
        foreach ($results['indexes'] as $element => $index) {
          if (!empty($index['insert'])) {
            $message[] = t('%count %element have been added', array('%count' => $index['insert'], '%element' => $element));
          }

          if (!empty($index['update'])) {
            $message[] = t('%count %element have been updated', array('%count' => $index['update'], '%element' => $element));
          }
        }
      }
      if (!empty($results['indexes']['fetch'])) {
        $message[] = t('Succesfully fetched data from %count file from MaPS System®.', array('%count' => $results['indexes']['fetch']));
      }
    }
  }
  // An error occurred.
  else {
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    list($class, $method) = $error_operation[0];
    $message_type = 'error';
    $message[] = t('An error occurred while processing @operation with arguments :<br />!args<hr />Error message is: !message',
      array(
        '@operation' => get_class($class) . '::' . $method,
        '!args' => maps_suite_highlight_php($error_operation[1], TRUE),
        '!message' => isset($results['error']) ? $results['error'] : t('not specified')));

  }

  drupal_set_message(theme('item_list', array('items' => $message)));
}

/**
 * Return the list of operations available for ignore.
 *
 * @param Profile $profile
 *   The related profile.
 *
 * @return array
 *   The list of operations.
 */
function maps_import_import_get_import_operations(Profile $profile) {
  $operations = maps_import_get_import_operations($profile);

  // Remove fetch operations from the list.
  //unset($operations['configuration_fetch']);
  //if ($operations['objects_fetch']) {
  //  unset($operations['objects_fetch']);
  //}

  // Merge the media and object mapping operations.
  //if (isset($operations['media_mapping']) && isset($operations['object_mapping'])) {
  //  unset($operations['media_mapping']);
  //}

  // If available, merge the link and object mappign operations.
  //if (module_exists('maps_links') && isset($operations['link_mapping']) && isset($operations['object_mapping'])) {
  //  unset($operations['link_mapping']);
  //}

  return $operations;
}
