<?php

/**
 * @file
 * Batch callbacks for taxonomy_orphanage.
 */

/**
 * Batch callback to remove orphaned taxonomy references from entities.
 */
function taxonomy_orphanage_batch_roundup($field, $limit, $display_finished, &$context) {
  $batch_limit = ($limit > 0 && $limit < 100) ? $limit : 100;
  $query = db_select('field_data_' . $field['field_name'], 'f');
  $query
    ->fields('f', array('entity_type', 'entity_id', $field['field_name'] . '_tid'))
    ->leftJoin('taxonomy_term_data', 't', 't.tid=f.' . $field['field_name'] . '_tid');
  $query
    ->where('t.tid IS NULL')
    ->orderBy('f.entity_id', 'ASC');

  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = $limit;
    $count_query = $query->countQuery();
    $count = $count_query->execute()->fetchField();
    if ($limit <= 0 || $count < $limit) {
      $context['sandbox']['max'] = $count;
    }

    if (!isset($context['results']['count'])) {
      $context['results']['count'] = 0;
      $context['results']['display_finished'] = $display_finished;
    }
  }

  // First collect all the tids that exist for this field on the entities.
  $terms = array();
  $query->range(0, $batch_limit);
  $res = $query->execute();
  foreach ($res as $target) {
    $context['sandbox']['progress']++;
    $entity = array_shift(entity_load($target->entity_type, array($target->entity_id)));
    if (!$entity) {
      continue;
    }
    foreach ($entity->{$field['field_name']}[$entity->language] as $k => $term) {
      if ($term['tid'] == $target->{$field['field_name'] . '_tid'}) {
        unset($entity->{$field['field_name']}[$entity->language][$k]);
        $context['results']['count']++;
        if (entity_save($target->entity_type, $entity) === FALSE) {
          throw new Exception('Failed to save entity type:' . $target->entity_type . ' id: ' . $target->entity_id);
        }
        break;
      }
    }
    if ($context['sandbox']['progress'] == $context['sandbox']['max']) {
      break;
    }
  }

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Finished batch callback.
 */
function taxonomy_orphanage_batch_finished($success, $results, $operations) {
  if ($success) {
    $msg = t('@count orphaned taxonomy references have been removed.', array('@count' => $results['count']));
    $dsm_type = 'status';
    $log_type = WATCHDOG_INFO;
  }
  else {
    $msg = t('There was a problem removing orphaned taxonomy terms.');
    $dsm_type = 'error';
    $log_type = WATCHDOG_ERROR;
  }

  watchdog('taxonomy_orphanage', $msg, array(), $log_type);
  if ($results['display_finished']) {
    drupal_set_message($msg, $dsm_type);
  }
}

