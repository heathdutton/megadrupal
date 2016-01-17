<?php
/**
 * @file
 * Pathauto hooks for the Election module.
 */

/**
 * Implements hook_pathauto().
 */
function election_pathauto($op) {
  switch ($op) {
    case 'settings':
      $settings = array();
      $settings['module'] = 'election';
      $settings['token_type'] = 'election';
      $settings['groupheader'] = t('Election paths');
      $settings['patterndescr'] = t('Default pattern (applies to all election types with blank patterns below)');
      $settings['patterndefault'] = 'election/[election:title]';
      $settings['batch_update_callback'] = 'election_pathauto_bulk_update_batch_process';
      $settings['batch_file'] = drupal_get_path('module', 'election') . '/election.pathauto.inc';
      foreach (election_types() as $type_machine_name => $type) {
        $settings['patternitems'][$type_machine_name] = t('Pattern for all %type paths', array('%type' => $type['name']));
      }
      return (object) $settings;

  }
}

/**
 * Allow bulk updating of paths. See the Pathauto module.
 *
 * @see election_pathauto()
 *
 * @param array &$context
 *   The Batch API context array.
 */
function election_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }
  $query = db_select('election', 'e');
  $query->leftJoin('url_alias', 'ua', "CONCAT('election/', e.election_id) = ua.source");
  $query->addField('e', 'election_id');
  $query->isNull('ua.source');
  $query->condition('e.election_id', $context['sandbox']['current'], '>');
  $query->orderBy('e.election_id');
  $query->addTag('pathauto_bulk_update');
  $query->addMetaData('entity', 'election');
  // Get the total amount of items to process.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();
    // If there are no elections to update, then stop immediately.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }
  $query->range(0, 25);
  $ids = $query->execute()->fetchCol();
  _election_pathauto_update_alias_multiple($ids, 'bulkupdate');
  $context['sandbox']['count'] += count($ids);
  $context['sandbox']['current'] = max($ids);
  $context['message'] = t('Updated alias for election @id.', array('@id' => end($ids)));
  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
}

/**
 * Implements hook_path_alias_types().
 *
 * Used primarily by the bulk delete form.
 */
function election_path_alias_types() {
  $objects = array(
    'election/' => t('Elections'),
  );
  return $objects;
}
