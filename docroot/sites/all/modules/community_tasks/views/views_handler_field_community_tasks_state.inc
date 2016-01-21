<?php

class views_handler_field_community_tasks_state extends views_handler_field_markup {

  function option_definition() {
    $options = parent::option_definition();
    $options['advanced_render'] = 1;
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['advanced_render'] = array(
      '#title' => t('Render as action'),
      '#description' => t("If unchecked this field merely shows the 'state' of the task."),
      '#type' => 'checkbox',
      '#return_value' => 1,
      '#default_value' => $this->options['advanced_render']
    );
    parent::options_form($form, $form_state);
  }

  function query() {
    $this->ensure_my_table();

    $this->add_additional_fields(array(
      'promote' => 'promote',
      'uid' => array('table' => 'node', 'field' => 'uid'),
      'nid' =>  array('table' => 'node', 'field' => 'nid')
    ));
  }

  function render($row) {
    $uid = &$row->{$this->aliases['uid']};
    $node = node_load($row->nid);
    if ($this->options['advanced_render']) {
      return _ctask_action_form($node);
    }
    else return ct_state_names(ctask_get_state($node));
  }
}