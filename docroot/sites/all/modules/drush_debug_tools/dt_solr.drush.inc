<?php

/**
 * Implementation hook_drush_command().
 */
function dt_solr_drush_command() {
  $items['dt-update-index'] = array(
    'callback' => 'drush_dt_update_index',
    'description' => 'Run search update index functions and show how long they take to execute.',
    'aliases' => array('dtindex'),
  );
  $items['dt-node-update-index'] = array(
    'callback' => 'drush_dt_node_update_index',
    'description' => 'Run search update index functions and show how long they take to execute.',
  );
  $items['dt-index-solr'] = array(
    'callback' => 'drush_dt_index_solr',
    'description' => 'Run search update index functions and show how long they take to execute.',
    'arguments' => array(
      'rows' => "The number of nodes to index. Defaults to number set in config.",
      'namespace' => "Defaults to apachesolr_search",
    ),
    'options' => array(
      'explain' => 'Print to the screen the nodes that will be sent for indexing. -v is an alias of this.',
      'test' => 'Only build the nodes, do not actually index them. -t is an alias of this.',
      'status' => 'Display how many nodes are left to process. -s is an alias of this.',
      'silent' => 'Do not show any information at all.',
    ),
    'aliases' => array('dtsolr'),
  );
  $items['dt-index-solr-build-node'] = array(
    'callback' => 'drush_dt_index_solr_build_node',
    'description' => 'Return the information that will be sent to Solr for indexing.',
    'arguments' => array(
      'node' => "The nid of the item to index.",
    ),
    'options' => array(
      'namespace' => 'The namespace of the solr build. Defaults to "apachesolr_search".',
    ),
  );
  return $items;
}

/**
 * Implements hook_drush_help().
 */
function dt_solr_drush_help($section) {
  switch ($section) {
    // This is necessary to tie drush_debug_tools commands from
    // different commandfiles together in the output of drush help.

    case 'meta:drush_debug_tools:title':
      return dt("Drush debug tools commands");
  }
}

function _dt_search_status() {
  $remaining = 0;
  $total = 0;
  if (drush_drupal_major_version() >= 7) {
    foreach (module_implements('search_status') as $module) {
      $status = module_invoke($module, 'search_status');
      $remaining += $status['remaining'];
      $total += $status['total'];
    }
  }
  else {
    foreach (module_implements('search') as $module) {
      // Special case. Apachesolr recommends disabling core indexing with
      // search_cron_limit = 0. Need to avoid infinite status loop.
      if ($module == 'node' && variable_get('search_cron_limit', 10) == 0) {
        continue;
      }
      $status = module_invoke($module, 'search', 'status');
      if (isset($status)) {
        $remaining += $status['remaining'];
        $total += $status['total'];
      }
    }
  }
  return array($remaining, $total);
}

function drush_dt_node_update_index() {
  $drupal_version = drush_drupal_major_version();
  switch ($drupal_version) {
    case 6:
      dt_node_update_index_d6();
      break;

    case 7:
      dt_node_update_index_d7();
      break;

    default:
      drush_log("dt-node-update-index currently only works with Drupal verison 6 and 7.", 'error');
      break;
  }
}

/**
 * Implementation of hook_update_index().
 */
function dt_node_update_index_d6() {
  $limit = (int)variable_get('search_cron_limit', 100);

  // Store the maximum possible comments per thread (used for ranking by reply count)
  variable_set('node_cron_comments_scale', 1.0 / max(1, db_result(db_query('SELECT MAX(comment_count) FROM {node_comment_statistics}'))));
  variable_set('node_cron_views_scale', 1.0 / max(1, db_result(db_query('SELECT MAX(totalcount) FROM {node_counter}'))));

  $result = db_query_range("SELECT n.nid FROM {node} n LEFT JOIN {search_dataset} d ON d.type = 'node' AND d.sid = n.nid WHERE d.sid IS NULL OR d.reindex <> 0 ORDER BY d.reindex ASC, n.nid ASC", 0, $limit);

  while ($node = db_fetch_object($result)) {
    drush_print("Indexing node: " . $node->nid);
    _node_index_node($node);
  }
}

