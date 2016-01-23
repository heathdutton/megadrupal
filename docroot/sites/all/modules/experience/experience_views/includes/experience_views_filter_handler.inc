<?php
/**
 * @file
 * A standard Views filter for a single experience field.
 */

// @codingStandardsIgnoreStart
class experience_views_filter_handler extends views_handler_filter_numeric {

  // Set default values for the experience filter.
  function option_definition() {
    $options = parent::option_definition();
    $options['label_position'] = array('default' => 'above');
    $options['include_fresher'] = array('default' => '0');
    $options['year_start'] = array('default' => '0');
    $options['year_end'] = array('default' => '30');
    return $options;
  }

  /**
  * Set the granularity of the experience parts to use in the filter.
  */
  function has_extra_options() { return TRUE; }

  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    $options = array('above' => t('Above'), 'within' => t('Within'));
    $description = t("The location of experience part labels, like 'Year', 'Month'. 'Above' displays the label as titles above each experience part. 'Within' inserts the label as the first option in the select list.");
    $form['label_position'] = array(
      '#type' => 'radios',
      '#options' => $options,
      '#default_value' => $this->options['label_position'],
      '#title' => t('Position of experience part labels'),
      '#description' => $description,
    );

    $form['include_fresher'] = array(
      '#type' => 'checkbox',
      '#default_value' => $this->options['include_fresher'],
      '#title' => t('Include fresher option'),
    );

    $form['year_start'] = array(
      '#type' => 'select',
      '#options' => range(0, 99),
      '#default_value' => $this->options['year_start'],
      '#title' => t('Starting year'),
    );

