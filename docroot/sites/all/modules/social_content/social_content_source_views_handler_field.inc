<?php
/**
 * @file
 * Social Content views handler.
 */

class social_content_source_views_handler_field extends views_handler_field {
  public function render($values) {
    return ucfirst($this->get_value($values));
  }
}