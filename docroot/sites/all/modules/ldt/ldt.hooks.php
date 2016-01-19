<?php
/**
 * @file
 * Hook definitions for Linked Data Tools
 *
 * @copyright Copyright(c) 2012 Christopher Skne
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */

/**
 * Provide information about library plugins for working with Linked Data.
 *
 * Defines hook_ldt_library_wrapper().
 *
 * This hook only needs to be implemented by providers of linked data libraries
 * for use with LDT.
 *
 * @return array
 *   An array of library configurations. Each library should provide two
 *   keys:
 *    'name' - The human readable name of the library. May be wrapped in t() if
 *      appropriate.
 *    'class' - The class to load for working with data.
 */
function hook_ldt_library_wrappers() {
  $wrappers = array();

  $wrappers['easyrdf'] = array(
    'name' => 'EasyRdf',
    'class' => '\Drupal\ldt\Plugins\EasyRdf',
  );

  return $wrappers;
}

/**
 * Define data formats accepted by LDT.
 *
 * Linked Data comes in different formats. This function returns an array
 * of supported formats. Some formats may not be compatible with some plugins,
 * it is up to the implementer to know which, currently.
 *
 * Defines hook_ldt_data_formats().
 *
 * @return array
 *   An array of format definitions. Each definition includes:
 *    'name' - Human readable name.
 *    'accept' - an array of mime type accept headers to set for requests
 *    'uri' - a URI for the format definition
 *    'extensions' - an array of possible file extensiosn
 *   The 'uri' and 'extensions' keys are not used by the default EasyRDF plugin
 *   however it provides its own data here, and the information is provided for
 *   use by other plugins.
 */
function hook_ldt_data_formats() {
  $formats = array();

  $formats['rdf+xml'] = array(
    'name' => 'RDF/XML',
    'accept' => 'application/rdf+xml',
  );

  return $formats;
}