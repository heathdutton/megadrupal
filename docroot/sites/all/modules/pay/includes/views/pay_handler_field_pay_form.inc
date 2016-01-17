<?php

/**
 * @file
 * A Views field handler that lists payment forms.
 */

class pay_handler_field_pay_form extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['display'] = array('default' => 'pay_form_default');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['display'] = array(
      '#type' => 'select',
      '#title' => t('Display form as'),
      '#options' => pay_form_displays_list(),
      '#default_value' => $this->options['display'],
    );
  }

  function render(&$values) {
    $key = $this->definition['pay_key'];
    if ($this->table != $this->query->base_table) {
      $key = $this->field_alias ? $this->field_alias : $this->table . '_' . $key;
    }
    if ($pay = pay_load_object($this->definition['pay_type'], $values->$key)) {

      /* TODO this is an inappropriate hack that presumes that a missing
       menu_path() should be node/XX/pay on node-based views. This case is
       only true on pay_node forms. This should move to that module somehow.
       */
      if (!$pay->menu_path() && ($nid = $values->nid)) {
        $pay->set_menu_path('node/' . $nid . '/pay');
      }

      // TODO Please change this theme call to use an associative array for the $variables parameter.
      return theme($this->options['display'], $pay);
    }
  }
}
