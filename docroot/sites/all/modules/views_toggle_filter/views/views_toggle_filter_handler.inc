<?php

/**
 * @file
 * Definition of views_toggle_filter_handler.
 */

/**
 * Views Toggle Filter handler.
 *
 * @ingroup views_filter_handlers
 */
class views_toggle_filter_handler extends views_handler_filter {

  function get_filter_options() {
    $filters = array();

    foreach ($this->view->display_handler->get_handlers('filter') as $filter_id => $handler) {
      if (($handler->is_exposed()) && ($filter_id !== $this->options['id'])) {
        $filters[$filter_id] = $handler->ui_name(TRUE) . ' ' . $handler->admin_summary();
      }
    }

    return $filters;
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['exposed'] = array('default' => TRUE, 'bool' => TRUE);

    $options['first_name'] = array('default' => '', 'translatable' => TRUE);
    $options['second_name'] = array('default' => '', 'translatable' => TRUE);
    $options['first_filters'] = array('default' => array());
    $options['second_filters'] = array('default' => array());

    return $options;
  }


  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Lock the exposed checkbox.
    $form['expose_button']['checkbox']['checkbox']['#disabled'] = TRUE;
    $form['expose_button']['checkbox']['#attributes']['class'][] = 'element-invisible';

    $form['first_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name of the first state'),
      '#default_value' => $this->options['first_name'],
    );

    $form['second_name'] = array(
      '#type' => 'textfield',
      '#title' => t('Name of the second state'),
      '#default_value' => $this->options['second_name'],
    );

    $filters = $this->get_filter_options();

    if (!empty($filters)) {
      $form['first_filters'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Filters to show in the first state'),
        '#options' => $filters,
        '#default_value' => $this->options['first_filters'],
      );

      $form['second_filters'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Filters to show in the second state'),
        '#options' => $filters,
        '#default_value' => $this->options['second_filters'],
      );
    }
  }

  /**
   * Display the filter on the administrative summary.
   */
  function admin_summary() {
    return t('Toggle filter');
  }

  function expose_form(&$form, &$form_state) {
    $form['#theme'] = 'views_ui_expose_filter_form';
    // #flatten will move everything from $form['expose'][$key] to $form[$key]
    // prior to rendering. That's why the pre_render for it needs to run first,
    // so that when the next pre_render (the one for fieldsets) runs, it gets
    // the flattened data.
    array_unshift($form['#pre_render'], 'views_ui_pre_render_flatten_data');
    $form['expose']['#flatten'] = TRUE;

    $form['expose']['identifier'] = array(
      '#type' => 'textfield',
      '#default_value' => $this->options['expose']['identifier'],
      '#title' => t('Filter identifier'),
      '#size' => 40,
      '#description' => t('This will appear in the URL after the ? to identify this filter. Cannot be blank.'),
      '#fieldset' => 'more',
    );
  }

  /**
   * Add a type selector to the value form
   */
  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    if (!empty($form_state['exposed'])) {

      $options = array(
        0 => $this->options['first_name'],
        1 => $this->options['second_name'],
      );

      $form['value'] = array(
        '#type' => 'radios',
        '#title' => '',
        '#options' => $options,
        '#default_value' => !empty($this->value) ? $this->value : 0,
      );

      $first_filters = array();
      $second_filters = array();

      foreach ($this->view->display_handler->get_handlers('filter') as $filter_id => $handler) {
        if (($handler->is_exposed()) && ($filter_id !== $this->options['id'])) {
          $identifier = $handler->options['expose']['identifier'];
          if (empty($identifier)) {
            continue;
          }
          if (!empty($this->options['first_filters'][$filter_id])) {
            $first_filters[$identifier] = TRUE;
          }
          if (!empty($this->options['second_filters'][$filter_id])) {
            $second_filters[$identifier] = TRUE;
          }
          if ((!empty($first_filters[$identifier])) && (!empty($second_filters[$identifier]))) {
            unset($first_filters[$identifier], $second_filters[$identifier]);
          }
        }
      }

      $form_state['views_toggle_filter'][$this->options['expose']['identifier']] = array(
        0 => $first_filters,
        1 => $second_filters,
      );
      if (!isset($form['#after_build'])) {
        $form['#after_build'] = array();
      }
      if (!in_array('views_toggle_filter_exposed_form_after_build', $form['#after_build'])) {
        $form['#after_build'][] = 'views_toggle_filter_exposed_form_after_build';
      }
    }
  }

  function accept_exposed_input($input) {
    if (!empty($this->options['expose']['identifier'])) {
      if (isset($input[$this->options['expose']['identifier']])) {
        $this->value = $input[$this->options['expose']['identifier']];
      }
    }

    return TRUE;
  }

  function exposed_translate(&$form, $type) {
    // Do not convert radios into select.
  }

  function query() {
    // Do nothing.
  }
}
