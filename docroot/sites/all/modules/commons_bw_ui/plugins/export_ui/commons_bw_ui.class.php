<?php

/**
 * @file
 * Definitions for the custom Commons Browsing Widget User Interface Export UI class.
 */

class commons_bw_ui extends ctools_export_ui {
  /**
   * Provide the actual editing form.
   */
  function edit_form(&$form, &$form_state) {
    parent::edit_form($form, $form_state);

    // Add the buttons if the wizard is not in use.
    if (empty($form_state['form_info'])) {
      // Add buttons.
      $form['buttons']['submit']['#attributes'] = array(
        'class' => array('enabled-for-ajax'),
      );

      // Disable view display selection if a view has not been chosen.
      if (empty($form_state['values']['name'])) {
        $form['buttons']['submit']['#disabled'] = TRUE;
      }
    }
  }
}
