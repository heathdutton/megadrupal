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
class views_handler_field_comment_link_status_toggle extends views_handler_field_comment_link {
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // @TODO - Allow for configurable text labels.
    unset($form['text']);
  }

  function access() {
    //needs permission to administer comments in general
    return user_access('administer comments');
  }

  function render($values) {
    $value = $this->get_value($values, 'cid');
    return $this->render_link($this->sanitize_value($value), $values);
  }

  function render_link($data, $values) {
    $status = $this->get_value($values, 'status');

    // @TODO - Allow for configurable text labels.
    // $text = !empty($this->options['text']) ? $this->options['text'] : t('approve');

    $cid = $this->get_value($values, 'cid');

    switch ($status) {
      case COMMENT_NOT_PUBLISHED:
        $text = t('approve');
        $this->options['alter']['path'] = "better_comments_admin/ajax/nojs/" . $cid . "/approve";
        $this->options['alter']['query'] = drupal_get_destination() + array('token' => drupal_get_token("better_comments_admin/ajax/nojs/$cid/approve"));
        break;

      case COMMENT_PUBLISHED:
        $text = t('reject');
        $this->options['alter']['path'] = "better_comments_admin/ajax/nojs/" . $cid . "/reject";
        $this->options['alter']['query'] = drupal_get_destination() + array('token' => drupal_get_token("better_comments_admin/ajax/nojs/$cid/reject"));
        break;
    }

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['link_class'] = 'use-ajax';

    return $text;
  }
}
