<?php

/**
 * @file
 * Definition of numeric_field_filter_handler_filter_numeric.
 */

/**
 * Number Field Filter class.
 *
 * Class extends views_handler_filter class allowing to filter not only against
 * constant values but also against values of other numeric fields attached to
 * the entity type on which the current view operates.
 */
class numeric_field_filter_handler_filter_numeric extends views_handler_filter {

  /**
   * @var bool
   *
   * I am really not sure how this parameter matters in a handler, but making it
   * TRUE helps to make work $this->accept_exposed_input() method.
   */
  var $always_multiple = TRUE;

  /**
   * Information about options for all kinds of purposes will be held here.
   * @code
   * 'option_name' => array(
   *  - 'default' => default value,
   *  - 'translatable' => (optional) TRUE/FALSE (wrap in t() on export if true),
   *  - 'contains' => (optional) array of items this contains, with its own
   *      defaults, etc. If contains is set, the default will be ignored and
   *      assumed to be array().
   *  - 'bool' => (optional) TRUE/FALSE Is the value a boolean value. This will
   *      change the export format to TRUE/FALSE instead of 1/0.
   *  - 'export' => (optional) FALSE or a callback for special export handling
   *      if necessary.
   *  - 'unpack_translatable' => (optional) callback for special handling for
   *      translating data within the option, if necessary.
   *  ),
   *
   * @return array
   *   Returns the options of this handler/plugin.
   *
   * @see views_object::export_option()
   * @see views_object::export_option_always()
   * @see views_object::unpack_translatable()
   */
  function option_definition() {
    $options = parent::option_definition();

    // Options for the default value.
    $default = array(
      'contains' => array(
        'formula' => array(
          'default' => NUMERIC_FIELD_FILTER_VIEWS_FIELD_TOKEN,
        ),
        'field' => array(
          'default' => NULL,
        ),
      ),
    );
    $options['value'] = array(
      'contains' => array(
        'base' => $default,
        'counterpart' => array(
          'contains' => array(
            'value' => $default,
            'min' => $default,
            'max' => $default,
          ),
        ),
      ),
    );

    // Options of base field.
    $options['base_field_definition'] = array(
      'default' => NULL,
    );
    if ($this->definition['field_name'] != NUMERIC_FIELD_FILTER_VIEWS_FIELD_CONSTANT) {
      // Loading field definition array.
      $options['base_field_definition']['default'] = field_info_field($this->definition['field_name']);
    }

    // Options of counterpart field.
    $options['counterpart_field'] = array(
      'default' => NUMERIC_FIELD_FILTER_VIEWS_FIELD_CONSTANT,
    );
    $options['counterpart_field_relationship'] = array(
      'default' => NUMERIC_FIELD_FILTER_VIEWS_FIELD_RELATIONSHIP_BASE,
    );
    $options['counterpart_field_definition'] = array(
      'default' => NULL,
    );
    $options['counterpart_field_table_alias'] = array(
      'default' => NULL,
    );
    return $options;
  }

  /**
   * Provide a list of options for the default operator form.
   * Should be overridden by classes that don't override operator_form
   */
  function operator_options($which = 'title') {
    $options = array();
    foreach ($this->operators() as $id => $info) {
      $options[$id] = $info[$which];
    }
    return $options;
  }

  /**
   * Options form subform for setting options.
   *
   * This should be overridden by all child classes and it must
   * define $form['value']
   *
   * @see options_form()
   */
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    $form['value']['#tree'] = TRUE;

    $form['value']['base'] = array(
      '#type' => 'fieldset',
      '#title' => t('Base Field'),
    );
    $this->value_form_element($form['value']['base'], $form_state, $this->options['base_field_definition'], $this->options['value']['base']);

    $form['value']['counterpart'] = array(
      '#type' => 'fieldset',
      '#title' => t('Counterpart Field'),
    );

