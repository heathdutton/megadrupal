<?php
/**
 * @file
 * Listing bean plugin.
 */

class PgbarBlockFixedBean extends BeanPlugin {
  /**
   * Builds extra settings for the block edit form.
   */
  public function form($bean, $form, &$form_state) {
    $form = array();
    field_attach_form('bean', $bean, $form, $form_state, entity_language('bean', $bean));
    $pgbar = &$form['pgbar_default'][LANGUAGE_NONE][0];
    // Showing the "Display a progress bar check-box doesn't make sense here".
    $pgbar['state']['#access'] = FALSE;
    $pgbar['options']['target']['offset']['#title'] = t('Current count');
    $pgbar['options']['target']['offset']['#description'] = t('This value is shown as the current state of the progress bar.');
    return $form;
  }
}
