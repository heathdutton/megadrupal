<?php

class views_string_select_filter extends views_handler_filter_string {

  function option_definition() {
    $options = parent::option_definition();
    $options['string_options'] = array('default' => '');
    $options['string_options_any'] = array('default' => 'Any', 'translatable' => TRUE);
    $options['string_options_widget'] = array('default' => 'select');
    $options['string_options_limit'] = array('default' => FALSE, 'boolean' => TRUE);
    $options['string_options_limit_method'] = array('default' => '');
    $options['string_options_limit_hide_filter'] = array('default' => FALSE, 'boolean' => TRUE);
    // @todo Add caching.
    $options['string_options_limit_cache_time'] = array('default' => 0);
    $options['string_options_limit_cache_granularity'] = array('default' => '');
    $options['string_options_limit_consider_exposed'] = array('default' => FALSE, 'boolean' => TRUE);
    return $options;
  }

  function operators() {
    $operators = parent::operators();
    $operators['in'] = array(
      'title' => t('Is one of'),
      'short' => t('in'),
      'method' => 'op_starts',
      'values' => 1,
    );
    return $operators;
  }

  function can_build_group() {
    // This filter is a group filter itself.
    return FALSE;
  }

  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);

    if (!empty($form_state['exposed'])) {
      // Run time.
      $options = $this->get_string_options();
      if (!empty($options)) {
        $available_options = array_keys($options);
        if ((!empty($this->options['string_options_limit'])) && (!empty($this->options['expose']['identifier']))) {
          $available_options = $this->limit_string_options($options);
          if ((empty($available_options)) && (!empty($this->options['string_options_limit_hide_filter']))) {
            $form['#access'] = FALSE;
          }
        }
        if (isset($form['value'])) {
          $form['value']['#type'] = 'select';
          $form['value']['#views_string_select_filter'] = TRUE;
          $form['value']['#options'] = $options;
          $form['value']['#available_options'] = $available_options;
          unset($form['value']['#size']);
          unset($form['value']['#default_value']);
          $form['value']['#empty_value'] = '';
          $form['value']['#empty_option'] = t('All');
          if ($this->options['expose']['required']) {
            // $form['value']['#default_value'] = reset($options);
          }
        }
      }

      return;
    }

    // Configuration time.
    $form['value'] = array(
      '#type' => 'value',
      '#value' => NULL,
    );
    $form['string_options'] = array(
      '#type' => 'textfield',
      '#title' => t('Variants of the string value'),
      '#size' => 30,
      '#maxlength' => 1000,
      '#default_value' => $this->options['string_options'],
      '#description' => t("Separate variants by '|' character, like this: A|B|C."),
    );

    $form['string_options_any'] = array(
      '#type' => 'textfield',
      '#title' => t('Label for "Any" value'),
      '#size' => 30,
      '#maxlength' => 256,
      '#default_value' => $this->options['string_options_any'],
      '#description' => t('Translatable label for "Any" value.'),
    );

    $form['string_options_widget'] = array(
      '#type' => 'select',
      '#title' => t('String value widget'),
      '#options' => array('select' => t('select'), 'radios' => t('radios')),
      '#default_value' => $this->options['string_options_widget'],
    );

    $form['string_options_limit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Limit variant list by view result'),
      '#default_value' => $this->options['string_options_limit'],
    );

    $form['string_options_limit_method'] = array(
      '#type' => 'select',
      '#title' => t('Method of variant list limiting'),
      '#options' => array('' => t('None'), 'hide' => t('Hide variant'), 'disable' => t('Disable variant')),
      '#default_value' => $this->options['string_options_limit_method'],
      '#dependency' => array(
        'edit-options-string-options-limit' => array(1),
      ),
    );

    $form['string_options_limit_hide_filter'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide filter if variant list is empty'),
      '#default_value' => $this->options['string_options_limit_hide_filter'],
      '#dependency' => array(
        'edit-options-string-options-limit' => array(1),
      ),
    );

    $form['string_options_limit_consider_exposed'] = array(
      '#type' => 'checkbox',
      '#title' => t('Consider current values of exposed filters for variant list limiting'),
      '#default_value' => $this->options['string_options_limit_consider_exposed'],
      '#dependency' => array(
        'edit-options-string-options-limit' => array(1),
      ),
    );

  }

  function expose_form(&$form, &$form_state) {
    parent::expose_form($form, $form_state);
    // Disable operator exposing.
    unset($form['expose']['use_operator'], $form['expose']['operator_label']);
    $form['expose']['operator_id'] = array(
      '#type' => 'value',
      '#value' => '',
    );
  }

  function exposed_translate(&$form, $type) {
    parent::exposed_translate($form, $type);
    if (empty($form['#views_string_select_filter'])) {
      return;
    }
    $form['#type'] = $this->options['string_options_widget'];
    // $form['#default_value'] = '';
    unset($form['#default_value']);
    unset($form['#options']['All']);

    if (!empty($this->options['string_options_limit'])) {
      $available_options = isset($form['#available_options']) ? $form['#available_options'] : $form['#options'];
      $options = $form['#options'];
      if (count($available_options) !== count($options)) {
        $disable = ($this->options['string_options_limit_method'] === 'disable');
        $select = ($form['#type'] === 'select');
        $form['#options'] = array();
        foreach ($options as $key => $value) {
          if (isset($available_options[$key])) {
            $form['#options'][$key] = $value;
            continue;
          }
          if ($disable) {
            if ($select) {
              $form['#options'][$value] = array();
            }
            else {
              $form['#options'][$key] = $value;
              $form[$key] = array('#disabled' => TRUE);
            }
          }
        }
      }
    }

    if ($form['#type'] === 'radios') {
      $any_option = array('' => $this->options['string_options_any']);
      $form['#options'] = $any_option + $form['#options'];
    }
  }

  function get_string_options() {
    $result = array();
    $options = explode('|', $this->options['string_options']);
    foreach ($options as $option) {
      $option = trim($option);
      if (strlen($option) > 0) {
        $result[$option] = $option;
      }
    }
    return $result;
  }

  function limit_string_options($options) {
    if (isset($this->view->views_string_select_filter_clone)) {
      // Prevent recursion.
      return $options;
    }

    if (empty($options)) {
      return $options;
    }

    $identifier = $this->options['expose']['identifier'];
    $exposed_input = array();
    if (!empty($this->options['string_options_limit_consider_exposed'])) {
      $exposed_input = $this->view->get_exposed_input();
      if (is_array($exposed_input)) {
        unset($exposed_input[$identifier]);
      }
    }
    if (empty($exposed_input)) {
      $exposed_input = array('' => '');
    }

    $view = $this->view->clone_view();

    // Prevent recursion.
    $view->views_string_select_filter_clone = TRUE;

    $view->set_arguments($this->view->args);
    $view->set_display($this->view->current_display);

    // Set appropriate exposed filters.
    $view->set_exposed_input($exposed_input);
    $view->exposed_data = $view->exposed_input;

    // Prepare to execute count query.
    $view->build();
    $filter = $view->display_handler->handlers['filter'][$this->options['id']];
    foreach (module_implements('views_pre_execute') as $module) {
      $function = $module . '_views_pre_execute';
      $function($view);
    }
    $query = $view->query;

    $result = array();

    foreach ($options as $key => $value) {
      // Re-apply filter to the query.
      $view->query = clone $query;
      $view->query->view = $view;
      $view->set_exposed_input(array($identifier => $key));
      $view->exposed_data = $view->exposed_input;

      $multiple_exposed_input = array(0 => NULL);
      if ($filter->multiple_exposed_input()) {
        $multiple_exposed_input = $filter->group_multiple_exposed_input($view->exposed_data);
      }
      foreach ($multiple_exposed_input as $group_id) {
        if (!empty($view->exposed_data)) {
          if ($filter->is_a_group()) {
            $converted = $filter->convert_exposed_input($view->exposed_data, $group_id);
            $filter->store_group_input($view->exposed_data, $converted);
            if (!$converted) {
              continue;
            }
          }
          $rc = $filter->accept_exposed_input($view->exposed_data);
          $filter->store_exposed_input($view->exposed_data, $rc);
          if (!$rc) {
            continue;
          }
        }
        $filter->query($view->display_handler->use_group_by());
      }

      $count_query = $view->query->query(TRUE);
      $count_query->addMetaData('view', $view);
      if (empty($view->query->options['disable_sql_rewrite'])) {
        $base_table_data = views_fetch_data($view->query->base_table);
        if (isset($base_table_data['table']['base']['access query tag'])) {
          $access_tag = $base_table_data['table']['base']['access query tag'];
          $count_query->addTag($access_tag);
        }
      }
      try {
        $count_query->preExecute();
        if ($count_query->countQuery()->execute()->fetchField() > 0) {
          $result[$key] = $value;
        }
      }
      catch (Exception $e) {}
    }

    $view->destroy();
    unset($view);

    return $result;
  }

}
