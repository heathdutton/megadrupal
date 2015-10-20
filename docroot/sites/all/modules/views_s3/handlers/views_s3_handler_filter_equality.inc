<?php

/**
 * @file views_s3_handler_filter_equality.inc
 * Basic S3 equality filter handler.
 */

/**
 * Simple filter to handle equality.
 */
class views_s3_handler_filter_equality extends views_handler_filter_equality {

  /**
   * Provide simple equality operator
   */
  function operator_options() {
    return array(
      '=' => t('Is equal to'),
    );
  }

  /**
   * Display the filter on the administrative summary
   */
  function admin_summary() {
    if ($this->options['exposed']) {
      return t('Exposed');
    }
    return parent::admin_summary();
  }

  /**
   * Add this filter to the request.
   */
  function query() {
    $this->query->request_arguments[$this->real_field] = $this->value;
  }

}