<?php
/**
 * @file
 * Drush plugin for classified.module
 *
 * @copyright (c) 2010-2011 Ouest Systemes Informatiques (OSInet)
 *
 * @author Frederic G. MARAND <fgm@osinet.fr>
 *
 * @license General Public License version 2 or later
 *
 * Original code, not derived from the ed_classified module.
 */

/**
 * Utility function to build a dt() parameters array about nodes.
 *
 * @param array $nodes
 *   Keyed by uid, and containing the nids and titles of purged nodes
 *
 * @return array
 *   A dt() parameters array
 */
function _classified_drush_get_action_stats($nodes) {
  $user_count = count($nodes);
  $node_count = 0;
  foreach ($nodes as $uid => $user_nodes) {
    $node_count += count($user_nodes);
  }

  $ret = array(
    '@node_count' => $node_count,
    '@user_count' => $user_count,
  );
  return $ret;
}

/**
 * Implements hook_drush_command().
 *
 * @return array
 *   Drush command description array.
 */
function classified_drush_command() {
  $items = array();

  $items['classified-purge'] = array(
    'description' => "Purge nodes past their expiration time + grace period",
    'aliases' => array('cl-p'),
  );

  $items['classified-expire'] = array(
    'description' => "Expire nodes past their expiration time",
    'aliases' => array('cl-e'),
  );

  $items['classified-notify'] = array(
    'description' => "Notify about node litefime stages",
    'aliases' => array('cl-n'),
    'arguments' => array(
      'kind' => 'half-life|pre-expire|purge',
    ),
  );

  foreach ($items as &$item) {
    $item['drupal dependencies'] = array('classified');
  }

  return $items;
}

/**
 * Implements hook_drush_help().
 *
 * @param string $section
 *   The name of a section.
 *
 * @return string
 *   The translated help string.
 */
function classified_drush_help($section) {
  switch ($section) {
    case 'drush:classified-notify':
      $ret = dt("Generate notification information for one of the three events in a Classified Ad lifetime.");
      break;

    case 'meta:classified:title':
      $ret = dt('Classified Ads commands');
      break;

    // Only used for drush help --filter without a category.
    case 'meta:classified:summary':
      $ret = dt('Expire or purge ads, invoke notification mechanisms.');
      break;

    default:
      $ret = NULL;
      break;
  }

  return $ret;
}


/**
 * Command callback for classified-expire
 *
 * @return void
 *   No output: nothing to return.
 */
function drush_classified_expire() {
  module_load_include('inc', 'classified', 'classified.scheduled');
  $expired = _classified_scheduled_build_expire();
  $params = _classified_drush_get_action_stats($expired);
  if ($params['@node_count']) {
    drush_print(dt('Expired @node_count ads by @user_count users.', $params));
  }
  else {
    drush_print(dt('Expire did not find any node to unpublish.'));
  }
}

/**
 * Command callback for classified-notify
 *
 * @return void
 *   No output: nothing to return.
 */
function drush_classified_notify($kind = NULL) {
  if (!in_array($kind, _classified_get_notify_kinds())) {
    drush_set_error(dt('Incorrect notify kind: @kind', array('@kind' => $kind)));
    return FALSE;
  }

  module_load_include('inc', 'classified', 'classified.scheduled');
  $notify = _classified_scheduled_build_notify($kind);
  $params = _classified_drush_get_action_stats($notify);
  $params['@kind'] = $kind;
  drush_log(dt('Notifications for @kind sent to @user_count users about @node_count ads.', $params), 'success');
}

/**
 * Command callback for classified-purge
 *
 * @return void
 *   No output: nothing to return.
 */
function drush_classified_purge() {
  module_load_include('inc', 'classified', 'classified.scheduled');
  $purged = _classified_scheduled_build_purge();
  $params = _classified_drush_get_action_stats($purged);
  if ($params['@node_count']) {
    drush_print(dt('Purged @node_count ads by @user_count users.', $params));
  }
  else {
    drush_print(dt('Purge did not find any node to delete.'));
  }
}