    // We have to make some choices when creating this as an exposed filter
    // form. For example, if the operator is locked and thus not rendered,
    // we can't render dependencies; instead we only render the form items we
    // need.
    // Var $which holds what set of elements should be generated:
    //   - 'all': all elements, i.e. 'value', 'min', 'max'
    //   - 'value': only 'value' set
    //   - 'minmax': only 'min' and 'max' sets.
    $which = 'all';
    // Var $source holds #states property of the element on which sets of values
    // depend (if they should depend), if no #states are required, $source is
    // FALSE.
    $source = FALSE;
    if (!empty($form['operator'])) {
      $source = ($form['operator']['#type'] == 'radios') ? ':input[name="options[operator]"]' : ':input[id="edit-options-operator"]';
    }
    if (!empty($form_state['exposed'])) {
      if (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator_id'])) {
        // Exposed and locked.
        $operators = $this->operator_options('values');
        $which = $operators[$this->operator] == 1 ? 'value' : 'minmax';
      }
      else {
        $source = ':input[id="edit-' . drupal_html_id($this->options['expose']['operator_id']) . '"]';
      }
    }

    if (in_array($which, array('all', 'value'))) {
      $form['value']['counterpart']['value'] = array(
        '#type' => 'item',
        '#title' => t('Value'),
      );
      if ($source) {
        $form['value']['counterpart']['value']['#states'] = array(
          'invisible' => array(
            $source => array(
              array('value' => 'between'),
              array('value' => 'not between'),
            ),
          ),
        );
      }
    }
    if (in_array($which, array('all', 'minmax'))) {
      $form['value']['counterpart']['min'] = array(
        '#type' => 'item',
        '#title' => t('Min'),
      );
      $form['value']['counterpart']['max'] = array(
        '#type' => 'item',
        '#title' => t('Max'),
      );
      if ($source) {
        $states = array(
          'visible' => array(
            $source => array(
              array('value' => 'between'),
              array('value' => 'not between'),
            ),
          ),
        );
        $form['value']['counterpart']['min']['#states'] = $states;
        $form['value']['counterpart']['max']['#states'] = $states;
      }

    }

