<?php

/**
 * @file
 * Class definition of InstagramFeedsPluginsNodeProcessor.
 */

/**
 * Creates nodes from feed items.
 */
class InstagramFeedsPluginsNodeProcessor extends FeedsNodeProcessor {

  /**
   * Retrieve the target entity's existing id if available. Otherwise return 0.
   *
   * @ingroup mappingapi
   *
   * @param FeedsSource $source
   *   The source information about this import.
   * @param FeedsParserResult $result
   *   A FeedsParserResult object.
   *
   * @return int
   *   The serial id of an entity if found, 0 otherwise.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    $query = db_select('feeds_item')
        ->fields('feeds_item', array('entity_id'))
        ->condition('entity_type', $this->entityType())
        ->condition('id', $source->id);

    // Iterate through all unique targets and test whether they do already
    // exist in the database.
    foreach ($this->uniqueTargets($source, $result) as $target => $value) {
      switch ($target) {
        case 'url':
          $entity_id = $query->condition('url', $value)->execute()->fetchField();
          break;

        case 'guid':
          $entity_id = $query->condition('guid', $value)->execute()->fetchField();
          break;
      }
      if (isset($entity_id)) {
        // Return with the content id found.
        return $entity_id;
      }
    }
    return 0;
  }

  /**
   * Process the result of the parsing stage.
   *
   * @param FeedsSource $source
   *   Source information about this import.
   * @param FeedsParserResult $parser_result
   *   The result of the parsing stage.
   */
  public function process(FeedsSource $source, FeedsParserResult $parser_result) {
    $state = $source->state(FEEDS_PROCESS);
    $mappings = _instagram_feeds_plugins_get_fields_mappings($source);
    $attempt = variable_get('instagram_feeds_download_attempts', 0);
    $missed_imgs = variable_get('instagram_feeds_missed_imgs', array());
    $mis_id = $source->id . '__' . $source->feed_nid;

    while ($item = $parser_result->shiftItem()) {
      $guid = $item[$mappings['unique']['source']];

      // Check if this item already exists.
      $entity_id = $this->existingEntityId($source, $parser_result);
      $skip_existing = $this->config['update_existing'] == FEEDS_SKIP_EXISTING;

      module_invoke_all('feeds_before_update', $source, $item, $entity_id);

      // If it exists, and we are not updating, pass onto the next item.
      if ($entity_id && $skip_existing) {
        if (isset($missed_imgs[$mis_id]['items'][$guid])) {
          unset($missed_imgs[$mis_id]['items'][$guid]);
        }
        if (isset($missed_imgs[$mis_id]['items']) && !count($missed_imgs[$mis_id]['items'])) {
          unset($missed_imgs[$mis_id]);
        }
        continue;
      }

      $hash = $this->hash($item);
      $changed = ($hash !== $this->getHash($entity_id));
      $force_update = $this->config['skip_hash_check'];

      // Do not proceed if the item exists, has not changed, and we're not
      // forcing the update.
      if ($entity_id && !$changed && !$force_update) {
        continue;
      }

      try {
        // Load an existing entity.
        if ($entity_id) {
          $entity = $this->entityLoad($source, $entity_id);

          // The feeds_item table is always updated with the info for the most
          // recently processed entity. The only carryover is the entity_id.
          $this->newItemInfo($entity, $source->feed_nid, $hash);
          $entity->feeds_item->entity_id = $entity_id;
          $entity->feeds_item->is_new = FALSE;
        }

        // Build a new entity.
        else {
          $entity = $this->newEntity($source);
          $this->newItemInfo($entity, $source->feed_nid, $hash);
        }

        // Set property and field values.
        $this->map($source, $parser_result, $entity);
        $this->entityValidate($entity);

        // Allow modules to alter the entity before saving.
        module_invoke_all('feeds_presave', $source, $entity, $item, $entity_id);
        if (module_exists('rules')) {
          rules_invoke_event('feeds_import_' . $source->importer()->id, $entity);
        }

        // Enable modules to skip saving at all.
        if (!empty($entity->feeds_item->skip)) {
          continue;
        }

        // This will throw an exception on failure.
        $this->entitySave($entity);

        // Allow modules to perform operations using the saved entity data.
        // $entity contains the updated entity after saving.
        module_invoke_all('feeds_after_save', $source, $entity, $item, $entity_id);

        // Track progress.
        if (empty($entity_id)) {
          $state->created++;
        }
        else {
          $state->updated++;
        }

        if (isset($missed_imgs[$mis_id]['items'][$guid])) {
          if (drupal_is_cli()) {
            echo 'Instagram Feeds: ', t('Missed image !guid was imported successfully.', array('!guid' => $guid)), "\n";
          }
          unset($missed_imgs[$mis_id]['items'][$guid], $missed_imgs[$mis_id]['attempt'][$guid],
              $missed_imgs[$mis_id]['last_attempt_time'][$guid]);
        }
        if (isset($missed_imgs[$mis_id]['items']) && !count($missed_imgs[$mis_id]['items'])) {
          unset($missed_imgs[$mis_id]);
        }
      }

      // Something bad happened, log it.
      catch (Exception $e) {
        drupal_set_message($e->getMessage(), 'warning');
        $message = $this->createLogMessage($e, $entity, $item);
        if ($attempt && !isset($missed_imgs[$mis_id]['items'][$guid])) {
          $missed_imgs[$mis_id]['items'][$guid] = $item;
          $missed_imgs[$mis_id]['attempt'][$guid] = 1;
          $missed_imgs[$mis_id]['last_attempt_time'][$guid] = REQUEST_TIME;
        }
        elseif ($attempt && $attempt > $missed_imgs[$mis_id]['attempt'][$guid]) {
          $missed_imgs[$mis_id]['attempt'][$guid]++;
          $missed_imgs[$mis_id]['last_attempt_time'][$guid] = REQUEST_TIME;
        }
        else {
          $state->failed++;
          if (isset($missed_imgs[$mis_id]['items'][$guid])) {
            unset($missed_imgs[$mis_id]['items'][$guid], $missed_imgs[$mis_id]['attempt'][$guid],
                $missed_imgs[$mis_id]['last_attempt_time'][$guid]);
            watchdog('Instagram Feeds', 'Re-importing error: @message', array('@message' => $message),
                WATCHDOG_ERROR);
          }
          if (isset($missed_imgs[$mis_id]['items']) && !count($missed_imgs[$mis_id]['items'])) {
            unset($missed_imgs[$mis_id]);
          }
        }
        $source->log('import', $message, array(), WATCHDOG_ERROR);
      }
    }
    variable_set('instagram_feeds_missed_imgs', $missed_imgs);

    // Set messages if we're done.
    if ($source->progressImporting() != FEEDS_BATCH_COMPLETE) {
      return;
    }
    $info = $this->entityInfo();
    $tokens = array(
      '@entity' => strtolower($info['label']),
      '@entities' => strtolower($info['label plural']),
    );
    $messages = array();
    if ($state->created) {
      $messages[] = array(
        'message' => format_plural(
            $state->created, 'Created @number @entity.', 'Created @number @entities.',
            array('@number' => $state->created) + $tokens
        ),
      );
    }
    if ($state->updated) {
      $messages[] = array(
        'message' => format_plural(
            $state->updated, 'Updated @number @entity.', 'Updated @number @entities.',
            array('@number' => $state->updated) + $tokens
        ),
      );
    }
    if ($state->failed) {
      $messages[] = array(
        'message' => format_plural(
            $state->failed, 'Failed importing @number @entity.', 'Failed importing @number @entities.',
            array('@number' => $state->failed) + $tokens
        ),
        'level' => WATCHDOG_ERROR,
      );
    }
    if (empty($messages)) {
      $messages[] = array(
        'message' => t('There are no new @entities.', array('@entities' => strtolower($info['label plural']))),
      );
    }
    foreach ($messages as $message) {
      drupal_set_message($message['message']);
      $source->log('import', $message['message'], array(), isset($message['level']) ? $message['level'] :
              WATCHDOG_INFO);
    }
  }

}
