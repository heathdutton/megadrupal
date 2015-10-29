<?php
// $Id;

/**
 * @file
 *   Views query plugin for the last.fm API.
 */

/**
 * Views query plugin for the last.fm API.
 *
 * This is where the magic happens. I have no idea if this is the best way to
 * go about things, it is just what made sense to me as I was putting this all
 * together. Comments and suggestions welcome.
 */
class lastfm_views_plugin_query extends views_plugin_query {
 
  // Our argument array. Anything that needs to be passed to the API gets
  // added to this array through the argumet and filter handlers. This array
  // then gets passed to url() along with $api_url and becomes the query 
  // string. 
  var $args;
  
  /**
   * Constructor; Create the basic query object and fill with default values.
   */
  function init($base_table, $base_field) { }

  function use_pager() {
    return FALSE;
  }

  /**
   * Generate a query and a countquery from all of the information supplied
   * to the object.
   *
   * @param $get_count
   *   Provide a countquery if this is true, otherwise provide a normal query.
   */
  function query($view, $get_count = FALSE) {  }

  /**
   * Executes the query and fills the associated view object with according
   * values.
   *
   * Values to set: $view->result, $view->total_rows, $view->execute_time,
   * $view->pager['current_page'].
   */
  function execute(&$view) {
    $start = microtime();
    
    // If we don't have a method, then just bail.
    if (empty($this->args['method'])) {
      return;
    }
    
    module_load_include('inc', 'lastfm', 'lastfm.api');
    $this->request = lastFmApiRequest::factory($this->args['method'], $view->base_table);
    unset($this->args['method']);
    foreach ($this->args as $k => $v) {
      $this->request->setFilter($k, $v);
    }

    $this->request->execute();

    $joins = array();
    if (isset($this->joins)) {
      $joins = $this->joins;
    }
    $view->result = $this->request->parse($joins);
    kpr($view);

    // Set row counter and execute time
    //$view->total_rows = count($data->{'@attr'}->total);
    $view->execute_time = microtime() - $start;

    // Someday we may have a pager but not today
    $view->pager['current_page'] = 0;
  }

  function add_relationship($right_table, $method, $arg) {
    $this->joins[$right_table] = array('method' => $method, 'arg' => $arg);
  }
}
