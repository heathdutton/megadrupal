<?php

/**
 * @file
 * Contains ViewsSaveHandlerFieldPath.
 */

/**
 * Handles display of the link to a saved view.
 */
class ViewsSaveHandlerFieldPath extends views_handler_field {

  /**
   * {@inheritdoc}
   */
  public function render($values) {
    $path = unserialize($this->get_value($values));
    if (!empty($path['options']['query'])) {
      $path['path'] .= '?' . drupal_http_build_query($path['options']['query']);
    }
    return $path['path'];
  }

}