/**
 * Implements hook_update_index().
 */
function dt_node_update_index_d7() {
  $limit = (int)variable_get('search_cron_limit', 100);

  $result = db_query_range("SELECT n.nid FROM {node} n LEFT JOIN {search_dataset} d ON d.type = 'node' AND d.sid = n.nid WHERE d.sid IS NULL OR d.reindex <> 0 ORDER BY d.reindex ASC, n.nid ASC", 0, $limit, array(), array('target' => 'slave'));

  foreach ($result as $node) {
    drush_print("Indexing node: " . $node->nid);
    _node_index_node($node);
  }
}

function drush_dt_update_index() {
  global $timers;
  register_shutdown_function("search_update_totals");
  $implementations = module_implements("update_index");
  foreach ($implementations as $module) {
    $function = $module . "_update_index";
    print($function . " - ");
    timer_start($function);
    call_user_func_array($function, array());
    timer_stop($function);
    print($timers[$function]["time"] . "ms");
    if (drush_drupal_major_version() >= 7) {
      if (module_hook($module, 'search_status')) {
        $status = module_invoke($module, 'search_status');
        print(' - ' . $status['remaining'] . ' remaining, ' . $status['total'] . ' total');
      }
    }
    else {
      if (module_hook($module, 'search')) {
        // Special case. Apachesolr recommends disabling core indexing with
        // search_cron_limit = 0. Need to avoid infinite status loop.
        if ($module == 'node' && variable_get('search_cron_limit', 10) == 0) {
          continue;
        }
        $status = module_invoke($module, 'search', 'status');
        if (!empty($status)) {
          print(' - ' . $status['remaining'] . ' remaining, ' . $status['total'] . ' total');
        }
      }
    }
    drush_print('');
  }
}

function drush_dt_index_solr($rows = 10, $namespace = "apachesolr_search") {
  if (module_exists($namespace)) {
    $drupal_version = drush_drupal_major_version();
    if ($drupal_version >= 6 && $drupal_version <= 7) {
      $explain = drush_get_option(array('explain', 'v'));
      $nodes = apachesolr_get_nodes_to_index($namespace, $rows);
      $results = array();

      $test = drush_get_option(array('test', 't'));
      if (!$test) {
        drush_dt_apachesolr_index_nodes($nodes, $namespace, $results);
      }

      if ($explain) {
        if (count($results)) {
          drush_print_r($results);
        }
        else {
          drush_print("No nodes requires indexing.");
        }
      }

      $status = drush_get_option(array('status', 's'));
      if ($status) {
        switch ($drupal_version) {
          case 6:
            $stats = module_invoke($namespace, 'search', 'status');
            break;

          case 7:
            $stats = apachesolr_index_status($namespace);
            break;
        }
        drush_print($stats['remaining'] . " items left to send.");
      }

      $remaining += $status['remaining'];
      $total += $status['total'];
    }
    else {
      drush_log("This command currently only works with Drupal version 6 and 7.", "error");
    }
  }
  else {
    drush_log("This command requires apachesolr_search to run.", "error");
  }
}

