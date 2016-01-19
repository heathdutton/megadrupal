<?php

/**
 * Field handler for item status.
 *
 * @ingroup views_field_handlers
 */
class queue_runner_handler_field_state extends views_handler_field {
  function render($values) {
    $options = array(
      QUEUERUNNER_STATE_WAITING => t('Waiting'),
      QUEUERUNNER_STATE_CLAIMED => t('Claimed'),
      QUEUERUNNER_STATE_RETRYING => t('Retrying'),
      QUEUERUNNER_STATE_FAILED => t('Failed'),
    );

    $classes = array(
      QUEUERUNNER_STATE_WAITING => 'waiting',
      QUEUERUNNER_STATE_CLAIMED => 'claimed',
      QUEUERUNNER_STATE_RETRYING => 'retrying',
      QUEUERUNNER_STATE_FAILED => 'failed',
    );

    $output = array(
      '#attached' => array(
        'css' => array(drupal_get_path('module', 'queue_runner') . '/views/queue_runner.css'),
      ),
      '#markup' => '<span class="queue-state-' . $classes[$values->{$this->field_alias}] . '">' . $options[$values->{$this->field_alias}] . '</span>',
    );

    return drupal_render($output);
  }
}

