<?php

/**
 * @file
 * Provides the Views row cache plugin.
 */

/**
 * Row-based caching of query results for Views displays.
 */
class views_row_cache_plugin_cache extends views_plugin_cache_time {

  /**
   * The name of the row cache bin.
   *
   * @var string
   */
  const CACHE_BIN = 'cache_views_row';

  /**
   * The original view result array.
   *
   * @var array
   */
  var $complete_result;

  /**
   * Associative array of cached rows flags keyed by row cid.
   *
   * @var array
   */
  var $cached_row_cids;

  /**
   * Associative array of row output keyed by row cid.
   *
   * @var array
   */
  var $row_output;

  /**
   * {@inheritdoc}
   */
  function option_definition() {
    $options = parent::option_definition();
    unset($options['results_lifespan']);
    unset($options['results_lifespan_custom']);
    return $options;
  }

  /**
   * {@inheritdoc}
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['results_lifespan']);
    unset($form['results_lifespan_custom']);
    $options = array(60, 300, 1800, 3600, 21600, 518400);
    $options = drupal_map_assoc($options, 'format_interval');
    $options = array(CACHE_PERMANENT => t('Permanent')) + $options + array('custom' => t('Custom'));
    $form['output_lifespan']['#options'] = $options;
  }

  /**
   * {@inheritdoc}
   */
  function summary_title() {
    $life_span = $this->get_lifespan('output');
    return $life_span ? format_interval($life_span, 1) : t('Permanent');
  }

  /**
   * {@inheritdoc}
   */
  function cache_start() {
  }

  /**
   * {@inheritdoc}
   */
  function cache_get($type) {
    if ($type == 'output') {
      $this->fetch_cached_rows();
    }
    return FALSE;
  }

  /**
   * Fetches cached rows and removes them from the result to be rendered.
   */
  function fetch_cached_rows() {
    $this->complete_result = $this->view->result;

    $indexes = array();
    foreach ($this->complete_result as $index => $row) {
      $cid = views_row_cache_get_cid($this->view, $row);
      $indexes[$cid] = $index;
    }

    $cids = array_keys($indexes);
    foreach (cache_get_multiple($cids, self::CACHE_BIN) as $cid => $cache) {
      // Always leave at least one row to be rendered to avoid triggering the
      // empty result set behavior.
      if (count($this->view->result) > 1) {
        $index = $indexes[$cid];
        unset($this->view->result[$index]);
      }
      // To avoid inconsistencies we always use the cached value even for
      // actually rendered rows.
      $this->row_output[$cid] = $cache->data;
      $this->cached_row_cids[$cid] = TRUE;
    }
  }

  /**
   * {@inheritdoc}
   */
  function cache_set($type) {
  }

  /**
   * {@inheritdoc}
   */
  function post_render(&$output) {
    $segments = preg_split('/<!--\s*views_row_cache_boundary:\s*(\S+)?\s*-->/', $output, NULL, PREG_SPLIT_DELIM_CAPTURE);

    // The first and last segments hold all the markup preceding the first row
    // and following the last row.
    $prefix = array_shift($segments);
    $suffix = array_pop($segments);

    foreach ($segments as $segment) {
      // Boundary elements are parts of the segments array, we should always
      // have a cid extracted from a boundary element, followed by the related
      // row output.
      if (!isset($cid)) {
        $cid = $segment;
      }
      else {
        // If the row is not cached we should save a new cache entry.
        if (empty($this->cached_row_cids[$cid])) {
          $row_output = trim($segment);
          cache_set($cid, $row_output, self::CACHE_BIN, $this->cache_set_expire('output'));
          $this->row_output[$cid] = $row_output;
        }
        // Always reset the cid so the next one will be picked.
        $cid = NULL;
      }
    }

    // Iterate over the original view result to inject row output in the correct
    // position.
    $content = array();
    foreach ($this->complete_result as $index => $row) {
      $cid = views_row_cache_get_cid($this->view, $row);
      if (!empty($this->row_output[$cid])) {
        $content[$index] = $this->row_output[$cid];
      }
    }

    // Re-assemble the complete view output.
    $output = $prefix . implode("\n", $content) . $suffix;

    // Restore the view result set.
    $this->view->result = $this->complete_result;

    // Replace row placeholders.
    $handler = new views_row_cache_placeholder_handler($this->view);
    $handler->replace($output);
  }

}
