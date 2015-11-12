<?php
/**
 * @file
 * Provides an EasyRDF wrapper for LDT.
 *
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */

namespace Drupal\ldt\Plugins;

/**
 * An EasyRdf wrapper for LDT.
 */
class EasyRdf
  extends \Drupal\ldt\Library\LibraryWrapper
  implements \Drupal\ldt\Library\LibraryWrapperInterface {

  /**
   * @var \EasyRdf_Graph
   */
  public $graph;

  /**
   * The URI of this graph
   *
   * @var string
   */
  public $uri;

  /**
   * Constructor function.
   *
   * @return \Drupal\ldt\Plugins\EasyRdf
   *   The tool.
   */
  public function __construct() {
    try {
      easyrdf();

      return $this;
    }
    catch (\Exception $e) {
      drupal_set_message('Failed to load EasyRdf (' . $e->getMessage() . ')', 'error');
    }

    return FALSE;
  }

  /**
   * Load a graph object to work with.
   *
   * Note that this uses EasyRDFs fetching, which is not as good as Guzzle's,
   * but it does attempt some form of content negotiation.
   *
   * @param string $uri
   *   URI of the graph.
   * @param string $format
   *   The format to use.
   *
   * @return \Drupal\ldt\Plugins\EasyRdf
   *   The tool.
   */
  public function graph($uri = NULL, $format = NULL) {

    $this->uri = $uri;

    $this->graph = new \EasyRdf_Graph($this->uri, NULL, $format);

    return $this;
  }

  /**
   * Set RDF data for the graph.
   *
   * @param string $data
   *   The data.
   * @param string $format
   *   (optional) Format expected by the parser.
   *
   * @return \Drupal\ldt\Plugins\EasyRdf
   *   The tool.
   */
  public function setGraphData($data, $format = LDT_DEFAULT_FORMAT) {
    if (!isset($this->graph) || !$this->graph instanceof \EasyRdf_Graph) {
      $this->graph();
    }

    $this->setUnparsedData($data);
    $this->setFormat($format);

    return $this;
  }

  /**
   * Parse the set data using the graph tool.
   *
   * This should be called after LibraryWrapperInterface::setGraphData();
   *
   * @return \Drupal\ldt\Library\LibraryWrapperInterface
   *   The tool.
   */
  public function parseGraphData() {
    $this->graph->parse($this->getUnparsedData(), $this->getFormat(), $this->getUri());

    return $this;
  }

  /**
   * Return Graph data.
   *
   * @param bool $shorten
   *   If TRUE, attempt to compact property URIs.
   *
   * @return bool|\EasyRdf_Graph
   *   The graph, or FALSE.
   */
  public function getGraphData($shorten = FALSE) {
    if (isset($this->graph)) {
      $graph_array = $this->graph->toArray();
      if ($shorten == TRUE) {
        foreach ($graph_array as $property => $values) {
          $pstr = \EasyRdf_Namespace::shorten($property);
          if (empty($pstr)) {
            $pstr = $property;
          }
          $graph_array[$pstr] = $values;
        }
      }
      return $graph_array;
    }

    return FALSE;
  }

  /**
   * Add a namespace to the graph.
   *
   * @param string $abbreviation
   *   An abbreviation to use for the namespace.
   * @param string $uri
   *   The URI of the namespace.
   *
   * @return \Drupal\ldt\Plugins\EasyRdf
   *   The tool.
   */
  public function addNamespace($abbreviation, $uri) {
    \EasyRdf_Namespace::set($abbreviation, $uri);

    return $this;
  }

  /**
   * Extract resources from a graph by property, returning native objects.
   *
   * @param string $property
   *   The property from which to load resources.
   * @param string|null $value
   *   (optional) A value expected in the property
   *
   * @return array
   *   An array of \EasyRdf_Graph objects. One for each resource.
   */
  public function extractRawResources($property, $value = NULL) {

    if (isset($value) && !empty($value)) {
      $resources = $this->graph->resourcesMatching($property, $value);
    }
    else {
      $resources = $this->graph->resourcesMatching($property);
    }

    return $resources;
  }

  /**
   * Extract resources from a graph by property.
   *
   * @param string $property
   *   The property from which to load resources.
   * @param string|null $value
   *   (optional) A value expected in the property
   *
   * @return array
   *   An array of \EasyRdf_Graph objects. One for each resource.
   */
  public function extractResources($property, $value = NULL) {
    $resources = array();

    if (isset($value) && !empty($value)) {
      $_resources = $this->graph->resourcesMatching($property, $value);
    }
    else {
      $_resources = $this->graph->resourcesMatching($property);
    }

    if (!empty($_resources)) {
      foreach ($_resources as $resource) {
        $uri = $resource->getUri();
        $resources[$uri] = $resource->toArray();
      }
    }

    return $resources;
  }

  /**
   * Given a list of resources, load their graphs.
   *
   * @param array $resources
   *   An array of resources. This can be one of three types:
   *    - An array of graph objects.
   *    - An array of array data, in which case the array keys are used.
   *    - A flat array, in which case the values are used.
   *
   * @return array
   *   An array of resources.
   */
  public function fetchResources($resources = NULL) {
    $this->resources = array();

    if (!empty($resources)) {
      foreach ($resources as $key => $resource) {

        if ($resource instanceof \EasyRdf_Resource) {
          $uri = $resource->getUri();
        }
        elseif (is_array($resource) && array_key_exists('type', $resource) && $resource['type'] == 'uri') {
          $uri = $resource['value'];
        }
        elseif (is_array($resource)) {
          $uri = $key;
        }
        else {
          $uri = $resource;
        }

        if (!array_key_exists($uri, $this->resources)) {

          $resource = $this->fetchResource($uri);
          $this->setResource($uri, $resource);
        }
      }
    }

    return $this->resources;
  }

  /**
   * Fetch an individual resource.
   *
   * @param string $uri
   *   The URI.
   *
   * @return array
   *   The resource, as an array.
   */
  public function fetchResource($uri) {

    $graph = $this->fetchRawResource($uri);

    return $graph->toArray();
  }

  /**
   * Fetch a resource in its native format.
   *
   * @param string $uri
   *   The URI.
   *
   * @return \EasyRdf_Graph();
   *   The resource, as an array.
   */
  public function fetchRawResource($uri) {

    // Get the data.
    $data = ldt_fetch_rdf($uri);

    // Load the data into the graph object.
    $graph = new \EasyRdf_Graph();
    $graph->parse($data);

    return $graph;
  }

  /**
   * Get the resource for the graph.
   *
   * @return \EasyRdf_Resource
   *   An EasyRdf_resource.
   */
  public function getGraphRawResource() {
    return $this->graph->resource($this->getUri());
  }

  /**
   * Get all defined namespaces.
   *
   * @return array
   *   An array of namespace information.
   */
  public function getNamespaces() {
    return \EasyRdf_Namespace::namespaces();
  }

  /**
   * Represent the resource in HTML.
   *
   * @return string
   *   An HTML representation of the resource.
   */
  public function asHtml() {
    return $this->graph->dump();
  }

  /**
   * Check if the object has a given property.
   */
  public function hasProperty($property) {
    return $this->graph->hasProperty($this->getUri(), $property);
  }

  /**
   * Check if the object has a given property and value.
   *
   * @todo fix!
   */
  public function hasPropertyValue($property, $value) {
    // return $this->graph->hasProperty($this->graph, $property);
  }

  /**
   * Return the value specified for a given property.
   *
   * @param string $property
   *   Name of the property to retrieve.
   *
   * @return array
   *   An array of property values
   */
  public function getPropertyValues($property) {

    $results = array();
    if ($property == 'foaf:primaryTopic') {
      $resources = $this->graph->primaryTopic();
    }
    else {
      $resources = $this->graph->all($this->getUri(), $property, NULL, $this->getLang());
    }

    if (!empty($resources)) {
      foreach ($resources as $resource) {
        if ($resource instanceof \EasyRdf_Literal) {
          $results[] = $resource->getValue();
        }
        elseif ($resource instanceof \EasyRdf_Resource) {
          $_resource = $resource->toArray();
          $results[] = $_resource['value'];
        }
      }
    }

    if (!empty($results)) {
      return $results;
    }

    return FALSE;
  }

  /**
   * Set the URI value directly.
   *
   * @param string $uri
   *   A string representing a URI.
   */
  public function setUri($uri) {
    $this->uri = $uri;
  }

  /**
   * Get the URI of this graph.
   *
   * @return string
   *   A string representing a URI.
   */
  public function getUri() {
    if (empty($this->uri)) {
      $this->uri = $this->graph->getUri();
    }

    return $this->uri;
  }

  /**
   * Helper to dump data.
   *
   * Calling dsm() on this object causes an Exception in EasyRdf, so we add this
   * to dump when required.
   *
   * @return array
   *   An array of debugging information.
   */
  public function dump() {
    $data = array(
      'graph' => $this->getGraphData(TRUE),
      'resources' => $this->resources,
      'uri' => $this->uri,
    );

    return $data;
  }
}
