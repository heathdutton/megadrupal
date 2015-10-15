<?php

/**
 * @file
 * Group interaction with Xapian.
 */

include_once('xapian.php');

define('XAPIAN_MINIMUM_BINDINGS', '1.0.2');

define('XAPIAN_WRITABLE', TRUE);
define('XAPIAN_FLUSH', TRUE);

define('XAPIAN_LOCAL', 0);
define('XAPIAN_REMOTE', 1);

define('XAPIAN_MATCHES_BEST_ESTIMATE', 0);
define('XAPIAN_MATCHES_LOWER_BOUND', 1);
define('XAPIAN_MATCHES_UPPER_BOUND', 2);

define('XAPIAN_DOCID_PREFIX', 'Q');
define('XAPIAN_TERM_PREFIX_NODETYPE', 'XNODETYPE');
define('XAPIAN_TERM_PREFIX_TAXTERM', 'XTAXONOMY');

/**
 * Initialize xapian db connection.
 *
 * @param bool $writable
 * @param bool $flush
 *   If set, instead of initializing the database we close it, flushing the
 *   buffers to disk.
 * @return object
 */
function _xapian_init_database($writable = FALSE, $flush = FALSE) {
  $database = &drupal_static(__FUNCTION__ . '_database');
  $writable_database = &drupal_static(__FUNCTION__ . '_writable_database');

  if ($flush) {
    if (is_object($writable_database)) {
      // Set the database handle to NULL to ensure that it gets flushed to
      // disk.
      $writable_database = NULL;
    }
    return;
  }

  if (!$writable && is_object($database)) {
    return $database;
  }

  if ($writable && is_object($writable_database)) {
    return $writable_database;
  }

  try {
    $database_type = variable_get('xapian_database_type', XAPIAN_LOCAL);

    switch ($database_type) {

      case XAPIAN_LOCAL:
        $database_path = variable_get('xapian_database_path', '');
        if (empty($database_path)) {
          watchdog('xapian', 'No database path given.');
          return NULL;
        }
        $db_source = drupal_realpath($database_path);
        if ($writable) {
          $writable_database = new XapianWritableDatabase($db_source, Xapian::DB_CREATE_OR_OPEN);
          return $writable_database;
        }
        else {
          $database = new XapianDatabase($db_source);
          return $database;
        }
        break;

      case XAPIAN_REMOTE:
        $database_host = variable_get('xapian_database_hostname', '');
        $database_port = variable_get('xapian_database_port', 6431);

        if (empty($database_host)) {
          watchdog('xapian', 'No database host has been configured, unable to connect to remote database.');
          return NULL;
        }
        if ($writable) {
          $writable_database = Xapian::remote_open_writable($database_host, (int)$database_port);
          return $writable_database;
        }
        else {
          $db_source = Xapian::remote_open($database_host, (int)$database_port);
          $database = new XapianDatabase($db_source);
          return $database;
        }
        break;

    }
  }
  catch (Exception $e) {
    watchdog('xapian', $e->getMessage() . ", writable($writable) flush($flush) type($database_type)");
    return NULL;
  }
}

/**
 * Queries the database.
 *
 * The xapian_query function queries the database using both a query string
 * and application-defined terms.
 *
 * @param $query_string A string (perhaps supplied by the user) containing
 *                      terms to search for.  This string will be parsed and
 *                      stemmed automatically.
 *
 * @param $start        An integer defining the first document to return
 *                      (That is, if $start = n, then the first node returned will
 *                      be that in the nth document found).
 *
 * @param $length       The number of results to return.
 *
 * @param $extra        An array containing arrays of extra terms to search
 *                      for.
 *
 * @return              An array of nids corresponding to the results.
 */
