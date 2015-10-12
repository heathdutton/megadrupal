<?php

/**
 * Field handler for collector operations.
 *
 * @ingroup views_field_handlers
 */
class queue_runner_handler_field_collector_operations extends views_handler_field {
  function render($values) {

    views_include('admin');
    views_ui_add_admin_css();

    $links = array();
    $collector = queue_runner_collector_load($values->coll_id);

    if (queue_runner_collector_cancel_allowed($collector)) {
      $links['cancel'] = array(
        'title' => t('Cancel'),
        'href' => 'admin/config/system/queue-runner/collector/cancel/' . $values->coll_id,
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

