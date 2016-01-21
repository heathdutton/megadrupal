<?php

/**
 * @file
 * Views handler intergration: displays the language field in a human readable way
 */

class lingwo_entry_views_handler_field_language extends views_handler_field {
  function render($values) {
    $languages = language_list();
    $value = isset($languages[$values->{$this->field_alias}]) ? t($languages[$values->{$this->field_alias}]->name) : '';
    return $value;
  }
}

