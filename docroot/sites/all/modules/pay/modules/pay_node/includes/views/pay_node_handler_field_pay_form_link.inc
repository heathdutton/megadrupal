<?php

/**
 * @file
 * A Views field handler for links to payment forms.
 */

class pay_node_handler_field_pay_form_link extends views_handler_field_node_link {

  function render($values) {
    if ($pay_form = pay_node_form_load($values->nid)) {
      if ($path = $pay_form->menu_path()) {
        $text = !empty($this->options['text']) ? $this->options['text'] : t('Make a payment');
        return l($text, $path);
      }
    }
  }
}
