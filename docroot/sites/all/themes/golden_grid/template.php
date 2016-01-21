<?php
/**
 * @file
 * Theme override and preprocess functions for the Golden Grid base theme.
 */

/**
 *  Implements hook_css_alter().
 */
function golden_grid_css_alter(&$css) {
  global $theme_info, $base_theme_info;

  $exclude = array(
    'misc/vertical-tabs.css',
    'modules/aggregator/aggregator.css',
    'modules/block/block.css',
    'modules/dblog/dblog.css',
    'modules/file/file.css',
    'modules/filter/filter.css',
    'modules/help/help.css',
    'modules/menu/menu.css',
    'modules/openid/openid.css',
    'modules/profile/profile.css',
    'modules/statistics/statistics.css',
    'modules/syslog/syslog.css',
    'modules/system/admin.css',
    'modules/system/maintenance.css',
    'modules/system/system.css',
    'modules/system/system.admin.css',
    'modules/system/system.maintenance.css',
    'modules/system/system.menus.css',
    'modules/system/system.messages.css',
    'modules/system/system.theme.css',
    'modules/taxonomy/taxonomy.css',
    'modules/tracker/tracker.css',
    'modules/update/update.css',
  );

  foreach ($exclude as $excluded) {
    unset($css[$excluded]);
  }
}

/**
 * Implements hook_preprocess_html().
 */
function golden_grid_preprocess_html(&$variables) {
  // Conditionally print RDFa attributes.
  $variables['rdf'] = new stdClass();

  if (module_exists('rdf')) {
    $variables['doctype'] = '<!DOCTYPE html PUBLIC "-//W3C//DTD HTML+RDFa 1.1//EN">' . "\n";
    $variables['rdf']->version = ' version="HTML+RDFa 1.1"';
    $variables['rdf']->namespaces = $variables['rdf_namespaces'];
    $variables['rdf']->profile = ' profile="' . $variables['grddl_profile'] . '"';
  }
  else {
    $variables['doctype'] = '<!DOCTYPE html>' . "\n";
    $variables['rdf']->version = '';
    $variables['rdf']->namespaces = '';
    $variables['rdf']->profile = '';
  }
}

/**
 * Implements hook_preprocess_comment_wrapper().
 *
 * Necessary to remove the fixed width from the form's subject field.
 */
function golden_grid_preprocess_comment_wrapper(&$variables) {
  if (!empty($variables['content']['comment_form']['subject']['#size'])) {
    unset($variables['content']['comment_form']['subject']['#size']);
  }
}
