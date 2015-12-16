<?php
/**
 * @file
 * Used to interact with Infusionsoft API.
 */

/**
 * A remote endpoint type for invoking XML-RPC services.
 */
class InfusionsoftXMLRPCEndpoint extends WSClientEndpoint {

  /**
   * @param WSClientServiceDescription $service
   */
  public function __construct(WSClientServiceDescription $service) {
    $this->service = $service;
    $this->url = $service->url;
  }

  /**
   * @return bool
   */
  public function client() {
    return TRUE;
  }

  /**
   * Calls the REST service.
   *
   * @param string $operation_name
   *   The name of the operation to execute.
   * @param array $arguments
   *   Arguments to pass to the service with this operation.
   *
   * @return array|bool|void
   */
  public function call($operation_name, $arguments) {

    // Add API Key as first parameter
    array_unshift($arguments, variable_get('infusionsoft_api_key'));

    if (isset($arguments['options'])) {
      $options = $arguments['options'];
      unset($arguments['options']);
    }
    else {
      $options = array();
    }

    $args = array($operation_name => $arguments);
    $return = xmlrpc($this->url, $args, $options);

    if (xmlrpc_error_msg()) {
      watchdog('infusionsoft', 'There was an error. Error Number: !err.  Message: !errmsg', array('!err' => xmlrpc_errno(), '!errmsg' => xmlrpc_error_msg(), '!url' => $this->url, '!args' => print_r($args, TRUE)));
    }

	if (variable_get('infusionsoft_debug_requests', 0)) {
      watchdog('infusionsoft', 'The following request was sent to Infusionsoft !url: !args | !options', array('!args' => print_r($args, TRUE), '!url' => $this->url, '!options' => print_r($options, TRUE)));
	}

    return $return;
  }
}
