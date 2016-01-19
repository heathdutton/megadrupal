<?php

/**
 * Views field handler to display the available State Flow events for the
 * selected node revision.
 */
class state_flow_handler_field_events extends views_handler_field {
  function query() {
    $this->ensure_my_table();
  }

  function render($values) {
    $state_events = array();
    $state_actions_str = '';
    $state_node = node_load($values->nid);
    $state_state_flow = state_flow_load_state_machine($state_node);
    if (!$state_state_flow->ignore()) {
      $state_events = $state_state_flow->get_available_events();
    }
    $state_actions = array();
    foreach ($state_events as $event) {
      $key = $event->key;
      $label = $event->get_option('label');
      if (state_flow_access($state_node, $key)) {
        $state_actions[] = l(t($label), 'node/' . $state_node->nid . '/revisions/' . $state_node->vid . '/workflow/' . $key, array('query' => array('destination' => $_GET['q']))) . ' ';
      }
    }
    $state_actions_str = implode(' | ', $state_actions);
    return $state_actions_str;
  }
}
