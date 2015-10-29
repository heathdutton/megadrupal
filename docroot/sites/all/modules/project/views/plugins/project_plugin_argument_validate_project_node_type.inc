<?php

/**
 * Validate whether an argument is a node type that is a project node type.
 *
 */
class project_plugin_argument_validate_project_node_type extends views_plugin_argument_validate {
  function validate_argument($argument) {
    $field = field_info_instance('node', 'field_project_type', $argument);
    if (!$field) {
      return FALSE;
    }
    $type = node_type_get_type($argument);
    $this->argument->validated_title = check_plain($type->name);
    return TRUE;
  }
}

