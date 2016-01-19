<?php

/**
 * Filter handler for state.  This is very basic, but cd_civicrm can override it.
 */
class cd_sunlight_handler_argument_district extends views_handler_argument_numeric {

  function query() {
    parent::query();

    // Add senators to the district condition.
    // First find the district condition.
    foreach ($this->query->where[0]['conditions'] as $clause_id => $clause) {
      if (preg_match('!\w+\.district!', $clause['field'], $matches)) {

        // Remove the old clause.
        $field = $matches[0];
        $districts = $clause['value'];
        unset($this->query->where[0]['conditions'][$clause_id]);

        $districts[] = 0;

        // Add a new group.
        $this->query->add_where($clause_id, $field, $districts, 'IN');

        break;
      }
    }

  }
}
