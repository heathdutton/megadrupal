<?php

/**
 * @file
 * Contains the Field Tracker Node Type field handler.
 */

/**
 * Views field handler for the Flag operations links (flag/unflag).
 *
 * @ingroup views
 */
class track_field_changes_handler_field_field_info extends views_handler_field {

  function render($values) {
    $node = node_load($values->nid);
    if ($values->track_field_changes_audit_field_name == 'title') {
      return t('Title');
    }
    elseif ($values->track_field_changes_audit_field_name == 'log') {
      return t('Log');
    }
    else {
      $field_info_instance = field_info_instance('node', $values->track_field_changes_audit_field_name, $node->type);
      return $field_info_instance['label'];
    }
  }

}
