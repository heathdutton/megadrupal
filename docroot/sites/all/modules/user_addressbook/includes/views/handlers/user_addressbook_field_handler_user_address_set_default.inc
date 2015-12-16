<?php

/**
 * @file
 *
 * A Views' field handler for editing a node title.
 *
 */
class user_addressbook_field_handler_user_address_set_default extends views_handler_field {

  function construct() {
    parent::construct();
    $this->additional_fields['address_id'] = 'address_id';
    $this->additional_fields['uid'] = 'uid';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Render the field contents.
   *
   * @param $values
   * @return string
   */
  function render($values) {
    // Render a Views form item placeholder.
    return '<!--form-item-' . $this->options['id'] . '--' . $this->view->row_index . '-->';
  }

  /**
   * Add to and alter the form.
   */
  function views_form(&$form, &$form_state) {
    // Create a container for our replacements
    $form[$this->options['id']] = array(
      '#type' => 'container',
      '#tree' => TRUE,
    );
    $wrapper_id = isset($this->view->args[1]) ? $this->view->args[1] : FALSE;
    $type_arg = isset($this->view->args[2]) ? $this->view->args[2] : FALSE;

    // Iterate over the result and add our replacement fields to the form.
    foreach($this->view->result as $row_index => $row) {
      $uid = $row->{$this->aliases['uid']};
      $user_address_types = user_addressbook_address_types();
      if ($type_arg) {
        $user_address_types = array($type_arg => $user_address_types[$type_arg]);
      }

      $title = $this->options['label'] ? $this->options['label'] : $this->definition['title'];
      // Add checkboxes field to the form.  This array convention
      // corresponds to the placeholder HTML comment syntax.
      foreach ($user_address_types as $type => $user_address_type) {
        if (!empty($user_address_type['title'])) {
          $title = t('Set as %type_name default address', array('%type_name' => $user_address_type['title']));
        }
        $user_default_address = user_addressbook_get_default_addresses($uid, $type);
        $form[$this->options['id']][$row_index][$type] = array(
          '#type' => 'checkbox',
          '#return_value' => $row->{$this->aliases['address_id']}.'-'.$type,
          '#disabled' => ($user_default_address == $row->{$this->aliases['address_id']}),
          '#title' => $title,
          '#default_value' => ($user_default_address == $row->{$this->aliases['address_id']}) ? TRUE : FALSE,
          '#element_validate' => array('user_addressbook_field_handler_user_address_set_default_validate'),
          '#ajax' => array(
            'callback' => 'user_addressbook_ajax_form_refresh',
            'wrapper' => $wrapper_id,
          ),
        );
      }
    }
    $this->options['label'] = '';

    // Submit to the current page if not a page display.
    if ($this->view->display_handler->plugin_name != 'page') {
      $form['#action'] = current_path();
    }
  }

}
