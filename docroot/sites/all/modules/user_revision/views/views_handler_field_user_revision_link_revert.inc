<?php
// $Id: 
/**
 * Field handler to present a link to revert a user to a revision
 */
class views_handler_field_user_revision_link_revert extends views_handler_field_user_link {
  function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'uid';
    $this->additional_fields['vid'] = 'vid';
  }

  function access() {
    return user_access('revert user revisions');
  }

  function render_link($data, $values) {
    if (!$this->access()) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('revert');
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "user/" . $values->{$this->aliases['uid']} . "/revisions/" . $values->{$this->aliases['vid']} . "/revert";
    $this->options['alter']['query'] = drupal_get_destination();

    return $text;
  }
}
