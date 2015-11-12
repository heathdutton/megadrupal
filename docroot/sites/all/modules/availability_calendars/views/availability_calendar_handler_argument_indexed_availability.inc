<?php
/**
 * Defines a contextual filter (argument) to filter on availability
 * filtered and indexed by search api.
 *
 * This filter inherits from availability_calendar_handler_argument_availability
 * as only the query building part differs.
 */
class availability_calendar_handler_argument_indexed_availability extends availability_calendar_handler_argument_availability {
  /**
   * Helper method for the query method that actually builds the query.
   * Extracted to allow it to be overridden.
   */
  protected function build_query() {
    if ($this->definition['filtered_availability_table'] === NULL) {
      watchdog('availability_calendar', t('Could not determine which table to join on to filter on availability. Skipping the filter on filtered availability.'), NULL, WATCHDOG_ERROR);
      return;
    }
    // We cannot build the condition we want to, as it is too complex for a
    // SearchApiQuery. So, we add a dummy condition that is going to be replaced
    // later on in our hook_query_TAG_alter() implementation. At that moment,
    // the SearchApiuQuery has been transformed into a Database query, which
    // allows us to build the condition we need.
    // Use a dummy string that should not be used in any other condition, so
    // that it will allow us to find back the condition later on.
    $token = "***availability_calendar_query_search_api_db_search_alter()***";
    $this->query->condition($this->real_field, $token, '=', $this->options['group']);
    // To be able to replace it with what we want, we need to pass some
    // information from this (views filter) handler to that hook.
    availability_calendar_query_search_api_db_search_alter(array(
        'from' => $this->min_date,
        'to_or_duration' => $this->max_date,
        'token' => $token,
        'filtered_availability_table' => $this->definition['filtered_availability_table'])
    );
  }
}
