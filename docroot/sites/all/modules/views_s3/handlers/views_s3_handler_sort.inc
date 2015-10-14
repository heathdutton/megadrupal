<?php
/**
 * @defgroup views_sort_handlers Views' sort handlers
 * @{
 * Handlers to tell Views how to sort queries
 */

/**
 * Base sort handler that has no options and performs a simple sort
 */
class views_s3_handler_sort extends views_handler_sort {

  /**
   * Called to add the sort to a query.
   */
  function query() {
    // Add the field.
    $this->query->add_orderby($this->table_alias, $this->real_field, $this->options['order']);
  }
}


/**
 * @}
 */
