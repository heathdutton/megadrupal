<?php


/**
 * Filter issues based on their status.
 *
 * Adds a special "- Open issues -" value that is used to select all of the
 * "open" issue status values as currently configured on the site.
 */
class project_issue_handler_filter_issue_status extends views_handler_filter_field_list {

  /**
   * Return the values to use for this filter.
   *
   * Adds the special "- Open issues -" choice in addition to all of the
   * currently configured issue statue values.
   */
  function get_value_options() {
    parent::get_value_options();
    $this->value_options = array('Open' => t('- Open issues -')) + $this->value_options;
  }

  /**
   * Generate the query based on this filter.
   *
   * This is where the special logic for the "- Open issues -" choice is
   * converted back into the real status values for the underlying query.
   */
  function query() {
    $values = array();
    foreach ($this->value as $key => $value) {
      if ($value == 'Open') {
        $values += project_issue_open_states();
      }
      else {
        $values[$key] = $value;
      }
    }
    $this->value = $values;
    parent::query();
  }
}

