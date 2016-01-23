<?php

/**
 * @file
 * Field handler to present a link to edit a user to a revision.
 */

/**
 * Views handler to present a link to edit a user to a revision.
 */
class UserRevisionEditLink
  extends views_handler_field_user_link {

  /**
   * Constructor for views handler revision edit link.
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['uid'] = 'uid';
    $this->additional_fields['vid'] = 'vid';
  }

  /**
   * Access control for user revision edit.
   */
  public function access() {
    return user_access('edit user revisions');
  }

  /**
   * Creates a renderable link text for user revision edit.
   */
  public function renderLink($data, $values) {
    if (!$this->access()) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "user/" . $values->{$this->aliases['uid']}
    . "/revisions/" . $values->{$this->aliases['vid']} . "/edit";
    $this->options['alter']['query'] = drupal_get_destination();

    return $text;
  }
}
