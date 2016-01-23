<?php

/**
 * @file
 * Definition of socrata_views_handler_field_html.
 */

/**
 * Field handler to provide simple renderer for pass-through HTML from Socrata
 *
 * @ingroup views_field_handlers
 */
class socrata_views_handler_field_html extends views_handler_field {
  /**
   * Collect options for field display
   */
  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  /**
   * Option form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

  /**
   * Filter harmful markup out of HTML fragment from Socrata.
   */
  function render($values) {
    $text = '';
    $value = $this->get_value($values);
    if (!empty($value)) {
      $text = $this->sanitize_value($value, 'xss');
    }

    return $text;
  }
}
