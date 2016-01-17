<?php
/**
 * @file
 * Field handler to present a link to reply to a comment.
 *
 * @ingroup views_field_handlers
 */

/**
 * Views field handler for comment reply link.
 */
class views_handler_field_comment_easy_reply_link_reply extends views_handler_field_comment_link {

  /**
   * Check for user rights to reply to comments.
   */
  function access() {
    // Check for permission to reply to comments.
    return user_access('post comments');
  }

  /**
   * Render a link to reply to a comment.
   */
  function render_link($data, $values) {
    if (!empty($this->options['text'])) {
      $text = $this->options['text'];
    }
    else {
      $text = t('reply');
    }
    $nid = $this->get_value($values, 'nid');
    $cid = $this->get_value($values, 'cid');
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "comment/reply/" . $nid . '/' . $cid;
    $this->options['alter']['fragment'] = 'edit-comment-body';
    $comment = comment_load($cid);
    $comment_number = _comment_easy_reply_get_comment_number($comment);
    $this->options['alter']['link_class'] = 'comment-easy-reply-number-' . $comment_number . ' comment-easy-reply-views-reply-link';
    $this->options['element_wrapper_class'] = 'comment-reply';
    comment_easy_reply_add_js(array('comment' => $comment));
    comment_easy_reply_add_css(array('comment' => $comment));
    return $text;
  }
}
