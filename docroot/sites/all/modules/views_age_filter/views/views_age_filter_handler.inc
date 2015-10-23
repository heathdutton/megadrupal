<?php
/**
 * @file
 * A standard Views filter for a single date field, using Date API form selectors and sql handling.
 */

class views_age_filter_handler extends date_views_filter_handler_simple {

  // Set default values for the date filter.
  function option_definition() {
    $options = parent::option_definition();
    $options['granularity']['default'] = 'second';
    $options['form_type']['default'] = 'date_text';
    $options['year_range']['default'] = '-50:+0';
    return $options;
  }

  public function operators() {
    $options = parent::operators();
    unset($options['regular_expression']);
    unset($options['contains']);
    return $options;
  }


  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);
    unset($form['form_type']);
    unset($form['granularity']);
    unset($form['year_range']);
    if (!$form['add_delta']['#access']) {
      $form['no_config'] = array(
        '#type' => 'markup',
        '#markup' => t('No configuration options.'),
      );
    }
  }

  function accept_exposed_input($input) {
    if (!empty($this->options['exposed'])) {
      $element_input = $input[$this->options['expose']['identifier']];
      $element_input['value'] = $this->get_filter_value('value', !empty($element_input['value']) || (string)$element_input['value'] == '0' ? $element_input['value'] . ' ' : '');
      $element_input['min'] = $this->get_filter_value('min', !empty($element_input['min']) || (string)$element_input['min'] == '0' ? $element_input['min'] . ' ' : '');
      $element_input['max'] = $this->get_filter_value('max', !empty($element_input['max']) || (string)$element_input['max'] == '0' ? $element_input['max'] . ' ' : '');
      if (is_array($element_input) && isset($element_input['default_date'])) {
        unset($element_input['default_date']);
      }
      if (is_array($element_input) && isset($element_input['default_to_date'])) {
        unset($element_input['default_to_date']);
      }

      $input[$this->options['expose']['identifier']] = $element_input;
    }
    $ret = parent::accept_exposed_input($input);
    foreach (array('value', 'min', 'max') as $key) {
      if (isset($this->value[$key]) && $this->value[$key]) {
        $this->value[$key] = drupal_substr($this->value[$key], 0, drupal_strlen($this->value[$key]) - 1);
      }
    }
    return $ret;
  }

  /**
   * A form element to select date part values.
   *
   * @param string $prefix
   *   A prefix for the date values, 'value', 'min', or 'max' .
   * @param string $source
   *   The operator for this element.
   * @param string $which
   *   Which element to provide, 'all', 'value', or 'minmax' .
   * @param array $operator_values
   *   An array of the allowed operators for this element.
   * @param array $identifier
   *   Identifier of the exposed element.
   * @param array $relative_id
   *   Form element id to use for the relative date field.
   *
   * @return
   *   The form date part element for this instance.
   */
  function date_parts_form($form_state, $prefix, $source, $which, $operator_values, $identifier, $relative_id) {
    module_load_include('inc', 'date_api', 'date_api_elements');
    switch ($prefix) {
      case 'min':
        $label = t('Minimum age');
        $relative_label = t('Minimum age');
        break;
      case 'max':
        $label = t('Maximum age');
        $relative_label = t('Maximum age');
        break;
      default:
        $label = '';
        $relative_label = t('Age');
        break;
    }

    $type = $this->options['form_type'];
    if ($type == 'date_popup' && !module_exists('date_popup')) {
      $type = 'date_text';
    }

    $format = $this->date_handler->views_formats($this->options['granularity'], 'sql');
    $granularity = array_keys($this->date_handler->date_parts($this->options['granularity']));
    $relative_value = ($prefix == 'max' ? $this->options['default_to_date'] : $this->options['default_date']);

    if (!empty($form_state['exposed'])) {
      // UI when the date selector is exposed.
      $default_date = $this->date_default_value($prefix);
      $id = 'edit-' . str_replace('_', '-', $this->field) . '-' . $prefix;
      $form[$prefix] = array(
        '#title' => check_plain($label),
        '#type' => 'textfield',
        '#size' => 20,
        '#element_validate' => array('element_validate_integer'),
        '#default_value' => !empty($this->value[$prefix]) || (string) $this->value[$prefix] == '0' ? $this->value[$prefix] : $default_date,
        '#prefix' => '<div id="' . $id . '-wrapper"><div id="' . $id . '">',
        '#suffix' => '</div></div>',
      );
      if ($which == 'all') {
        $form[$prefix]['#pre_render'][] = 'ctools_dependent_pre_render';
        $form[$prefix]['#dependency'] = array($source => $operator_values);
      }
      if (!isset($form_state['input'][$identifier][$prefix])) {
        $form_state['input'][$identifier][$prefix] = $this->value[$prefix];
      }
    }
    else {
      // UI when the date selector is on the views configuration screen.
      $default_date = '';
      $id = 'edit-options-value-' . $prefix;
      $form[$prefix . '_group'] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('date-views-filter-fieldset')),
      );
      $form[$prefix . '_group'][$prefix . '_choose_input_type'] = array(
        '#type' => 'value',
        '#value' => 'relative',
      );
      $form[$prefix . '_group'][$relative_id] = array(
        '#type' => 'textfield',
        '#title' => check_plain($relative_label),
        '#default_value' => $relative_value,
        '#element_validate' => array('element_validate_integer'),
        '#description' => t('Enter the age to filter by.'),
      );
      if ($which == 'all') {
        $form[$prefix . '_group']['#pre_render'][] = 'ctools_dependent_pre_render';
        $form[$prefix . '_group']['#dependency'] = array($source => $operator_values);
      }
    }
    return $form;
  }

  function date_default_value($prefix, $options = NULL) {
    $default_date = '';
    if (empty($options)) {
      $options = $this->options;
    }
    // If this is a remembered value, use the value from the SESSION.
    if (!empty($this->options['expose']['remember'])) {
      $display_id = ($this->view->display_handler->is_defaulted('filters')) ? 'default' : $this->view->current_display;
      if (!empty($_SESSION['views'][$this->view->name][$display_id]['date_filter'][$prefix])) {
        return $_SESSION['views'][$this->view->name][$display_id]['date_filter'][$prefix];
      }
    }

    // This is a date that needs to be constructed from options like 'now' .
    $default_date = $prefix == 'max' ? $options['default_to_date'] : $options['default_date'];
    if (empty($default_date) && (string) $default_date !== '0') {
      $default_date = $options['value'][$prefix];
    }
    return $default_date;
  }

  /**
   * Value validation.
   *
   * TODO add in more validation.
   *
   * We are setting an extra option using a value form
   * because it makes more sense to set it there.
   * That's not the normal method, so we have to manually
   * transfer the selected value back to the option.
   */
  function value_validate($form, &$form_state) {
    $options = &$form_state['values']['options'];

    if ($options['operator'] == 'between' || $options['operator'] == 'not between') {
      if ($options['expose']['required'] && empty($options['value']['min_group']['default_date']) && (string) $options['value']['min_group']['default_date'] !== '0') {
        form_set_error('options][value][min_group][default_date', t('Minimum age not specified.'));
      }
      else {
        $this->options['default_date'] = $options['value']['min_group']['default_date'];
        // NULL out the value field, user wanted the relative value to take hold.
        $options['value']['min_group']['min'] = NULL;
      }
      if ($options['expose']['required'] && empty($options['value']['max_group']['default_to_date']) && (string) $options['value']['max_group']['default_to_date'] !== '0') {
        form_set_error('options][value][max_group][default_to_date', t('Maximum age not specified.'));
      }
      else {
        $this->options['default_to_date'] = $options['value']['max_group']['default_to_date'];
        // NULL out the value field, user wanted the relative value to take hold.
        $options['value']['max_group']['max'] = NULL;
      }
    }
    elseif ($options['expose']['required'] && in_array($options['operator'], array('<', '<=', '=', '!=', '>=', '>'))) {
      if (empty($options['value']['value_group']['default_date']) && (string) $options['value']['value_group']['default_date'] !== '0') {
        form_set_error('options][value][value_group][default_date', t('Age not specified.'));
      }
      else {
        $this->options['default_date'] = $options['value']['value_group']['default_date'];
        // NULL out the value field, user wanted the relative value to take hold.
        $options['value']['value_group']['value'] = NULL;
      }
    }
    // Flatten the form structure for views, so the values can be saved.
    foreach (array('value', 'min', 'max') as $key) {
      $options['value'][$key] = $options['value'][$key . '_group'][$key];
    }
  }

  function op_between($field, $min = NULL, $max = NULL, $operator = NULL) {
    if (is_null($operator)) {
      $operator = $this->operator;
    }
    // Add the delta field to the view so we can later find the value that matched our query.
    list($table_name, $field_name) = explode('.', $field);
    if (!empty($this->options['add_delta']) && (substr($field_name, -6) == '_value' || substr($field_name, -7) == '_value2')) {
      $this->query->add_field($table_name, 'delta');
      $real_field_name = str_replace(array('_value', '_value2'), '', $this->real_field);
      $this->query->add_field($table_name, 'entity_id', 'date_id_' . $real_field_name);
      $this->query->add_field($table_name, 'delta', 'date_delta_' . $real_field_name);
    }

    $min_value = $this->get_filter_value('min', is_null($min) ? $this->value['min'] : $min);
    $max_value = $this->get_filter_value('max', is_null($max) ? $this->value['max'] : $max);

    $min_comp_date = empty($min_value) ? new DateObject('now', date_default_timezone()) : new DateObject('now -' . $min_value . ' years', date_default_timezone());
    $max_comp_date = empty($max_value) ? new DateObject('now -1 year +1 second', date_default_timezone()) : new DateObject('now -' . $max_value . ' years -1 year +1 second', date_default_timezone());

    $field_min = $this->date_handler->sql_field($field, NULL, $min_comp_date);
    $field_min = $this->date_handler->sql_format($this->format, $field_min);
    $field_max = $this->date_handler->sql_field($field, NULL, $max_comp_date);
    $field_max = $this->date_handler->sql_format($this->format, $field_max);

    $placeholder_min = $this->placeholder();
    $placeholder_max = $this->placeholder();

    $group = !empty($this->options['date_group']) ? $this->options['date_group'] : $this->options['group'];
    if ($operator == 'between') {
      $this->query->add_where_expression($group, "$field_min <= $placeholder_min AND $field_max >= $placeholder_max", array($placeholder_min => $min_comp_date->format($this->format, TRUE), $placeholder_max => $max_comp_date->format($this->format, TRUE)));
    }
    else {
      $this->query->add_where_expression($group, "$field_min > $placeholder_min OR $field_max < $placeholder_max", array($placeholder_min => $min_comp_date->format($this->format, TRUE), $placeholder_max => $max_comp_date->format($this->format, TRUE)));
    }
  }

  function op_simple($field, $value = NULL, $operator = NULL) {
    $value = is_null($value) ? $this->get_filter_value('value', $this->value['value']) : $value;
    $operator = is_null($operator) ? $this->operator : $operator;
    switch($operator) {
      case '<':
      case '>':
      case '>=':
      case '<=':
        // Add the delta field to the view so we can later find the value that matched our query.
        list($table_name, $field_name) = explode('.', $field);
        if (!empty($this->options['add_delta']) && (substr($field_name, -6) == '_value' || substr($field_name, -7) == '_value2')) {
          $this->query->add_field($table_name, 'delta');
          $real_field_name = str_replace(array('_value', '_value2'), '', $this->real_field);
          $this->query->add_field($table_name, 'entity_id', 'date_id_' . $real_field_name);
          $this->query->add_field($table_name, 'delta', 'date_delta_' . $real_field_name);
        }

        // Age > 18 => more than 18 years, 11 months, 30 days, 23h59m59s = 19 years (included) or more (>=19).
        // Age < 18 => 17 years, 11 months, 30 days, 23h59m59s (included) or less = 18 years (excluded) or less (<18).
        // Age >= 18 => 18 years (included) or more (>=18).
        // Age <= 18 => 19 years (excluded) or less (<19).
        if ($operator == '>') {
          $operator = '>=';
          $value += 1;
        }
        if ($operator == '<=') {
          $operator = '<';
          $value += 1;
        }
        $comp_date = new DateObject('now -' . $value . 'years', date_default_timezone());
        $field = $this->date_handler->sql_field($field, NULL, $comp_date);
        $field = $this->date_handler->sql_format($this->format, $field);
        $placeholder = $this->placeholder();
        $group = !empty($this->options['date_group']) ? $this->options['date_group'] : $this->options['group'];
        $this->query->add_where_expression($group, "$placeholder $operator $field", array($placeholder => $comp_date->format($this->format, TRUE)));
        break;
      case '=':
      case '!=':
        // Age = 18 => Age between 18 years and 18 years, 11 months, 30 days, 23h59m59s
        // Age != 18 => Age not between 18 years and 18 years, 11 months, 30 days, 23h59m59s
        $this->op_between($field, $value, $value, $operator == '=' ? 'between' : 'not between');
        break;
    }
  }
}
