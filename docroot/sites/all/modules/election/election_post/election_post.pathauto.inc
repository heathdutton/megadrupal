<?php
/**
 * @file
 * Pathauto hooks for the Election Post module.
 */

/**
 * Implements hook_pathauto().
 */
function election_post_pathauto($op) {
  switch ($op) {
    case 'settings':
      $settings = array();
      $settings['module'] = 'election_post';
      $settings['token_type'] = 'election_post';
      $settings['groupheader'] = t('Election post paths');
      $settings['patterndescr'] = t('Default path pattern');
      $settings['patterndefault'] = 'election/[election_post:election:title]/post/[election_post:title]';
      $settings['batch_update_callback'] = 'election_post_pathauto_bulk_update_batch_process';
      $settings['batch_file'] = drupal_get_path('module', 'election_post') . '/election_post.pathauto.inc';
      foreach (election_types() as $type_machine_name => $type) {
        $post_machine_name = $type['post machine name'];
        $post_name = election_get_post_name($type_machine_name);
        $settings['patternitems'][$post_machine_name] = t('Pattern for all %post paths', array('%post' => $post_name));
      }
      return (object) $settings;

  }
}

/**
 * Allow bulk updating of paths. See the Pathauto module.
 *
 * @see election_post_pathauto()
 *
 * @param array &$context
 *   The Batch API context array.
 */
function election_post_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }
  $query = db_select('election_post', 'ep');
  $query->leftJoin('url_alias', 'ua', "CONCAT('election-post/', ep.post_id) = ua.source");
  $query->addField('ep', 'post_id');
  $query->isNull('ua.source');
  $query->condition('ep.post_id', $context['sandbox']['current'], '>');
  $query->orderBy('ep.post_id');
  $query->addTag('pathauto_bulk_update');
  $query->addMetaData('entity', 'election_post');
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
  _election_post_pathauto_update_alias_multiple($ids, 'bulkupdate');
  $context['sandbox']['count'] += count($ids);
  $context['sandbox']['current'] = max($ids);
  $context['message'] = t('Updated alias for election post @id.', array('@id' => end($ids)));
  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
}


/**
 * Implements hook_path_alias_types().
 *
 * Used primarily by the bulk delete form.
 */
function election_post_path_alias_types() {
  $objects = array(
    'election-post/' => t('Election posts'),
  );
  return $objects;
}
