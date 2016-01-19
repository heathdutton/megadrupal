<?php
/**
 * @file migrate_parcel.module
 * Contains UI page callbacks and forms.
 */

/**
 * Menu callback for the overview page.
 */
function migrate_parcel_overview_page() {
  $entity_info = entity_get_info();
  $parcels = migrate_parcel_get_parcels();
  // This gets us all the migrations rather than just ours, but it's easier.
  $migrations = migrate_migrations();

  $rows = array();
  foreach ($parcels as $parcel_name => $parcel) {
    migrate_parcel_load($parcel_name);

    $row = array();
    $row[] = l($parcel_name, "admin/content/migrate/parcels/parcel/$parcel_name");
    $row[] = $entity_info[$parcel['entity']]['label'];
    $row[] = $entity_info[$parcel['entity']]['bundles'][$parcel['bundle']]['label'];

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $parcel['entity']);
    $query->entityCondition('bundle', $parcel['bundle']);
    $count_entities = $query->count()->execute();

    $row[] = $count_entities;

    $migration = $migrations[$parcel_name];
    if (method_exists($migration, 'importedCount')) {
      $imported = $migration->importedCount();
    }
    $row[] = $imported;

    $rows[] = $row;
  }

  $header = array(
    t('Parcel'),
    t('Entity type'),
    t('Bundle'),
    t('Existing entities'),
    t('Imported entities'),
  );

  return theme('table', array(
    'rows' => $rows,
    'header' => $header,
  ));
}

/**
 * Detail page for a single parcel.
 */
function migrate_parcel_view($parcel_info) {
  $machine_name = $parcel_info['machine_name'];
  $entity_type = $parcel_info['entity'];
  $entity_info = entity_get_info($entity_type);
  $migrations = migrate_migrations();
  $migration = $migrations[$machine_name];

  $rows = array();

  // Basics.
  $rows[] = array(t('Machine name'), $parcel_info['machine_name']);
  $rows[] = array(t('Description'), $parcel_info['description']);
  $rows[] = array(t('Entity'), $entity_info['label']);
  $rows[] = array(t('Bundle'), l($entity_info['bundles'][$parcel_info['bundle']]['label'], _field_ui_bundle_admin_path($entity_type, $parcel_info['bundle'])));

  // Migration details.
  if (method_exists($migration, 'importedCount')) {
    $imported = $migration->importedCount();
  }

  $rows[] = array(t('Imported entities'), $imported);

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', $entity_type);
  $query->entityCondition('bundle', $parcel_info['bundle']);
  $count_entities = $query->count()->execute();

  $rows[] = array(t('Total entities'), $count_entities);

  // Get entities which don't exist in the parcel.
  $query = migrate_parcel_query_entity_join_map($parcel_info);
  // Add the NULL condition.
  $query->isNull('m.sourceid1');
  $count = $query->countQuery()->execute()->fetchField();
  $row = array(t('Entities not in parcel'), $count);
  if ($count) {
    $row[] = l('clean up parcel', "admin/content/migrate/parcels/parcel/$machine_name/cleanup");
  }
  $rows[] = $row;

  // Pad out the operations column.
  foreach ($rows as &$row) {
    if (count($row) == 2) {
      $row[] = '';
    }
  }

  $header = array(
    t('Property'),
    t('Status'),
    t('Operations'),
  );

  return theme('table', array(
    'rows' => $rows,
    'header' => $header,
  ));
}

/**
 * Form builder for creating an exported parcel.
 */
