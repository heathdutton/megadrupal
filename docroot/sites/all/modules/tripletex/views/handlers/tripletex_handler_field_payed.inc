<?php

/**
 * @file
 * Definition of tripletex_handler_field_payed.
 */

class tripletex_handler_field_payed extends views_handler_field_boolean {


  function pre_render($values) {
    parent::pre_render($values);
    //drupal_set_message('pre_render:<pre>'.print_r($values, TRUE).'</pre>');
  }

  function render($values) {

    //drupal_set_message('render:<pre>'.print_r($values, TRUE).'</pre>');

    return parent::render($values);
  }

}