function xapian_query($query_string, $start = 0, $length = 10, $extra = array(), $query_weight = NULL) {
  try {
    $start_time = microtime(TRUE);

    $db = _xapian_init_database();
    if (!is_object($db)) {
      return array(0, array());
    }

    $enquire = new XapianEnquire($db);
    $query_parser = new XapianQueryParser();
    $stemmer = new XapianStem(variable_get('xapian_stem_language', 'english'));
    $query_parser->set_stemmer($stemmer);
    $query_parser->set_database($db);
    $query_parser->set_stemming_strategy(XapianQueryParser::STEM_SOME);
    $query = $query_parser->parse_query($query_string);

    // Build subqueries from $extra array.
    if (is_array($extra)) {
      foreach ($extra as $subq) {
        if (!empty($subq)) {
          $subquery = new XapianQuery(XapianQuery::OP_OR, $subq);
          $query = new XapianQuery(XapianQuery::OP_AND, array($subquery, $query));
        }
      }
    }

    $enquire->set_query($query);
    $matches = $enquire->get_mset((int)$start, (int)$length);

    $results = array();
    $i = $matches->begin();
    while (!$i->equals($matches->end())) {
      $document = $i->get_document();
      if (is_object($document)) {
        $stored_nid = (int)($document->get_data());
        $efq = new EntityFieldQuery();
        $efq->entityCondition('entity_type', 'node')
          ->entityCondition('entity_id', $stored_nid)
          ->addTag('node_access');
        $efq_result = $efq->execute();
        if (!isset($efq_result['node']) || !isset($efq_result['node'][$stored_nid])) {
          // Node cannot be found, i.e. user does not have access.
          $i->next();
          continue;
        }
        $node = node_load($stored_nid);

        // Render the node.
        $build = node_view($node, 'search_index');
        unset($build['#theme']);
        $node->rendered = drupal_render($build);

        $node_extra = module_invoke_all('node_search_result', $node);
        $uri = entity_uri('node', $node);

        $results[] = array(
          'link' => url($uri['path'], array_merge($uri['options'], array('absolute' => TRUE))),
          'type' => check_plain(node_type_get_name($node)),
          'title' => $node->title,
          'user' => theme('username', array('account' => $node)),
          'date' => $node->changed,
          'node' => $node,
          'extra' => $node_extra,
          'score' => (int)($i->get_percent()),
          'snippet' => search_excerpt($query_string, $node->rendered),
        );
      }
      $i->next();
    }

    if (variable_get('xapian_log_queries', FALSE)) {
      $time_taken = (microtime(TRUE) - $start_time) * 1000;
      watchdog('xapian', '<p>Query: %desc </p><p>Query time: %timems</p>',
        array('%desc' => $query->get_description(), '%time' => $time_taken));
    }

    $count_type = variable_get('xapian_node_count_type', XAPIAN_MATCHES_BEST_ESTIMATE);

    switch ($count_type) {
      case XAPIAN_MATCHES_LOWER_BOUND:
        $count = $matches->get_matches_lower_bound();
        break;

      case XAPIAN_MATCHES_UPPER_BOUND:
        $count = $matches->get_matches_upper_bound();
        break;

      case XAPIAN_MATCHES_BEST_ESTIMATE:
      default:
        $count = $matches->get_matches_estimated();
        break;
    }

    return array($count, $results);
  }
  catch (Exception $e) {
    watchdog('xapian', $e->getMessage());
    return array(0, array());
  }
}

/**
 * Build list of available languages.
 */
function _xapian_languages() {
  $list = explode(' ', XapianStem::get_available_languages());
  foreach ($list as $language) {
    $languages[$language] = drupal_ucfirst($language);
  }
  return $languages;
}

/**
 * Function called for every node to be indexed
 *
 * @param object $node
 */
