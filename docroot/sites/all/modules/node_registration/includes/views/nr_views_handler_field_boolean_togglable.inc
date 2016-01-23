<?php

class nr_views_handler_field_boolean_togglable extends views_handler_field_boolean {

  /**
   * Override views_handler_field_boolean::option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['togglable'] = array('default' => 0);

    return $options;
  }

  /**
   * Override views_handler_field_boolean::options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['togglable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Togglable'),
      '#default_value' => $this->options['togglable'],
      '#description' => t('Make this flag togglable by people with event node edit permission. The label will become a link.')
    );
  }

  /**
   * Override views_handler_field_boolean::render().
   */
  function render($values) {
    $html = parent::render($values);

    if ($this->options['togglable']) {
      $rid = $values->registration_id;
      $registration = node_registration_load($rid);
      $node = node_load($registration->nid);

      if (node_access('update', $node) && node_registration_access($registration, 'update')) {
        drupal_add_library('system', 'drupal.ajax');

        $new_value = (int) !$this->get_value($values);
        $token = drupal_get_token("nr_toggle_{$this->field}_{$new_value}");
        $options = array(
          'query' => drupal_get_destination() + array('token' => $token),
          'attributes' => array('class' => array('use-ajax')),
        );
        $html = l($html, 'registration/' . $rid . '/toggle/nojs/' . $this->field . '/' . $new_value, $options);
      }
    }

    return $html;
  }

}
