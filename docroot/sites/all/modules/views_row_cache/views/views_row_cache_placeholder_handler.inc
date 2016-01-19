<?php

/**
 * @file
 * Provides the Views row cache placeholder handler.
 */

/**
 * The views row cache placeholder handler.
 */
class views_row_cache_placeholder_handler {

  /**
   * The view object whose output contains placeholders to be replaced.
   *
   * @var object
   */
  var $view;

  /**
   * Associative array of result rows keyed by row identifiers.
   *
   * @var array
   */
  var $row_map;

  /**
   * Constructs a handler object.
   *
   * @param object $view
   *   The view whose output will be processed.
   */
  function __construct($view) {
    $this->view = $view;
  }

  /**
   * Replaces placeholders in the view output.
   *
   * @param string $output
   *   The view output.
   */
  function replace(&$output) {
    $this->row_map = array();
    foreach ($this->view->result as $row) {
      $row_uid = views_row_cache_get_row_id($this->view, $row);
      $this->row_map[$row_uid] = $row;
    }
    $output = preg_replace_callback('/\[views_row_cache:([a-z_]+):(\d+)(:[a-z_:]+)?\]/', array($this, 'process_placeholders'), $output);
  }

  /**
   * Replaces placeholders with their actual values.
   *
   * @param array $matches
   *   The PREG matches array.
   */
  function process_placeholders($matches) {
    $replacement = '';
    $row_id = !empty($matches[2]) ? $matches[2] : FALSE;
    if (function_exists($function = $matches[1])) {
      $row = isset($this->row_map[$row_id]) ? $this->row_map[$row_id] : NULL;
      $args = !empty($matches[3]) ? explode(':', trim($matches[3], ':')) : array();
      $replacement = $function($this->view, $row, $row_id, $args);
    }
    return $replacement;
  }

}
