<?php
// $Id: matrix_handler_filter.inc,v 1.1.2.1 2011/01/18 10:10:05 aaron1234nz Exp $

/**
 * Basic textfield filter to handle string filtering commands
 * including equality, like, not like, etc.
 */
class matrix_handler_filter extends views_handler_filter_string {

  function construct() {
    parent::construct();
    $this->value_title = t('Cell');
    $this->value_options = NULL;
  }

  function row_list() {
    $field = field_info_field($this->definition['field_name']);

    if ($field['type'] == 'matrix_text' && $field['settings']['spreadsheet_style'] == 1) {
      for ($i = 1; $i<= $field['settings']['rows_count']; $i++) {
        $output[$i] = $i;
      }
    }
    elseif ($field['type'] == 'matrix_custom') {
      $settings = unserialize($field['settings']['settings']);
      foreach ($settings['rows'] as $row_id => $row) {
        $output[$row_id] = t('@title (Row @i)', array('@title' => $row['title'], '@i' => $row_id));
      }
    }
    else {
      for ($i = 1; $i<= $field['settings']['rows_count']; $i++) {
        $output[$i] =  t('Row @i', array('@i' => $i));
      }
    }
    array_unshift($output, t('None'));
    return $output;
  }

  function col_list() {
    $field = field_info_field($this->definition['field_name']);

    if ($field['type'] == 'matrix_text' && $field['settings']['spreadsheet_style'] == 1) {
      for ($i = 1; $i<= $field['settings']['cols_count']; $i++) {
        $output[$i] = matrix_make_letter($i);
      }
    }
    elseif ($field['type'] == 'matrix_custom') {
      $settings = unserialize($field['settings']['settings']);
      foreach ($settings['cols'] as $col_id => $col) {
        $output[$row_id] = t('@title (Column @i)', array('@title' => $col['title'], '@i' => $col_id));
      }
    }
    else {
      for ($i = 1; $i<= $field['settings']['cols_count']; $i++) {
        $output[$i] =  t('Column @i', array('@i' => $i));
      }
    }
    array_unshift($output, t('None'));
    return $output;
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['value']['col'] = 1;
    $options['value']['row'] = 1;

    return $options;
  }

  /**
   * Provide a simple textfield for equality
   */
  function value_form(&$form, &$form_state) {
    if (isset( $this->options['value'])) {
      list($row_default, $col_default, $value_default) = explode('_____', $this->value);
    }

    // We have to make some choices when creating this as an exposed
    // filter form. For example, if the operator is locked and thus
    // not rendered, we can't render dependencies; instead we only
    // render the form items we need.
    $which = 'all';
    if (!empty($form['operator'])) {
      $source = ($form['operator']['#type'] == 'radios') ? 'radio:options[operator]' : 'edit-options-operator';
    }
    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];