function _xapian_index_node($node, $delete = FALSE) {
  $stemmer = &drupal_static(__FUNCTION__ . 'stemmer');
  $indexer = &drupal_static(__FUNCTION__ . 'indexer');

  // Track which content is being indexed until the Xapian cache is flushed.
  // If we  fail to flush the cache to disk for any reason, we will re-attempt
  // to index this content.  See bug #272140 for full details.
  // TODO Make this query more natural in dbtng?
  //   original db_query('UPDATE {xapian_index_queue} SET status = status + 1, indexed = %d WHERE nid = %d', time(), $node->nid);
  $status = db_query('SELECT status FROM {xapian_index_queue} WHERE nid = :nid', array(':nid' => $node->nid))
    ->fetchField();
  db_update('xapian_index_queue')
    ->fields(array('status' => $status + 1, 'indexed' => REQUEST_TIME))
    ->condition('nid', $node->nid)
    ->execute();

  // Provide mechanism to override default indexing behaviour.
  $function = $node->type .'_xapian_index';

  // Default to standard node indexing.
  if (!function_exists($function)) {
    $function = 'node_xapian_index';
  }

  // If not published, remove node from index
  if ($node->status != 1) {
    xapian_remove_node_from_index($node);
    return TRUE;
  }

  $db = _xapian_init_database(XAPIAN_WRITABLE);
  if (!is_object($db)) {
    watchdog('xapian', 'Could not get writable database.');
    return FALSE;
  }

  $terms = $function($node);

  if (is_array($terms)) {

    try {
      if ($delete) {
        watchdog('xapian', 'Delete attempt Node with id %node', array('%node' => $node->nid));
        $db->delete_document(XAPIAN_DOCID_PREFIX . $node->nid);
      }
      else{
        if (!is_object($indexer)) {
          $indexer = new XapianTermGenerator();
        }

        if (!is_object($stemmer)) {
          $stemmer = new XapianStem(variable_get('xapian_stem_language', 'english'));
        }
        $indexer->set_stemmer($stemmer);

        $document = new XapianDocument();
        $indexer->set_document($document);

        foreach ($terms as $term) {
          if (!$term['weight']) {
            $term['weight'] = 1;
          }
          switch ($term['type']) {
            // General text
            case 'text':
              $indexer->index_text($term['data'], $term['weight']);
              break;

            // Taxonomy term
            case 'term':
              $document->add_term($term['data'], $term['weight']);
              break;
          }
        }

        $document->set_data($node->nid);

        $document->add_term(XAPIAN_DOCID_PREFIX . $node->nid);
        $document->add_term(XAPIAN_TERM_PREFIX_NODETYPE . drupal_strtolower($node->type));

        $db->replace_document(XAPIAN_DOCID_PREFIX . $node->nid, $document);
      }
    }
    catch (Exception $e) {
      watchdog('xapian', $e->getMessage());
    }
  }

  return TRUE;
}

/**
 * Function to remove a node from the index after deletion
 *
 * @param object $node
 */
function xapian_remove_node_from_index($node) {
  $db = _xapian_init_database(XAPIAN_WRITABLE);
  if ($db) {
    if (is_object($node)) {
      if ($node->nid) {
        $db->delete_document(XAPIAN_DOCID_PREFIX . $node->nid);
        // Flush the changes to disk.
        _xapian_init_database(XAPIAN_WRITABLE, XAPIAN_FLUSH);
      }
    }
  }
}

/**
 * Provide default error handler for xapian
 *
 * @param int $errno
 * @param string $errstr
 * @param string $errfile
 * @param int $errline
 * @param unknown_type $errcontext
 */
function _xapian_requirements_error_handler($errno, $errstr, $errfile = NULL, $errline = NULL, $errcontext = NULL) {
  $GLOBALS['xapian_include_error'] = $errstr;
}

/**
 * Attempt to include xapian.php.  If there are errors, Xapian is not
 * available, otherwise it is available.
 *
 * @return bool
 */
function xapian_available() {
  $available = &drupal_static(__FUNCTION__, FALSE);

  if ($available === FALSE) {
    $GLOBALS['xapian_include_error'] = NULL;
    set_error_handler('_xapian_requirements_error_handler');
    include_once('xapian.php');
    restore_error_handler();

    if (NULL == $GLOBALS['xapian_include_error']) {
      $available = TRUE;
    }
  }

  return $available;
}

/**
 * Get the version string of Xapian.
 */
function xapian_get_version() {
  return (function_exists('xapian_version_string')) ? xapian_version_string() : Xapian::version_string();
}
