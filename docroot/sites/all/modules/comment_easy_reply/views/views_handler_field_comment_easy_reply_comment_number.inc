<?php
/**
 * @file
 * Field handler to show the comment's number provided by Comment Easy Reply
 * module.
 *
 * @ingroup views_field_handlers
 */

/**
 * Views field handler for comment number permalink.
 */
class views_handler_field_comment_easy_reply_comment_number extends views_handler_field_comment_link {
  /**
   * Override init function to provide generic option to link to comment.
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
  }

  /**
   * The comment number field's option form.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['comment_easy_reply_reply_from_numlink'] = array('default' => FALSE);
    $options['comment_easy_reply_replytip_activated'] = array('default' => TRUE);
    if (isset($options['link_to_node'])) {
        unset($options['link_to_node']);
    }
    if (isset($options['text'])) {
        unset($options['text']);
    }
    return $options;
  }

  /**
   * Provide link-to-comment option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['comment_easy_reply_reply_from_numlink'] = array(
      '#title' => t('Allow users to reply to a comment by clicking on comment #NUM link.'),
      '#description' => t('If this option is not selected, the comment #NUM link will be a permalink.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['comment_easy_reply_reply_from_numlink'],
    );
    $form['comment_easy_reply_replytip_activated'] = array(
      '#title' => t('Activate reply tooltip.'),
      '#description' => t('This checkbox controls the tooltip that appears passing the mouse over comment #NUM link.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['comment_easy_reply_replytip_activated'],
    );
    if (isset($form['link_to_node'])) {
        unset($form['link_to_node']);
    }
    if (isset($form['text'])) {
        unset($form['text']);
    }
  }

  /**
   * Render the comment's number link.
   */
  function render_link($data, $values) {
    $cid = $this->get_value($values, 'cid');
    $comment = comment_load($cid);
    $variables = array('comment' => $comment, 'comment_easy_reply' => array());
    if ($this->options['comment_easy_reply_reply_from_numlink']) {
      $variables['comment_easy_reply']['reply_from_numlink'] = TRUE;
    }
    else {
      $variables['comment_easy_reply']['reply_from_numlink'] = FALSE;
    }
    if ($this->options['comment_easy_reply_replytip_activated']) {
      $variables['comment_easy_reply']['replytip_activated'] = TRUE;
    }
    else {
      $variables['comment_easy_reply']['replytip_activated'] = FALSE;
    }
    if (isset($_GET['page'])) {
      // If a pager is active in views, rebuild a custom comment array using
      // the Views' order.
      $variables['comments_per_page'] = $this->view->get_items_per_page();
      if (!empty($this->view->result)) {
        $custom_source = array();
        $current_page = 0;
        for ($i = 0; $i < $this->view->total_rows; $i++) {
          if ($i > 0 && ($i % $variables['comments_per_page'] == 0)) {
            $current_page++;
          }
          if ($_GET['page'] == $current_page) {
            foreach ($this->view->result as $pos => $result_comment) {
              $custom_source[$result_comment->cid] = $pos + 1;
            }
          }
          else {
            $custom_source[$current_page . '_' . $i] = $i;
          }
        }
      }
      $variables['custom_source'] = $custom_source;
    }
    return theme('comment_easy_reply_comment_number_link', $variables);
  }
}
