<?php

/**
 * @file
 * Contains the Track Field Changes Before Value handler.
 */

class track_field_changes_handler_field_value_before extends views_handler_field {

  function render($values) {

    $output = '';

    $value = unserialize($values->track_field_changes_audit_before_value_text);

    switch ($value['field_name']) {
      case 'title':
        $output = $value['value'];
        break;

      case 'field_image':
        if (is_array($value['value'])) {
          $fields = $value['value'];
          if (!empty($fields)) {
            foreach ($fields as $key => $field) {
              $output .= $field[filename];
              $output .= ' ';
            }
          }
        }
        break;

      default:
        $node = node_load($values->nid);
        if (is_array($value['value'])) {
          $fields = $value['value'];
          if (!empty($fields)) {
            foreach ($fields as $key => $field) {
              $field_value = field_view_value('node', $node, $value['field_name'], $field);
              $output .= render($field_value);
              // TODO improve multiple value.
              $output .= ' ';
            }
          }
        }
    }

    return $output;

  }

}
