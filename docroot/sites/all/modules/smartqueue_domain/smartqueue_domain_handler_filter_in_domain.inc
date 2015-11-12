<?php
/**
* @file
* Handler to filter for nodes that are not in a queue.
*/
class smartqueue_domain_handler_filter_in_domain extends views_handler_filter_boolean_operator {
  function query() {
    $this->ensure_my_table();
    $extras = $this->query->table_queue['nodequeue_nodes_node']['join']->extra;
    if (is_array($extras)) {
      foreach ($extras as $extra) {
        if ($extra['field'] == 'qid') {
          $qid = $extra['value'];
        }
      }
    }
    $subqueues = nodequeue_load_subqueues_by_queue($qid);
    $sqid = array();
    foreach ($subqueues as $subqueue) {
      if (smartqueue_domain_views_check_subqueue($subqueue)) {
        $sqid[] = $subqueue->sqid;
      }
    }
    if (!empty($sqid)) {
      $this->query->add_where($this->options['group'], $this->table_alias . ".sqid",  $sqid, ($this->value) ? "IN" : "NOT IN");
    }
  }
}
