<?php

/**
 * @file
 *   File for string argument implementation
 */

/**
 * Simple argument to handle strings.
 */
class views_s3_handler_argument_string extends views_handler_argument_string {

  function option_definition() {
    $options = parent::option_definition();
    unset($options['glossary']);
    unset($options['summary']);
    unset($options['summary_options']);

    return $options;
  }

  function default_actions ($which = NULL) {
    $defaults = parent::default_actions($which);
    unset($defaults['summary']);

    return $defaults;
  }
  
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    unset($form['glossary']);
  }

  /**
   * Add this filter to the request.
   */
  function query() {
    $this->query->request_arguments[$this->real_field] = $this->argument;
  }

}