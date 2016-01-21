<?php

/**
 * @file
 * Views handler to show building licenses
 */
class views_handler_field_license extends views_handler_field {
  function render($values) {
    $value = $values->{$this->field_alias};
    switch ($value) {
      case -1:
        return t('Not available');
        break;
      case 0:
        return t('No');
        break;
      case 1:
        return t('Yes');
        break;
    }
  }
}