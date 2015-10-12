<?php
/**
 * @file
 * Principal API for the Linked Data Tools module.
 *
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 *
 * Functions in this file should be considered the main API, whereas functions
 * in the module file are those required for these to function effectively.
 */

/**
 * Invoke a new linked data tool from a plugin.
 *
 * @param string $name
 *   Name of the wrapper.
 *
 * @return \Drupal\ldt\Library\LibraryWrapperInterface
 *   A library wrapper.
 */
function ldt_tool($name) {
  $wrappers = ldt_get_library_wrappers();
  if (array_key_exists($name, $wrappers)) {
    if (class_exists($wrappers[$name]['class'])) {
      return new $wrappers[$name]['class']();
    }
  }
}

/**
 * Load a linked data tool and load RDF data into it.
 *
 * @param string $tool
 *   Machine name of the RDF tool to use (e.g. 'arc2' or 'easyrdf')
 * @param string $data
 *   Data to load into the graph
 * @param string $uri
 *   A URI to attach to the graph.
 * @param string $format
 *   The data format.
 *
 * @return \Drupal\ldt\Library\LibraryWrapperInterface
 *   The graph object, or false.
 */
function ldt_graph_data($tool, $data, $uri = NULL, $format = 'rdf+xml') {
  try {
    $tool = ldt_tool($tool);

    $tool->setGraphData($data, $format);

    if (!empty($uri)) {
      $tool->setUri($uri);
    }

    return $tool;
  }
  catch (\Exception $e) {
    drupal_set_message('Failed to load graph (' . $e->getMessage() . ')', 'error');
  }

  return FALSE;
}

/**
 * Load a linked data tool and load RDF data into it from a URI.
 *
 * This will load cached data, if available.
 *
 * @param string $uri
 *   A URI.
 * @param string $format
 *   (optional) An optional ldt data format. Defaults to rdfxml.
 * @param string $tool
 *   (optional) Machine name of the RDF tool to use (defaults to 'easyrdf')
 *
 * @return \Drupal\ldt\Library\LibraryWrapperInterface|bool
 *   The graph object, or false.
 */
function ldt_graph_uri($uri, $format = LDT_DEFAULT_FORMAT, $tool = LDT_DEFAULT_LIBRARY) {
  try {
    $tool = ldt_tool($tool);

    $data = ldt_fetch_rdf($uri);

    // Data may already by loaded as an array...
    if (is_array($data)) {
      $format = 'php';
    }

    if (!empty($data)) {
      $tool->setGraphData($data, $format);
      $tool->setUri($uri);

      $tool->parseGraphData();

      return $tool;
    }
  }
  catch (\Exception $e) {
    drupal_set_message('Failed to load graph. More information can be found in the log.', 'error');
    watchdog_exception('ldt', $e, '"%message", in %file:%line', array(
      '%message' => $e->getMessage(),
      '%file' => $e->getFile(),
      '%line' => $e->getLine(),
    ));
  }

  return FALSE;
}

/**
 * Helper to make a request for RDF data.
 *
 * This first checks the local cache, then it will request the data using
 * Guzzle. If Guzzle is not available, it will fallback on
 * drupal_http_request(). Guzzle itself is expected to be autoloaded, so it can
 * be provided via http://drupal.org/project/guzzle or any other mechanism
 * which supports autoloading.
 *
 * @param string $uri
 *   The URI to request data from
 * @param string $format
 *   (optional) The data format, as defined by hook_ldt_data_formats(). Defaults
 *   to 'rdf+xml' (RDF XML).
 * @param string $tool_type
 *   (optional) The tool to use. Defaults to EasyRdf
 * @param bool $flush
 *   (optional) If TRUE, the resource will be
 *
 * @return string
 *   The data returned from the request..
 */
