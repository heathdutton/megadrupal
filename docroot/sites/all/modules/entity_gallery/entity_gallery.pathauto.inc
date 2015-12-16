<?php

/**
 * @file
 * Pathauto integration for entity galleries.
 *
 * @ingroup pathauto
 */

/**
 * Implements hook_path_alias_types().
 */
function entity_gallery_path_alias_types() {
  $objects['gallery/'] = t('Galleries');

  return $objects;
}

/**
 * Implements hook_pathauto().
 */
function entity_gallery_pathauto($op) {
  switch ($op) {
    case 'settings':
      $settings = array();
      $settings['module'] = 'entity_gallery';
      $settings['token_type'] = 'entity_gallery';
      $settings['groupheader'] = t('Gallery paths');
      $settings['patterndescr'] = t('Default path pattern (applies to all gallery types with blank patterns below)');
      $settings['patterndefault'] = 'galleries/[entity_gallery:title]';
      $settings['batch_update_callback'] = 'entity_gallery_pathauto_bulk_update_batch_process';
      $settings['batch_file'] = drupal_get_path('module', ' entity_gallery') . '/entity_gallery.pathauto.inc';

      $languages = array();
      if (module_exists('locale')) {
        $languages = array(LANGUAGE_NONE => t('language neutral')) + locale_language_list('name');
      }

      foreach (entity_gallery_type_get_names() as $entity_gallery_type => $entity_gallery_name) {
        if (count($languages) && variable_get('language_gallery_type_' . $entity_gallery_type, 0)) {
          $settings['patternitems'][$entity_gallery_type] = t('Default path pattern for @entity_gallery_type (applies to all @entity_gallery_type gallery types with blank patterns below)', array('@entity_gallery_type' => $entity_gallery_name));
          foreach ($languages as $lang_code => $lang_name) {
            $settings['patternitems'][$entity_gallery_type . '_' . $lang_code] = t('Pattern for all @language @entity_gallery_type paths', array('@entity_gallery_type' => $entity_gallery_name, '@language' => $lang_name));
          }
        }
        else {
          $settings['patternitems'][$entity_gallery_type] = t('Pattern for all @entity_gallery_type paths', array('@entity_gallery_type' => $entity_gallery_name));
        }
      }
      return (object) $settings;
    default:
      break;
  }
}

/**
 * Batch processing callback; Generate aliases for entity galleries.
 */
function entity_gallery_pathauto_bulk_update_batch_process(&$context) {
  if (!isset($context['sandbox']['current'])) {
    $context['sandbox']['count'] = 0;
    $context['sandbox']['current'] = 0;
  }

  $query = db_select('entity_gallery', 'eg');
  $query->leftJoin('url_alias', 'ua', "CONCAT('gallery/', eg.egid) = ua.source");
  $query->addField('eg', 'egid');
  $query->isNull('ua.source');
  $query->condition('eg.egid', $context['sandbox']['current'], '>');
  $query->orderBy('eg.egid');
  $query->addTag('pathauto_bulk_update');
  $query->addMetaData('entity', 'entity_gallery');

  // Get the total amount of items to process.
  if (!isset($context['sandbox']['total'])) {
    $context['sandbox']['total'] = $query->countQuery()->execute()->fetchField();

    // If there are no entity galleries to update, the stop immediately.
    if (!$context['sandbox']['total']) {
      $context['finished'] = 1;
      return;
    }
  }

  $query->range(0, 25);
  $egids = $query->execute()->fetchCol();

  pathauto_entity_gallery_update_alias_multiple($egids, 'bulkupdate');
  $context['sandbox']['count'] += count($egids);
  $context['sandbox']['current'] = max($egids);
  $context['message'] = t('Updated alias for entity gallery @egid.', array('@egid' => end($egids)));

  if ($context['sandbox']['count'] != $context['sandbox']['total']) {
    $context['finished'] = $context['sandbox']['count'] / $context['sandbox']['total'];
  }
}
