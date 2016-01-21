<?php

/**
 * @file
 * Contains a Views field handler to take care of displaying deletes links
 * as fields
 */


class breadcrumb2_handler_delete_link_field extends breadcrumb2_handler_link_field {
  function construct() {
    parent::construct();
  }


  function render($values) {
    // Check access.
    if (!user_access('administer breadcrumbs')) {
      return;
    }
    
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $bid = $values->{$this->aliases['bid']};
    
    return l($text, 'breadcrumb/' . $bid . '/delete');
  }
}
