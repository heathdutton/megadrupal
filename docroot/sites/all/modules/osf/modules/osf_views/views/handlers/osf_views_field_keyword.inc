<?php

/**
 * Field handler to translate a record type into its readable form.
 */
class osf_views_field_keyword extends views_handler_field {
  function option_definition() {    
    $options = parent::option_definition();
    return $options;
  }

  /**
   * Provide machine_name option for to node type display.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }
  
  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);

    // Change the UI name to make it easier to read to the Views user
    $form_state['values']['options']['ui_name'] = "OSF ";
  }  
}