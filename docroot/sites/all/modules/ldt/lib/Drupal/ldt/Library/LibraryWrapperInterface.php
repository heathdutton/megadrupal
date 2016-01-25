<?php
/**
 * @file
 * Provides a definition of a library wrapper.
 *
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */

namespace Drupal\ldt\Library;

/**
 * Defines a library wrapper for LDT.
 */
interface LibraryWrapperInterface {

  /**
   * Load a graph object to work with.
   *
   * @param string $uri
   *   URI of the graph.
   * @param string $format
   *   The format to use.
   *
   * @return \Drupal\ldt\Library\LibraryWrapperInterface
   *   The tool.
   */
  public function graph($uri = NULL, $format = LDT_DEFAULT_FORMAT);

  /**
   * Set RDF data for the graph.
   *
   * @param string $data
   *   The data.
   * @param string $format
   *   (optional) Format expected by the parser.
   *
   * @return \Drupal\ldt\Library\LibraryWrapperInterface
   *   The tool.
   */
  public function setGraphData($data, $format = LDT_DEFAULT_FORMAT);

  /**
   * Parse the source data using the graph tool.
   *
   * This should be called after LibraryWrapperInterface::setGraphData();
   *
   * @return \Drupal\ldt\Library\LibraryWrapperInterface
   *   The tool.
   */
  public function parseGraphData();

  /**
   * Return Graph data.
   *
   * This should be called after LibraryWrapperInterface::parseGraphData();
   *
   * @return mixed
   *   The graph, or FALSE.
   */
  public function getGraphData();

  /**
   * Add a namespace to the graph.
   *
   * @param string $abbreviation
   *   An abbreviation to use for the namespace.
   * @param string $uri
   *   The URI of the namespace.
   *
   * @return \Drupal\ldt\Library\LibraryWrapperInterface
   *   The tool.
   */
  public function addNamespace($abbreviation, $uri);

  /**
   * Helper to extract resources from a graph by property.
   *
   * @param string $property
   *   The property from which to load resources.
   * @param string|null $value
   *   (optional) A value expected in the property
   *
   * @return array
   *   An array of graph objects.
   */
  public function extractResources($property, $value = NULL);

  /**
   * Fetch an individual resource.
   *
   * @param string $uri
   *   The URI.
   *
   * @return array
   *   The resource, as an array.
   */
  public function fetchResource($uri);

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
  public function fetchResources($resources = NULL);

  /**
   * Load a resource from the cache into this object.
   *
   * This can be used to check if a resource exists locally before attempting an
   * expensive request to retrieve it.
   *
   * @param string $uri
   *   The URI of the resource.
   *
   * @return bool|array
   *   FALSE if the resource is not found, or the resource.
   */
  public function loadResource($uri);

  /**
   * Get all defined namespaces.
   */
  public function getNamespaces();

  /**
   * Represent the resource in HTML.
   *
   * @return string
   *   An HTML representation of the resource.
   */
  public function asHtml();

  /**
   * Check if the object has a given property.
   */
  public function hasProperty($property);

  /**
   * Check if the object has a given property and value.
   */
  public function hasPropertyValue($property, $value);

  /**
   * Return the value specified for a given property.
   */
  public function getPropertyValues($property);

  /**
   * Set the URI value directly.
   *
   * @param string $uri
   *   A string representing a URI.
   */
  public function setUri($uri);

  /**
   * Get the URI of this graph.
   */
  public function getUri();
}
