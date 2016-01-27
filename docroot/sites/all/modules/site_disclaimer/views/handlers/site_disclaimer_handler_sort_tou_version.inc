<?php

/**
 * @file
 * Views field sort handler for Site Disclaimer module.
 */

/**
 * Sort handler to sort on site_disclaimer version.
 */
class site_disclaimer_handler_sort_tou_version extends views_handler_sort {

/*
 * function option_definition() {
 *   $options = parent::option_definition();
 *   $options['form_data_fieldname'] = array('default' => 'site_disclaimer');
 *   return $options;
 * }
 */
  function pre_query() {
    // Save sort-by parameters for later... will be used in site_disclaimer_views_pre_render()
    $this->query->site_disclaimer_handler_sort_tou_version[] = array_merge(is_array($this->options) ? $this->options : array(),
      array(
        'form_data_fieldname' => 'site_disclaimer',
      ));
//watchdog('debug', 'IN tou_sort_handler pre_query query=<pre>' . print_r($this->query,1) . '</pre>');
  }
}

