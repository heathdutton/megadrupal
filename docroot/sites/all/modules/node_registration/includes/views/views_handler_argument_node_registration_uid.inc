<?php

class views_handler_argument_node_registration_uid extends views_handler_argument {

  function init(&$view, &$options) {
    parent::init($view, $options);

    if (!empty($options['show_non_registered_nodes']) && !isset($options['show_non_registered_nodes'])) {
      $this->options['show_non_registered_nodes'] = 0;
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['show_non_registered_nodes'] = array(
      'default' => FALSE,
      'bool' => TRUE,
    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['show_non_registered_nodes'] = array(
      '#type' => 'checkbox',
      '#title' => t('Filter by user, but also preserve one of each node (Left Join). (Works only if related to node table.)'),
      '#default_value' => $this->options['show_non_registered_nodes'],
      '#fieldset' => 'argument_present',
    );
  }

  function query($group_by = FALSE) {
    $this->ensure_my_table();

    if ($this->relationship && $this->query->relationships[$this->relationship]['link'] == "node") {
      $this->query->table_queue[$this->table_alias]['join']->extra[] = array(
        'table' => $this->table_alias,
        'field' => $this->real_field,
        'value' => $this->argument,
      );

      if (!$this->options['show_non_registered_nodes']) {
        $this->query->table_queue[$this->table_alias]['join']->type = "INNER";
      }
    }
    else {
      $this->query->add_where(0, "$this->table_alias.$this->real_field", $this->argument);
    }
  }

}