    // Extra processing each of added counterpart value sets.
    foreach (element_children($form['value']['counterpart']) as $child) {
      $this->value_form_element($form['value']['counterpart'][$child], $form_state, $this->options['counterpart_field_definition'], $this->options['value']['counterpart'][$child]);
    }
  }

  /**
   * If a handler has 'extra options' it will get a little settings widget and
   * another form called extra_options.
   */
  function has_extra_options() {
    // We want to let user choose counterpart field in extra options.
    return TRUE;
  }

  /**
   * Provide a form for setting options.
   */
  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    $default_value = NUMERIC_FIELD_FILTER_VIEWS_FIELD_CONSTANT;
    if ($this->options['counterpart_field'] != NUMERIC_FIELD_FILTER_VIEWS_FIELD_CONSTANT) {
      $default_value = $this->options['counterpart_field_relationship'] . '__' . $this->options['counterpart_field'];
    }
    $form['counterpart_field'] = array(
      '#type' => 'select',
      '#title' => t('Choose Counterpart Field'),
      '#required' => TRUE,
      '#options' => $this->counterpart_field_options(),
      '#description' => t('Please, choose against what field you want to filter the base field %base_field.', array(
        '%base_field' => $this->options['base_field_definition']['field_name'],
      )),
      '#default_value' => $default_value,
    );

    // Additionally we define some hidden elements, whose values may be
    // overriden in validate callback.
    $form['counterpart_field_relationship'] = array(
      '#type' => 'value',
      '#value' => $this->options['counterpart_field_relationship'],
    );
    $form['counterpart_field_definition'] = array(
      '#type' => 'value',
      '#value' => $this->options['counterpart_field_definition'],
    );
  }

  /**
   * Validate the options form.
   */
  function extra_options_validate($form, &$form_state) {
    if ($form_state['values']['options']['counterpart_field'] != NUMERIC_FIELD_FILTER_VIEWS_FIELD_CONSTANT) {
      // We need to fetch relationship and load Field API definition array of
      // the selected field.
      list ($relationship, $counterpart) = explode('__', $form_state['values']['options']['counterpart_field']);
      $form_state['values']['options']['counterpart_field_relationship'] = $relationship;
      $form_state['values']['options']['counterpart_field'] = $counterpart;

      $form_state['values']['options']['counterpart_field_definition'] = field_info_field($counterpart);
    }
  }

  /**
   * Ensure the main table for this handler is in the query. This is used
   * a lot.
   */
  function ensure_my_table() {
    $return = parent::ensure_my_table();
    if (!is_null($return) && $this->options['counterpart_field'] != NUMERIC_FIELD_FILTER_VIEWS_FIELD_CONSTANT) {
      // Additionally we need to ensure the table of the counterpart field.
      // @todo: add support for revisions.
      $counterpart_table = array_keys($this->options['counterpart_field_definition']['storage']['details']['sql']['FIELD_LOAD_CURRENT']);
      $counterpart_table = array_pop($counterpart_table);

      // Additionally we find the relationship hanlder for our counterpart field
      // if one is necessary, relationship handler will tell us what alias uses
      // the base table of the entity to which our counterpart field is
      // attached. Once we know the base table alias of the counterpart entity
      // ensuring our counterpart field table is just as easy as calling
      // $this->query->ensure_table() with already found arguments.
      $counterpart_relationship = $this->options['counterpart_field_relationship'] == NUMERIC_FIELD_FILTER_VIEWS_FIELD_RELATIONSHIP_BASE ? NULL : $this->view->relationship[$this->options['counterpart_field_relationship']]->alias;
      $this->options['counterpart_field_table_alias'] = $this->query->ensure_table($counterpart_table, $counterpart_relationship);
    }
    return $return;
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
    $operartos = $this->operator_options('values');
    switch ($operartos[$this->operator]) {
      case 1:
        // For one value operator the skeleton is the folloing:
        // [base field] [operator] [counterpart field].
        // Adding base field expression.
        $expression = $this->sql_expression_generate($this->options['base_field_definition'], $this->table_alias, $this->value['base']);
        // Adding operator.
        $expression .= ' ' . $this->operator . ' ';
        // Adding counterpart field expression.
        $expression .= $this->sql_expression_generate($this->options['counterpart_field_definition'], $this->options['counterpart_field_table_alias'], $this->value['counterpart']['value']);
        $this->query->add_where_expression($this->options['group'], $expression);
        break;

      case 2:
        // For 2 values operator the base field should be between|not between
        // the 'min' and 'max' values of the filter.
        // Basically we are gonna have something like the following:
        // [base field] >|<= [counterpart min] AND|OR
        // [base field] <|>= [counterpart max]
        $base_field = $this->sql_expression_generate($this->options['base_field_definition'], $this->table_alias, $this->value['base']);
        $counterpart_min = $this->sql_expression_generate($this->options['counterpart_field_definition'], $this->options['counterpart_field_table_alias'], $this->value['counterpart']['min']);
        $counterpart_max = $this->sql_expression_generate($this->options['counterpart_field_definition'], $this->options['counterpart_field_table_alias'], $this->value['counterpart']['max']);
        $expression = $base_field . ' [operator1] ' . $counterpart_min . ' [conjunction] ' . $base_field . ' [operator2] ' . $counterpart_max;
        $replace = array();
        switch ($this->operator) {
          case 'between':
            $replace = array(
              '[operator1]' => '>',
              '[conjunction]' => 'AND',
              '[operator2]' => '<',
            );
            break;

          case 'not between':
            $replace = array(
              '[operator1]' => '<=',
              '[conjunction]' => 'OR',
              '[operator2]' => '>=',
            );
            break;
        }
        $expression = str_replace(array_keys($replace), array_values($replace), $expression);
        $this->query->add_where_expression($this->options['group'], $expression);
        break;
    }
  }

  /**
   * Display the filter on the administrative summary
   */
  function admin_summary() {
    if ($this->is_a_group()) {
      return t('grouped');
    }
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }

    $operators = $this->operators();

    $output = $operators[$this->operator]['short'] . ' ';

    $value = $this->value['counterpart'];
    switch ($operators[$this->operator]['values']) {
      case 1:
        $output .= $this->admin_summary_value_format($value['value']['formula'], $this->options['counterpart_field_definition']);
        break;

      case 2:
        $output .= t('@min and @max', array(
          '@min' => $this->admin_summary_value_format($value['min']['formula'], $this->options['counterpart_field_definition']),
          '@max' => $this->admin_summary_value_format($value['max']['formula'], $this->options['counterpart_field_definition']),
        ));
        break;
    }

    return $output;
  }

  /**
   * Determine if a filter can be converted into a group.
   * Only exposed filters with operators available can be converted into groups.
   */
  function can_build_group() {
    // We do not support group for this filter.
    // @todo: implement support for group for this filter.
    return FALSE;
  }

  /*function build_group_validate($form, &$form_state) {
    parent::build_group_validate($form, $form_state);
    // The only thing we have to alter is that parent mistakenly sees our
    // values as empty. So validation of whether title is present and value is
    // missing and vice-versa we have to do on our own.
    if (!empty($form_state['values']['options']['group_info']['group_items'])) {
      foreach ($form_state['values']['options']['group_info']['group_items'] as $id => $group) {
        if (empty($group['remove'])) {

          $is_title_defined = !empty($group['title']);
          $is_value_defined = FALSE;
          if (!is_array($group['value'])) {
            $is_value_defined = trim($group['value']) != '';
          }
          else {
            $is_value_defined = trim($group['value']['base']['formula']) != '';
            if ($is_value_defined) {
              // We also need to check the counterpart, however, this one
              // depends on the operator.
            }
          }
            (is_array($group['value']) && count(array_filter($group['value'], '_views_array_filter_zero')) > 0);

          // Check if the title is defined but value wasn't defined.



          if (!empty($group['title'])) {
            if ((!is_array($group['value']) && trim($group['value']) == "") ||
              (is_array($group['value']) && count(array_filter($group['value'], '_views_array_filter_zero')) == 0)) {
              form_error($form['group_info']['group_items'][$id]['value'],
                t('The value is required if title for this item is defined.'));
            }
          }

          // Check if the value is defined but title wasn't defined.
          if ((!is_array($group['value']) && trim($group['value']) != "") ||
            (is_array($group['value']) && count(array_filter($group['value'], '_views_array_filter_zero')) > 0)) {
            if (empty($group['title'])) {
              form_error($form['group_info']['group_items'][$id]['title'],
                t('The title is required if value for this item is defined.'));
            }
          }
        }
      }
    }
  }*/

  /**
   * Define a set of operators supported by this filter handler.
   *
   * @return array
   *   Array of meta information about all supported operators by this filter
   *   handler
   */
  function operators() {
    $operators = array(
      '<' => array(
        'title' => t('Is less than'),
        'short' => t('<'),
        'values' => 1,
      ),
      '<=' => array(
        'title' => t('Is less than or equal to'),
        'short' => t('<='),
        'values' => 1,
      ),
      '=' => array(
        'title' => t('Is equal to'),
        'short' => t('='),
        'values' => 1,
      ),
      '!=' => array(
        'title' => t('Is not equal to'),
        'short' => t('!='),
        'values' => 1,
      ),
      '>=' => array(
        'title' => t('Is greater than or equal to'),
        'short' => t('>='),
        'values' => 1,
      ),
      '>' => array(
        'title' => t('Is greater than'),
        'short' => t('>'),
        'values' => 1,
      ),
      'between' => array(
        'title' => t('Is between'),
        'short' => t('between'),
        'values' => 2,
      ),
      'not between' => array(
        'title' => t('Is not between'),
        'short' => t('not between'),
        'values' => 2,
      ),
    );

    return $operators;
  }

  /**
   * Generate form element for value form of the filter handler.
   *
   * The generated form element will have formula text field for entering
   * formula and a widget for entering value in the format expected by the type
   * of the supplied Field API field definition array.
   *
   * @param array $form
   *   Part of a form definition array into which the generated form element
   *   should be placed
   * @param array $form_state
   *   Form state array
   * @param array $field
   *   Field API field definition array based on which the form element
   *   will be generated
   * @param array $default_value
   *   Currently saved value, it will used for pulling up the default values in
   *   the form elements
   *
   * @return array
   *   Form API element array
   */
  function value_form_element(&$form, &$form_state, $field, $default_value) {
    $form['formula'] = array(
      '#type' => 'textfield',
      '#title' => t('Formula'),
      '#description' => t('Please, enter here the formula to alter field value before it gets compared to another field. You may use placeholder %placeholder to denote the actual value of field in your formula.', array(
        '%placeholder' => NUMERIC_FIELD_FILTER_VIEWS_FIELD_TOKEN,
      )),
      '#default_value' => $default_value['formula'],
      '#element_validate' => array('_numeric_field_filter_formula_element_validate'),
    );

    $form['field'] = array();

    // If it is a real field and not just a constant.
    if ($field) {
      // We take the 1st instance we come across.
      $entity_type = reset(array_keys($field['bundles']));
      $bundle = reset($field['bundles'][$entity_type]);
      $instance = field_info_instance($entity_type, $field['field_name'], $bundle);

      $form['field'] += numeric_field_filter_converter_method_invoke($field, 'ValueFormElement', $form, $form_state, $field, $instance, $default_value['field']);
    }
  }

  /**
   * Supportive method to format a chunk of $this->admin_summary() output.
   *
   * Sanitize and format the given $formula according to $value argument.
   *
   * @param string $formula
   *   Content of the 'formula' property of value of this filter
   * @param array $field
   *   Field API field definition array of the field that stands for
   *   NUMERIC_FIELD_FILTER_VIEWS_FIELD_TOKEN placeholder in the formula
   *
   * @return string
   *   Sanitized and formatted formula string ready to be placed into admin
   *   summary
   */
  function admin_summary_value_format($formula, $field) {
    if ($field) {
      // If the field is not constant, we substitute the placeholder with the
      // field's name.
      $formula = str_replace(NUMERIC_FIELD_FILTER_VIEWS_FIELD_TOKEN, $field['field_name'], $formula);
    }
    return check_plain($formula);
  }

  /**
   * Supportive function.
   *
   * Provide an #options array of available counterpart fields.
   *
   * @return array
   *   Array of counterpart fields found in the entity type on which filter
   *   is configured to work and on all available relationships
   */
  function counterpart_field_options() {
    $base_tables = array();
    $base_tables[NUMERIC_FIELD_FILTER_VIEWS_FIELD_RELATIONSHIP_BASE] = array(
      'table' => $this->view->base_table,
      'title' => t('Base Table'),
    );

    // Checking available relationships to collect info about extra tables
    // available in this view.
    $relationships = $this->view->display_handler->get_option('relationships');
    foreach ($relationships as $join_field => $relationship) {
      $relationship_handler = views_get_handler($relationship['table'], $relationship['field'], 'relationship');
      // Ignore invalid/broken relationships.
      if (empty($relationship_handler)) {
        continue;
      }
      $base_tables[$join_field] = array(
        'table' => $relationship_handler->definition['base'],
        'title' => $relationship_handler->label(),
      );
    }

    $options = array();
    // Trying to figure out what other numeric fields we have available. In
    // order to do so, we scan one by one available tables in this view trying
    // to look up if this table is a base table of some entity, if it's so, we
    // scan the fields attached to that entity trying to find any other numeric
    // fields.
    foreach ($base_tables as $join_field => $base_table) {
      foreach (entity_get_info() as $entity_type => $entity_info) {
        if ($entity_info['base table'] == $base_table['table']) {
          foreach (field_info_instances($entity_type) as $bundle => $instances) {
            foreach ($instances as $instance) {
              $field = field_info_field($instance['field_name']);
              if (in_array($field['type'], _numeric_field_filter_field_types())) {
                $key = t('@base_table (@entity_type)', array(
                  '@base_table' => $base_table['title'],
                  '@entity_type' => $entity_type,
                ));
                if (!isset($options[$key])) {
                  $options[$key] = array();
                }
                $options[$key][$join_field . '__' . $field['field_name']] = t('@instance (@bundle)', array(
                  '@instance' => $instance['label'],
                  '@bundle' => $bundle,
                ));
              }
            }
          }
        }
      }
    }

    // Lastly adding 'constant' to the options.
    $options = array(NUMERIC_FIELD_FILTER_VIEWS_FIELD_CONSTANT => 'Constant') + $options;

    return $options;
  }

  /**
   * Generate SQL expression based on the supplied parameters.
   *
   * Apart from the supplied parameters method also uses some of the handler
   * object properties of the handler where it is executed.
   */
  function sql_expression_generate($field, $table_alias, $values) {
    $formula = $values['formula'];
    if ($field) {
      // @todo: implement real revision support.
      $revision = FALSE;
      $sql_expression = numeric_field_filter_converter_method_invoke($field, 'fieldConvertToExpression', $field, $table_alias, $values['field'], $revision);
      $sql_expression = str_replace(NUMERIC_FIELD_FILTER_VIEWS_FIELD_TOKEN, $sql_expression, $formula);
      return $sql_expression;
    }
    // It is a constant factor, we are simply supposed to return the formula as
    // it is.
    return $formula;
  }
}
