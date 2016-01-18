<?php
/**
 * @file
 * Contains a Views field handler to take care of displaying deletes links
 * as fields
 */

class party_handler_field_delete_link extends views_handler_field {
  function construct() {
    parent::construct();

    $this->additional_fields['pid'] = 'pid';
    $this->additional_fields['type'] = 'type';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $type = $values->{$this->aliases['type']};

    //Creating a dummy party to check access against
    $dummy_party = (object) array('type' => $type);
    if (!party_access('delete', $dummy_party)) {
      return;
    }

    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $party_id = $values->{$this->aliases['pid']};

    return l($text, 'party/' . $party_id . '/party/delete', array('query' => array('destination' => $_GET['q'])));
  }
}
