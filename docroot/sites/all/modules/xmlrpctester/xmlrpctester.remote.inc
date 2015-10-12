<?php
/**
 * @file
 * Remote server testing functions and forms.
 */

/**
 * Form to call the local method with.
 * @param $form_state
 *   Usual form values.
 * @param $method
 *   Fully loaded method array.
 */
function xmlrpctester_remote_call_form($form, $form_state, $method) {
  $server = _xmlrpctester_server();

  $form['description'] = array(
    '#type' => 'markup',
    '#value' => t('The method %method will be called at %server using the following arguments. The output will be displayed as status message. Errors should be caught, but if you are using your own error reporting, then it may not work. Feel free to file a patch.',
      array(
        '%method' => $method->method,
        '%server' => $server,
      )
    ),
  );

  $form['method'] = array(
    '#type' => 'value',
    '#value' => $method->method,
  );

  $form['server'] = array(
    '#type' => 'value',
    '#value' => $server,
  );

  // Remove the return type.
  unset($method->parameters[0]);
  $form['arguments'] = array(
    '#tree'  => TRUE,
    '#type'  => 'fieldset',
    '#title' => t('Method parameters'),
  );

  $descriptions = array(
    'struct'  => t('Separate the members of the structure with comma. It will be converted to a structure. Specify member name - value pairs using :, eg.: name: MyName.'),
    'int'     => t('Enter an integer value.'),
    'string'  => t('Value will be converted string.'),
    'boolean' => t('Enter a value that will be converted to boolean.'),
  );

  if (!is_array($method->parameters)) {
    drupal_set_message(t('Method signature can not be found. Please add this method as a custom call.'), 'error');
  }
  elseif (count($method->parameters)) {
    foreach ($method->parameters as $key => $type) {
      $form['arguments'][$key] = array(
        '#tree' => TRUE,
      );
      $form['arguments'][$key]['value'] = array(
        '#title'          => t('Parameter @num', array('@num' => $key)),
        '#type'           => 'textfield',
        '#description'    => $descriptions[$type],
        '#default_value'  => isset($form_state['post']['arguments'][$key]['value']) ?
          $form_state['post']['arguments'][$key]['value'] : '',
      );
      $form['arguments'][$key]['type'] = array(
        '#type' => 'value',
        '#value' => $type,
      );
    }
  }
  else {
    $form['arguments']['no_arg'] = array(
      '#markup' => t('%method has no parameters defined.', array('%method' => $method->method)),
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send'),
  );
  $form['#redirect'] = FALSE;
  $form['#multistep'] = TRUE; // So we dont lose the form values and stay on the form.
  return $form;
}

/**
 * Submit a remote call.
 */
function xmlrpctester_remote_call_form_submit($form, $form_values) {
  $method = $form_values['values']['method'];
  $arguments = isset($form_values['values']['arguments']) ? $form_values['values']['arguments'] : array();
  $server = $form_values['values']['server'];

  $argument_values = array();

  // Casting the argument to the type the service method reported.
  foreach ($arguments as $key => $argument) {
    $value = $argument['value'];
    if ('struct' == $argument['type']) {
      $type = 'array';
      $raw_structure = explode(',', $value);
      $keyed_structure = array();
      foreach ($raw_structure as $k => $v) {
        if (strpos($v, ':')) {
          list($k, $v) = explode(':', $v);
        }
        $keyed_structure[$k] = $v;
      }
      $value = $keyed_structure;
    }
    else {
      $type = $argument['type'];
      settype($value, $type);
    }

    $argument_values[] = $value;
  }

  // The parameters cant be passed on nicely as an array to xmlrpc(), because
  // it expects a string for the url and the parameters as further arguments.
  // call_user_func_array will convert our parameters array to such a function call.
  $result = xmlrpc($server, array($method => $argument_values));

  if (is_array($result) && isset($result['error'])) {
    drupal_set_message(t('Error: @error', array('@error' => $result['error'])), 'error');
  }
  else {
    $error = xmlrpc_error();
    if (!is_null($error)) {
      if (isset($error->is_error)) {
        drupal_set_message(t('Error: @error', array('@error' => $error->message)), 'error');
        if (-32700 == $error->code) { // Parse error.
          // Lets try to find out whats wrong.
          $remote = drupal_http_request($server);
          $remote_output = filter_xss($remote->data);
          drupal_set_message($remote_output, 'error');
        }
      }
    }
    else {
      drupal_set_message(t('Method successful.'));
      if (!isset($result)) {
        drupal_set_message(t('Method result is empty.'));
      }
      if (module_exists('devel')) {
        dsm($result);
      }
      elseif (is_array($result)) {
        drupal_set_message(t('Method result is an array:'));
        foreach ($result as $key => $item) {
          drupal_set_message(t('@key => @item', array('@key' => $key, '@item' => $item)));
        }
      }
      else {
        drupal_set_message(t('Callback result: @result', array('@result' => $result)));
      }
    }
  }
}
