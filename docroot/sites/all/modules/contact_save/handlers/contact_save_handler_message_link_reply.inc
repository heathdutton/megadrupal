<?php

/**
 * @file
 * Definition of contact_save_handler_message_link_delete.
 */

/**
 * Field handler to present a link to delete a message.
 *
 * @ingroup views_field_handlers
 */
class contact_save_handler_message_link_reply extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['mail'] = 'mail';
    $this->additional_fields['subject'] = 'subject';
  }

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

  function access() {
    return user_access('administer contact forms');
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $mail = $this->get_value($values, 'mail');
    $subject = $this->get_value($values, 'subject');

    $value = url(
      'mailto:' . $mail,
      array(
        'query' => array(
          'subject' => $subject,
        ),
      )
    );

    return $this->render_link($value, $values);
  }

  function render_link($data, $values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('reply');

    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = $data;

    return $text;
  }
}
