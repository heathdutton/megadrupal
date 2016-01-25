<?php
/**
 * @file
 * Field handler to present the comment title with Comment Easy Reply.
 *
 * @ingroup views_field_handlers
 */

/**
 * Views field handler for comment title.
 */
class views_handler_field_comment_easy_reply_comment_title extends views_handler_field_comment {

  /**
   * Defines an additional option definition for the Comment Easy Reply module.
   */
  function option_definition() {
    $options = parent::option_definition();
    // Defining a new option in comment_body field form.
    $options['comment_easy_reply_format'] = array(
      'default' => FALSE,
    );
    return $options;
  }

  /**
   * The comment_body field's option form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['comment_easy_reply_format'] = array(
      '#title' => t('Process with Comment Easy Reply filter'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['comment_easy_reply_format'],
      '#description' => t('If checked, comment body will be parsed by Comment Easy Reply module, turning every @#NUM into links.'),
      '#weight' => 20,
    );
  }
  
  function render_link($data, $values) {
    if (isset($this->options['comment_easy_reply_format']) && $this->options['comment_easy_reply_format']) {
      $data = _comment_easy_reply_comment_body_replace($data, $values->nid);
      $variables = array('comment' => $values);
      comment_easy_reply_add_js($variables);
      comment_easy_reply_add_css($variables);
    }
    parent::render_link($data, $values);
    return $data;
  }
}
