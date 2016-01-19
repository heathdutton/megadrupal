<?php

/**
 * @file Provides function to build service description for Guzzle factory and some helper functions
 */

/**
 * Builds object that describes given endpoint and can be encoded to json service description
 */
function services_guzzle_build_documentation_from_internals($endpoint) {
  $resources = services_get_resources_apply_settings($endpoint->name);

  $operations = array();
  foreach ($resources as $resource) {
    $resource_name = $resource['key'];
    $resource_name_camelized = _services_guzzle_camelize($resource_name);
    foreach ($resource as $controller_type => $controllers) {
      if ($controller_type == 'key' || $controller_type == 'endpoint') {
        continue;
      }

      $controller_type_name_camelized = _services_guzzle_camelize($controller_type);

      // Smart removal last "s" form the name. So "Relationships" becomes "Relationship" and "Properties" becomes "Property".
      $controller_type_name_camelized  = preg_replace(array('/ies$/', '/s$/'), array('y', ''), $controller_type_name_camelized);

      foreach ($controllers as $controller_name => $controller) {
        $controller_name_camelized = _services_guzzle_camelize($controller_name);
        $operation_name = $resource_name_camelized . $controller_type_name_camelized . $controller_name_camelized;

        /* @var $guzzle_controller_description ServicesGuzzleControllerDescription */
        $guzzle_controller_description = _services_guzzle_controller_description_factory($controller_type, $resource_name, $controller_name, $controller);

        $controller += array('help' => '');
        $operations[$operation_name] = (object) array(
          'httpMethod' => $guzzle_controller_description->getHttpMethod(),
          'uri' => $guzzle_controller_description->getUri(),
          'summary' => $controller['help'],
        );

        if (isset($controller['args'])) {
          $operations[$operation_name]->parameters = _services_guzzle_extract_arguments($operations[$operation_name]->uri, $controller);
        }
      }
    }
  }

  return (object) array(
    'name' => $endpoint->name,
    'baseUrl' => url($endpoint->path, array('absolute' => TRUE)),
    'operations' => (object) $operations,
  );
}

/**
 * Camelize the string.
 *
 * Example: from abc_def_gh we will get AbcDefGh.
 *
 * @see http://stackoverflow.com/questions/1993721/how-to-convert-camelcase-to-camel-case
 *
 * @return string
 */
function _services_guzzle_camelize($word) {
  return preg_replace('/(^|_)([a-z])/e', 'strtoupper("\\2")', $word);
}

/**
 * Creates instance of ServicesGuzzleControllerDescription abstract class.
 */
function _services_guzzle_controller_description_factory($controller_type, $resource_name, $controller_name, $controller) {
  if ($controller_type == 'actions') {
    $class = 'ServicesGuzzleControllerDescriptionAction';
  }

  if ($controller_type == 'targeted_actions') {
    $class = 'ServicesGuzzleControllerDescriptionTargetedAction';
  }

  if ($controller_type == 'relationships') {
    $class = 'ServicesGuzzleControllerDescriptionRelationship';
  }

  // Controller is "operations" type.
  if ($controller_type == 'operations') {
    $class = 'ServicesGuzzleControllerDescriptionOperation' . ucfirst($controller_name);
  }

  return new $class($resource_name, $controller_name, $controller);
}

/**
 * Convert arguments of controller to operation parameters.
 *
 * @param string $operation_uri
 *   This is URI of the operation. We need to have possibility to change it.
 * @param array $controller
 */
function _services_guzzle_extract_arguments(&$operation_uri, $controller) {
  $parameters = array();
  foreach ($controller['args'] as $controller_argument) {
    $controller_argument_name = $controller_argument['name'];
    $controller_argument += array(
      'optional' => TRUE,
      'description' => '',
    );

    $parameter = array(
      'description' => $controller_argument['description'],
      'required' => !$controller_argument['optional'],
    );

    _services_guzzle_extract_arguments__location($parameter, $operation_uri, $controller_argument, $controller_argument_name);

    _services_guzzle_extract_arguments__type($parameter, $controller_argument);

    $parameters[$controller_argument_name] = (object) $parameter;
  }

  return (object) $parameters;
}

/**
 * Set 'location' property of the parameter.
 *
 * @param $parameter
 * @param $operation_uri
 * @param $controller_argument
 */
function _services_guzzle_extract_arguments__location(&$parameter, &$operation_uri, $controller_argument, $controller_argument_name) {
  $controller_arguments_source = $controller_argument['source'];
  $array_key_exists = function($key, $array) {
    return is_array($array) && array_key_exists($key, $array);
  };

  // path arguments
  if ($array_key_exists('path', $controller_arguments_source)) {
    $parameter['location'] = 'uri';
    $uri_array = explode('/', $operation_uri);
    $argument_uri_array_position = $controller_argument['source']['path'] + 1;
    $uri_array[$argument_uri_array_position] = '{' . $controller_argument_name . '}';
    $operation_uri = implode('/', $uri_array);
  }

  // data arguments
  if ($array_key_exists('data', $controller_arguments_source) || $controller_arguments_source == 'data') {
    $parameter['location'] = 'json';
  }

  // param arguments
  if ($array_key_exists('param', $controller_arguments_source)) {
    $parameter['location'] = 'query';
  }
}

/**
 * Set "type" of parameter.
 *
 * @param $parameter
 * @param $controller_argument
 */
function _services_guzzle_extract_arguments__type(&$parameter, $controller_argument) {
  $parameter_type = '';
  switch ($controller_argument['type']) {
    case 'int':
      $parameter_type = 'integer';
      break;
    case 'string':
      $parameter_type = 'string';
      break;
    case 'object':
      $parameter_type = 'object';
      break;
    case 'array':
      $parameter_type = 'array';
      break;
  }

  if (!empty($parameter_type)) {
    $parameter['type'] = $parameter_type;
  }
}

/**
 * Output json pretty .
 *
 * http://www.daveperrett.com/articles/2008/03/11/format-json-with-php/
 *
 * @param string $json
 *
 * @return string
 */
function _services_guzzle_json_pretty($json) {
  $result      = '';
  $pos         = 0;
  $strLen      = strlen($json);
  $indentStr   = '  ';
  $newLine     = "\n";
  $prevChar    = '';
  $outOfQuotes = TRUE;

  for ($i=0; $i <= $strLen; $i++) {

    // Grab the next character in the string.
    $char = substr($json, $i, 1);

    // Are we inside a quoted string?
    if ($char == '"' && $prevChar != '\\') {
      $outOfQuotes = !$outOfQuotes;

      // If this character is the end of an element,
      // output a new line and indent the next line.
    }
    elseif (($char == '}' || $char == ']') && $outOfQuotes) {
      $result .= $newLine;
      $pos --;
      for ($j=0; $j<$pos; $j++) {
        $result .= $indentStr;
      }
    }

    // Add the character to the result string.
    $result .= $char;

    // If the last character was the beginning of an element,
    // output a new line and indent the next line.
    if (($char == ',' || $char == '{' || $char == '[') && $outOfQuotes) {
      $result .= $newLine;
      if ($char == '{' || $char == '[') {
        $pos ++;
      }

      for ($j = 0; $j < $pos; $j++) {
        $result .= $indentStr;
      }
    }

    $prevChar = $char;
  }

  return $result;
}