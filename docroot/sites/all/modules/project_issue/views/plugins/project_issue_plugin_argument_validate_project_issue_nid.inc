<?php

/**
 * Validate whether an argument is a project issue node.
 */
class project_issue_plugin_argument_validate_project_issue_nid extends views_plugin_argument_validate {
  function validate_argument($argument) {
    if (is_numeric($argument) && ($issue = node_load($argument))) {
      return project_issue_node_is_issue($issue);
    }
    return FALSE;
  }
}
