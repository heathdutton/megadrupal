<?php

/**
 * @file
 * This file contains a cache that just implements the views cache plugin.
 */

/**
 * Plugin that caches views infinitely. This is not really
 * that useful unless you want rules to invalidate your cache.
 */
class autocache_views_cache_plugin extends views_plugin_cache {
  /**
   * Return a string to display as the clickable title for the
   * access control.
   */
  function summary_title() {
    return t('Autocache');
  }

  /**
   * Initialize the plugin.
   *
   * @param view $view
   *   The view object.
   * @param views_plugin_display $display
   *   The display handler.
   */
  function init(&$view, &$display) {
    parent::init($view, $display);
  }

  /**
   * Since rules will be triggering our cache invalidation,
   * the cache will never expire.
   *
   * @param string $type
   *   Not used in this plugin.
   *
   * @return FALSE
   *   Returns always false.
   */
  function cache_expire($type) {
    return FALSE;
  }

  /**
   * We override the default caching mechanism, since it nukes everything.
   * We are just going to flush the actual display.
   */
  function cache_flush() {
    cache_clear_all($this->view->name . ':' . $this->display->id, $this->table, TRUE);
  }

  /**
   * @see views_object::option_definition()
   */
  function option_definition() {
    return array();
  }

  /**
   * Generate query results cache key
   */
  function get_results_key() {

    if (isset($this->_result_key)) {
      return $this->_result_key;
    }

    $cid = array('results');
    $cid = array_merge($cid, $this->collect_identifiers());

    $cid = implode('-', $cid);
    $this->_result_key = $this->get_base_key() . $cid;

    $this->_result_key = autocache_register_cache_key($this->_result_key);

    return $this->_result_key;
  }

  /**
   * Generate cache output key
   */
  function get_output_key() {

    if (isset($this->_output_key)) {
      return $this->_output_key;
    }

    $cid = array('output');
    $cid = array_merge($cid, $this->collect_identifiers());

    $cid = implode('-', $cid);
    $this->_output_key = $this->get_base_key() . $cid;
    $this->_output_key = autocache_register_cache_key($this->_output_key);
    
    return $this->_output_key;
  }

  /**
   * Collect cid identifiers for this view
   */
  function collect_identifiers() {

    $cid = array();

    if (isset($this->_identifiers)) {
      return $this->_identifiers;
    }

    if (isset($this->view->args) && count($this->view->args) > 0) {
      $cid[] = serialize($this->view->args);
    }

    if (isset($this->view->exposed_input) && count($this->view->exposed_input) > 0) {
      $cid[] = serialize($this->view->exposed_input);
    }

    // If the domain module exists we'll add per domain caching to the mix
    if (module_exists('domain')) {
      $domain = domain_get_domain();
      $cid[] = 'domain:' . $domain['domain_id'];
    }

    // Since the ouput is cached - we'll add the pagers to the cache key
    if (isset($this->view->query->pager)) {
      $cid[] = $this->view->query->pager->current_page;
      $cid[] = $this->view->query->pager->options['offset'];
      $cid[] = $this->view->query->pager->options['items_per_page'];
    }
    
    $this->_identifiers = $cid;

    return $cid;
  }


  function cache_get($type) {
    $ret = parent::cache_get($type);
    if (!$ret) {
      return FALSE;
    }
    if ($type == 'results' &&
        strpos($this->view->base_table, 'search_api_index') === 0 &&
        module_exists('search_api_facetapi')) {
      // Execute the solr search to make facets visible...
      $this->view->query->getSearchApiQuery()->execute();
    }
    return TRUE;
  }

  function cache_set($type) {
    parent::cache_set($type);
  }

  /**
   * Get the base key for this view
   */
  function get_base_key() {

    if (isset($this->_base_key)) {
      return $this->_base_key;
    }

    $cid = array();
    $cid[] = 'ac';

    // Handle search_api
    if (module_exists('search_api') &&
        strpos($this->view->base_table, 'search_api_index') === 0) {
      $id = substr($this->view->base_table, 17);
      $index = search_api_index_load($id);
      if ($index) {
        $cid[] = $entity_type = $index->item_type;
      } else {
        $cid[] = $entity_type = '';
      }
    }
    // Regular views
    else {
      $cid[] = $entity_type = $this->view->base_table;
    }

    $types = array();

    foreach ($this->view->filter as &$filter) {
      if ($filter instanceof views_handler_filter_node_type) {
        $types += $filter->value;
      }
    }

    $cid[] = autocache_register_bundle_set($this->view->base_table, $types);

    $cid[] = $this->view->name;
    $cid[] = $this->view->current_display;

    $this->_base_key = implode('-', $cid) . '-';

    return $this->_base_key;
  }

}
