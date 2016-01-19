<?php
/**
 * @file
 * Provides an argument handler for checkbox fields.
 */

class socrata_views_handler_argument_checkbox extends socrata_views_handler_argument {
  /**
   * Override the default argument fixed value form element
   * to provide only the boolean options.
   */
  function default_argument_form(&$form, &$form_state) {
    parent::default_argument_form($form, $form_state);
    if (!empty($form['argument_default']['fixed']['argument'])) {
      $form['argument_default']['fixed']['argument']['#type'] = 'select';
      $form['argument_default']['fixed']['argument']['#options'] = array('true' => t('True'), 'false' => t('False'));
    }
  }

  /**
   * Set up the query for this argument.
   *
   * The argument sent may be found at $this->argument.
   */
  function query($group_by = FALSE) {
    // Try to reduce all default arguments down to TRUE or FALSE.
    $arg = ('true' == $this->argument) ? TRUE : (('false' == $this->argument) ? FALSE : !empty($this->argument));
    if ($arg) {
      $this->query->add_where(0, $this->real_field, 'true', '=');
    }
    else {
      // We have to do this because depending on how values are added/imported
      // into checkbox fields, FALSE might actually be stored as NULL.
      $this->query->add_where_expression(0, "({$this->real_field} is null or {$this->real_field} = false)");
    }
  }
}
