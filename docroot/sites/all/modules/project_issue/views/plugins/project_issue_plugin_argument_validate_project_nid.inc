<?php

/**
 * Extend the project node validator to limit to projects that have issue
 * queue enabled.
 */
class project_issue_plugin_argument_validate_project_nid extends project_plugin_argument_validate_project_nid {
  function limit_query_to_project($query) {
    parent::limit_query_to_project($query);
    $query
      ->fieldCondition('field_project_has_issue_queue', 'value', 1);
  }
}

