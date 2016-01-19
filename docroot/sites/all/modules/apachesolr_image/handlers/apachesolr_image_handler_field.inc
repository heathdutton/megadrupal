<?php
/**
 * @file
 * Image field plugin for apachesolr.
 */

class apachesolr_image_handler_field extends apachesolr_views_handler_field {
  // Permisive html.
  function render($values) {
    $value = $this->get_value($values);
    return $this->sanitize_value($value, 'xss_admin');
  }

  // Limit to first item.
  function get_value($values, $field = NULL) {
    $alias = isset($field) ? $this->aliases[$field] : $this->field_alias;
    if ($this->options['limit_first']) {
      if (isset($values->{$alias}) && is_array($values->{$alias})) {
        $values->{$alias} = array_slice($values->{$alias}, 0, 1);
      }
    }
    return parent::get_value($values, $field);
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['limit_first'] = array(
      '#type' => 'checkbox',
      '#title' => t('Show first value only.'),
      '#description' => t('When a multiple field is displayed, limit results to only first value.'),
      '#default_value' => !empty($this->options['limit_first']),
    );
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['limit_first'] = array('default' => FALSE, 'bool' => TRUE);
    return $options;
  }
}
