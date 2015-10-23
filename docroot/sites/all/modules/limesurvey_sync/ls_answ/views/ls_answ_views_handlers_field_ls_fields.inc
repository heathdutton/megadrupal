<?php
/**
 * @file
 * Field handler to explicit the {ls_answ} table fields values.
 */

/**
 * A handler to display the {ls_answ} table fields values.
 *
 * @ingroup views_field_handlers
 */
class ls_answ_views_handlers_field_ls_fields extends views_handler_field {

  /**
   * A handler to display the {ls_answ} table fields values.
   *
   * @param $values
   *   An object from the view module.
   *
   * @return
   *   A string. The formated ls_anws field results.
   */
  function render($values) {
    module_load_include('inc', 'ls_answ', 'ls_answ');
    // The '$this->field_alias' value is prefixed by 'ls_survey_', we will use
    // '$this->options['id']'.
    $values->{$this->field_alias} = ls_answ_display_sync_fields($this->options['id'], $values->{$this->field_alias});
    return parent::render($values);
  }
}
