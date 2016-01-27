<?php
// $Id: views_handler_field_simple_contest_statistics_status.inc,v 1.1.2.1 2010/07/09 04:36:13 vinoth.3v Exp $

/**
 * Display simple_contest_statistics_status
 */
class views_handler_field_simple_contest_statistics_status extends views_handler_field {
  function render($values) {
    switch ($values->{$this->field_alias}) {
      case SIMPLE_CONTEST_STATUS_INACTIVE:
      default:
        return t('Inactive');
      case SIMPLE_CONTEST_STATUS_ACTIVE:
        return t('Active');
      case SIMPLE_CONTEST_STATUS_CANCELED:
        return t('Canceled');
      case SIMPLE_CONTEST_STATUS_COMPLETED:
        return t('Completed');
    }
  }
}
