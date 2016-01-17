<?php

/**
 * @file
 *  Search log Views filter by module.
 */

class views_handler_filter_search_log_module extends views_handler_filter_in_operator {
  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Module');
      foreach (module_implements('search_info') as $module) {
        $modules[$module] = $module;
      }
      $this->value_options = $modules;
    }
  }
}
