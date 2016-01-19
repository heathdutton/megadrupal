<?php

/**
 * Field handler to present a term edit link.
 *
 * @ingroup views_field_handlers
 */
class CommentAbuseHandlerFieldViewComplaints extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    if (!empty($values->cid)) {
      return l(t('View complaints'), 'admin/comment-abuse/view-complaints/' . $values->cid, array('query' => drupal_get_destination()));
    }
  }
}
