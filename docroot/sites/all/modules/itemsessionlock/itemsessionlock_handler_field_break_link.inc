<?php

/**
 * @file
 * Definition of itemsessionlock_handler_field_break_link.
 */

/**
 * Field handler to provide simple renderer that generates item break link.
 *
 * @ingroup views_field_handlers
 */
class itemsessionlock_handler_field_break_link extends views_handler_field {

  /**
   * Override init function to provide generic option to link to node.
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['module'] = 'module';
    $this->additional_fields['type'] = 'type';
    $this->additional_fields['iid'] = 'iid';
  }

  function render($values) {
    $value = $this->get_value($values);
    $actual = array(
      'iid' => $this->get_value($values, 'iid'),
      'module' => $this->get_value($values, 'module'),
      'type' => $this->get_value($values, 'type'),
    );

    return $this->render_link($actual, $values);
  }

  function render_link($data, $values) {
    return l(t('break'), 'itemsessionlock/' . $data['module'] . '/' . $data['type'] . '/break/any/' . $data['iid'], array('query' => array('destination' => current_path())));
  }

}
