<?php
/**
 * @file
 * Rules module integration.
 */

/**
 * Implements hook_rules_action_info().
 */
function btsync_rules_action_info() {
  $actions = array();

  foreach (btsync_methods_get_all() as $method) {
    $actions[$method->method] = array(
      'label' => $method->name,
      'group' => t('BitTorrent Sync API'),
      'base' => 'btsync_rules_action',
      'provides' => array(
        'response' => array(
          'label' => t('Response'),
          'type' => isset($method->return['#list']) && $method->return['#list']
            ? 'list<btsync_' . $method->method . '>'
            : 'btsync_' . $method->method,
        ),
      ),
    );

    // Parameters.
    $parameters = $method->arguments;
    if (!empty($parameters)) {
      $actions[$method->method]['parameter'] = array();
      foreach ($parameters as $key => $parameter) {
        $parameter += array(
          'description' => '',
          'type' => 'text',
          'default value' => '',
          'required' => FALSE,
        );

        $actions[$method->method]['parameter'][$key] = array(
          'label' => $parameter['name'],
          'description' => $parameter['description'],
          'type' => $parameter['type'],
          'default value' => $parameter['default value'],
          'optional' => !$parameter['required'],
        );
      }
    }
  }

  return $actions;
}

/**
 * Implements hook_rules_data_info().
 */
function btsync_rules_data_info() {
  $data = array();

  foreach (btsync_methods_get_all() as $method) {
    $return = $method->return;
    if (!empty($return)) {
      $data['btsync_' . $method->method] = array(
        'label' => t('BitTorrent Sync API: @name response', array('@name' => $method->name)),
        'wrap' => TRUE,
        'property info' => $method->return,
      );
      if (isset($method->return['#list'])) {
        unset($data['btsync_' . $method->method]['property info']['#list']);
      }
    }
  }

  return $data;
}

/**
 * Callback for dynamically generated rules actions.
 */
function btsync_rules_action() {
  list($method, $data, $settings) = _btsync_rules_process(func_get_args());

  $response = btsync_method_callback($method->method, $data);
  if ($response) {
    return array(
      'response' => $response
    );
  }

  // ERROR
}

/**
 * Processes the dynamically generated actions.
 */
function _btsync_rules_process($arguments) {
  $element = $arguments[count($arguments) - 2];
  $method = btsync_methods_get_method($element->getElementName());

  $parameters = $method->arguments;
  $data = array();
  if (!empty($parameters)) {
    foreach (array_keys($parameters) as $parameter) {
      $data[$parameter] = array_shift($arguments);

      // Preprocess the parameters.
      if (method_exists($method, 'preprocessArgument')) {
        $method->preprocessArgument($parameter, $data[$parameter]);
      }

      // Remove optional or empty parameters.
      if ((!isset($parameters[$parameter]['required']) || $parameters[$parameter]['required'] == FALSE) && (!isset($parameters[$parameter]['type']) || 'boolean' != $parameters[$parameter]['type']) && empty($data[$parameter])) {
        unset($data[$parameter]);
      }
    }
  }

  $settings = array_shift($arguments);

  return array($method, $data, $settings);
}