      if (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator'])) {
        // exposed and locked.
        $which = in_array($this->operator, $this->operator_values(1)) ? 'value' : 'none';
      }
      else {
        $source = 'edit-' . drupal_html_id($this->options['expose']['operator']);
      }
    }

    if ($which == 'all' || $which == 'value') {
      $form['value'] = array(
        '#type' => 'textfield',
        '#title' => t('Value'),
        '#size' => 30,
        '#default_value' => $value_default,
      );
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $this->value;
      }

      if ($which == 'all') {
        $form['value'] += array(
          '#process' => array('ctools_dependent_process'),
          '#dependency' => array($source => $this->operator_values(1)),
          '#prefix' => '<div id="edit-options-value-wrapper">',
        );
        $form['row'] = array(
          '#type' => 'select',
          '#title' => t('Row'),
          '#options' => $this->row_list(),
          '#default_value' => $row_default,
          '#process' => array('ctools_dependent_process'),
        );
        $form['col'] = array(
          '#type' => 'select',
          '#title' => t('Column'),
          '#options' => $this->col_list(),
          '#default_value' => $col_default,
          '#process' => array('ctools_dependent_process'),
          '#suffix' => '</div>',
        );
      }
    }

    if (!isset($form['value'])) {
      // Ensure there is something in the 'value'.
      $form['value'] = array(
        '#type' => 'value',
        '#value' => NULL
      );
    }
    if (!isset($form['row'])) {
      // Ensure there is something in the 'row'.
      $form['row'] = array(
        '#type' => 'value',
        '#value' => NULL
      );
    }
    if (!isset($form['col'])) {
      // Ensure there is something in the 'value'.
      $form['col'] = array(
        '#type' => 'value',
        '#value' => NULL
      );
    }
  }

   function value_submit($form, &$form_state) {
    // Views does not apper to want to save anything from this form other than 'value' so the row, col, and value will be concatinated into the value cell.
    $form_state['values']['options']['value'] = $form_state['values']['options']['row'] .'_____'. $form_state['values']['options']['col'] .'_____'. $form_state['values']['options']['value'];
  }


  function admin_summary() {
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }

    $options = $this->operator_options('short');
    $op = check_plain($options[$this->operator]);

    $rows = $this->row_list();
    $cols = $this->col_list();

    if (isset($this->value)) {
      list($row, $col, $value) = explode('_____', $this->value);
      return t("Cell at row '%row', column '%col' %op %value", array('%row' => $rows[$row], '%col' => $cols[$col], '%op' => $op, '%value' => $value));
    }
    return $output;
  }

  function operator() {
    return $this->operator == '=' ? 'LIKE' : 'NOT LIKE';
  }


  /**
   * Add this filter to the query.
   *
   * Due to the nature of fapi, the value and the operator have an unintended
   * level of indirection. You will find them in $this->operator
   * and $this->value respectively.
   */
  function query() {
    $this->ensure_my_table();
    $field = substr("$this->table_alias.$this->real_field", 0, -5);

    $info = $this->operators();

    if (!empty($info[$this->operator]['method'])) {
      $this->{$info[$this->operator]['method']}($field);
    }
  }

  function op_equal($field) {
    $operator = $this->operator();
    list($row, $col, $value) = explode('_____', $this->value);

    $this->query->add_where($this->options['group'], $field .'value', $value, $operator);
    $this->query->add_where($this->options['group'], $field .'row', $row, '=');
    $this->query->add_where($this->options['group'], $field .'col', $col, '=');
  }

  function op_contains($field) {
    $placeholder = $this->placeholder();
    list($row, $col, $value) = explode('_____', $this->value);

    $this->query->add_where_expression($this->options['group'], $field. "value LIKE $placeholder", array($placeholder => '%' . db_like($value) . '%'));
    $this->query->add_where($this->options['group'], $field .'row', $row, '=');
    $this->query->add_where($this->options['group'], $field .'col', $col, '=');
  }

  function op_word($field) {
    list($row, $col, $value) = explode('_____', $this->value);
    $where = $this->operator == 'word' ? db_or() : db_and();

    preg_match_all('/ (-?)("[^"]+"|[^" ]+)/i', ' ' . $value, $matches, PREG_SET_ORDER);
    foreach ($matches as $match) {
      $phrase = false;
      // Strip off phrase quotes
      if ($match[2]{0} == '"') {
        $match[2] = substr($match[2], 1, -1);
        $phrase = true;
      }
      $words = trim($match[2], ',?!();:-');
      $words = $phrase ? array($words) : preg_split('/ /', $words, -1, PREG_SPLIT_NO_EMPTY);
      foreach ($words as $word) {
        $placeholder = $this->placeholder();
        $where->where($field ."value LIKE $placeholder", array($placeholder => '%' . db_like(trim($word, " ,!?")) . '%'));
      }
    }

    if (!$where) {
      return;
    }

    // previously this was a call_user_func_array but that's unnecessary
    // as views will unpack an array that is a single arg.
    $this->query->add_where($this->options['group'], $where);
    $this->query->add_where($this->options['group'], $field .'row', $row, '=');
    $this->query->add_where($this->options['group'], $field .'col', $col, '=');
  }

  function op_starts($field) {
    $placeholder = $this->placeholder();
    list($row, $col, $value) = explode('_____', $this->value);

    $this->query->add_where_expression($this->options['group'], $field ."value LIKE $placeholder", array($placeholder => db_like($value) . '%'));
    $this->query->add_where($this->options['group'], $field .'row', $row, '=');
    $this->query->add_where($this->options['group'], $field .'col', $col, '=');
  }

  function op_not_starts($field) {
    $placeholder = $this->placeholder();
    list($row, $col, $value) = explode('_____', $this->value);
    $this->query->add_where($this->options['group'], $field .'row', $row, '=');
    $this->query->add_where($this->options['group'], $field .'col', $col, '=');
    $this->query->add_where_expression($this->options['group'], $field ."value NOT LIKE $placeholder", array($placeholder => db_like($value) . '%'));
  }

  function op_ends($field) {
    $placeholder = $this->placeholder();
    list($row, $col, $value) = explode('_____', $this->value);

    $this->query->add_where($this->options['group'], $field .'row', $row, '=');
    $this->query->add_where($this->options['group'], $field .'col', $col, '=');
    $this->query->add_where_expression($this->options['group'], $field ."value LIKE $placeholder", array($placeholder => '%' . db_like($value)));
  }

  function op_not_ends($field) {
    $placeholder = $this->placeholder();
    list($row, $col, $value) = explode('_____', $this->value);

    $this->query->add_where($this->options['group'], $field .'row', $row, '=');
    $this->query->add_where($this->options['group'], $field .'col', $col, '=');
    $this->query->add_where_expression($this->options['group'], $field ."value NOT LIKE $placeholder", array($placeholder => '%' . db_like($value)));
  }

  function op_not($field) {
    $placeholder = $this->placeholder();
    list($row, $col, $value) = explode('_____', $this->value);

    $this->query->add_where($this->options['group'], $field .'row', $row, '=');
    $this->query->add_where($this->options['group'], $field .'col', $col, '=');
    $this->query->add_where_expression($this->options['group'], $field ."value NOT LIKE $placeholder", array($placeholder => '%' . db_like($value) . '%'));
  }

  function op_shorter($field) {
    $placeholder = $this->placeholder();
    list($row, $col, $value) = explode('_____', $this->value);

    $this->query->add_where($this->options['group'], $field .'row', $row, '=');
    $this->query->add_where($this->options['group'], $field .'col', $col, '=');
    $this->query->add_where($this->options['group'], "LENGTH(". $field ."value) < $placeholder", array($placeholder => $value), 'formula');
  }

  function op_longer($field) {
    $placeholder = $this->placeholder();
    list($row, $col, $value) = explode('_____', $this->value);

    $this->query->add_where($this->options['group'], $field .'row', $row, '=');
    $this->query->add_where($this->options['group'], $field .'col', $col, '=');
    $this->query->add_where($this->options['group'], "LENGTH(". $field ."value) > $placeholder", array($placeholder => $value), 'formula');
  }

  function op_empty($field) {
    if ($this->operator == 'empty') {
      $operator = "=";
    }
    else {
      $operator = "<>";
    }
    list($row, $col, $value) = explode('_____', $this->value);

    $this->query->add_where($this->options['group'], $field .'row', $row, '=');
    $this->query->add_where($this->options['group'], $field .'col', $col, '=');
    $this->query->add_where($this->options['group'], $field .'value', NULL, $operator);
  }
}
