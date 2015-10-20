<?php
/**
 * @file
 */

/**
 * Views http headers area plugin.
 */
class views_http_headers_area extends views_handler_area {

  function option_definition() {
    $options = parent::option_definition();
    $options['headers'] = array('default' => '');
    $options['append'] = array('default' => FALSE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['headers'] = array(
      '#type' => 'textarea',
      '#rows' => 5,
      '#title' => t('Headers'),
      '#description' => t('Enter HTTP headers you want to be output when this view is diplayed in header|value format.'),
      '#default_value' => $this->options['headers'],
    );

    $form['append'] = array(
      '#type' => 'checkbox',
      '#title' => t('Append values to existing headers.'),
      '#description' => t('When checked, the header values specified here will be appended to existing headers. When unchecked, headers will be replaced.'),
      '#default_value' => $this->options['append'],
    );
  }

  function render($empty = FALSE) {
    $headers = explode("\n", $this->options['headers']);
    $append = (bool)$this->options['append'];

    foreach ($headers as $header) {
      if (empty($header))
        continue;

      list ($name, $value) = explode('|', $header);

      // Sanitise the headers. Note that though the header name should be sane
      // the header VALUE may contain just about anything.
      $name = check_plain(trim($name));
      $value = trim(strip_tags($value));

      if (empty($name) || empty($value))
        continue;

      drupal_add_http_header($name, $value, $append);
    }
  }
}
