<?php

/**
 * @file
 * Helper functions file for Entity Documentation Confluence module.
 */

/**
 * Exports the documentation to Confluence.
 *
 * Handles the retrieve, create and delete actions between drupal and
 * Confluence restful API.
 * This function runs, only if the required administration settings saved
 * successfully.
 * Handles two possible scenarios during the request for new bundle page
 * creation.
 * 1) If the requested page not exists inside confluence pages then creates it.
 * 2) If the requested page already exist then first deletes it and then create
 * it again.
 *
 * @param string $entity
 *   The entity type that will be documented. For example 'node'.
 * @param string $bundle
 *   The bundle name. For example 'article'.
 * @param array $variable
 *   An associative array containing:
 *   - parent_id: The confluence page parent id.
 *   - space_key: The confluence space key.
 *   - url: The url to make the request.
 *   - encoded: (optional) base64_encode data for authorized requests.
 *
 * @return bool|int
 *   Returns the status code '200' if everything went as expected, otherwise
 *   returns FALSE.
 */
function entity_documentation_confluence_export_documentation($entity, $bundle, array $variable) {

  $status = FALSE;

  // First Check if the entity parent page exist, to find out what to do next.
  $title = _edc_create_page_title($entity);
  $entity_parent_response = entity_documentation_confluence_page_exist_by_title($entity, $bundle, $variable, $title);

  if (is_numeric($entity_parent_response)) {
    // This means that something went wrong and the response code is not the
    // expected one.
    return $entity_parent_response;
  }
  else {

    switch (count($entity_parent_response->results)) {

      // There is not an entity parent page in confluence.
      case '0':

        // If any, we need to unset the stored entity parent page id.
        $temp_variable = _edc_unstore_entity_parent_id($entity, $variable);

        $response = entity_documentation_confluence_create_and_store_entity_parent_page($entity, $bundle, $temp_variable);
        $updated_variable = $response['variable'];

        // Check if bundle page exist.
        $title = _edc_create_page_title($bundle);
        $bundle_response = entity_documentation_confluence_page_exist_by_title($entity, $bundle, $updated_variable, $title);

        if (count($bundle_response->results) == '0') {
          // Then create the bundle page.
          $page = entity_documentation_confluence_create_confluence_page($entity, $bundle, $updated_variable);
          $status = isset($page->id) ? '200' : FALSE;
        }
        else {
          // First delete the existing page and then create the updated one..
          entity_documentation_confluence_delete_confluence_page($entity, $bundle, $updated_variable, $bundle_response->results[0]->id);

          // Then create the bundle page.
          $page = entity_documentation_confluence_create_confluence_page($entity, $bundle, $updated_variable);
          $status = isset($page->id) ? '200' : FALSE;
        }
        break;

      // The bundle page exist, so we need to update it.
      case '1':
        // Store the entity parent page id.
        $entity_parent_id = $entity_parent_response->results[0]->id;
        $updated_variable = _edc_store_entity_parent_id($entity, $variable, $entity_parent_id);

        // Check if bundle page exist.
        $title = _edc_create_page_title($bundle);
        $bundle_response = entity_documentation_confluence_page_exist_by_title($entity, $bundle, $updated_variable, $title);

        if (count($bundle_response->results) == '0') {
          // Then create the bundle page.
          $page = entity_documentation_confluence_create_confluence_page($entity, $bundle, $updated_variable);
          $status = isset($page->id) ? '200' : FALSE;
        }
        else {
          // First delete the existing page and then create the updated one..
          entity_documentation_confluence_delete_confluence_page($entity, $bundle, $updated_variable, $bundle_response->results[0]->id);

          // Then create the bundle page.
          $page = entity_documentation_confluence_create_confluence_page($entity, $bundle, $updated_variable);
          $status = isset($page->id) ? '200' : FALSE;
        }
        break;

    }
  }

  return $status;
}

/**
 * Creates confluence entity parent page, and stores this page id.
 *
 * @param string $entity
 *   The entity type that will be documented. For example 'node'.
 * @param string $bundle
 *   The bundle name. For example 'article'.
 * @param array $variable
 *   An associative array containing:
 *   - parent_id: The confluence page parent id.
 *   - space_key: The confluence space key.
 *   - url: The url to make the request.
 *   - encoded: (optional) base64_encode data for authorized requests.
 *
 * @return array
 *   The response and the updated variable array.
 */
function entity_documentation_confluence_create_and_store_entity_parent_page($entity, $bundle, array $variable) {
  $extras['page_id'] = $variable['parent_id'];
  // Create confluence entity parent page.
  $values['response'] = entity_documentation_confluence_create_request('add', $entity, $bundle, $variable, $extras);

  // Update the storage.
  $values['variable'] = _edc_store_entity_parent_id($entity, $variable, $values['response']->id);

  return $values;
}

