<?php

/**
 * Field handler to provide simple renderer that turns a URL into a clickable link.
 *
 * @ingroup views_field_handlers
 */
class signup_share_track_views_handler_field_share_link extends views_handler_field {

  function option_definition() {
    $options = parent::option_definition();

    $options['display_as_link'] = array('default' => TRUE);

    return $options;
  }

  /**
   * Provide link to the page being visited.
   */
  function options_form(&$form, &$form_state) {
    $form['display_as_link'] = array(
      '#title' => t('Display as link'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['display_as_link']),
    );
    parent::options_form($form, $form_state);
  }

  function render($values) {
    global $base_url;
    $value = $this->get_value($values);
    if (!empty($this->options['display_as_link'])) {
      return l('join/' . $value, 'join/' . $value, array('html' => TRUE));
    }
    else {
      return $this->sanitize_value($base_url. '/join/' . $value, 'url');
    }
  }

}