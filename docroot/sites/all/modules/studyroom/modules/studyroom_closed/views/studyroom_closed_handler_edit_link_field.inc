<?php

/**
 * @file
 * Definition of studyroom_closed_handler_edit_link_field.
 */

/**
 * Contains a field handler to display the edit links as fields.
 */
class studyroom_closed_handler_edit_link_field extends studyroom_closed_handler_link_field {
  /**
   * Render the field.
   */
  public function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $studyroom_closed_id = $values->{$this->aliases['studyroom_closed_id']};

    return l($text, STUDYROOM_CLOSED_MENU_ADMIN_SPACE . '/' . $studyroom_closed_id . '/edit');
  }
}