/**
 * Create confluence page.
 *
 * @param string $entity
 *   The entity type that will be documented. For example 'node'.
 * @param string $bundle
 *   The bundle name. For example 'article'.
 * @param array $variable
 *   An associative array containing:
 *   - parent_id: The confluence page parent id.
 *   - space_key: The confluence space key.
 *   - url: The url to make the request.
 *   - encoded: (optional) base64_encode data for authorized requests.
 *
 * @return array
 *   The response array.
 */
function entity_documentation_confluence_create_confluence_page($entity, $bundle, array $variable) {
  $extras['page_id'] = $variable[$entity . '_parent_id'];
  // Every new confluence page will be created under an entity parent page.
  return entity_documentation_confluence_create_request('add', $entity, $bundle, $variable, $extras);
}

/**
 * Delete confluence page by page id.
 *
 * @param string $entity
 *   The entity type that will be documented. For example 'node'.
 * @param string $bundle
 *   The bundle name. For example 'article'.
 * @param array $variable
 *   An associative array containing:
 *   - parent_id: The confluence page parent id.
 *   - space_key: The confluence space key.
 *   - url: The url to make the request.
 *   - encoded: (optional) base64_encode data for authorized requests.
 * @param int $page_id
 *   Page id for deletion.
 *
 * @return array
 *   The response array.
 */
function entity_documentation_confluence_delete_confluence_page($entity, $bundle, array $variable, $page_id) {
  $extras['page_id'] = $page_id;
  // Delete request for the given page id.
  return entity_documentation_confluence_create_request('delete', $entity, $bundle, $variable, $extras);
}

/**
 * Function wrapper that makes the http request.
 *
 * @param string $method
 *   The request method can be 'retrieve', 'add' or 'delete'.
 * @param string $entity
 *   The entity type that will be documented. For example 'node'.
 * @param string $bundle
 *   The bundle name. For example 'article'.
 * @param array $variable
 *   An associative array containing:
 *   - parent_id: The confluence page parent id.
 *   - space_key: The confluence space key.
 *   - url: The url to make the request.
 *   - encoded: (optional) base64_encode data for authorized requests.
 * @param array $extras
 *   Is an associative array containing one of:
 *   - page_id: (optional) The page id.
 *   - page_title: (optional). The page title.
 *
 * @return array|int
 *   Returns the decoded response data array or the response code status.
 */
function entity_documentation_confluence_create_request($method, $entity, $bundle, array $variable, array $extras) {

  // Get the http request arguments and make the request.
  $arguments = entity_documentation_confluence_get_http_arguments($method, $entity, $bundle, $variable, $extras);
  $response = drupal_http_request($arguments['uri'], $arguments['options']);

  // Based on the response return the appropriate values.
  if ($response->code == '400') {
    watchdog('entity_documentation_confluence', 'Bad request.');
    return $response->code;
  }
  elseif ($response->code != '200' && $response->code != '204') {
    watchdog('entity_documentation_confluence', 'Http request failed. Response data @data',
      array('@data' => isset($response->data) ? $response->data : $response->error), WATCHDOG_ERROR, NULL);
    return $response->code;
  }
  else {
    $decoded = json_decode($response->data);
    return $decoded;
  }
}

/**
 * Function that generates the http request arguments.
 *
 * Based on the selected $method this function uses the module's form settings
 * to built the http arguments for the request. Builds either Authorized
 * requests or non Authorized requests automatically. Also uses json format
 * for the request body value.
 *
 * @param string $method
 *   The request method can be 'retrieve', 'add' or 'delete'.
 * @param string $entity
 *   The entity type that will be documented. For example 'node'.
 * @param string $bundle
 *   The bundle name. For example 'article'.
 * @param array $variable
 *   An associative array containing:
 *   - parent_id: The confluence page parent id.
 *   - space_key: The confluence space key.
 *   - url: The url to make the request.
 *   - encoded: (optional) base64_encode data for authorized requests.
 * @param array $extras
 *   Is an associative array containing one of:
 *   - page_id: (optional) The page id.
 *   - page_title: (optional). The page title.
 *
 * @return array
 *   Returns the http request arguments as an array.
 */