    $form['year_end'] = array(
      '#type' => 'select',
      '#options' => range(0, 99),
      '#default_value' => $this->options['year_end'],
      '#title' => t('Ending year'),
    );
  }

  /**
  * Provide a simple experiencefield
  */
  function value_form(&$form, &$form_state) {
    $form['value']['#tree'] = TRUE;

    // We have to make some choices when creating this as an exposed
    // filter form. For example, if the operator is locked and thus
    // not rendered, we can't render dependencies; instead we only
    // render the form items we need.
    // Below section copied from views_handler_filter_numeric.inc.
    $which = 'all';
    $source = '';
    if (!empty($form['operator'])) {
      $source = ($form['operator']['#type'] == 'radios') ? 'radio:options[operator]' : 'edit-options-operator';
    }

    if (!empty($form_state['exposed'])) {
      $identifier = $this->options['expose']['identifier'];

      if (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator_id'])) {
        // exposed and locked.
        $which = in_array($this->operator, $this->operator_values(2)) ? 'minmax' : 'value';
      }
      else {
        $source = 'edit-' . drupal_html_id($this->options['expose']['operator_id']);
      }
    }

    if ($which == 'all' || $which == 'value') {
      $form['value'] += $this->experience_parts_form($form_state, 'value', $source, $which, $this->operator_values(1), $identifier, 'default_experience');
    }

    if ($which == 'all' || $which == 'minmax') {
      $form['value'] += $this->experience_parts_form($form_state, 'min', $source, $which, $this->operator_values(2), $identifier, 'default_experience');
      $form['value'] += $this->experience_parts_form($form_state, 'max', $source, $which, $this->operator_values(2), $identifier, 'default_to_experience');
    }

  }

  /**
  * A form element to select experience part values.
  *
  * @param string $prefix
  *   A prefix for the experience values, 'value', 'min', or 'max' .
  * @param string $source
  *   The operator for this element.
  * @param string $which
  *   Which element to provide, 'all', 'value', or 'minmax' .
  * @param array $operator_values
  *   An array of the allowed operators for this element.
  * @param array $identifier
  *   Identifier of the exposed element.
  * @param array $relative_id
  *   Form element id to use for the relative experience field.
  *
  * @return
  *   The form experience part element for this instance.
  */
  function experience_parts_form(&$form_state, $prefix, $source, $which, $operator_values, $identifier, $relative_id) {
    switch ($prefix) {
      case 'min':
        $label = t('Experience start');
        break;
      case 'max':
        $label = t('Experience end');
        break;
      default:
        $label = '';
        break;
    }

    $type = 'experience-select';

    $year_options = array();
    if ($this->options['include_fresher']) {
      $year_options['fresher'] = t('Fresher');
    }

    $year_start = $this->options['year_start'];
    $year_end = $this->options['year_end'];

    $year_options += range($year_start, $year_end);

    if (!empty($form_state['exposed'])) {
      // UI when the experience selector is exposed.
      $id = 'edit-' . str_replace('_', '-', $this->field) . '-' . $prefix;
      $form[$prefix] = array(
        '#title' => check_plain($label),
        '#type' => $type,
        '#prefix' => '<div id="' . $id . '-wrapper"><div id="' . $id . '-inside-wrapper" class="container-inline-experience">',
        '#suffix' => '</div></div>',
      );
      $form[$prefix]['year'] = array(
        '#type' => 'select',
        '#title' => t('Years'),
        '#options' => $year_options,
        '#empty_option' => '',
        '#attributes' => array('class' => array('year-entry')),
      );
      $form[$prefix]['month'] = array(
        '#type' => 'select',
        '#title' => t('Months'),
        '#options' => range(0, 11),
        '#empty_option' => '',
        '#attributes' => array('class' => array('month-entry')),
      );
      $form[$prefix]['#attached']['js'][] = drupal_get_path('module', 'experience') . '/experience.js';
      $form[$prefix]['#tree'] = TRUE;
      $form[$prefix]['#theme_wrappers'] = array('experience_select');
      if ($this->options['label_position'] == 'within') {
        $form[$prefix]['year']['#empty_option'] = t('-Year');
        $form[$prefix]['month']['#empty_option'] = t('-Month');
        $form[$prefix]['year']['#title_display'] = 'invisible';
        $form[$prefix]['month']['#title_display'] = 'invisible';
      }

      if ($which == 'all') {
        $form[$prefix]['#pre_render'][] = 'ctools_dependent_pre_render';
        $form[$prefix]['#dependency'] = array($source => $operator_values);
      }
    }
    else {
      // UI when the experience selector is on the views configuration screen.
      $id = 'edit-options-value-' . $prefix;
      $form[$prefix . '_group'] = array(
        '#type' => 'fieldset',
        '#attributes' => array('class' => array('experience-views-filter-fieldset')),
      );
      $form[$prefix . '_group'][$prefix] = array(
        '#title' => check_plain($label),
        '#type' => $type,
        '#prefix' => '<div id="' . $id . '-wrapper"><div id="' . $id . '-inside-wrapper" class="container-inline-experience">',
        '#suffix' => '</div></div>',
      );
      $form[$prefix . '_group'][$prefix]['year'] = array(
        '#type' => 'select',
        '#title' => t('Years'),
        '#options' => $year_options,
        '#empty_option' => '',
        '#attributes' => array('class' => array('year-entry')),
      );
      $form[$prefix . '_group'][$prefix]['month'] = array(
        '#type' => 'select',
        '#title' => t('Months'),
        '#options' => range(0, 11),
        '#empty_option' => '',
        '#attributes' => array('class' => array('month-entry')),
      );
      $form[$prefix . '_group'][$prefix]['#attached']['js'][] = drupal_get_path('module', 'experience') . '/experience.js';
      $form[$prefix . '_group'][$prefix]['#tree'] = TRUE;
      $form[$prefix . '_group'][$prefix]['#theme_wrappers'] = array('experience_select');
      if ($this->options['label_position'] == 'within') {
        $form[$prefix . '_group'][$prefix]['year']['#empty_option'] = t('-Year');
        $form[$prefix . '_group'][$prefix]['month']['#empty_option'] = t('-Month');
        $form[$prefix . '_group'][$prefix]['year']['#title_display'] = 'invisible';
        $form[$prefix . '_group'][$prefix]['month']['#title_display'] = 'invisible';
      }

      if ($which == 'all') {
        $form[$prefix . '_group']['#pre_render'][] = 'ctools_dependent_pre_render';
        $form[$prefix . '_group']['#dependency'] = array($source => $operator_values);
      }
    }

    return $form;
  }

  function accept_exposed_input($input) {
    if (empty($this->options['exposed'])) {
      return TRUE;
    }

    // rewrite the input value so that it's in the correct format so that
    // the parent gets the right data.
    if (!empty($this->options['expose']['identifier'])) {
      $value = &$input[$this->options['expose']['identifier']];
      if (!is_array($value)) {
        $value = array(
          'value' => $value,
        );
      }
    }

    $rc = parent::accept_exposed_input($input);

    if (empty($this->options['expose']['required'])) {
      // We have to do some of our own checking for non-required filters.
      $info = $this->operators();
      if (!empty($info[$this->operator]['values'])) {
        switch ($info[$this->operator]['values']) {
          case 1:
            if ($value['value']['year'] === '' && $value['value']['month'] === '') {
              return FALSE;
            }
            break;
          case 2:
            if ($value['min']['year'] === '' && $value['max']['year'] === '' && $value['min']['month'] === '' && $value['max']['month'] === '') {
              return FALSE;
            }
            break;
        }
      }
    }

    return $rc;
  }

  function op_between($field) {
    $min_value = $this->get_filter_value($this->value['min']);
    $max_value = $this->get_filter_value($this->value['max']);

    if ($this->operator == 'between') {
      $this->query->add_where($this->options['group'], $field, array($min_value, $max_value), 'BETWEEN');
    }
    else {
      $this->query->add_where($this->options['group'], db_or()->condition($field, $min_value, '<=')->condition($field, $max_value, '>='));
    }
  }

  function op_simple($field) {
    $value = $this->get_filter_value($this->value['value']);
    $this->query->add_where($this->options['group'], $field, $value, $this->operator);
  }

  function op_regex($field) {
    $value = $this->get_filter_value($this->value['value']);
    $this->query->add_where($this->options['group'], $field, $value, 'RLIKE');
  }

  /**
  * Helper function to see if we need to swap in the default value.
  *
  * Views exposed filters treat everything as submitted, so if it's an empty value we have to
  * see if anything actually was submitted. If nothing has really been submitted, we need
  * to swap in our default value logic.
  */
  function get_filter_value($input) {
    if (!empty($input)) {
      if ($input['year'] == '' && $input['month'] == '') {
        $value = '';
      }
      else {
        if ($input['year'] == 'fresher') {
          $value = 0.001;
        }
        else {
          $year = $input['year'] ? $input['year'] : 0;
          $month = $input['month'] ? $input['month'] : 0;
          if ($month < 10) {
            $month = '0' . $month;
          }
          $value = $year . '.' . $month;
        }
      }
    }
    return $value;
  }
}
// @codingStandardsIgnoreEnd
