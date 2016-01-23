<?php

/**
 * Field handler for item status.
 */
class voipqueue_handler_field_status extends advancedqueue_handler_field_status {
  function render($values) {
    if ($values->{$this->field_alias} == VOIPQUEUE_STATUS_DELAYED) {
      $output = array(
        '#attached' => array(
          'css' => array(drupal_get_path('module', 'advancedqueue') . '/css/advancedqueue.css'),
        ),
        '#markup' => '<span class="' . check_plain($values->{$this->field_alias}) . ' advancedqueue-status-queued">' . t('Scheduled') . '</span>',
      );

      return drupal_render($output);
    }

    return parent::render($values);
  }
}
