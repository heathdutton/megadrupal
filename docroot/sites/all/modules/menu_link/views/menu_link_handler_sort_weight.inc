<?php

/**
 * Handler for menu weight field of menu_link module
 */
class menu_link_handler_sort_weight extends views_handler_sort {

  function option_definition() {
    $options = parent::option_definition();

    $options['lowest'] = array('default' => 1);
    $options['highest'] = array('default' => 3);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $level_vals = array();
    for ($i = 1; $i < 10; $i++) {
      $level_vals[$i] = $i;
    }

    $form['lowest'] = array(
      '#type' => 'select',
      '#options' => $level_vals,
      '#default_value' => isset($this->options['lowest']) ? $this->options['lowest'] : 1,
      '#title' => t('Start level'),
      '#description' => t('This tells the sort handler from which value the sort will begin.'),
    );

    $form['highest'] = array(
      '#type' => 'select',
      '#options' => $level_vals,
      '#default_value' => isset($this->options['highest']) ? $this->options['highest'] : 3,
      '#title' => t('End level'),
      '#description' => t('Set the deepest / highest level the sort will attempt to sort'),
    );
  }

  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);

    $vals = $form_state['values']['options'];

    // the lowest limit has to be bigger than the selected highest
    if ($vals['lowest'] < $this->vals['highest']) {
      form_set_error('lowest', 'The lowest level has to be bigger than the highest value');
    }
  }

  /**
   * Called to add additional OrderBy statements to the query.
   */
  function query() {
    $this->ensure_my_table();

    $params = array(
      'function' => $this->options['group_type'],
    );

    $highest = $this->options['highest'];

    $join = new views_join();
    $join->definition = array(
      'left_field' => 'menu_link_mlid',
      'field' => 'mlid',
      'table' => 'menu_links',
      'left_table' => 'field_data_menu_link',
    );
    $join->table = 'menu_links';
    $join->left_table = 'field_data_menu_link';
    $join->left_field = 'menu_link_mlid';
    $join->field = 'mlid';
    $join->type = 'LEFT';

    $this->query->add_relationship('menu_links', $join, 'menu_links');
    $this->query->add_table('menu_links', 'menu_links', $join);

    $from = 1;
    if (isset($this->options['lowest']) && ($this->options['lowest'] > 0)) {
      $from = $this->options['lowest'];
    }

    for ($i = $from; $i <= $highest; $i++) {
      $index = $i;
      $addjoin = new views_join();
      $addjoin->definition = array(
        'left_field' => 'p' .$index,
        'field' => 'mlid',
        'table' => 'menu_links',
        'left_table' => 'menu_links',
      );
      $addjoin->table = 'menu_links';
      $addjoin->left_table = 'menu_links';
      $addjoin->left_field = 'p' .$index;
      $addjoin->field = 'mlid';
      $addjoin->type = 'LEFT';

      $alias = 'nml_p' .$index;
      $this->query->add_relationship('nml_p' .$index, $addjoin, 'nml_p' .$index);
      $this->query->add_table('menu_links', $alias, $addjoin);
      $this->query->add_orderby(NULL, $alias . '.weight', $this->options['order'], NULL, $params);
    }
  }

  function ui_name($short = FALSE) {
    return $this->get_field(parent::ui_name($short));
  }
}
