<?php

/**
 * Field handler for item status.
 *
 * @ingroup views_field_handlers
 */
class queue_runner_handler_field_log_level extends views_handler_field {
  function render($values) {
    $severity = watchdog_severity_levels();

    $classes = array(
      WATCHDOG_DEBUG => 'queue-log-debug',
      WATCHDOG_INFO => 'queue-log-info',
      WATCHDOG_NOTICE => 'queue-log-notice',
      WATCHDOG_WARNING => 'queue-log-warning',
      WATCHDOG_ERROR => 'queue-log-error',
      WATCHDOG_CRITICAL => 'queue-log-critical',
      WATCHDOG_ALERT => 'queue-log-alert',
      WATCHDOG_EMERGENCY => 'queue-log-emerg',
    );

    $output = array(
      '#attached' => array(
        'css' => array(drupal_get_path('module', 'queue_runner') . '/views/queue_runner.css'),
      ),
      '#markup' => '<span class="' . $classes[$values->{$this->field_alias}] . '">' . $severity[$values->{$this->field_alias}] . '</span>',
    );

    return drupal_render($output);
  }
}

