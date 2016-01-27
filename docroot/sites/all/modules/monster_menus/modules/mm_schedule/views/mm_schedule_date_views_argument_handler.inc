<?php
/**
 * @file
 * Override of Date API views argument handler date_views_argument_handler.
 */

/**
 * Date API argument handler.
 */
class mm_schedule_date_views_argument_handler extends date_views_argument_handler {
  function date_forbid() {
    if (empty($this->argument)) {
      return TRUE;
    }
    $this->date_range = $this->date_handler->arg_range($this->argument);
    $this->min_date = $this->date_range[0];
    $this->max_date = $this->date_range[1];
    if (isset($this->view->mm_node) && isset($this->view->mm_first_month) && $this->view->mm_first_month > 1) {
      $granularity = $this->date_handler->arg_granularity($this->argument);
      if (!empty($granularity) && $granularity == 'year') {
        $this->max_date = clone $this->min_date;
        date_modify($this->max_date, '+1 year');
        date_modify($this->max_date, '-1 second');
      }
    }

    $this->limit = date_range_years($this->options['year_range']);

    // See if we're outside the allowed date range for our argument.
    if (date_format($this->min_date, 'Y') < $this->limit[0] || date_format($this->max_date, 'Y') > $this->limit[1]) {
      $this->forbid = TRUE;
      $this->view->build_info['fail'] = TRUE;
      return TRUE;
    }
    return FALSE;
  }
}
