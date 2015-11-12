<?php
/**
 * @file
 * Functions relating to the rehashing functions for the Felix module.
 *
 * When differentiators for Felix regions change, existing blocks need to have
 * their hash values updated in order for them to continue working.
 */
/**
 * Start batch.
 */
function felix_batch_rehash_start($region, $added, $removed, $default_values) {
  $batch = array(
    'operations' => array(
      array(
        'felix_bash_rehash_blocks',
        array(
          $region,
          $added,
          $removed,
          $default_values,
        ),
      ),
    ),
    'finished' => 'felix_bash_rehash_blocks_finished',
    'title' => t('Updating blocks'),
    'init_message' => t('Preparing to update blocks...'),
    'progress_message' => t('Updating blocks...'),
    'error_message' => t('Batch has encountered an error.'),
    'file' => drupal_get_path('module', 'felix') . '/felix.rehash.inc',
  );
  batch_set($batch);

  if (function_exists('drush_backend_batch_process')) {
    // Drush does not start the batch automatically, but we cannot skip
    // this batch without side effects.
    drush_backend_batch_process();
  }
}

/**
 * Batch operation callback.
 */
function felix_bash_rehash_blocks($region, $added, $removed, $default_values, &$context) {
  module_load_include('inc', 'apachesolr', 'apachesolr.index');
  if (empty($context['sandbox'])) {
    $context['sandbox']['progress'] = 0;
    $sql = 'SELECT COUNT(*) FROM {felix_block} WHERE region = :region';
    $count = db_query($sql, array(':region' => $region->name))->fetchField();
    $context['sandbox']['max'] = $count;
    $context['sandbox']['fbid'] = 0;
  }

  $blocks = db_select('felix_block', 'f')
    ->fields('f', array('fbid', 'hash_parts'))
    ->condition('f.fbid', $context['sandbox']['fbid'], '>')
    ->condition('f.region', $region->name)
    ->orderBy('f.fbid', 'asc')
    ->range(0, 50)
    ->execute()
    ->fetchAll();
  foreach ($blocks as $block) {
    $context['sandbox']['progress']++;
    $context['sandbox']['fbid'] = $block->fbid;
    $block->hash_parts = unserialize($block->hash_parts);
    $parts = array();
    foreach ($block->hash_parts as $name => $value) {
      if (!in_array($name, $removed)) {
        $parts[$name] = $value;
      }
    }
    foreach ($added as $name) {
      $parts[$name] = empty($default_values[$name]) ? '' : $default_values[$name];
    }
    ksort($parts);
    $parts = serialize($parts);
    db_update('felix_block')->fields(array(
      'hash' => md5($parts),
      'hash_parts' => $parts,
    ))->condition('fbid', $block->fbid)->execute();
  }

  $context['message'] = t('Updated @current of @total blocks.', array(
    '@current' => $context['sandbox']['progress'],
    '@total' => $context['sandbox']['max'],
  ));
  $context['finished'] = $context['sandbox']['progress'] >= $context['sandbox']['max'];
  if ($context['finished']) {
    $context['results']['count'] = $context['sandbox']['progress'];

    if ($removed) {
      // Since we made the hash less specific, we may end up with duplicate blocks.
      // Remove duplicates for this region.
      $sql = 'SELECT GROUP_CONCAT(fbid) as ids
      FROM {felix_block}
      WHERE region = :region
      GROUP BY hash, module, delta, data
      HAVING (COUNT(*) > 1)';
      $duplicates = db_query($sql, array(':region' => $region->name))->fetchAll();
      foreach ($duplicates as $duplicate) {
        $ids = explode(',', $duplicate->ids);
        db_delete('felix_block')->condition('fbid', array_slice($ids, 1))->execute();
      }
    }
  }
}

/**
 * Batch 'finished' callback
 */
function felix_bash_rehash_blocks_finished($success, array $results, array $operations) {
  if (isset($results['count'])) {
    drupal_set_message(format_plural($results['count'], '1 block updated successfully. ', '@count blocks successfully updated.'));
  }
  if (!drupal_is_cli()) {
    drupal_goto('admin/structure/felix');
  }
}
