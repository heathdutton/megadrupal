<?php

/**
 * @file
 * Filter class.
 *
 */

/**
 * Combine filters in to a single filter.
 */
class views_filters_combine_handler_filter extends views_handler_filter {
  // Don't display empty space where the operator would be.
  var $no_operator = TRUE;
  var $no_single = TRUE;
  var $always_multiple = TRUE;

  function admin_summary() {
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['filters'] = array('default' => array());

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $filters = $this->view->display_handler->get_handlers('filter');
    foreach ((array) $filters as $id => $filter) {
      if ($id != $this->options['id']) {
        $options[$id] = t($filter->definition['group']) . ':' . t($filter->definition['title']);
      }
    }

    $form['filters'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#title' => t('Filters'),
      '#description' => t('Choose the filters whose value will be replaced by the one specified here.'),
      '#default_value' => $this->options['filters'],
    );

  }

  function query() {
    // Do nothing.
    $filters = (array) $this->view->display_handler->get_handlers('filter');
    foreach (array_filter($this->options['filters']) as $id) {
      $filters[$id]->value = $this->value;
    }
  }

  public function expose_form(&$form, &$form_state) {
    $filters = (array) $this->view->display_handler->get_handlers('filter');
    foreach (array_filter($this->options['filters']) as $id) {
      $filters[$id]->expose_form($form, $form_state);
    }
    $form['expose']['identifier'] = array(
      '#type' => 'textfield',
      '#default_value' => $this->options['expose']['identifier'],
      '#title' => t('Filter identifier'),
      '#size' => 40,
      '#description' => t('This will appear in the URL after the ? to identify this filter. Cannot be blank.'),
      '#fieldset' => 'more',
    );
  }

  public function expose_validate($form, &$form_state) {
    $filters = (array) $this->view->display_handler->get_handlers('filter');
    foreach (array_filter($this->options['filters']) as $id) {
      $filters[$id]->expose_validate($form, $form_state);
    }
  }

  public function expose_submit($form, &$form_state) {
    $filters = (array) $this->view->display_handler->get_handlers('filter');
    foreach (array_filter($this->options['filters']) as $id) {
      $filters[$id]->expose_submit($form, $form_state);
    }
  }

  public function exposed_form(&$form, &$form_state) {
    $filters = (array) $this->view->display_handler->get_handlers('filter');
    $form[$this->options['id']] = array();
    foreach (array_filter($this->options['filters']) as $id) {
      $exposed = $filters[$id]->options['exposed'];
      $filters[$id]->options['exposed'] = TRUE;
      $filters[$id]->exposed_form($form, $form_state);
      $filters[$id]->options['exposed'] = $exposed;
      $form[$this->options['id']] += $form[$id];
      unset($form[$id]);
    }
  }

  public function exposed_validate(&$form, &$form_state) {
    $filters = (array) $this->view->display_handler->get_handlers('filter');
    foreach (array_filter($this->options['filters']) as $id) {
      $filters[$id]->exposed_validate($form, $form_state);
    }
  }

  public function exposed_submit(&$form, &$form_state) {
    $filters = (array) $this->view->display_handler->get_handlers('filter');
    foreach (array_filter($this->options['filters']) as $id) {
      $filters[$id]->exposed_submit($form, $form_state);
    }
  }

  function value_form(&$form, &$form_state) {
    $filters = (array) $this->view->display_handler->get_handlers('filter');
    foreach (array_filter($this->options['filters']) as $id) {
      $filters[$id]->value_form($form, $form_state);
    }
  }

  public function value_validate($form, &$form_state) {
    $filters = (array) $this->view->display_handler->get_handlers('filter');
    foreach (array_filter($this->options['filters']) as $id) {
      $filters[$id]->value_validate($form, $form_state);
    }
  }

  public function value_submit($form, &$form_state) {
    $filters = (array) $this->view->display_handler->get_handlers('filter');
    foreach (array_filter($this->options['filters']) as $id) {
      $filters[$id]->value_submit($form, $form_state);
    }
  }
}
