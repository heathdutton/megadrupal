<?php

/**
 * Validate whether an argument is a project node.
 *
 * This supports either numeric arguments (nid) or strings (machine_name) and
 * converts either into the one needed by the underlying argument's field.
 * This validator also sets the
 * argument's title to the full title of the project node.
 */
class project_plugin_argument_validate_project_nid extends views_plugin_argument_validate {
  function validate_argument($argument) {
    // Default to the project machine name but allow using a different one too.
    $field = field_info_field(isset($this->argument->definition['field_name']) ? $this->argument->definition['field_name'] : 'field_project_machine_name');
    $query = new EntityFieldQuery;
    $this->limit_query_to_project($query);

    if (is_numeric($argument)) {
      // Numeric argument is a node id. This is one reason for forcing projects
      // to be nodes at the moment.
      $query
        ->propertyCondition('nid', $argument);
    }
    else {
      // Search the machine name field.
      $query
        ->fieldCondition($field['field_name'], 'value', $argument);
    }
    $result = $query->execute();
    if (empty($result['node'])) {
      // No nodes = not a project node.
      return FALSE;
    }
    $nodes = node_load_multiple(array_keys($result['node']));
    foreach ($nodes as $project) {
      if (is_numeric($argument) && $this->argument->definition['handler'] == 'views_handler_argument_string') {
        // Convert numeric id to machine name.
        $this->argument->argument = $project->{$field['field_name']}[LANGUAGE_NONE][0]['value'];
      }
      elseif (!is_numeric($argument) && $this->argument->definition['handler'] != 'views_handler_argument_string') {
        // Convert machine name to numeric id.
        $this->argument->argument = $project->nid;
      }
      $this->argument->validated_title = check_plain($project->title);
      return TRUE;
    }
  }

  function limit_query_to_project($query) {
    $query
      // Projects are currently restricted to nodes!
      ->entityCondition('entity_type', 'node')
      // If there is a field_project_type, this is a project.
      // This part is what we care about.
      ->fieldCondition('field_project_type', 'value', NULL, 'IS NOT NULL');
  }
}

