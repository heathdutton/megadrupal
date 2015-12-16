<?php

/**
 * @file
 * Definition of views_handler_field_comment_link_approve.
 */

/**
 * Provides a comment approve link.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_comment_link_preview extends views_handler_field_comment_link {
  function access() {
    //needs permission to administer comments in general
    return user_access('administer comments');
  }

  function render_link($data, $values) {
    $status = $this->get_value($values, 'status');

    $text = !empty($this->options['text']) ? $this->options['text'] : t('preview');
    $cid = $this->get_value($values, 'cid');

    $this->options['alter']['path'] = "better_comments_admin/ajax/nojs/" . $cid . "/preview";
    $this->options['alter']['query'] = drupal_get_destination();
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['link_class'] = 'use-ajax';

    return $text;
  }
}
