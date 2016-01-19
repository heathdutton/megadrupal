<?php
/**
 * @file
 * Definition of views_handler_filter_raty.
 */

/**
 * Simple filter to handle greater than/less than filters.
 *
 * @ingroup views_filter_handlers
 */
class views_handler_filter_raty extends views_handler_filter_numeric {
  function init(&$view, &$options) {
    parent::init($view, $options);

    // Include the Raty library.
    $raty_path = raty_load_js('input');
    if (!empty($raty_path)) {
      $raty_settings = array('raty_path' => $raty_path, 'score' => 0);
      drupal_add_js(array('raty_input' => $raty_settings), 'setting');
    }
  }

  /**
   * Extends views_handler_filter option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['raty_settings'] = array(
      'raty_star_count' => 5,
    );

    return $options;
  }

  /**
   * Extends views_handler_filter options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $raty_options = empty($this->options['raty_settings']) ? array() : $this->options['raty_settings'];
    if (empty($raty_options)) {
      $options = $this->option_definition();
      $raty_options = $options['raty_settings'];
    }

    $form['raty_settings'] = array(
      '#type' => 'fieldset',
      '#title' => t('Raty Settings'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#weight' => 50,
      '#tree' => TRUE,
    );
    // Maximum stars value.
    $form['raty_settings']['raty_star_count'] = array(
      '#type' => 'textfield',
      '#title' => t('Star count (max)'),
      '#fieldset' => 'raty_settings',
      '#default_value' => $raty_options['raty_star_count'],
    );

  }

  /**
   * Extends views_handler_filter value_form().
   */
  function value_form(&$form, &$form_state) {
    $form['value']['#tree'] = TRUE;

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

      if (empty($this->options['expose']['use_operator']) || empty($this->options['expose']['operator_id'])) {
        // exposed and locked.
        $which = in_array($this->operator, $this->operator_values(2)) ? 'minmax' : 'value';
      }
      else {
        $source = 'edit-' . drupal_html_id($this->options['expose']['operator_id']);
      }
    }

    $raty_label = empty($this->options['expose']['label']) ? t('Label here') :
      $this->options['expose']['label'];
    $raty_identifier = empty($this->options['expose']['identifier']) ?
      $this->options['id'] : $this->options['expose']['identifier'];

    if ($which == 'all') {
      $form['value']['value'] = array(
        '#type' => 'textfield',
        '#size' => 3,
        '#default_value' => $this->value['value'],
        '#attributes' => array(
          'class' => array($raty_identifier),
          'style' => array('float: left;'),
        ),
        '#dependency' => array($source => $this->operator_values(1)),
        '#views_filter_data' => array(
          'type' => 'single',
          'title' => $raty_label,
          'identifier' => $raty_identifier,
          'raty_settings' => $this->options['raty_settings'],
        ),
        '#views_admin' => TRUE,
        '#theme_wrappers' => array('raty_views_filter'),
      );
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier]['value'])) {
        $form_state['input'][$identifier]['value'] = $this->value['value'];
      }
    }
    elseif ($which == 'value') {
      // When exposed we drop the value-value and just do value if
      // the operator is locked.
      $form['value'] = array(
        '#type' => 'textfield',
        //'#title' => empty($form_state['exposed']) ? t('Value') : '',
        '#size' => 3,
        '#default_value' => $this->value['value'],
        '#attributes' => array(
          'class' => array($raty_identifier),
        ),
        '#views_filter_data' => array(
          'type' => 'single',
          'title' => empty($form_state['exposed']) ? t('Value') : '',
          'identifier' => $raty_identifier,
          'raty_settings' => $this->options['raty_settings'],
        ),
        '#theme_wrappers' => array('raty_views_filter'),
      );
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier])) {
        $form_state['input'][$identifier] = $this->value['value'];
      }
    }

    if ($which == 'all' || $which == 'minmax') {
      $form['value']['min'] = array(
        '#type' => 'textfield',
        //'#title' => empty($form_state['exposed']) ? t('Min') : '',
        '#size' => 3,
        '#default_value' => $this->value['min'],
        '#attributes' => array(
          'class' => array($raty_identifier . '-min'),
        ),
        '#views_filter_data' => array(
          'type' => 'min',
          'title' => empty($form_state['exposed']) ? t('Min') : '',
          'identifier' => $raty_identifier . '-min',
          'raty_settings' => $this->options['raty_settings'],
        ),
        '#theme_wrappers' => array('raty_views_filter'),
      );
      $form['value']['max'] = array(
        '#type' => 'textfield',
        //'#title' => empty($form_state['exposed']) ? t('And max') : t('And'),
        '#size' => 3,
        '#default_value' => $this->value['max'],
        '#attributes' => array(
          'class' => array($raty_identifier . '-max'),
        ),
        '#views_filter_data' => array(
          'type' => 'max',
          'title' => empty($form_state['exposed']) ? t('And max') : t('And'),
          'identifier' => $raty_identifier . '-max',
          'raty_settings' => $this->options['raty_settings'],
        ),
        '#theme_wrappers' => array('raty_views_filter'),
      );
      if ($which == 'all') {
        $dependency = array(
          '#dependency' => array($source => $this->operator_values(2)),
        );
        $form['value']['min'] += $dependency;
        $form['value']['max'] += $dependency;
      }
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier]['min'])) {
        $form_state['input'][$identifier]['min'] = $this->value['min'];
      }
      if (!empty($form_state['exposed']) && !isset($form_state['input'][$identifier]['max'])) {
        $form_state['input'][$identifier]['max'] = $this->value['max'];
      }

      if (!isset($form['value'])) {
        // Ensure there is something in the 'value'.
        $form['value'] = array(
          '#type' => 'value',
          '#value' => NULL
        );
      }
    }
  }

  /**
   * Do some minor translation of the exposed input
   */
  function accept_exposed_input($input) {
    if (empty($this->options['exposed'])) {
      return TRUE;
    }

    // Rewrite the input value so that it's in the correct format so that
    // the parent gets the right data.
    if (!empty($this->options['expose']['identifier'])) {
      $value = &$input[$this->options['expose']['identifier']];
      if (!is_array($value)) {
        $value = array(
          'value' => $value,
        );
        $raty_settings = array('score' => $value['value']);
      }
      else {
        $raty_settings = array('min_score' => $value['min'], 'max_score' => $value['max']);
      }
    }

    drupal_add_js(array('raty_views' => $raty_settings), 'setting');

    $rc = parent::accept_exposed_input($input);

    if (empty($this->options['expose']['required'])) {
      // We have to do some of our own checking for non-required filters.
      $info = $this->operators();
      if (!empty($info[$this->operator]['values'])) {
        switch ($info[$this->operator]['values']) {
          case 1:
            if ($value['value'] === '') {
              return FALSE;
            }
            break;
          case 2:
            if ($value['min'] === '' && $value['max'] === '') {
              return FALSE;
            }
            break;
        }
      }
    }

    return $rc;
  }
}
