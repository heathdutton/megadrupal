<?php

/**
 * @file
 * CPS drush commands.
 */

/**
 * Implements hook_drush_command().
 */
function cps_drush_command() {
  $items['cps-prune-revisions'] = array(
    'description' => dt('Prune untracked revisions. Caution! Do not use on production without extensive testing first.'),
    'options' => array(
      'type' => array(
        'description' => dt('Only prune entities from this entity type'),
      ),
      'limit' => array(
        'description' => dt('Limit to pruning N revisions per entity-type, defaults to unlimited'),
      ),
      'progress' => array(
        'description' => dt('Whether to show progress'),
      ),
    ),
    'examples' => array(
      'drush cps-prune-revisions --type=node --progress'  => dt('Prune untracked node revisions.'),
    ),
    'aliases' => array('cpspr'),
  );

  return $items;
}

/**
 * Purge revisions.
 */
function drush_cps_prune_revisions() {
  if (!variable_get('cps_purge_revisions', FALSE)) {
    throw new Exception('Revision purging must be explicitly enabled via the cps_purge_revisions variable to use this command.');
  }
  if (drush_confirm('Are you sure you want to delete untracked CPS revisions?')) {
    $entity_info = entity_get_info();
    $type_option = drush_get_option('type');
    foreach ($entity_info as $type => $info) {
      if (cps_is_supported($type) && (!$type_option || $type_option == $type)) {
        $limit = drush_get_option($limit);
        _drush_cps_prune_revisions($type, $limit);
      }
    }
  }
}

/**
 * Prune revisions that aren't tracked by CPS.
 *
 * @param $type
 *   The entity type to prune revisions for.
 * @param $limit
 *   The maximum number of revisions to prune.
 */
function _drush_cps_prune_revisions($type, $limit) {
  if (!cps_is_supported($type)) {
    throw new Exception($type . ' is not CPS-enabled, so revisions cannot be pruned.');
  }
  $progress = drush_get_option('progress');
  if ($progress) {
    print 'Processing ' . $type . "...\n";
    $show_progress = '';
    $backspace = chr(8);
    $done = 0;
  }

  $info = entity_get_info($type);
  $base_table = $info['base table'];
  $revision_table = $info['revision table'];
  $revision_key = $info['entity keys']['revision'];

  // Get all the revisions from the revision table, that do not appear in any of
  // the following tables:
  //  - the base table
  //  - the {cps_entity} table
  //  - the {cps_entity_revision} table.
  $subquery1 = db_select($base_table)
    ->fields($base_table, array($revision_key));
  $subquery2 = db_select('cps_entity')
    ->fields('cps_entity', array('revision_id'))
    ->condition('entity_type', $type);
  $subquery3 = db_select('cps_entity_revision')
    ->fields('cps_entity_revision', array('revision_id'))
    ->condition('entity_type', $type);

   $query =  db_select($revision_table)
    ->fields($revision_table, array($revision_key))
    ->condition($revision_key, $subquery1, 'NOT IN')
    ->condition($revision_key, $subquery2, 'NOT IN')
    ->condition($revision_key, $subquery3, 'NOT IN');

  $revision_ids = $query->execute()->fetchCol();

  // Allow other modules, such as those that track revision IDs directly, to
  // preserve revisions.
  drupal_alter('cps_purge_revisions', $entity_type, $revision_ids);

  $total = count($revision_ids);

  $memory_limit = ini_get('memory_limit');

  foreach ($revision_ids as $revision_id) {
    entity_revision_delete($type, $revision_id);
    entity_get_controller($type)->resetCache();
    // Show progress.
    if ($progress && (++$done % 100) == 0) {
      $prev_progress = $show_progress;
      $show_progress = "$done / $total";
      if ($prev_progress != $show_progress) {
        if ($prev_progress) {
          print str_repeat($backspace, strlen($prev_progress));
        }
        print $show_progress;
      }
    }
    $usage = memory_get_usage(TRUE);
    if (!drupal_check_memory_limit($usage * 1.2, ini_get('memory_limit'))) {
      print "\n Approaching memory limit, re-starting command. \n";
      $options = array(
        'progress' => drush_get_option('progress'),
        'type' => drush_get_option('type'),
      );
      drush_invoke_process('@self', 'cps-prune-revisions', array(), $options);
      return;
    }
  }

  // When showing progress, show the final progress.
  if ($progress) {
    if ($show_progress) {
      print str_repeat($backspace, strlen($show_progress));
    }
    print "complete!\n";
  }
}

