<?php

/**
 * @file
 * Definition of views_handler_field_url.
 */

/**
 * Field handler to provide simple renderer that turns a URL into a clickable link.
 *
 * @ingroup views_field_handlers
 */
class cloudwords_views_handler_label extends views_handler_field {

  function render($values) {
    $value = $this->get_value($values);
    $this->options['alter']['make_link'] = FALSE;

    if ($translatable = cloudwords_translatable_load($values->ctid)) {
      $path = $translatable->uri();
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = $path['path'];
    }

    return $this->sanitize_value($value);
  }
}
