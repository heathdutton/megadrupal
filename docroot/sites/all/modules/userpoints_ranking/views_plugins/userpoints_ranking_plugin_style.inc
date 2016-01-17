<?php

/**
 * @file
 * Table PDF style plugin
 */


/**
 * Style plugin to render each item as a row in a table.
 *
 * @ingroup views_style_plugins
 */
class userpoints_ranking_plugin_style extends views_plugin_style_table {
  function render() {
    $results = array();
    
    $this->view->row_index = 0;
    foreach ($this->view->result as $user) {
      $results[$this->view->row_index] = $user->uid;
      $this->view->row_index++;
    }
    unset($this->view->row_index);
        
    // Check if we are in the preview mode. If so then produce a preview.
    $period_id = $this->display->handler->get_option('userpoints_ranking_period_id');
      
    if (empty($period_id)) {
      return theme('userpoints_ranking_view_preview', array('view' => $this->view, 'results' => $results));
    }
    else {
      return $results;
    }    
  }
}
