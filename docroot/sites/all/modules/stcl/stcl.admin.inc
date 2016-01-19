<?php

/**
 * @file
 * SQL Storage cleanup routines.
 */

/**
 * Module's admin form.
 */
function stcl_admin($form, &$form_state) {
  $form = array();

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Cleanup')
  );
  $form['submit2'] = array(
    '#type' => 'submit',
    '#value' => t('Get stats')
  );

  return $form;
}

/**
 * Admin form submit handler.
 */
function stcl_admin_submit(&$form, &$form_state) {

  $values = $form_state['values'];

  $entities = entity_get_info();
  // We should get list of tables.
  // Thats why we can't use field_info_field_map().
  $fields = field_info_fields();

  $all_ids = array();
  foreach ($fields as $field_name => $data) {
    if (!isset($data['storage']) || $data['storage']['type'] != 'field_sql_storage') {
      continue;
    }
    // We should obtain array of tbl_name => entity_name => entity_id for delete.
    foreach ($data['bundles'] as $entity_name => $bundles) {

      reset($data['storage']['details']['sql']['FIELD_LOAD_CURRENT']);
      reset($data['storage']['details']['sql']['FIELD_LOAD_REVISION']);
      $tbl_current = key(
        $data['storage']['details']['sql']['FIELD_LOAD_CURRENT']
      );
      $tbl_revision = key(
        $data['storage']['details']['sql']['FIELD_LOAD_REVISION']
      );
      // Preparing for array_merge.
      if (!isset($all_ids[$tbl_current])) {
        $all_ids[$tbl_current] = array();
      }
      if (!isset($all_ids[$tbl_revision])) {
        $all_ids[$tbl_revision] = array();
      }
      if (!$entities[$entity_name]['fieldable']) {
        continue;
      }
      // Obtain abandoned ids.
      $id_field_name = $entities[$entity_name]['entity keys']['id'];
      $base_table = $entities[$entity_name]['base table'];

      $base_subquery = db_select($base_table, 'b')
        ->fields('b', array($id_field_name));
      $base_query = db_select($tbl_current, 'c')
        ->fields('c', array('entity_id'))
        ->condition('entity_type', $entity_name)
        ->condition('entity_id', $base_subquery, 'NOT IN')
        ->execute()
        ->fetchAllAssoc('entity_id');
      $current_ids = array_keys($base_query);
      if (count($current_ids) > 0) {
        $all_ids[$tbl_current] = array_merge(
          $all_ids[$tbl_current],
          array($entity_name => $current_ids)
        );
      }

      if (isset($entities[$entity_name]['revision table'])) {
        $revision_id_field_name = $entities[$entity_name]['entity keys']['revision'];
        $revision_table = $entities[$entity_name]['revision table'];

        $revisions_subquery = db_select($revision_table, 'r')
          ->fields('r', array($revision_id_field_name));
        $revisions_query = db_select($tbl_revision, 'cr')
          ->fields('cr', array('entity_id'))
          ->condition('entity_type', $entity_name)
          ->condition('entity_id', $revisions_subquery, 'NOT IN')
          ->execute()
          ->fetchAllAssoc('entity_id');
        $revision_ids = array_keys($base_query);

        if (count($revision_ids) > 0) {
          $all_ids[$tbl_revision] = array_merge(
            $all_ids[$tbl_revision],
            array($entity_name => $revision_ids)
          );
        }
      }
      else {
        // Entity revisions table undefined.
        // We still have to cleanup revisions storage.
        if (count($current_ids) > 0) {
          $all_ids[$tbl_revision] = array_merge(
            $all_ids[$tbl_revision],
            array($entity_name => $current_ids)
          );
        }
      }
      // Cleanup empty data.
      if (count($all_ids[$tbl_revision]) == 0) {
        unset($all_ids[$tbl_revision]);
      }
      if (count($all_ids[$tbl_current]) == 0) {
        unset($all_ids[$tbl_current]);
      }
    }
  }

  if (count($all_ids) > 0) {
    // Run cleanup.
    if ($values['op'] == $values['submit']) {
      stcl_cleanup_transaction($all_ids);
    }
    // Run get info.
    if ($values['op'] == $values['submit2']) {
      stcl_get_info($all_ids);
    }
  }
  else {
    drupal_set_message(
      t('Nothing should be cleared, you have a clean database.')
    );
  }
}

/**
 * Helper function with transaction for final cleanup.
 *
 * @param array $all_ids
 *   Associative array tbl_name => entity_name => entity_id for cleanup.
 */
function stcl_cleanup_transaction($all_ids) {
  $ecounter = &drupal_static(__FUNCTION__);
  $transaction = db_transaction();
  try {
    foreach ($all_ids as $tbl_name => $data) {
      foreach ($data as $entity_name => $ids) {
        $ecounter += count($ids);
        db_delete($tbl_name)
          ->condition('entity_type', $entity_name)
          ->condition('entity_id', $ids, 'IN')
          ->execute();
      }
    }
  }
  catch (Exception $e) {
    $transaction->rollback();
    $ecounter = 0;
    watchdog_exception('stcl', $e->getMessage());
  }
  drupal_set_message(
    t('Deleted !count items from database', array('!count' => $ecounter))
  );
}

/**
 * Helper function for displaying information.
 *
 * @param array $all_ids
 *   Associative array tbl_name => entity_name => entity_id for cleanup.
 */
function stcl_get_info($all_ids) {
  $ecounter = array();
  foreach ($all_ids as $tbl_name => $data) {
    foreach ($data as $entity_name => $ids) {
      $count = count($ids);
      $ids_to_display = ($count > 5) ? implode(
          ',',
          array_slice($ids, 0, 5)
        ) . ',...' : implode(',', $ids);
      $ecounter[] = array(
        $entity_name,  $tbl_name,
        $count,
        $ids_to_display
      );
    }
  }
  if (count($ecounter) > 0) {
    drupal_set_message(
      t('Abandoned database items.') . theme(
        'table',
        array(
          'header' => array(
            t('Entity'),
            t('Table name'),
            t('Counter'),
            t('Item list')),
          'rows' => $ecounter
        )
      )
    );
    return;
  }
  drupal_set_message(
    t('Nothing should be cleared, you have a clean database.')
  );
}
