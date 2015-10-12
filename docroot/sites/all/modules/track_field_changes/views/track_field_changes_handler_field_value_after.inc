<?php

/**
 * @file
 * Contains the Track Field Changes After Value handler.
 */

class track_field_changes_handler_field_value_after extends views_handler_field {

  function render($values) {

    $output = '';

    $value = unserialize($values->track_field_changes_audit_after_value_text);

    switch ($value['field_name']) {
      case 'title':
        $output = $value['value'];
        break;

      case 'field_image':
        // TODO i18n.
        $fields = $value['value'];
        if (!empty($fields)) {
          foreach ($fields as $key => $field) {
            $field = file_load($field[fid]);
            $output .= $field->filename;
            $output .= ' ';
          }
        }
        break;

      default:
        $node = node_load($values->nid);
        // TODO i18n.
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

    return $output;

  }

}
