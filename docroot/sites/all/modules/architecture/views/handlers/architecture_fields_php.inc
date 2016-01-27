<?php

/**
 * @file
 * Views handler override for field that exposes raw data.
 */


/**
 * @inheritdoc
 */
class architecture_php_fields_views_handler extends views_handler_field {

  /**
  * Find any PHP in the body of this field.
  */
  function render($values) {
    if ($value = $this->get_value($values)) {
      try {
        // Try searching the field values.
        return ($this->architecture_scour_fields($value . '_value', 'field_data_' . $value, '<?php'));
      }
      catch (Exception $e) {
        // If the field is a weird type, the query'll fail. No big.
        return 'Could not evaluate field.';
      }
    }
  }

  /**
   * Searches the given column in the given table for a given string (needle).
   *
   * @param string $column
   * @param string $table
   * @param string $needle
   * @return string
   */
  protected function architecture_scour_fields($column, $table, $needle) {
    // @todo: add some kind of sql injection protection here.
    // I'm not super worried because a user would have to somehow sneak their
    // malicious code into a field's machine name.
    $column = check_plain($column);
    $table = check_plain($table);
    $needle = check_plain($needle);

    $query = 'SELECT `entity_id`, `entity_type` FROM `' . $table . '`  WHERE `' . $column . '` LIKE \'%' . $needle . '%\'';
      $result = db_query($query)->fetchAllAssoc('entity_id');
      $result_summary = '';
      foreach ($result as $obj) {
        $id = $obj->entity_id;
        $type = $obj->entity_type;
        // If there are results, try to link to them.
        $result_summary .= '<a href="/' . $type . '/' . $id . '">' . $id . '</a>, ';
      }
      $num = count($result);
      return $num ? ($num . ' occurrences in entities: ' . trim($result_summary, ', ')) : ($num . ' occurrences');
  }
}