function drush_dt_apachesolr_index_nodes($rows, $namespace, &$results) {
  if (!$rows) {
    // Nothing to do.
    return FALSE;
  }

  $explain = drush_get_option(array('explain'));

  try {
    // Get the $solr object
    $solr = apachesolr_get_solr();
    // If there is no server available, don't continue.
    if (!$solr->ping(variable_get('apachesolr_ping_timeout', 4))) {
      throw new Exception(t('No Solr instance available during indexing.'));
    }
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    return FALSE;
  }
  module_load_include('inc', 'apachesolr', 'apachesolr.index');
  $documents = array();
  $old_position = apachesolr_get_last_index($namespace);
  $position = $old_position;
  $position['nodes_processed'] = 0;

  // Invoke hook_apachesolr_document_handlers to find out what modules build $documents
  // from nodes in this namespace.
  $callbacks = module_invoke_all('apachesolr_document_handlers', 'node', $namespace);
  $callbacks = array_filter($callbacks, 'is_callable');

  foreach ($rows as $row) {
    $result = drush_invoke_process('@self', 'dt-index-solr-build-node', array($row->nid), array('namespace' => $namespace));

    if (!empty($result['output'])) {
      $result = unserialize($result['output']);
      $documents[] = $result;
      $results[$row->nid]->result = $result;
      $results[$row->nid]->nid = $row->nid;
      $results[$row->nid]->changed = $row->changed;

      // Variables to track the last item changed.
      $position['last_change'] = $row->changed;
      $position['last_nid'] = $row->nid;
      $position['nodes_processed']++;
    }
    else {
      drush_print('Node failed to build, skipping: ' . $row->nid);
    }
  }

  if (count($documents)) {
    try {
      watchdog('Apache Solr', 'Adding @count documents.', array('@count' => count($documents)));
      // Chunk the adds by 20s
      $docs_chunk = array_chunk($documents, 20);
      foreach ($docs_chunk as $docs) {
        $solr->addDocuments($docs);
      }

      $drupal_version = drush_drupal_major_version();
      switch ($drupal_version) {
        case 7:
          // Set the timestamp to indicate an index update.
          apachesolr_index_set_last_updated(REQUEST_TIME);
          break;

        case 6:
          // Set the timestamp to indicate an index update.
          apachesolr_index_updated(time());
          break;
      }
    }
    catch (Exception $e) {
      $nids = array();
      if (!empty($docs)) {
        foreach ($docs as $doc) {
          $nids[] = $doc->entity_id;
        }
      }
      watchdog('Apache Solr', 'Indexing failed on one of the following nodes: @nids <br /> !message', array(
          '@nids' => implode(', ', $nids),
          '!message' => nl2br(strip_tags($e->getMessage())),
        ), WATCHDOG_ERROR);
      return FALSE;
    }
  }

  // Save the new position in case it changed.
  if ($namespace && $position != $old_position) {
    $stored = variable_get('apachesolr_index_last', array());
    $stored[$namespace] = $position;
    variable_set('apachesolr_index_last', $stored);
  }

  return $position;
}

function drush_dt_index_solr_build_node($nid) {
  $namespace = drush_get_option(array('namespace'), 'apachesolr_search');

  module_load_include('inc', 'apachesolr', 'apachesolr.index');

  // Invoke hook_apachesolr_document_handlers to find out what modules build $documents
  // from nodes in this namespace.
  $callbacks = module_invoke_all('apachesolr_document_handlers', 'node', $namespace);
  $callbacks = array_filter($callbacks, 'is_callable');

  // Always build the content for the index as an anonynmous user.
  global $user;
  drupal_save_session(FALSE);
  $user = drupal_anonymous_user();
  $documents = array();

  try {
    // Build node. Set reset = TRUE to avoid static caching of all nodes that get indexed.
    if ($node = node_load($nid, NULL, TRUE)) {
      foreach ($callbacks as $callback) {
        // The callback can either return a $document or an array of $documents.
        $documents[] = $callback($node, $namespace);
      }
    }
  }
  catch (Exception $e) {
    // Something bad happened - log the error.
    watchdog('Apache Solr', 'Error constructing documents to index: <br /> !message', array('!message' => "Node ID: {$row->nid}<br />" . nl2br(strip_tags($e->getMessage()))), WATCHDOG_ERROR);
  }

  // Flatten $documents
  $tmp = array();
  apachesolr_flatten_documents_array($documents, $tmp);
  $documents = $tmp;

  $explain = drush_get_option(array('explain'));
  if ($explain) {
    drush_print('Result for node: ' . $nid);
    drush_print_r($documents);
  }

  $result = serialize($documents);
  return $result;
}

