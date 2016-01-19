<?php

/**
 * Implementation hook_drush_command().
 */
function drush_debug_tools_drush_command() {
  $items['dt-node-access-rebuild'] = array(
    'callback' => 'drush_dt_node_access_rebuild',
    'description' => 'Rebuild the node access table. Should only be used if you really know what you are doing.',
    'options' => array(
      'start' => 'The offset the command should start from.',
      'limit' => 'The max nodes to process in the pass. Defaults to 10000.',
      'delete' => 'Delete permissions per node.',
      'delete-all' => 'Delete all node access permissions before starting rebuild.',
      'reverse' => 'Process the nodes in reverse order. Newest gets processed first.',
    ),
    'aliases' => array('dtnar'),
  );
  $items['dt-theme-info'] = array(
    'callback' => 'drush_dt_theme_info',
    'description' => 'Print out all the theming information for a particular theme hook.',
    'aliases' => array('dtti'),
  );
  return $items;
}

function drush_dt_node_access_rebuild() {
  $drupal_version = drush_drupal_major_version();
  switch ($drupal_version) {
    case 6:
      dt_node_access_rebuild_d6();
      break;

    case 7:
      dt_node_access_rebuild_d7();
      break;

    default:
      drush_log("dt-node-access-rebuild currently only works with Drupal verison 6 and 7.", 'error');
      break;
  }
}

function dt_node_access_rebuild_d6() {
  drush_print('Starting node access rebuild');

  $start = drush_get_option(array('start'), 0);
  $limit = drush_get_option(array('limit'), 10000);
  $delete = drush_get_option(array('delete', 'd'));
  $delete_all = drush_get_option(array('delete-all', 'da'));
  $reverse = drush_get_option(array('reverse', 'r'));

  if ($delete_all) {
    db_query("DELETE FROM {node_access}");
    drush_print('Deleted all node access records');
  }
  // Only recalculate if the site is using a node_access module.
  if (count(module_implements('node_grants'))) {
    $order = $reverse ? 'DESC' : 'ASC';
    $query = "SELECT nid FROM {node} ORDER BY nid $order";
    $result = db_query($query);
    while ($node = db_fetch_object($result)) {
      // Try to allocate enough time to rebuild node grants
      if (function_exists('set_time_limit')) {
        @set_time_limit(30);
      }
      if ($delete && !$delete_all) {
        db_query("DELETE FROM {node_access} WHERE nid=%d", $node->nid);
      }
      $loaded_node = node_load($node->nid, NULL, TRUE);
      // To preserve database integrity, only aquire grants if the node
      // loads successfully.
      if (!empty($loaded_node)) {
        node_access_acquire_grants($loaded_node);
        if (drush_get_option(array('verbose', 'v'))) {
          drush_print('Node access grands rebuilt for node ' . $node->nid);
        }
      }
      else {
        drush_print('Could not load node ' . $node->nid);
      }
    }
  }
  elseif ($delete_all) {
    drush_print('No node_grants hooks found. Setting default node access records.');
    // Not using any node_access modules. Add the default grant.
    db_query("INSERT INTO {node_access} VALUES (0, 0, 'all', 1, 0, 0)");
  }

  drush_print('Content permissions have been rebuilt.');
  node_access_needs_rebuild(FALSE);
  cache_clear_all();
}

function dt_node_access_rebuild_d7() {
  drush_print('Starting node access rebuild');

  $start = drush_get_option(array('start'), 0);
  $limit = drush_get_option(array('limit'), 10000);
  $delete = drush_get_option(array('delete', 'd'));
  $delete_all = drush_get_option(array('delete-all', 'da'));
  $reverse = drush_get_option(array('reverse', 'r'));

  if ($delete_all) {
    db_delete('node_access')->execute();
    drush_print('Deleted all node access records');
  }

  // Only recalculate if the site is using a node_access module.
  if (count(module_implements('node_grants'))) {
    $order = $reverse ? 'DESC' : 'ASC';
    $query = "SELECT nid FROM {node} ORDER BY nid $order";
    $nids = db_query($query)->fetchCol();
    foreach ($nids as $nid) {
      // Try to allocate enough time to rebuild node grants
      if (function_exists('set_time_limit')) {
        @set_time_limit(30);
      }
      if ($delete && !$delete_all) {
        db_delete('node_access')->condition('nid', $nid)->execute();
      }

      $node = node_load($nid, NULL, TRUE);
      // To preserve database integrity, only acquire grants if the node
      // loads successfully.
      if (!empty($node)) {
        node_access_acquire_grants($node);
        if (drush_get_option(array('verbose', 'v'))) {
          drush_print('Node access grands rebuilt for node ' . $nid);
        }
      }
      else {
        drush_print('Could not load node ' . $node->nid);
      }
    }
  }
  elseif ($delete_all) {
    drush_print('No node_grants hooks found. Setting default node access records.');
    // Not using any node_access modules. Add the default grant.
    db_insert('node_access')->fields(array(
        'nid' => 0,
        'realm' => 'all',
        'gid' => 0,
        'grant_view' => 1,
        'grant_update' => 0,
        'grant_delete' => 0,
      ))->execute();
  }

  drush_print('Content permissions have been rebuilt.');
  node_access_needs_rebuild(FALSE);
  cache_clear_all();
}

function drush_dt_theme_info($hook) {
  $drupal_version = drush_drupal_major_version();
  switch ($drupal_version) {
    case 6:
      dt_theme_info_d6($hook);
      break;

    case 7:
      dt_theme_info_d7($hook);
      break;

    default:
      drush_log("dt-theme-info currently only works with Drupal verison 6 and 7.", 'error');
      break;
  }
}

function dt_theme_info_d6($hook) {
  static $hooks = NULL;
  if (!isset($hooks)) {
    init_theme();
    $hooks = theme_get_registry();
  }

  if (!isset($hooks[$hook])) {
    drush_log('no theming functions found for hook ' . $hook, 'error');
    return;
  }

  drush_print_r($hooks[$hook]);
}

function dt_theme_info_d7($hook) {
  static $hooks = NULL;

  if (!isset($hooks)) {
    drupal_theme_initialize();
    $hooks = theme_get_registry(FALSE);
  }

  // If there's no implementation, check for more generic fallbacks. If there's
  // still no implementation, log an error and return an empty string.
  if (!isset($hooks[$hook])) {
    // Iteratively strip everything after the last '__' delimiter, until an
    // implementation is found.
    while ($pos = strrpos($hook, '__')) {
      $hook = substr($hook, 0, $pos);
      if (isset($hooks[$hook])) {
        break;
      }
    }
    if (!isset($hooks[$hook])) {
      // Only log a message when not trying theme suggestions ($hook being an
      // array).
      if (!isset($candidate)) {
        drush_log('no theming functions found for hook ' . $hook, 'error');
      }
      return '';
    }
  }

  drush_print_r($hooks[$hook]);
}

