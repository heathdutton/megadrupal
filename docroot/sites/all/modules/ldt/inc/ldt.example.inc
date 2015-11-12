<?php
/**
 * @file
 * Example for usage of the LDT API
 *
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */

/**
 * Helper to fetch a default (RDF/XML) resource.
 *
 * @return bool|Drupal\ldt\Library\LibraryWrapperInterface
 *   A LDT library wrapper.
 */
function ldt_example_get_dbpedia_rdfxml() {
  // Load a representation from DBPedia in RDF-XML.
  $resource = ldt_graph_uri('http://dbpedia.org/data/Generalife.rdf');
  return $resource;
}

/**
 * Helper to fetch an RDFa resource.
 *
 * @return bool|Drupal\ldt\Library\LibraryWrapperInterface
 *   A LDT library wrapper.
 */
function ldt_example_get_rdfa() {
  // Load a resource containing embedded RDFa.
  $resource = ldt_graph_uri('http://cas.awm.gov.au/item/P00355.011', 'rdfa');
  return $resource;
}

/**
 * Helper to fetch an RDF/XML resource.
 *
 * @return bool|Drupal\ldt\Library\LibraryWrapperInterface
 *   A LDT library wrapper.
 */
function ldt_example_get_bbc_rdfxml() {
  // Load a resource from the BBC containing RDF in JSON.
  $resource = ldt_graph_uri('http://www.bbc.co.uk/music/artists/5441c29d-3602-4898-b1a1-b77fa23b8e50.rdf', 'rdfxml');

  // Retrieve a php array of resources, for example.
  $resources = $resource->getGraphData();

  return $resource;
}