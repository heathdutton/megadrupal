<?php

/**
 * Field handler which allows to render the job message with replaced variables.
 *
 * @ingroup views_field_handlers
 */
class queue_runner_handler_field_log_message extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['variables'] = 'variables';
    $this->additional_fields['wid'] = 'wid';
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['format'] = array('default' => 'link');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['format'] = array(
      '#type' => 'select',
      '#title' => t('Format'),
      '#description' => t("Choose whether the field should display the raw text or display formatted text with replaced variables with it's values."),
      '#default_value' => $this->options['format'],
      '#options' => array(
        'link' => t('Link to full log event'),
        'formatted' => t('Formatted'),
        'raw' => t('Raw'),
      ),
    );
  }

  function render($values) {
    if ($message = $this->get_value($values)) {
      if ($this->options['format'] == 'raw') {
        $output = $this->sanitize_value($message, 'xss');
      }
      else {
        $variables = unserialize($this->get_value($values, 'variables'));
        $message = t($this->sanitize_value($message, 'xss'), $variables);
        if ($this->options['format'] == 'link') {
          $wid = $this->get_value($values, 'wid');
          // Truncate message to 56 chars like theme_dblog_message().
          $output = truncate_utf8(filter_xss($message, array()), 56, TRUE, TRUE);
          $output = l($output, 'admin/reports/queue-runner-event/' . $wid, array('html' => TRUE));
        }
        else {
          // Formatted output.
          $output = $message;
        }
      }
      return $output;
    }
  }
}

