<?php

/**
 * @file
 * Drush commands for MongoDB Indexer.
 */

/**
 * Implements hook_drush_command().
 */
function mongodb_indexer_drush_command() {
  $items = array();
  $items['mongodb-indexer-mark-reindexing'] = array(
    'callback' => 'mongodb_indexer_mark_reindex',
    'description' => 'Mark entity bundle for reindexing.',
    'aliases' => array('mi-mr'),
  );
  $items['mongodb-indexer-start-reindexing'] = array(
    'callback' => 'mongodb_indexer_start_reindexing',
    'description' => 'Index all queue items from reindexing queue.',
    'aliases' => array('mi-i'),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function mongodb_indexer_drush_help($section) {
  switch ($section) {
    case 'mongodb-indexer-mark-reindexing':
      return dt("Mark entity bundle for reindexing.");

    case 'mongodb-indexer-start-reindexing':
      return dt("Index all queue items from reindexing queue.");
  }
}

/**
 * Mark all selected entity type and bundle items as need to be indexed.
 */
function mongodb_indexer_mark_reindex() {
  // Get selected entity and bundle.
  $selected_entity_bundle = mongodb_indexer_get_selected_entity_bundle();
  $options = array(
    1 => 'all',
  );
  // The next choice should be 2 as 0 is for cancel and 1 is for 'all'.
  $choice_index = 2;
  foreach ($selected_entity_bundle as $key => $row) {
    $bundle = $row['bundle'] ? $row['bundle'] : FALSE;
    if (!$bundle) {
      $label = $row['entity_type'];
    }
    else {
      $label = $row['bundle'] . ' of entity type ' . $row['entity_type'];
    }
    $options[$choice_index] = $label;
    $selected_entity_bundle[$key]['choice'] = $choice_index;
    $selected_entity_bundle[$key]['label'] = $label;
    $choice_index++;
  }

  $choice = drush_choice($options, t('Select entity bundle to reindex'));
  if ($choice == 0) {
    return FALSE;
  }
  foreach ($selected_entity_bundle as $row) {
    // An array contain max MONGODB_INDEXER_DEFAULT_BATCH_SIZE items.
    $items_to_index = array();

    $entity_type = $row['entity_type'];
    $bundle = $row['bundle'];
    $label = $row['label'];
    if (($row['choice'] == $choice) || ($choice == 1)) {
      // Do an EFQ to get all ids of this entity type and bundle.
      $query = new EntityFieldQuery();
      $query->entityCondition('entity_type', $entity_type);
      if ($bundle) {
        $query->entityCondition('bundle', $bundle);
      }
      $entities = $query->execute();
      if (!empty($entities[$entity_type])) {
        $count = 0;
        $total_left = count($entities[$entity_type]);
        $output = format_plural(
          $total_left,
          "Preparing to add to reindex queue 1 item of !label.",
          "Preparing to add to reindex queue @count items of !label.",
          array(
            '!label' => $label,
          )
        );
        drush_log($output, "ok");
        foreach ($entities[$entity_type] as $id => $entity) {
          // User has a weird default user with id = 0
          if (!$id) {
            continue;
          }
          $items_to_index[] = array(
            'entity_id' => $id,
            'entity_type' => $entity_type,
            'bundle' => $bundle,
          );
          $count++;

          if ($count == MONGODB_INDEXER_DEFAULT_BATCH_SIZE) {
            $count = 0;
            if (!empty($items_to_index)) {
              $total_left = $total_left - count($items_to_index);
              $output = format_plural(
                count($items_to_index),
                "Adding a batch of 1 item of !label. Remaining items !total_left.",
                "Adding a batch of @count items of !label. Remaining items !total_left.",
                array(
                  '!label' => $label,
                  '!total_left' => $total_left,
                )
              );
              drush_log($output, "ok");
              mongodb_indexer_mark_items_reindex($items_to_index);
              // Reset item to index.
              $items_to_index = array();
            }
          }
        }

        // This is for the last bit of the items,
        // which could not be exact MONGODB_INDEXER_DEFAULT_BATCH_SIZE number.
        if (!empty($items_to_index)) {
          $total_left = $total_left - count($items_to_index);
          $output = format_plural(
            count($items_to_index),
            "Adding a batch of 1 item of !label. Remaining items !total_left.",
            "Adding a batch of @count items of !label. Remaining items !total_left.",
            array(
              '!label' => $label,
              '!total_left' => $total_left,
            )
          );
          drush_log($output, "ok");
          mongodb_indexer_mark_items_reindex($items_to_index);
          // Reset item to index.
          $items_to_index = array();
          drush_log("Finised adding items of " . $label . " to queue for reindexing.", "ok");
        }
      }
      else {
        drush_log("No items of " . $label . " found. Nothing was added to the queue.", "ok");
      }
    }
  }

  drush_log("Everything is Done!", "completed");
}

/**
 * Index all queue in queue table.
 */
function mongodb_indexer_start_reindexing() {
  if (!drush_confirm("Are you sure?")) {
    return FALSE;
  }
  drush_log("Start reindexing. This may take a while...", "ok");

  // Create a batch process for processing items in the reindexing queue.
  _mongodb_indexer_batch_reindexing_create();
  // Start the batch process.
  drush_backend_batch_process();

  drush_log("Reindexing done!", "completed");
}
