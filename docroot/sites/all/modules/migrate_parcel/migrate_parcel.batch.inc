<?php
/**
 * @file
 * Contains Batch API callbacks.
 */

/**
 * Batch process callback: create the folder to write files to.
 */
function migrate_parcel_batch_process_prepare($info, &$context) {
  $directory = $info['directory'];
  $result = file_prepare_directory($directory, FILE_MODIFY_PERMISSIONS | FILE_CREATE_DIRECTORY);
}

/**
 * Batch process callback: create the info file.
 */
function migrate_parcel_batch_process_info($info, &$context) {
  $info_file_properties = array(
    'machine_name' => $info['machine_name'],
    'description' => $info['description'],
    'entity' => $info['entity'],
    'bundle' => $info['bundle'],
  );
  $output = '';
  // Don't export empty properties: it confuses drupal_parse_info_file().
  foreach (array_filter($info_file_properties) as $key => $value) {
    $output .= "$key = $value\n";
  }

  $filename = $info['directory'] . '/' . $info['machine_name'] . '.info';

  file_unmanaged_save_data($output, $filename, FILE_EXISTS_REPLACE);
}

/**
 * Batch process callback: write entities to the CSV file.
 *
 * @param $info
 *  An array of info for the batch process:
 *    - machine_name: The parcel (and migration) machine name.
 *    - entity: The machine name of the entity type to use.
 *    - bundle: The machine name of the bundle to use.
 *    - directory: The complete path to the directory to save the file into,
 *      eg "public://migrate_parcel/$machine_name".
 */
function migrate_parcel_batch_process_csv($info, &$context) {
  $entity_type = $info['entity'];
  $bundle_name = $info['bundle'];
  $entity_info = entity_get_info();
  // The number of entities to process each time.
  $limit = 5;

  if (empty($context['sandbox'])) {
    // Store the id key. YAGNI?
    $context['sandbox']['id'] = $entity_info[$entity_type]['entity keys']['id'];

    // Get the fields from the migrate destination class.
    // We export the fields that Migrate understands which we are capable of
    // getting from the entity with Entity Property API.
    $context['sandbox']['fields'] = migrate_parcel_get_fields($entity_type, $bundle_name);

    // Open the file.
    $context['sandbox']['filepath'] = $info['directory'] . '/' . $info['machine_name'] . '.csv';
    $context['sandbox']['handle'] = fopen($context['sandbox']['filepath'], 'w+');
    // Write the CSV header.
    $result = fputcsv($context['sandbox']['handle'], $context['sandbox']['fields']);

    // Get the entity ids.
    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $entity_type);
    $query->entityCondition('bundle', $bundle_name);
    //$query->propertyOrderBy($options['order'], 'ASC');
    $result = $query->execute();

    if (!isset($result[$entity_type])) {
      // Nothing to do!
      return;
    }

    // Store the entity ids in the sandbox. This array will be worn down as
    // we successively load entities from its ids.
    $context['sandbox']['ids'] = array_keys($result[$entity_type]);
    $context['sandbox']['total'] = count($result[$entity_type]);

    // Initialize results count.
    $context['results']['count'] = 0;
    // Store other things for the finished callback.
    $context['results']['directory'] = $info['directory'];
  }

  // Reopen file in case of memory out.
  if (!is_resource($context['sandbox']['handle'])) {
    // Reopen file in case of memory out, in append mode.
    $context['sandbox']['handle'] = fopen($context['sandbox']['filepath'], 'a+');
  }

  // Load a batch of $limit entities.
  $ids = array_splice($context['sandbox']['ids'], 0, $limit);
  $entities = entity_load($entity_type, $ids);

  foreach ($entities as $entity) {
    // Get the metadata wrapper for this entity.
    $wrapper = entity_metadata_wrapper($entity_type, $entity);
    $properties = $wrapper->getPropertyInfo();

    // Build up an array of values to export.
    $line = array_fill_keys($context['sandbox']['fields'], '');
    foreach ($context['sandbox']['fields'] as $field) {
      // Skip fields that Entity property API doesn't know how to handle.
      // My, this is a mishmash: who is the authority here, Migrate fields or
      // Entity properties??
      if (!isset($properties[$field])) {
        continue;
      }

      $property_info = $properties[$field];
      // WTF EntityAPI? 'type' is not always set? http://drupal.org/node/1717320
      $property_info += entity_property_info_defaults();
      $type = entity_property_extract_innermost_type($property_info['type']);

      if (isset($entity_info[$type])) {
        // If the type is an entity type, then this is a referenced entity.
        // Get the label.
        // The alternative here would be to use 'raw' and export an id.
        $value = $wrapper->{$field}->label();
      }
      else {
        // Otherwise, get the value.
        // @todo: should we use raw() here?
        $value = $wrapper->{$field}->value();
      }

      // An array value is imploded.
      // We can't sniff for the property type being 'list<FOO>' because, yet
      // again, EntityAPI has lousy DX. See http://drupal.org/node/1717336.
      if (is_array($value)) {
        // @todo: escape separator character.
        $value = implode('|', $value);
      }

      $line[$field] = $value;
    }

    //dsm($line);
    // Write the line to the CSV file.
    $result = fputcsv($context['sandbox']['handle'], $line);
    /*
    // Cribbed from taxonomy_csv. why do they do this by hand?
    $line = implode(',', $line) . "\n";
    $result = fwrite($context['sandbox']['handle'], $line);
    */

    $context['sandbox']['progress']++;
    $context['results']['count']++;

    if ($result === FALSE) {
      // TODO: report a problem!
    }
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['total'];
  }
}

/**
 * Batch 'finished' callback.
 */
function migrate_parcel_batch_finished($success, $results, $operations) {
  if ($success) {
    // Here we do something meaningful with the results.
    $message = t("Exported !count entities to @file. You should move this folder to 'sites/all/parcels'.", array(
      '!count' => $results['count'],
      '@file' => $results['directory'],
    ));
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
  }
  drupal_set_message($message);
}
