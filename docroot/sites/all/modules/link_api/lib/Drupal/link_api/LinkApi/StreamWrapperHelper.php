<?php
/**
 * @file
 * Helper for working links formatted as stream wrappers.
 *
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */

namespace Drupal\link_api\LinkApi;

/**
 * Class StreamWrapperHelper
 * @package Drupal\link_api\LinkApi
 */
class StreamWrapperHelper {

  /**
   * Given a URI as a stream wrapper, get a local absolute link.
   *
   * This is the technical equivalent of file_create_url() for a stream wrapper,
   * without returning the global base_path as well.
   *
   * @see file_create_url()
   *
   * @param string $uri
   *   The URI
   *
   * @return string
   *   The full URI excluding the base path.
   */
  public function AbsoluteUri($uri) {

    // Allow the URI to be altered, e.g. to serve a file from a
    // CDN or static file server.
    drupal_alter('file_url', $uri);

    try {
      $scheme_wrapper = file_stream_wrapper_get_instance_by_uri($uri);
      $path = str_replace('\\', '/', $uri);
      $uri = '/' . $scheme_wrapper->getDirectoryPath() . '/' . drupal_encode_path($this->getStreamWrapperPath($path));

      return $uri;
    }
    catch(\Exception $e) {
      watchdog_exception('link_api', $e, 'Failed to create or load a stream wrapper');
    }
  }

  /**
   * Get the path from a stream wrapper URI.
   *
   * @see DrupalStreamWrapperInterface::getTarget()
   *
   * @param string $uri
   *   A stream wrapper-based URI.
   *
   * @return string
   *   The part of the uri which is not a stream wrapper prefix.
   */
  public function getStreamWrapperPath($uri) {

    list($scheme, $target) = explode('://', $uri, 2);

    // Remove erroneous leading or trailing, forward-slashes and backslashes.
    return trim($target, '\/');
  }
}
