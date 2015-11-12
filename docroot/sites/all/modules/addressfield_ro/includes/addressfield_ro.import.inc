<?php

/**
 * @file
 * Importer for addressfield_ro module.
 */

/**
 * Form builder addressfield_ro_import_form for romanian addresses import.
 */
function addressfield_ro_import_form($form, $form_state) {
  $form['title'] = array(
    '#type' => 'markup',
    '#markup' => '<h2>' . t('Addresses Importer') . '</h2>',
  );

  $form['info'] = array(
    '#type' => 'item',
    '#markup' => t('This will import addresses from the CSV file that shipped with the module.'),
  );
  $form['t_container'] = array('#type' => 'container');
  $form['t_container']['truncate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Clear existing addresses from database before importing'),
    '#checked' => FALSE,
  );
  $form['t_container']['message'] = array(
    '#type' => 'item',
    '#markup' => '<b>' . t('This operation canot be undone!') . '</b>',
    '#states' => array(
      // Hide the settings when the cancel notify checkbox is disabled.
      'invisible' => array(
        ':input[name="truncate"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );

  return $form;
}

/**
 * Submit handler for addressfield_ro_import_form.
 */
function addressfield_ro_import_form_submit($form, &$form_state) {
  $batch = addressfield_ro_import_batch();

  if (!$batch) {
    drupal_set_message(t('There was a problem importing addresses.'));
    return;
  }

  // Clear all database addresses when user selected this option.
  if ($form_state['values']['truncate']) {
    db_truncate(ADDRESSFIELD_RO_BASE_TABLE)->execute();
  }

  batch_set($batch);

  drupal_set_message(t('Addresses successfully imported.'));
  $form_state['redirect'] = addressfield_ro_basepath();
}

/**
 * Batch builder, used to import addresses from a CSV file.
 *
 * @param string $file
 *   The file name.
 * @param int $batch_items
 *   The number of address items to be added in one worker.
 *
 * @return array
 *   The batch array, as expected by the batch_set().
 */
function addressfield_ro_import_batch($file = 'addressfield_ro.csv', $batch_items = 100) {
  // Open the file for reading.
  $file = drupal_get_path('module', 'addressfield_ro') . "/includes/{$file}";
  if (!file_exists($file)) {
    return FALSE;
  }
  $handle = fopen($file, "r");
  if ($handle == FALSE) {
    return FALSE;
  }

  // Validate the number of items per worker. It can't be a negative number.
  if ($batch_items < 1) {
    $batch_items = 100;
  }
  // Batch operations.
  $operations = array();
  // Items counter and container.
  $i = 0;
  $items = array();
  $counter = 1;
  // Loop trough the CSV line by line.
  while (($data = fgetcsv($handle, 1000)) !== FALSE) {
    // Gather addresses to be processed per request.
    if (isset($data[0]) && isset($data[1])) {
      $items[] = array(
        'locality' => $data[0],
        'administrative_area' => $data[1],
      );
      $i++;
    }
    // Add addresses and reset the counter.
    if ($batch_items == $i) {
      $i = 0;
      $operations[] = array('addressfield_ro_worker', array($items, $counter));
      $items = array();
      $counter++;
    }
  }
  // Close the file handler.
  fclose($handle);

  // Return the batch.
  return array(
    'operations' => $operations,
    'file' => drupal_get_path('module', 'addressfield_ro') . '/includes/addressfield_ro.import.inc',
  );
}

/**
 * Batch worker, importing items.
 *
 * @param array $items
 *   Batch $items.
 * @param array $context
 *   Batch Context.
 */
function addressfield_ro_worker(array $items, $counter, array &$context) {

  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
  }

  $context['message'] = t('Processing item %count', array('%count' => $counter));

  // Prepare the field culomns to be insered.
  $insert = db_insert(ADDRESSFIELD_RO_BASE_TABLE)
      ->fields(array('locality', 'administrative_area'));
  // Add every row item as values.
  foreach ($items as $item) {
    $insert->values($item);
  }
  try {
    $insert->execute();
  }
  catch (Exception $e) {
    $msg = t('Error importing addesses: @error', array('@error' => $e->getMessage()));
    watchdog('addressfield_ro', $msg);
  }
}
