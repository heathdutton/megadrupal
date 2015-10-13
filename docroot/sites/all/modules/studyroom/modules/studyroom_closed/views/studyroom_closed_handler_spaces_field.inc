<?php

/**
 * @file
 * Definition of studyroom_closed_handler_spaces_field.
 */

/**
 * Contains a field handler to display the list of spaces.
 */
class studyroom_closed_handler_spaces_field extends views_handler_field {
  /**
   * Construct a new field handler adding our field.
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['spaces_all'] = 'spaces_all';
  }

  /**
   * Render the field.
   */
  public function render($values) {
    if ($values->studyroom_closed_spaces_all) {
      return 'All spaces';
    }
    else {
      $spaces = (array) unserialize($values->studyroom_closed_spaces);
      $studyroom_spaces = studyroom_space_load_multiple($spaces);

      $spaces_list = array();
      foreach ($studyroom_spaces as $space_id => $space) {
        if ($space_id != 0) {
          $spaces_list[$space_id] = $space->label();
        }
      }
      // Display list.
      if (!empty($spaces_list)) {
        return implode(', ', $spaces_list);
      }
    }
  }
}