function migrate_parcel_export_form($form, &$form_state) {
  $entity_info = entity_get_info();

  // Build grouped options for the select form element.
  $options = array();
  foreach ($entity_info as $entity_type => $entity_data) {
    $options[$entity_data['label']] = array();
    foreach ($entity_data['bundles'] as $bundle_name => $bundle_data) {
      $options[$entity_data['label']][$entity_type . ':' . $bundle_name] = $entity_data['label'] . ': ' . $bundle_data['label'];
    }
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Machine name'),
    '#description' => t('The machine name for the migration.'),
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#required' => TRUE,
  );

  $form['export_bundles'] = array(
    '#type' => 'select',
    '#title' => t('Entities to export'),
    '#description' => t('Select a bundle. All entities of this bundle will be exported.'),
    '#options' => $options,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Export parcel'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Validation handler.
 */
function migrate_parcel_export_form_validate($form, &$form_state) {
  if (preg_match('@[^a-zA-Z0-9]@', $form_state['values']['name'])) {
    form_set_error('name', t('Migration machine name may only contain uppercase and lowercase letters and numbers.'));
  }
}

/**
 * Submit handler.
 */
function migrate_parcel_export_form_submit($form, &$form_state) {
  $export_bundle = $form_state['values']['export_bundles'];

  list($entity_type, $bundle_name) = explode(':', $export_bundle);
  $machine_name = $form_state['values']['name'];

  $info = array(
    'machine_name'      => $machine_name,
    'description'       => $form_state['values']['description'],
    'entity'       => $entity_type,
    'bundle'       => $bundle_name,
    // @TODO: configurable filepath & making a subdirectory!
    'directory'         => "public://migrate_parcel/$machine_name"
  );
  // @TODO: put it in "sites/all/parcels/$machine_name/$machine_name.csv"?

  $batch = array(
    'operations' => array(
      array('migrate_parcel_batch_process_prepare', array($info)),
      array('migrate_parcel_batch_process_info', array($info)),
      array('migrate_parcel_batch_process_csv', array($info)),
    ),
    'finished' => 'migrate_parcel_batch_finished',
    'title' => t('Processing entities'),
    'init_message' => t('Export is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Entity export has encountered an error.'),
    'file' => drupal_get_path('module', 'migrate_parcel') . '/migrate_parcel.batch.inc',
  );
  batch_set($batch);
}

/**
 * Update form.
 */
function migrate_parcel_update_form($form, &$form_state) {
  $entity_info = entity_get_info();
  $parcels = migrate_parcel_get_parcels();
  // This gets us all the migrations rather than just ours, but it's easier.
  $migrations = migrate_migrations();

  // TODO: convert this to a table.
  $options = array();
  foreach ($parcels as $parcel_name => $parcel) {
    $migration = $migrations[$parcel_name];
    if (method_exists($migration, 'importedCount')) {
      $imported = $migration->importedCount();
      $processed = $migration->processedCount();
    }

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', $parcel['entity']);
    $query->entityCondition('bundle', $parcel['bundle']);
    $count_entities = $query->count()->execute();

    $options[$parcel_name] = t("@name (@entity: @bundle) Imported: !count-imported, existing entities: !count-entities", array(
      '@name' => $parcel_name,
      '@entity' => $entity_info[$parcel['entity']]['label'],
      '@bundle' => $entity_info[$parcel['entity']]['bundles'][$parcel['bundle']]['label'],
      '!count-imported' => $imported,
      '!count-entities' => $count_entities,
    ));
  }
  $form['#parcels'] = $parcels;

  $form['parcels'] = array(
    '#type' => 'radios',
    '#title' => t('Parcels to update'),
    '#description' => t('Select an entity parcel to update.'),
    '#options' => $options,
    '#required' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Update parcel'),
    '#weight' => 40,
  );

  return $form;
}

/**
 * Submit handler for the parcel update form.
 */
function migrate_parcel_update_form_submit($form, &$form_state) {
  $parcel_name = $form_state['values']['parcels'];
  $parcels = $form['#parcels'];

  // We can just take the parcel info array as our info for the batch.
  $info = $parcels[$parcel_name];
  // Though we need to add in the folder path.
  $info['directory'] = dirname($info['source_file']);

  $batch = array(
    'operations' => array(
      // This writes direct to the existing CSV location.
      // That's okay as we're not writing code, just data.
      array('migrate_parcel_batch_process_csv', array($info)),
    ),
    'finished' => 'migrate_parcel_batch_finished',
    'title' => t('Processing entities'),
    'init_message' => t('Export is starting.'),
    'progress_message' => t('Processed @current out of @total.'),
    'error_message' => t('Entity export has encountered an error.'),
    'file' => drupal_get_path('module', 'migrate_parcel') . '/migrate_parcel.batch.inc',
  );
  batch_set($batch);
}

/**
 * Form callback to clean up entities not present in a parcel.
 *
 * @param $parcel_info
 *  A parcel info array.
 */
function migrate_parcel_cleanup_form($form, &$form_state, $parcel_info) {
  $entity_type = $parcel_info['entity'];
  $entity_info = entity_get_info($entity_type);

  $query = migrate_parcel_query_entity_join_map($parcel_info);
  // Add the NULL condition.
  $query->isNull('m.sourceid1');

  // Add the label-ish field.
  if (isset($entity_info['entity keys']['label'])) {
    $query->addField('e', $entity_info['entity keys']['label'], 'label');
  }
  // @todo: deal with the else!

  $result = $query->execute();
  $entity_data = $result->fetchAllKeyed();

  if (!count($entity_data)) {
    drupal_set_message(t('No entities to clean up.'));
    return $form;
  }

  $form['#entity_ids'] = array_keys($entity_data);
  $form['#parcel_info'] = $parcel_info;

  $form['warning'] = array(
    '#markup' => '<p class="warning">' . t('WARNING! You are about to delete !count @entity entities.', array(
      '!count' => count($entity_data),
      '@entity' => $entity_info['label'],
    )) . '</p>',
  );

  $form['info'] = array(
    '#type' => 'fieldset',
    '#title' => t('Entity list'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $items = array();
  foreach ($entity_data as $id => $label) {
    $items[] = "$id => $label";
  }
  $form['info']['list'] = array(
    '#markup' => '<p>' . theme('item_list', array(
      'items' => $items,
    )) . '</p>',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete entities'),
  );

  return $form;
}

/**
 * Submit callback to delete entities.
 */
function migrate_parcel_cleanup_form_submit($form, &$form_state) {
  // TODO. Use Batch API.
  $parcel_info = $form['#parcel_info'];
  $entity_ids = $form['#entity_ids'];

  $status = entity_delete_multiple($parcel_info['entity'], $entity_ids);

  if ($status === FALSE) {
    drupal_set_message(t('There was a problem deleting the entities.'), 'warning');
    return;
  }

  drupal_set_message(t('Deleted !count entities.', array(
    '!count' => count($entity_ids),
  )));

  $form_state['redirect'] = 'admin/content/migrate/parcels/parcel/' . $parcel_info['machine_name'];
}

/**
 * Helper function to query entities joined to the map table.
 *
 * This products a left query from the entity base table to the migrate map
 * table, and thus can be used as a starting point for getting entities which
 * are not imported.
 *
 * @param $parcel_info
 *  A parcel info array.
 *
 * @return
 *  A SelectQuery object, not yet executed. It has these table aliases:
 *  - 'e': The entity base table.
 *  - 'm': The migrate map table.
 *  The id field on the entity table is also added, with alias 'id'.
 *  The query has no conditions.
 */
function migrate_parcel_query_entity_join_map($parcel_info) {
  $machine_name = $parcel_info['machine_name'];
  $entity_type = $parcel_info['entity'];
  $entity_info = entity_get_info($entity_type);

  // Query on the entity table.
  $query = db_select($entity_info['base table'], 'e');
  // Add the entity id field.
  $id_field = $entity_info['entity keys']['id'];
  $query->addField('e', $id_field, 'id');
  // Join to the migrate map table.
  // @TODO: this is a hack until I figure out how to get the table name
  // cleanly out of the migration.
  $migrate_table = 'migrate_map_' . drupal_strtolower($machine_name);
  $query->leftJoin($migrate_table, 'm', "e.$id_field = m.destid1");
  if ($entity_type == 'taxonomy_term') {
    // *SIGH* Taxonomy terms never quite made it to be true entities, did they?
    $vocabularies = taxonomy_vocabulary_get_names();
    $vid = $vocabularies[$parcel_info['bundle']]->vid;
    $query->condition('vid', $vid);
  }
  else {
    $query->condition($entity_info['entity keys']['bundle'], $parcel_info['bundle']);
  }

  return $query;
}
