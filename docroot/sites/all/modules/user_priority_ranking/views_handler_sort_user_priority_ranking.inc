<?php

/**
 * @file
 * Handler that supports sorting on flag priority. This works best if you use
 * sum aggeration. See README.txt for my information.
 */

class views_handler_sort_user_priority_ranking extends views_handler_sort {

  function option_definition() {
    $options = parent::option_definition();
    // Make default order Decending.
    $options['order'] = array('default' => 'DESC');
    return $options;
  }
}