function ldt_fetch_rdf($uri, $format = LDT_DEFAULT_FORMAT, $tool_type = LDT_DEFAULT_LIBRARY, $flush = FALSE) {
  $data = FALSE;

  // Check if the resource is cached first.
  if ($flush == FALSE) {
    $tool = ldt_tool($tool_type);
    $cached_resource = $tool->loadResource($uri);
    if (!empty($cached_resource) && is_array($cached_resource)) {
      return $cached_resource;
    }
  }

  // Check formats.
  $format = ldt_data_format($format);
  if (empty($format)) {
    watchdog('ldt', 'Invalid data format');

    return FALSE;
  }

  // Use Guzzle if it exists.
  if (class_exists('Guzzle\Http\Client')) {
    try {
      $client = new \Guzzle\Http\Client();
      $request = $client->get($uri);
      if (array_key_exists('accept', $format) && is_array($format['accept']) && !empty($format['accept'])) {
        foreach ($format['accept'] as $accept) {
          $request->addHeader('Accept', $accept);
        }
      }
      $response = $request->send();
      if ($response->isSuccessful()) {
        $data = $response->getBody(TRUE);
      }
      else {
        // Probably an invalid request.
        ldt_log_http_error($response->getStatusCode(), 'Request failed');
      }
    }
    catch(\Guzzle\Http\Exception\BadResponseException $e) {
      $response = $e->getResponse();
      if (isset($response) && is_object($response) && $response instanceof \Guzzle\Http\Message\Response) {
        $status = $response->getStatusCode();
        $message = $response->getReasonPhrase();
      }
      else {
        $status = 0;
        $message = 'Unknown error (this may mean the endpoint is missing or not responding)  ';
      }
      ldt_log_http_error($status, $message);
    }
  }
  else {
    // Use drupal_http_request as a fallback.
    $vars = array(
      'headers' => array(),
    );
    if (array_key_exists('accept', $format) && is_array($format['accept']) && !empty($format['accept'])) {
      $accept_headers = array();
      foreach ($format['accept'] as $accept) {
        $accept_headers[] = $accept;
      }
      $vars['headers']['Accept'] = $accept_headers;
    }
    $result = drupal_http_request($uri, $vars);
    if (isset($result->data)) {

      $data = $result->data;
    }
  }

  return $data;
}

/**
 * Extract resources specified as SKOS 'narrower'.
 *
 * @param \Drupal\ldt\Library\LibraryWrapperInterface $tool
 *   An EasyRdf Graph object.
 *
 * @return array
 *   An array of resource objects.
 */
function ldt_extract_skos_narrower($tool) {

  $namespaces = $tool->getNamespaces();
  if (!array_key_exists('skos', $namespaces)) {
    $tool->addNamespace('skos', 'http://www.w3.org/2004/02/skos/core#');
  }

  $resources = $tool->extractResources('skos:narrower');
  $results = $tool->fetchResources($resources);

  return $results;
}

/**
 * Extract resources specified as SKOS 'broader'.
 *
 * @param string $uri
 *   The URI to import from.
 * @param string $tool_plugin
 *   Name of the plugin to use.
 * @param string $format
 *   The data format.
 *
 * @return array
 *   An array of resource objects.
 */
function ldt_extract_skos_broader($uri, $tool_plugin = LDT_DEFAULT_LIBRARY, $format = LDT_DEFAULT_FORMAT) {

  $tool = ldt_graph_uri($uri, LDT_DEFAULT_FORMAT, $tool_plugin);
  $namespaces = $tool->getNamespaces();
  if (!array_key_exists('skos', $namespaces)) {
    $tool->addNamespace('skos', 'http://www.w3.org/2004/02/skos/core#');
  }

  $resources = $tool->extractResources('skos:broader');

  $tool = ldt_tool($tool_plugin);
  $namespaces = $tool->getNamespaces();
  if (!array_key_exists('skos', $namespaces)) {
    $tool->addNamespace('skos', 'http://www.w3.org/2004/02/skos/core#');
  }

  $results = $tool->fetchResources($resources);

  return $results;
}

/**
 * Extract resources specified as Dublin Core Terms 'subject'.
 *
 * @param string $uri
 *   The URI to import from.
 * @param string $tool_plugin
 *   Name of the plugin to use.
 * @param string $format
 *   The data format.
 *
 * @return array
 *   An array of resource objects.
 */
function ldt_extract_dc_subjects($uri, $tool_plugin = LDT_DEFAULT_LIBRARY, $format = LDT_DEFAULT_FORMAT) {

  $tool = ldt_graph_uri($uri, $format, $tool_plugin);
  $namespaces = $tool->getNamespaces();
  if (!array_key_exists('dcterms', $namespaces)) {
    $tool->addNamespace('dcterms', 'http://purl.org/dc/terms/');
  }

  $resources = $tool->extractResources('dcterms:subject');

  $tool = ldt_tool($tool_plugin);
  $namespaces = $tool->getNamespaces();
  if (!array_key_exists('dcterms', $namespaces)) {
    $tool->addNamespace('dcterms', 'http://purl.org/dc/terms/');
  }

  $results = $tool->fetchResources($resources);

  return $results;
}