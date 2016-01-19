<?php
/**
 * @file
 */

/**
 * Views area form input field handler.
 */
class views_filterfield extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();
    $options['views_filterfield_field'] = array('default' => FALSE);
    $options['views_filterfield_separator'] = array('default' => '+');
    $options['views_filterfield_case'] = array('default' => 0);
    $options['views_filterfield_transform_dash'] = array('default' => 0);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $filters = $this->get_filter();
    foreach ($filters as $filter => $handler) {
      $title = (empty($handler->options['ui_name']))
        ? t('@group: @title', array('@group' => $handler->definition['group'], '@title' => $handler->definition['title']))
        : $handler->options['ui_name'];
        $options[$filter] = check_plain($title);
    }

    $form['views_filterfield_field'] = array(
      '#type' => 'select',
      '#title' => t('Filter'),
      '#description' => t('Select the filter whose value you want to display.'),
      '#options' => $options,
      '#default_value' => $this->options['views_filterfield_field'],
      '#required' => TRUE,
    );

    $form['views_filterfield_separator'] = array(
      '#type' => 'radios',
      '#title' => t('Multiple value display'),
      '#description' => t('How should multiple values be displayed? This can affect how a value is passed to an embedded view.'),
      '#options' => array(
        '+' => t('1+2+3 (for OR)'),
        ',' => t('1,2,3 (for AND)'),
      ),
      '#default_value' => $this->options['views_filterfield_separator'],
      '#required' => TRUE,
    );

    // The options array here is in a weird order so that the list matches
    // what Views uses and the 6 users who had already downloaded this module
    // don't suddenly get the wrong transform.
    $form['views_filterfield_case'] = array(
      '#type' => 'radios',
      '#title' => t('Text case'),
      '#description' => t('Do you want to change the text case of the generated filter string?'),
      '#options' => array(
        0 => t('No transform'),
        3 => t('Upper case'),
        1 => t('Lower case'),
        4 => t('Capitalize first letter'),
        2 => t('Capitalize each word'),
      ),
      '#default_value' => $this->options['views_filterfield_case'],
      '#required' => TRUE,
    );

    $form['views_filterfield_transform_dash'] = array(
      '#type' => 'checkbox',
      '#title' => t('Transform spaces to dashes'),
      '#value' => 1,
      '#default_value' => $this->options['views_filterfield_transform_dash'],
      '#required' => FALSE,
    );
  }

  /**
   * Implements views_handler_field#query().
   */
  function query() {
    $this->field_alias = 'views_filterfield_' . $this->position;
  }

  /**
   * Implements views_handler_field#render().
   */
  function render($values) {
    $output = array('all');

    $filter = $this->get_filter($this->options['views_filterfield_field']);

    if (!empty($this->view->exposed_input[$filter['expose']['identifier']])) {
      if (is_array($this->view->exposed_input[$filter['expose']['identifier']])) {
        $output = $this->view->exposed_input[$filter['expose']['identifier']];
      }
      else {
        // Turn a single value into an array, so the transform array_walk
        // function works.
        $output = array($this->view->exposed_input[$filter['expose']['identifier']]);
      }
    }

    // Lots of create_function() calls next, as array_walk does not generally
    // play nice with internal functions, as it passes too many args. These
    // will simply throw a warning and not work.
    // @see http://php.net/manual/en/function.array-walk.php

    // Transform spaces to dashes.
    if (!empty($options['views_filterfield_transform_dash'])) {
      array_walk($output, create_function('&$val', '$val = strtr($val, array(" " => "-"));'));
    }

    // Transform case as needed by walking the array of values.
    switch ($this->options['views_filterfield_case']) {
      case 1: // Lower case.
        array_walk($output, create_function('&$val', '$val = drupal_strtolower($val);'));
        break;

      case 2: // Title case.
        array_walk($output, create_function('&$val', '$val = ucwords($val);'));
        break;

      case 3: // Upper case.
        array_walk($output, create_function('&$val', '$val = drupal_strtoupper($val);'));
        break;

      case 4: // Sentence case.
        array_walk($output, create_function('&$val', '$val = drupal_ucfirst($val);'));
        break;
    }

    // Turn the transformed array values into a delimited string.
    $separator = $this->options['views_filterfield_separator'];
    $output = implode($separator, $output);

    // Do some basic sanity checking. We don't want crazy values, do we?
    return filter_xss($output);
  }

  /**
   * Helper to fetch the selected filter (or all filters).
   */
  function get_filter($filter = NULL) {
    $filters = &drupal_static(__FUNCTION__);

    if (!isset($filters[$filter]) || empty($filters)) {
      $filters = $this->view->display_handler->get_option('filters');
    }
    return (!empty($filter)) ? $filters[$filter] : $filters;
  }
}