function entity_documentation_confluence_get_http_arguments($method, $entity, $bundle, array $variable, array $extras) {

  switch ($method) {
    case 'retrieve_by_id':
      // The retrieve request for a confluence page can be either by a page id
      // or by a page title. This separation is depending on the $page_id value.
      if (isset($extras['page_id'])) {
        // Retrieve page by page id.
        $uri = url($variable['url'] . '/rest/api/content/' . $extras['page_id'] . '?type=page&spaceKey=' . $variable['space_key'] . '&expand=ancestors');

        $options = array(
          'method' => 'GET',
          'headers' => array(),
        );

        if (isset($variable['encoded'])) {
          // Request Headers.
          $options['headers'] = array(
            'Authorization' => 'Basic ' . $variable['encoded'],
            'os_authType' => 'Basic',
          );
        }
      }
      else {
        watchdog('entity_documentation_confluence', 'Page id is required for the retrieve request.
        See function entity_documentation_confluence_get_http_arguments().');
        return FALSE;
      }
      break;

    case 'retrieve_by_title':
      // The retrieve request for a confluence page by page title.
      if (isset($extras['page_title'])) {
        $uri = url($variable['url'] . '/rest/api/content?type=page&spaceKey=' . $variable['space_key'] . '&title=' . $extras['page_title'] . '&expand=ancestors');

        $options = array(
          'method' => 'GET',
          'headers' => array(),
        );

        if (isset($variable['encoded'])) {
          // Request Headers.
          $options['headers'] = array(
            'Authorization' => 'Basic ' . $variable['encoded'],
            'os_authType' => 'Basic',
          );
        }
      }
      else {
        watchdog('entity_documentation_confluence', 'Page title is required for the retrieve request.
        See function entity_documentation_confluence_get_http_arguments().');
        return FALSE;
      }
      break;

    case 'add':
      $uri = url($variable['url'] . '/rest/api/content');
      // Get json data.
      $json = _entity_documentation_confluence_get_json_data($entity, $bundle, $variable);

      $options = array(
        'method' => 'POST',
        'data' => $json,
        'headers' => array(),
      );

      // Header without Authorization.
      $options['headers'] = array(
        'Accept' => 'application/json',
        'Content-Type' => 'application/json',
        'dataType' => 'json',
        'spaceKey' => $variable['space_key'],
      );

      // Request Headers with Authorization.
      if (isset($variable['encoded'])) {
        $options['headers']['Authorization'] = 'Basic ' . $variable['encoded'];
        $options['headers']['os_authType'] = 'Basic';
      }
      break;

    case 'delete':
      // The delete request requires a $page_id value.
      $options = array(
        'method' => 'DELETE',
        'headers' => array(),
      );

      // Header without Authorization.
      $options['headers'] = array(
        'Content-Type' => 'application/json',
        'spaceKey' => $variable['space_key'],
      );

      // Request Headers with Authorization.
      if (isset($variable['encoded'])) {
        $options['headers']['Authorization'] = 'Basic ' . $variable['encoded'];
        $options['headers']['os_authType'] = 'Basic';
      }

      $uri = url($variable['url'] . '/rest/api/content/' . $extras['page_id']);
      break;

    default:
      // The selected method is not valid.
      watchdog('entity_documentation_confluence', 'Invalid method @method in function entity_documentation_confluence_get_http_arguments().',
        array('@method' => $method), WATCHDOG_ERROR, NULL);
      return FALSE;
  }

  // Arguments to return.
  $arguments = array(
    'uri' => $uri,
    'options' => $options,
  );

  // Creating hook to alter the request arguments will call all modules
  // implementing hook_edc_http_arguments_alter.
  drupal_alter('edc_http_arguments_alter', $arguments);

  return $arguments;
}

/**
 * Creates the json data for the request body.
 *
 * @param string $entity
 *   The entity type that will be documented. For example 'node'.
 * @param string $bundle
 *   The bundle name. For example 'article'.
 * @param array $variable
 *   An associative array containing:
 *   - parent_id: The confluence page parent id.
 *   - space_key: The confluence space key.
 *   - url: The url to make the request.
 *   - encoded: (optional) base64_encode data for authorized requests.
 *
 * @return string
 *   Returns the json array.
 */
function _entity_documentation_confluence_get_json_data($entity, $bundle, array $variable) {

  if (isset($variable[$entity . '_parent_id']) && $variable[$entity . '_parent_id'] != NULL) {
    // Generate the confluence page body value of a bundle page.
    $new_bundle_page_name = _edc_create_page_title($bundle);
    // Get documentation array.
    module_load_include('inc', 'entity_documentation', 'includes/entity_documentation.functions');
    $documentation_array = ed_bundle_array($entity, $bundle);

    // Get the content we want to convert into Confluence page by passing the
    // required values.
    $html = theme(
      'entity_documentation_confluence',
      array(
        'description' => $documentation_array[$entity][$bundle]['params']['description'],
        'properties' => $documentation_array[$entity][$bundle]['params']['properties'],
        'field_columns' => $documentation_array[$entity][$bundle]['field_columns'],
        'fields' => $documentation_array[$entity][$bundle]['fields'],
      )
    );

    // Built the json array for the request.
    $json = array(
      'type' => 'page',
      'title' => $new_bundle_page_name,
      'space' => array(
        'key' => $variable['space_key'],
      ),
      'ancestors' => array(
        array('id' => $variable[$entity . '_parent_id']),
      ),
      'body' => array(
        'storage' => array(
          'value' => $html,
          'representation' => 'storage',
        ),
      ),
    );

    $json = drupal_json_encode($json);
  }
  else {
    // Generate the entity parent page title.
    $entity_parent_name = _edc_create_page_title($entity);

    // Built the json array for the request.Body value is null.
    $json = array(
      'type' => 'page',
      'title' => $entity_parent_name,
      'space' => array(
        'key' => $variable['space_key'],
      ),
      'ancestors' => array(
        array('id' => $variable['parent_id']),
      ),
      'body' => array(
        'storage' => array(
          'value' => '',
          'representation' => 'storage',
        ),
      ),
    );

    $json = drupal_json_encode($json);
  }

  return $json;
}


/**
 * Check if page exist by title.
 *
 * @param string $entity
 *   The entity type that will be documented. For example 'node'.
 * @param string $bundle
 *   The bundle name. For example 'article'.
 * @param array $variable
 *   An associative array containing:
 *   - parent_id: The confluence page parent id.
 *   - space_key: The confluence space key.
 *   - url: The url to make the request.
 *   - encoded: (optional) base64_encode data for authorized requests.
 * @param string $title
 *   The page title you looking for.
 *
 * @return array
 *   The response array.
 */
function entity_documentation_confluence_page_exist_by_title($entity, $bundle, array $variable, $title) {
  $extras['page_title'] = preg_replace('/ /', '+', $title);
  // Make a retrieve request.
  return entity_documentation_confluence_create_request('retrieve_by_title', $entity, $bundle, $variable, $extras);
}

/**
 * Check if page exist by page id.
 *
 * @param string $entity
 *   The entity type that will be documented. For example 'node'.
 * @param string $bundle
 *   The bundle name. For example 'article'.
 * @param array $variable
 *   An associative array containing:
 *   - parent_id: The confluence page parent id.
 *   - space_key: The confluence space key.
 *   - url: The url to make the request.
 *   - encoded: (optional) base64_encode data for authorized requests.
 * @param int $page_id
 *   The page id to call.
 *
 * @return array
 *   The response array.
 */
function entity_documentation_confluence_page_exist_by_id($entity, $bundle, array $variable, $page_id) {
  $extras['page_id'] = $page_id;
  // Make a retrieve request for the requested page id.
  return entity_documentation_confluence_create_request('retrieve_by_id', $entity, $bundle, $variable, $extras);
}


/**
 * Store entity parent id.
 *
 * @param string $entity
 *   The entity type that will be documented. For example 'node'.
 * @param array $variable
 *   An associative array containing:
 *   - parent_id: The confluence page parent id.
 *   - space_key: The confluence space key.
 *   - url: The url to make the request.
 *   - encoded: (optional) base64_encode data for authorized requests.
 * @param int $parent_id
 *   The parent page id.
 *
 * @return array
 *   The updated variable.
 */
function _edc_store_entity_parent_id($entity, array $variable, $parent_id) {

  $variable[$entity . '_parent_id'] = $parent_id;
  $serialized = serialize($variable);
  variable_set('ed_confluence_api_values', $serialized);

  return $variable;
}

/**
 * Store entity parent id.
 *
 * @param string $entity
 *   The entity type that will be documented. For example 'node'.
 * @param array $variable
 *   An associative array containing:
 *   - parent_id: The confluence page parent id.
 *   - space_key: The confluence space key.
 *   - url: The url to make the request.
 *   - encoded: (optional) base64_encode data for authorized requests.
 *
 * @return array
 *   The updated variable.
 */
function _edc_unstore_entity_parent_id($entity, array $variable) {

  if (isset($variable[$entity . '_parent_id'])) {
    unset($variable[$entity . '_parent_id']);
    $serialized = serialize($variable);
    variable_set('ed_confluence_api_values', $serialized);
  }

  return $variable;
}

/**
 * Create human readable page name from machine name.
 *
 * @param string $name
 *   The machine name.
 *
 * @return string
 *   Returns the human readable page name.
 */
function _edc_create_page_title($name) {

  $page_name = preg_replace('/_/', ' ', $name);
  $page_name = ucwords($page_name);

  $page_name = $page_name . '[' . $name . ']';
  return $page_name;
}
