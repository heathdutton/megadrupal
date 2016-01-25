<?php
/**
 * @file
 * Admin UI pages for testing Freebase APIs.
 */

/**
 * Menu callback; Overview and diagnostics for freebase services.
 */
function freebase_api_admin_page() {
  $page = array();
  $page['columns'] = array(
    '#type' => 'container',
    '#attributes' => array(),
    // D7 dies if #attributes is blank. Doh.
    'lookup' => array(
      '#type' => 'container',
      '#attributes' => array('style' => 'width:30%; float:left'),
    ),
    'result' => array(
      '#type' => 'container',
      '#attributes' => array('style' => 'width:60%; float:right'),
    ),
  );

  // Test out the topic lookup, inc autocomplete.
  $page['columns']['lookup']['freebase_api_topic_lookup_form'] = drupal_get_form('freebase_api_topic_lookup_form');
  // Test out the search function.
  $page['columns']['lookup']['freebase_api_search_lookup_form'] = drupal_get_form('freebase_api_search_lookup_form');
  // Test out the Schema lookup, and a filtered autocomplets.
  $page['columns']['lookup']['freebase_api_schema_lookup_form'] = drupal_get_form('freebase_api_schema_lookup_form');
  // Set up result pane. Content gets filled in via AJAX.
  $page['columns']['result']['transaction'] = array(
    '#type' => 'container',
    '#title' => 'Transaction',
    '#attributes' => array('id' => 'transaction-results'),
  );
  return $page;
}


/**
 * AJAX-enabled FAPI subform for making a request to the Freebase topic service.
 *
 * This is a simplified variation of the wsclient_tester form really.
 */
