<?php

/**
 * Filter handler for state.  This is very basic, but cd_civicrm can override it.
 */
class cd_sunlight_handler_filter_district extends views_handler_filter_in_operator {

  function get_value_options() {
    if (!isset($this->value_options)) {
      $query = db_select('cd_sunlight_legislators', 'l');
      $query->addField('l', 'district');
      $query->addField('l', 'district');
      $this->value_options = $query->groupBy('district')
        ->orderBy('district')
        ->execute()
        ->fetchAllKeyed();
    }
  }

  function query() {
    parent::query();

    // Add senators to the district condition.
    // First find the district condition.
    foreach ($this->query->where[0]['conditions'] as $clause_id => $clause) {
      if (strpos($clause['field'], '.district') !== FALSE) {

        // Remove the old clause.
        $field = $this->query->where[0]['conditions'][$clause_id]['field'];
        $districts = $this->query->where[0]['conditions'][$clause_id]['value'];
        unset($this->query->where[0]['conditions'][$clause_id]);

        if (!is_array($districts)) {
          // This happens if condition was "WHERE district = N";
          $districts = array($districts);
        }
        $districts[] = 0;

        // Add a new group.
        $this->query->add_where($clause_id, $field, $districts, 'IN');

        break;
      }
    }

  }
}
