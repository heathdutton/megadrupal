<?php

class views_arguments_handler_filter_booleanable_anything extends views_handler_filter_boolean_operator {

  function init(&$view, &$options) {
    parent::init($view, $options);

    // Copied from views_handler_field_boolean //
    $default_formats = array(
      'yes-no' => array(t('Yes'), t('No')),
      'true-false' => array(t('True'), t('False')),
      'on-off' => array(t('On'), t('Off')),
      'enabled-disabled' => array(t('Enabled'), t('Disabled')),
    );
    $custom_format = array('custom' => array(t('Custom')));
    $this->formats = array_merge($default_formats, $custom_format);
    // Copied from views_handler_field_boolean //
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['anythings'] = array('default' => array());

    // Copied from views_handler_field_boolean //
    $options['type'] = array('default' => 'yes-no');
    $options['type_custom_true'] = array('default' => '', 'translatable' => TRUE);
    $options['type_custom_false'] = array('default' => '', 'translatable' => TRUE);
    // Copied from views_handler_field_boolean //

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['anythings'] = array(
      '#type' => 'select',
      '#title' => t('Select handlers'),
      '#options' => $this->_get_handlers($size),
      '#multiple' => TRUE,
      '#size' => $size,
      '#default_value' => $this->options['anythings'],
      '#required' => TRUE,
      '#description' => t('Selected Filters and Arguments will be completely skipped when inactive. Relationships will be made not-required (so they still LEFT JOIN).'),
    );

    // Copied from views_handler_field_boolean //
    $options = array();
    foreach ($this->formats as $key => $item) {
      $options[$key] = implode('/', $item);
    }
    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Boolean format'),
      '#options' => $options,
      '#default_value' => $this->options['type'],
    );
    $form['type_custom_true'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom output for TRUE'),
      '#default_value' => $this->options['type_custom_true'],
      '#states' => array(
        'visible' => array(
          'select[name="options[type]"]' => array('value' => 'custom'),
        ),
      ),
    );
    $form['type_custom_false'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom output for FALSE'),
      '#default_value' => $this->options['type_custom_false'],
      '#states' => array(
        'visible' => array(
          'select[name="options[type]"]' => array('value' => 'custom'),
        ),
      ),
    );
    // Copied from views_handler_field_boolean //
  }

  function get_value_options() {
    if ($this->options['type'] == 'custom') {
      $this->value_options = array(
        '1' => $this->options['type_custom_true'],
        '0' => $this->options['type_custom_false'],
      );
    }
    else {
      $format = @$this->formats[ $this->options['type'] ] ?: $this->formats['yes-no'];
      $this->value_options = array_combine(array('1', '0'), $format);
    }
  }

  function exposed_form(&$form, &$form_state) {
    parent::exposed_form($form, $form_state);

    // This is weirdly the right place:
    // - handlers are ready
    // - exposed input is ready
    // - view & query have not been built
    $this->_pre_build();
  }

  function query() {
    // We're not a real field, so we're explicitly NOT doing anything.
  }

  function admin_summary() {
    if ($this->options['exposed']) {
      if ($this->options['expose']['label']) {
        return $this->options['expose']['label'] . ' (' . count($this->options['anythings']) . ')';
      }
      return 'exposed';
    }

    return '';
  }

  protected function _pre_build() {
    $on = $this->_is_on();

    foreach ($this->options['anythings'] as $id) {
      list($type, $name) = explode(':', $id);
      switch ($type) {
        case 'filters':
          if (!$on) {
            unset($this->view->filter[$name]);
          }
          break;

        case 'arguments':
          if (!$on) {
            unset($this->view->argument[$name]);
          }
          break;

        case 'relationships':
          if (isset($this->view->relationship[$name])) {
            $this->view->relationship[$name]->options['required'] = (int) $on;
          }
          break;
      }
    }
  }

  protected function _get_handlers(&$size = 0) {
    $this->view->init_handlers();
    $handlers = array();
    foreach (array('filters', 'relationships', 'arguments') as $type) {
      $type1 = rtrim($type, 's');
      foreach ($this->view->display_handler->handlers[$type1] as $name => $handler) {
        if (strpos($handler->options['field'], 'booleanable_') === FALSE) {
          $handlers[$type][$type . ':' . $name] = $handler->ui_name();
          $size++;
        }
      }
    }

    $size += count($handlers);
    return $handlers;
  }

  protected function _get_value() {
    $id = $this->options['expose']['identifier'];
    if (isset($this->view->exposed_input[$id])) {
      return $this->view->exposed_input[$id];
    }

    return $this->options['value'];
  }

  protected function _is_on() {
    return $this->_get_value() != '0';
  }

}
