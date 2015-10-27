<?php
/**
 * Filter by term queue machine name
 * Views handler for filtering a taxonomy term view by a term queue
 */
class views_handler_filter_term_queue_machine_name extends views_handler_filter_in_operator {
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    $this->value_options = array();
    
    $queues = term_queue_list();
    foreach ($queues as $queue) {
      $this->value_options[$queue->machine_name] = $queue->title;
    }
  }
}
