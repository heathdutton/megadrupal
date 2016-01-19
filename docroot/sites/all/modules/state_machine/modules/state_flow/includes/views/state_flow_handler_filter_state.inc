<?php

/**
 * @file
 * Views filter handler for the State Flow state.
 */

class state_flow_handler_filter_state extends views_handler_filter_in_operator {

  /**
   * Provide value options for the state filter based on available workflow
   * states for any content types that could be included in this view.
   */
  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    // Determine the content types that could be part of this view. If the node
    // type filter is enabled, then limit to the selected types.
    $node_types = array();
    if (isset($this->view->filter) && is_array($this->view->filter)) {
      foreach ($this->view->filter as $key => $value) {
        if (get_class($value) === 'views_handler_filter_node_type') {
          $type_names = FALSE;
          if (!empty($this->view->exposed_input[$key]) && !empty($this->view->filter[$key]->value_options) && array_key_exists($this->view->exposed_input[$key], $this->view->filter[$key]->value_options)) {
            $type_names = array($this->view->exposed_input[$key] => 1);
          }
          elseif (!empty($this->view->filter[$key]->options['value'])) {
            $type_names = $this->view->filter[$key]->options['value'];
          }
          if (is_array($type_names)) {
            $node_types += $type_names;
          }
        }
      }
    }
    if (empty($node_types)) {
      $node_types = node_type_get_types();
    }

    $state_options = array();
    $machines = array();
    ctools_include('plugins');

    // For all possible content types, load the appropriate state machine and
    // determine possible states.
    foreach ($node_types as $type => $value) {
      $machine_type = variable_get('state_flow_' . $type, 'state_flow');
      if (!isset($machines[$machine_type])) {
        $plugin = ctools_get_plugins('state_flow', 'plugins', $machine_type);
        $class = ctools_plugin_get_class($plugin, 'handler');
        if ($class) {
          $machines[$machine_type] = $class;
        }
      }
      if (isset($machines[$machine_type]) && class_exists($machines[$machine_type])) {
        $obj = new stdClass();
        $state_flow = new $machines[$machine_type]($obj);
        $state_options = array_merge($state_options, $state_flow->get_states_options());
      }
    }

    $this->value_options = array();
    foreach ($state_options as $key => $value) {
      $this->value_options[$key] = $value['label'];
    }
  }

}
