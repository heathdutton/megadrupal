<?php

class views_handler_area_system_message extends views_handler_area {

  function option_definition() {
    $options = parent::option_definition();
    $options['class'] = array('default' => '');
    $options['type'] = array('default' => 'status');
    $options['text'] = array('default' => '', 'translatable' => TRUE, 'format_key' => 'format');
    $options['format'] = array('default' => 'plain_text');

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['class'] = array(
      '#type' => 'textfield',
      '#title' => t('Message CSS class'),
      '#default_value' => $this->options['class'],
      '#description' => t('A custom CSS class to add to the message container.'),
    );
    $form['type'] = array(
      '#type' => 'select',
      '#title' => t('Type'),
      '#options' => array(
        'status' => t('Status'),
        'warning' => t('Warning'),
        'error' => t('Error'),
      ),
      '#default_value' => $this->options['type'],
      '#description' => t('Type of system message.'),
    );

    $form['text'] = array(
      '#type' => 'text_format',
      '#title' => t('Message'),
      '#default_value' => $this->options['text'],
      '#rows' => 3,
      '#format' => isset($this->options['format']) ? $this->options['format'] : filter_default_format(),
    );

    // Get a list of the available arguments for token replacement.
    $count = 0; // This lets us prepare the key as we want it printed.
    foreach ($this->view->display_handler->get_handlers('argument') as $arg => $handler) {
      $options[t('Arguments')]['%' . ++$count] = t('@argument title', array('@argument' => $handler->ui_name()));
      $options[t('Arguments')]['!' . $count] = t('@argument input', array('@argument' => $handler->ui_name()));
    }

    if (!empty($options)) {
      foreach (array_keys($options) as $type) {
        if (!empty($options[$type])) {
          $items = array();
          foreach ($options[$type] as $key => $value) {
            $items[] = $key . ' == ' . $value;
          }
          $output .= theme('item_list', array(
            'items' => $items,
            'title' => $type,
          ));
        }
      }

      $form['token_help'] = array(
        '#type' => 'fieldset',
        '#title' => t('Replacement patterns'),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#value' => $output,
      );
    }
  }

  function options_submit(&$form, &$form_state) {
    $form_state['values']['options']['format'] = $form_state['values']['options']['text']['format'];
    $form_state['values']['options']['text'] = $form_state['values']['options']['text']['value'];
    parent::options_submit($form, $form_state);
  }

  function render($empty = FALSE) {
    if (!$empty || !empty($this->options['empty'])) {

      $type = $this->options['type'];
      $message = $this->options['text'];

      if (!empty($this->options['class'])) {
        $class = ' ' . $this->options['class'];
      }
      else {
        $class = NULL;
      }

      // Tokenize
      if (!empty($class)) {
        $class = $this->view->style_plugin->tokenize_value($class, 0);
      }
      if (!empty($message)) {
        $format = isset($this->options['format']) ? $this->options['format'] : filter_default_format();
        $message = check_markup($this->view->style_plugin->tokenize_value($message, 0), $format);
      }

      $message = array(
        'type' => $type,
        'class' => $class,
        'message' => $message,
      );
      return theme('views_system_message', $message);
    }
  }
}
