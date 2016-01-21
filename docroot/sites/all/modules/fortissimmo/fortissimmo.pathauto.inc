<?php

/**
 * @file
 * Provides function to integrate with Pathauto
 */

/**
 * Implements hook_pathauto() for Fortissimmo aliases.
 */
function fortissimmo_pathauto($op) {
  switch ($op) {
    case 'settings':
      $settings = array();
      $settings['module'] = 'fortissimmo';
      $settings['token_type'] = 'fortissimmo';
      $settings['groupheader'] = t('Fortissimmo property paths');
      $settings['patterndescr'] = t('Pattern for Fortissimmo property detail page paths');
      $settings['patterndefault'] = t('immo/[fortissimmo:title]');
      $settings['bulkname'] = t('Fortissimmo property paths');
      $settings['bulkdescr'] = t('Generate aliases for all existing fortissimmo property pages.');
      $settings['batch_update_callback'] = 'fortissimmo_pathauto_bulkupdate';
      return (object) $settings;
    default:
      break;
  }
}

/**
 * Implements hook_path_alias_types().
 *
 * Specify the source pattern for paths of this type
 * Used primarily by the bulk delete form.
 */
function fortissimmo_path_alias_types() {
  return array('fortissimmo/property/' => t('Fortissimmo properties'));
}