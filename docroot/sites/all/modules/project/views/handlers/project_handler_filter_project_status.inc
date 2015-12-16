<?php

/**
 * Filter by project status (sandbox vs. full).
 */
class project_handler_filter_project_status extends views_handler_filter_boolean_operator {
  function get_value_options() {
    // Since this is operating on {project_projects}.sandbox, 0 is full, 1 is
    // sandbox.
    $this->value_options = array(
      0 => t('Full projects'),
      1 => t('Sandbox projects'),
    );
  }
}
