<?php

/**
 * @file
 * Contains hook implementations for integration with the Pathauto module.
 */

/**
 * Implements hook_path_alias_types().
 */
function views_save_path_alias_types() {
  $objects['views-save/'] = t('Saved views');
  return $objects;
}

/**
 * Implements hook_pathauto().
 */
function views_save_pathauto($op) {
  switch ($op) {
    case 'settings':
      $settings = array();
      $settings['module'] = 'views_save';
      $settings['token_type'] = 'views_save';
      $settings['groupheader'] = t('Views Save paths');
      $settings['patterndescr'] = t('Default path pattern (applies to all saved view types with blank patterns below)');
      $settings['patterndefault'] = 'saved-views/[views_save:title]';
      $settings['batch_update_callback'] = 'views_save_pathauto_bulk_update_batch_process';
      $settings['batch_file'] = drupal_get_path('module', 'views_save') . '/views_save.pathauto.inc';

      foreach (views_save_get_bundles() as $bundle => $name) {
        $settings['patternitems'][$bundle] = t('Pattern for all @views_save_type paths', array('@views_save_type' => $name));
      }
      return (object) $settings;

    default:
      return NULL;
  }
}

/**
 * Batch processing callback to generate aliases for saved views.
 */
function views_save_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }

  $query = db_select('views_save', 'v');
  $query->leftJoin('url_alias', 'ua', "CONCAT('views_save/', v.id) = ua.source");
  $query->addField('v', 'id');
  $query->isNull('ua.source');
  $query->condition('v.id', $context['sandbox']['current'], '>');
  $query->orderBy('v.id');
  $query->addTag('pathauto_bulk_update');
  $query->addMetaData('entity', 'views_save');

  // Get the total amount of items to process.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

    // If there are no saved views to update, the stop immediately.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }

  $query->range(0, 25);
  $nids = $query->execute()->fetchCol();

  pathauto_views_save_update_alias_multiple($nids, 'bulkupdate');
  $context['sandbox']['count'] += count($nids);
  $context['sandbox']['current'] = max($nids);
  $context['message'] = t('Updated alias for views_save @nid.', array('@nid' => end($nids)));

  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
}

/**
 * Update the URL aliases for multiple saved views.
 *
 * @param array $nids
 *   An array of views_save IDs.
 * @param string $op
 *   Operation being performed on the saved views ('insert', 'update' or
 *   'bulkupdate').
 * @param array $options
 *   An optional array of additional options.
 */
function pathauto_views_save_update_alias_multiple(array $nids, $op, array $options = array()) {
  $options += array('message' => FALSE);

  $saves = views_save_load_multiple($nids);
  foreach ($saves as $save) {
    views_save_update_alias($save, $op, $options);
  }

  if (!empty($options['message'])) {
    drupal_set_message(format_plural(count($nids), 'Updated URL alias for 1 views_save.', 'Updated URL aliases for @count saved views.'));
  }
}
