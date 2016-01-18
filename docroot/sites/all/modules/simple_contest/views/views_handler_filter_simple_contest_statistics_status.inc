<?php
// $Id: views_handler_filter_simple_contest_statistics_status.inc,v 1.1.2.1 2010/07/09 04:36:13 vinoth.3v Exp $

/**
 * Filter based on simple_contest_statistics_status
 */
class views_handler_filter_simple_contest_statistics_status extends views_handler_filter_in_operator {
  function get_value_options() {
    $this->value_options = array(
      SIMPLE_CONTEST_STATUS_INACTIVE => t('Inactive'),
      SIMPLE_CONTEST_STATUS_ACTIVE => t('Active'),
      SIMPLE_CONTEST_STATUS_CANCELED => t('Canceled'),
      SIMPLE_CONTEST_STATUS_COMPLETED => t('Completed'),
    );
  }
}
