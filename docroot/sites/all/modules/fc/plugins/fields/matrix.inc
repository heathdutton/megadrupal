<?php
/**
 * @file
 * Field Complete - Provides field-based completeness for any entity - matrix field plugin.
 */

/**
 * Simply use the matrix 'empty' hook to see if each entry has content
 */
function fc_matrix_field_is_complete($items, $instance, $field, &$matrix = array()) {
  $module = $field['module'];
  $zero_as_empty = !empty($instance['settings']['fc']['fc_check_zero']);

  foreach ($items as $delta => $item) {
    if ($zero_as_empty && (string) $item['value'] === '0') {
      $item['value'] = FALSE;
    }
    $matrix[$item['row'] - 1][$item['col'] - 1] = !module_invoke($module, 'field_is_empty', $item, $field);
  }
}

function fc_matrix_field_cardinality($function, $items, $instance, $field) {
  // Create the matrix, if not fixed size we have to calculate
  // the size first. And it's simpler just to do it anyway.

  $cols = $rows = 0;
  foreach ($items as $item) {
    $cols = $item['col'] > $cols ? $item['col'] : $cols;
    $rows = $item['row'] > $rows ? $item['row'] : $rows;
  }

  if ($field['settings']['cols_count']==-1) {
    $field['settings']['cols_count'] = $cols;
  }

  if ($field['settings']['rows_count']==-1) {
    $field['settings']['rows_count'] = $rows;
  }
  $row = array_fill(0, $field['settings']['cols_count'], FALSE);
  $matrix = array_fill(0, $field['settings']['rows_count'], $row);

  // Fill in the values
  $function($items, $instance, $field, $matrix);

  $completed = FALSE;
  switch ($instance['settings']['fc']['fc_check_cells']) {

    // As long as something is set...
    case MATRIX_COMPLETE_ANY:
      foreach ($matrix as $col => $row) {
        if (in_array(TRUE, $row)) {
          $completed = TRUE;
          break;
        }
      }
      break;

    // Everything must be set...
    case MATRIX_COMPLETE_ALL:
      $completed = TRUE;
      foreach ($matrix as $col => $row) {
        if (in_array(FALSE, $row)) {
          $completed = FALSE;
          break;
        }
      }
      break;

    // At least one row must be set...
    case MATRIX_COMPLETE_ROW:
      foreach ($matrix as $col => $row) {
        if (!in_array(FALSE, $row)) {
          $completed = TRUE;
          break;
        }
      }
      break;

    // At least one col must be set...
    case MATRIX_COMPLETE_COL:
      // Transpose the matrix
      $xitram = array();
      foreach ($matrix as $i => $row) {
        foreach ($row as  $j => $v) {
          $xitram[$j][$i] = $v;
        }
      }
      $matrix = $xitram;

      // Now just check the rows
      foreach ($matrix as $col => $row) {
        if (!in_array(FALSE, $row)) {
          $completed = TRUE;
          break;
        }
      }
      break;
  }

  return $completed;
}

function fc_matrix_incomplete_process($parent, $function, $cardinality, $items, $instance, $field) {
  $incomplete = new fcIncomplete($instance['field_name'], $parent);
  $incomplete->complete = $cardinality($function, $items, $instance, $field);
  return $incomplete;
}
