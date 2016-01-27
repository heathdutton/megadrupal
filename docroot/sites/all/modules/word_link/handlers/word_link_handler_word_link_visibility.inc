<?php

/**
 * @file
 * Definition of word_link_handler_word_link_visibility.
 */

/**
 * Field handler to present a word link visibility.
 *
 * @ingroup views_field_handlers
 */
class word_link_handler_word_link_visibility extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['visibility'] = 'visibility';
    $this->additional_fields['except'] = 'except';
  }

  function access() {
    return user_access('edit word link');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $output = NULL;
    $visibility = $this->get_value($values, 'visibility');
    $except = $this->get_value($values, 'except');

    if (!empty($except)) {
      $visibility = $visibility ? t('Only on') . ': ' : t('Except') . ': ';
      $output = $visibility . str_replace(array("\r\n", "\n", "\r"), " | ", check_plain(trim($except)));
    }

    return $output;
  }
}
