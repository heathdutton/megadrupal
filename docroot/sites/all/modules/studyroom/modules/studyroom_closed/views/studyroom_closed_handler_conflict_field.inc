<?php

/**
 * @file
 * Definition of studyroom_closed_handler_conflict_field.
 */

/**
 * Contains a field handler to take care of displaying if any conflicts.
 */
class studyroom_closed_handler_conflict_field extends views_handler_field_custom {
  /**
   * Set the default options.
   */
  public function option_definition() {
    $options = parent::option_definition();

    // Override the empty text.
    $options['empty']['default'] = 'None';
    return $options;
  }

  /**
   * Modify options form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Remove some elements.
    unset($form['exclude']);
    unset($form['element_label_colon']);
    unset($form['hide_empty']);
    unset($form['empty_zero']);
    unset($form['hide_alter_empty']);
  }

  /**
   * Do not allow advanced rendering.
   */
  public function allow_advanced_render() {
    return FALSE;
  }

  /**
   * Do not allow click sorting.
   */
  public function click_sortable() {
    return FALSE;
  }

  /**
   * Render the field.
   */
  public function render($values) {
    $closed_id = $values->studyroom_closed_id;
    $count = db_select('studyroom_closed_conflict', 'c')->fields('c')->condition('closed', $closed_id)->execute()->rowCount();
    if (!empty($count) && $count) {
      $text = !empty($this->options['text']) ? $this->options['text'] : format_plural($count, 'Conflict', 'Conflicts');
      return l($text, STUDYROOM_CLOSED_MENU_ADMIN_SPACE . '/' . $closed_id . '/conflict') . " ($count)";
    }
  }
}
