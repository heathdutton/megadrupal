<?php

/**
 * @file
 * Views handler intergration: displays the language field in a human readable way
 */

class lingwo_entry_views_handler_field_language_pair extends views_handler_field {
  function render($values) {
    $languages = language_list();
    $pair = explode('-', $values->{$this->field_alias});
    for ($i = 0; $i < count($pair); $i++) {
      $pair[$i] = isset($languages[$pair[$i]]) ? t($languages[$pair[$i]]->name) : '';
    }

    return implode('-', $pair);
  }
}

