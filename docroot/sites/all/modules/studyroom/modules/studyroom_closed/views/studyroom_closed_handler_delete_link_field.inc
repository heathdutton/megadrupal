<?php

/**
 * @file
 * Definition of studyroom_closed_handler_delete_link_field.
 */

/**
 * Contains a field handler to display deletes links as fields.
 */
class studyroom_closed_handler_delete_link_field extends studyroom_closed_handler_link_field {
  /**
   * Render the field.
   */
  public function render($values) {
    $type = $values->{$this->aliases['type']};

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $studyroom_closed_id = $values->{$this->aliases['studyroom_closed_id']};

    return l($text, STUDYROOM_CLOSED_MENU_ADMIN_SPACE . '/' . $studyroom_closed_id . '/delete');
  }
}