function freebase_api_topic_lookup_form($form, &$form_state) {
  $form = array();
  $form['box'] = array(
    '#type' => 'fieldset',
    '#title' => t('Topic lookup'),
  );
  $form['box']['help'] = array(
    '#markup' => t('
      Demonstrates the <a href="https://developers.google.com/freebase/v1/topic">topic-by-id lookup web service</a>,
      as well as a simple
      variation of an autocomplete helper.
      Enter any freetext to auto-search, or enter a Freebase topic ID directly
      then press "Lookup".
    '),
  );

  // Encapsulating the endpoint args in a tree branch makes it easier to pass
  // the required vals straight through later instead of filtering out the
  // needed args. So ... nest 'parameters'.
  $form['box']['parameters'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );
  $form['box']['parameters']['id'] = array(
    '#type' => 'textfield',
    '#title' => t('Freebase ID'),
    '#default_value' => @$form_state['values']['id'],
    '#description' => t('Enter any topic name or, a Freebase topic ID, Such as <code>/en/bob_dylan</code> or <code>/m/09cws</code>'),
    '#autocomplete_path' => 'freebase_api/topic_autocomplete_callback',
    '#size' => 30,
  );
  $form['box']['actions'] = array(
    '#type' => 'actions',
  );
  $form['box']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Lookup'),
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'freebase_api_topic_lookup_callback',
      'wrapper' => 'transaction-results',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  return $form;
}


/**
 * AJAX callback to lookup FB Topic.
 *
 * Takes the form submission, makes the request and returns a renderable
 * element displaying the results.
 */
function freebase_api_topic_lookup_callback($form, $form_state) {
  $operation_name = 'topic';
  // Lots of paranoia, as I want to be able to apply to an existing modified
  // system, do not assume that it's clean or working.
  // The user may have manually deleted the operation.
  $service = wsclient_service_load('freebase');
  if (empty($service) || !method_exists($service, 'invoke')) {
    $message = t('Service was not loaded, cannot invoke it. This requires the WSClient Web service called "Google Freebase" [freebase] to provide the operation called [topic]');
    $element['response']['#markup'] = $message;
    watchdog('freebase_api', $message, E_USER_ERROR);
    return $element;
  }
  $operation = $service->operations[$operation_name];
  if (empty($operation)) {
    $message = t('Invalid method, cannot invoke it. This requires the WSClient Web service called "Google Freebase" [freebase] to provide the operation called [topic]');
    $element['response']['#markup'] = $message;
    watchdog('freebase_api', $message, E_USER_ERROR);
    return $element;
  }

  // This element has the results added to it and is returned.
  $element = array(
    '#type' => 'container',
    '#title' => 'Transaction',
    '#attributes' => array('id' => 'transaction-results'),
  );

  // Ready to actually invoke the call. Take it careful.
  $args = (array) $form_state['values']['parameters'];
  try {
    $response = $service->invoke($operation_name, $args);
  }
  catch (Exception $e) {
    $response = $e->getMessage();
  }

  if (isset($response['property'])) {
    // Looks like a success, see if we can render something.
    $element['response']['#markup'] = theme('freebase_api_topic_summary', $response, TRUE);
  }
  else {
    $message = t('Empty response from lookup');
    $element['response']['#markup'] = $message;
    watchdog('freebase_api', $message, E_USER_WARNING);
  }

  return $element;
}

/**
 * AJAX-enabled FAPI subform for making a request to the Freebase seach service.
 *
 * This is a simplified variation of the wsclient_tester form really.
 */
function freebase_api_search_lookup_form($form, &$form_state) {
  $form = array();
  $form['box'] = array(
    '#type' => 'fieldset',
    '#title' => t('Search'),
  );
  $form['box']['help'] = array(
    '#markup' => t('
      Demonstrates the <a href="https://developers.google.com/freebase/v1/search">free search</a> web service.
    '),
  );

  // Encapsulating the endpoint args in a tree branch makes it easier to pass
  // the required vals straight through later instead of filtering out the
  // needed args. So ... nest 'parameters'.
  $form['box']['parameters'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );
  $form['box']['parameters']['query'] = array(
    '#type' => 'textfield',
    '#title' => t('Search term'),
    '#default_value' => @$form_state['values']['query'],
    '#size' => 30,
  );
  $form['box']['actions'] = array(
    '#type' => 'actions',
  );
  $form['box']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Lookup'),
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'freebase_api_search_lookup_callback',
      'wrapper' => 'transaction-results',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  return $form;
}

/**
 * AJAX callback to perform FB search.
 *
 * Takes the form submission, makes the request and returns a renderable
 * element displaying the results.
 */
function freebase_api_search_lookup_callback($form, $form_state) {
  $operation_name = 'search';
  // Lots of paranoia, as I want to be able to apply to an existing modified
  // system, do not assume that it's clean or working.
  // The user may have manually deleted the operation.
  $service = wsclient_service_load('freebase');
  if (empty($service) || !method_exists($service, 'invoke')) {
    $element['response']['#markup'] = htmlspecialchars('Service was not loaded, cannot invoke it. This requires the WSClient Web service called "Google Freebase" [freebase] to provide the operation called [topic] ', E_USER_ERROR);
    return $element;
  }
  $operation = $service->operations[$operation_name];
  if (empty($operation)) {
    $element['response']['#markup'] = htmlspecialchars('Invalid method, cannot invoke it. This requires the WSClient Web service called "Google Freebase" [freebase] to provide the operation called [topic] ', E_USER_ERROR);
    return $element;
  }

  // This element has the results added to it and is returned.
  $element = array(
    '#type' => 'container',
    '#title' => 'Transaction',
    '#attributes' => array('id' => 'transaction-results'),
  );

  // Ready to actually invoke the call. Take it careful.
  $args = $form_state['values']['parameters'];
  try {
    $response = $service->invoke($operation_name, $args);
    $element['title']['#markup'] = t('<h2>Search: %query</h2>', array('%query' => $args['query']));
    $element['response']['#markup'] = theme('freebase_api_search_results', $response);
  }
  catch (Exception $e) {
    $element['response']['#markup'] = $e->getMessage();
  }

  return $element;
}


/**
 * Given a key, return suggestions pulled from Freebase.
 *
 * THIS IS NOT AS EFFICIENT AS CALLING THE API CLIENT-SIDE and is here for
 * diagnostics mainly.
 *
 * @see freebase_api_menu
 *
 * @param string $string
 *   Query.
 * @param array $params
 *   Options.
 *
 * @return NULL
 *   Actually triggers the JSON output directly.
 */
function freebase_api_topic_autocomplete_callback($string = "", $params = array()) {
  $matches = array();
  if ($string) {
    try {
      $service = wsclient_service_load('freebase');
      $operation_name = 'search';
      // $operation = $service->operations[$operation_name];
      $args = array('query' => $string) + $params;
      $response = $service->invoke($operation_name, $args);
      if ($response) {
        $matches = array();
        foreach ($response['result'] as $result) {
          $specifier = isset($result['notable']['name']) ? $result['notable']['name'] : @$result['id'];
          $matches[$result['mid']] = $result['name'] . ' (' . $specifier . ')';
        }
      }
    }
    catch (Exception $e) {
      $matches = array('#error' => "Whoops " . $e->getMessage());
    }
  }
  drupal_json_output($matches);
  return NULL;
}

/**
 * Given a key, return suggestions pulled from Freebase.
 *
 * @see freebase_api_menu
 *
 * @param string $string
 *   Query.
 * @param array $params
 *   Options.
 *
 * @return NULL
 *   The JSON response is sent back directly, not by this func.
 */
function freebase_api_schema_autocomplete_callback($string = "", $params = array()) {
  // Add specific filters to the search, then just use the generic callback.
  $params['filter'] = "(all type:/type/type )";
  return freebase_api_topic_autocomplete_callback($string, $params);
}

/**
 * AJAX-enabled FAPI subform for making requests to the Freebase schema service.
 *
 * This is not a topic, and the lookup is actually a complex MQL query.
 */
function freebase_api_schema_lookup_form($form, &$form_state) {
  $form = array();
  $form['box'] = array(
    '#type' => 'fieldset',
    '#title' => t('MQL'),
  );

  $form['box']['help'] = array(
    '#markup' => t('
      Makes a specialized <a href="https://developers.google.com/freebase/v1/mqlread">lookup using MQL</a>
       to pull back just info about
      "type" items. This produces a schema that can be used to define complex
      data types for everyday use.
      Freebase contains hundreds of "types" in dozens of domains, defining
      classes such as "Automobile", "Travel Destination", "Musical Artist"
      or "Religious Organisation".
    '),
  );

  // Encapsulating the endpoint args in a tree branch makes it easier to pass
  // the required vals straight through later instead of filtering out the
  // needed args. So ... nest 'parameters'.
  $form['box']['parameters'] = array(
    '#type' => 'container',
    '#tree' => TRUE,
  );
  $form['box']['parameters']['id'] = array(
    '#type' => 'textfield',
    '#title' => t('Schema ID'),
    '#default_value' => @$form_state['values']['id'],
    '#description' => t('Any schema type name, or a Freebase Schema ID, Such as <code>/book/book</code> or <code>/people/person</code>'),
    '#autocomplete_path' => 'freebase_api/schema_autocomplete_callback',
    '#size' => 30,
  );

  $form['box']['actions'] = array(
    '#type' => 'actions',
  );
  $form['box']['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Lookup'),
    '#ajax' => array(
      'event' => 'click',
      'callback' => 'freebase_api_schema_lookup_callback',
      'wrapper' => 'transaction-results',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  return $form;
}


/**
 * AJAX callback. Displays a schema.
 *
 * Takes the form submission,
 * makes the request and returns a renderable element displaying the results.
 */
function freebase_api_schema_lookup_callback($form, $form_state) {
  $service = wsclient_service_load('freebase');
  $operation_name = 'mqlread';
  // $operation = $service->operations[$operation_name];

  // This element has the results added to it and is returned.
  $element = array(
    '#type' => 'container',
    '#title' => 'Transaction',
    '#attributes' => array('id' => 'transaction-results'),
  );

  $type_id = $form_state['values']['parameters']['id'];
  // This retrieves most of everything I want to know about the data
  // model and its properties.
  $schema_query_string = '{
"/type/object/id": "' . $type_id . '",
"/type/object/name": null,
"/type/object/type": "/type/type",
"/type/type/properties": [{
  "/type/object/id": null,
  "/type/object/name": null,
  "/type/property/expected_type": null,
  "/type/property/reverse_property": null,
  "/type/property/unique": null,
  "optional":"optional"
}],
"/freebase/type_hints/included_types": [{
  "/type/object/id": null,
  "/type/object/name": null,
  "/type/property/expected_type": null,
  "/type/type/properties": [{
    "/type/object/id": null,
    "/type/object/name": null,
    "/type/property/expected_type": null,
    "/type/property/reverse_property": null,
    "/type/property/unique": null,
    "optional":"optional"
  }],
  "optional":"optional"
}]

}';
  $args = array('query' => $schema_query_string);

  // Ready to actually invoke the call. Take it careful.
  try {
    $response = $service->invoke($operation_name, $args);
  }
  catch (Exception $e) {
    $response = $e->getMessage();
  }

  // See if we can render something, Even errors.
  $element['response']['#markup'] = theme('freebase_api_schema_summary', $response);

  return $element;
}
