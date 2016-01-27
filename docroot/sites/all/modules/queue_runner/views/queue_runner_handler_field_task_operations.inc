<?php

/**
 * Field handler for task operations.
 *
 * @ingroup views_field_handlers
 */
class queue_runner_handler_field_task_operations extends views_handler_field {
  function render($values) {

    views_include('admin');
    views_ui_add_admin_css();

    $links = array();
    $failure = queue_runner_failure_load($values->item_id);

    if (queue_runner_failure_operation_allowed('cancel', $failure)) {
      $links['cancel'] = array(
        'title' => t('Cancel'),
        'href' => 'admin/config/system/queue-runner/cancel/' . $values->item_id,
        'query' => drupal_get_destination(),
      );
    }

    if (queue_runner_failure_operation_allowed('retry', $failure)) {
      $links['retry'] = array(
        'title' => t('Retry'),
        'href' => 'admin/config/system/queue-runner/retry/' . $values->item_id,
        'query' => drupal_get_destination(),
      );
    }

    $output = array(
      '#theme' => 'links__ctools_dropbutton',
      '#links' => $links,
      '#attached' => array(
        'css' => array(
          drupal_get_path('module', 'queue_runner')  . '/css/queue-runner.css',
        ),
      ),
    );

    return drupal_render($output);
  }
}

