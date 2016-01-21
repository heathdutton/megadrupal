<?php

/**
 * @file
 * Handler code.
 */

/**
 * Handler for field locale_info.
 */
class SmartlingHandlerFieldEntityProgress extends views_handler_field {

  /**
   * Render.
   *
   * @param object $values
   *   Entity.
   *
   * @return string
   *   Return locale value.
   */
  public function render($values) {
    $progress = intval($this->get_value($values));
    if ($progress >= 100) {
      $progress_status = 'done';
      $title = t('All locales for module are 100% translated.');
    }
    elseif ($progress > 0) {
      $progress_status = 'in-progress';
      $title = t('At least one locale is still in progress.');
    }
    elseif ($progress <= 0) {
      $progress_status = 'undone';
      $title = t('No translation is in progress or no information is available.');
    }

    if (!empty($values->smartling_weather_interface_entity_module_title)) {
      $query_title = $values->smartling_weather_interface_entity_module_title;
    }
    else {
      $query_title = !empty($values->title) ? $values->title : '';
    }

    // @todo rewrite via renderable array.
    // @todo remove title query if title is empty.
    return l('', 'admin/content/smartling-submissions', array(
      'attributes' => array(
        'class' => array('progress-status', $progress_status),
        'title' => $title,
      ),
      'query' => array('title' => $query_title),
    ));
  }
}
