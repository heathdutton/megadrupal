<?php

/**
 * Sort handler for NULL values on FieldAPI fields.
 */
class views_handler_sort_null_field extends views_handler_sort {

  /**
   * Provide a list of options for the default sort form.
   * Should be overridden by classes that don't override sort_form
   */
  function sort_options() {
    // ASC puts NULL last, as ISNULL is 1 for NULLs, and 0 for non-NULLs.
    return array(
      'ASC' => t('Sort NULL last'),
      'DESC' => t('Sort NULL first'),
    );
  }

  /**
   * Display whether or not the sort order is ascending or descending
   */
  function admin_summary() {
    if (!empty($this->options['exposed'])) {
      return t('Exposed');
    }
    switch ($this->options['order']) {
      case 'ASC':
      case 'asc':
      default:
        return t('NULL last');
        break;
      case 'DESC';
      case 'desc';
        return t('NULL first');
        break;
    }
  }

  /**
   * Called to add the sort to a query.
   */
  function query() {
    $this->ensure_my_table();

    // Make an alias.
    $alias = implode('_', array($this->table_alias, $this->real_field, 'isnull'));

    // Add the field.
    $this->query->add_orderby(NULL, "ISNULL($this->table_alias.$this->real_field)", $this->options['order'], $alias);
  }

}
