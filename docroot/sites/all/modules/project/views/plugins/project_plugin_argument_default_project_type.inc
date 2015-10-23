<?php

/**
 * Provide a default argument of the currently relevant project type.
 */
class project_plugin_argument_default_project_type extends views_plugin_argument_default {
  function get_argument() {
    // If we're on one of the project menu items (e.g. for project browsing),
    // the 2nd element in the URL *should* be the project type.
    if (arg(0) == 'project') {
      return arg(1);
    }

    // If we're viewing a node, return the node type.
    if (arg(0) === 'node' && is_numeric(arg(1))) {
      $node = node_load(arg(1));
      return $node->type;
    }
  }
}
