<?php

/**
 * Field handler to present a link to an event.
 */
class eventbrite_api_handler_field_ticket_link_clone_other extends eventbrite_api_handler_field_ticket_link_edit {

  function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('clone to other event');
    $tid = $this->get_value($values, 'tid');

    return l($text, 'admin/eventbrite/ticket/add/' . $tid);
  }
}
