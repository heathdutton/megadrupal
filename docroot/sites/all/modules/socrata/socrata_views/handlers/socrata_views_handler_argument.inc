<?php
/**
 * @file
 * Provides an argument handler for numeric values,
 * based loosely on wbquery_argument_text from wbapi module.
 */

class socrata_views_handler_argument extends views_handler_argument {
  /**
   * Provide a list of default behaviors for this argument if the argument
   * is not present.
   *
   * Override this method to provide additional (or fewer) default behaviors.
   */
  function default_actions($which = NULL) {
    $defaults = array(
      'ignore' => array(
        'title' => t('Display all values'),
        'method' => 'default_ignore',
        // Generate a breadcrumb to here.
        'breadcrumb' => TRUE,
      ),
      'not found' => array(
        'title' => t('Hide view / Page not found (404)'),
        'method' => 'default_not_found',
        // This is a hard fail condition.
        'hard fail' => TRUE,
      ),
      'empty' => array(
        'title' => t('Display empty text'),
        'method' => 'default_empty',
        // Generate a breadcrumb to here.
        'breadcrumb' => TRUE,
      ),
      'default' => array(
        'title' => t('Provide default argument'),
        'method' => 'default_default',
        'form method' => 'default_argument_form',
        'has default argument' => TRUE,
        // This can only be used for missing argument, not validation failure.
        'default only' => TRUE,
      ),
    );

    if ($which) {
      if (!empty($defaults[$which])) {
        return $defaults[$which];
      }
    }
    else {
      return $defaults;
    }
  }

  /**
   * Set up the query for this argument.
   *
   * The argument sent may be found at $this->argument.
   */
  function query($group_by = FALSE) {
    $this->query->add_where(0, $this->real_field, $this->argument, '=');
  }
}
