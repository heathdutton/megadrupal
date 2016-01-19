<?php

/**
 * @file
 *   Formats Scald atom attachment in Views.
 */

/**
 * Field handler to provide the attachment for statuses.
 */
class statuses_scald_views_handler_field_attachment extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['sid'] = array('table' => 'statuses_scald', 'field' => 'sid');
    $this->additional_fields['atom_id'] = array('table' => 'statuses_scald', 'field' => 'atom_id');
  }

  function query() {
    $this->add_additional_fields();
  }

  function render($values) {
    $attachment = statuses_scald_render_attachment($values->{$this->aliases['atom_id']});
    if ($attachment) {
      return '<div class="fbsmp clearfix">' . $attachment . '</div>';
    }
  }
}
