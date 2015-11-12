<?php

/**
 * @file
 * Definition of ViewsNegateHandlerArgumentFieldListString.
 */

/**
 * Extended string hander to add a negate condition for list type field.
 */
class ViewsNegateHandlerArgumentFieldListString extends views_handler_argument_field_list_string {
  /**
   * Option defination for negate option.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['negate'] = array('default' => 0);
    return $options;
  }

  /**
   * Field for negate option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['negate'] = array(
      '#type' => 'checkbox',
      '#title' => t('Negate'),
      '#description' => t('If selected, the value entered for the filter will be excluded rather than limiting the view.'),
      '#default_value' => $this->options['negate'],
      '#fieldset' => 'more',
    );
  }

  /**
   * Add condition in query.
   */
  function query($group_by = FALSE) {
    if ($this->options['negate'] != 1) {
      return parent::query();
    }
    else {
      $argument = $this->argument;
      if (!empty($this->options['transform_dash'])) {
        $argument = strtr($argument, '-', ' ');
      }

      if (!empty($this->options['break_phrase'])) {
        views_break_phrase_string($argument, $this);
      }
      else {
        $this->value = array($argument);
        $this->operator = 'or';
      }

      if (!empty($this->definition['many to one'])) {
        if (!empty($this->options['glossary'])) {
          $this->helper->formula = TRUE;
        }
        $this->helper->ensure_my_table();
        $this->helper->add_filter();
        return;
      }

      $this->ensure_my_table();
      $formula = FALSE;
      if (empty($this->options['glossary'])) {
        $field = "$this->table_alias.$this->real_field";
      }
      else {
        $formula = TRUE;
        $field = $this->get_formula();
      }

      if (count($this->value) > 1) {
        $operator = 'NOT IN';
        $argument = $this->value;
      }
      else {
        $operator = '!=';
      }

      if ($formula) {
        $placeholder = $this->placeholder();
        if ($operator == 'NOT IN') {
          $field .= " NOT IN($placeholder)";
        }
        else {
          $field .= '!=' . $placeholder;
        }
        
        $field .= " OR $this->table_alias.$this->real_field IS NULL";
        $placeholders = array(
          $placeholder => $argument,
        );
        $this->query->add_where_expression(0, $field, $placeholders);
      }
      else {
        $this->query->add_where(
          NULL,
          db_or()
            ->condition($field, $argument, $operator)
            ->condition("$this->table_alias.$this->real_field", NULL, 'IS NULL')
        );
      }
    }
  }
}
