<?php
/**
 * @file
 * Provides a helper class for working with links. This is designed to provide
 * functions for URL manipulations without requiring a LinkApiLink object.
 *
 * @copyright Copyright(c) 2012 Christopher Skene
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Chris Skene chris at xtfer dot com
 */

namespace Drupal\link_api\LinkApi;

/**
 * Class LinkApiHelper
 */
class LinkApiHelper {

  /**
   * Normalise a URL.
   *
   * This takes a given URL and runs all the available tidying and sorting
   * functions over it.
   */
  static public function normaliseURL($url) {

    // Remove session IDs.
    $url = self::stripSessionID($url);

    // Sort the query string, if necessary.
    $url_parts = parse_url($url);
    if (isset($url_parts['query']) && !empty($url_parts['query'])) {
      $url_parts['query'] = self::sortQueryString($url_parts['query'], LINK_API_QUERY_SORT_CLEAN);
    }

    // Build the URL.
    $url = self::buildURL($url_parts);

    // Add a trailing slash.
    $url = self::trailingSlash($url);

    return $url;
  }

  /**
   * Add a trailing slash.
   *
   * For example, "http://www.example.com" becomes "http://www.example.com/".
   *
   * This explicitly requests the default or index document, which slightly
   * speeds up page retrieval for the visitor, and normalises the URL in the
   * database
   *
   * Originally from the links module.
   *
   * @param string $url
   *   A URL
   *
   * @return string
   *   The URL with a trailing slash
   */
  static public function trailingSlash($url) {
    $url = trim($url);
    if (preg_match('!^[A-Z0-9]+://[^/?]+$!i', $url)) {
      $url .= '/';
    }
    return $url;
  }

  /**
   * Orders the parameters in an HTTP GET query string.
   *
   * This helps to detect URLs that are the same except for the
   * parameter order.
   * For exammple:
   *    xyz=98&def=yes&abc=49
   * becomes:
   *    abc=49&def=yes&xyz=98
   *
   * Accepts either "&" or the W3C-preferred "&amp;" as a delimiter
   * between parameters.
   *
   * @param string $query
   *   The query string
   * @param string $mode
   *   (optional) The method to use for sorting. Either
   *   LINK_API_QUERY_SORT_CLEAN to remove empty parameters, or
   *   LINK_API_QUERY_SORT_RETAIN, to retain empty parameters.
   *
   * @return string
   *   A sorted query string.
   */
  static public function sortQueryString($query, $mode = LINK_API_QUERY_SORT_CLEAN) {

    // Convert all escaped ampersands prior to parse_str, or the output is
    // wrong.
    $query = str_replace('&amp;', '&', $query);

    parse_str($query, $results);
    ksort($results);

    // Remove empty query strings.
    if ($mode == LINK_API_QUERY_SORT_CLEAN) {
      foreach ($results as $key => $result) {
        if (empty($result)) {
          unset ($results[$key]);
        }
      }
    }

    // Return a built query string using & as the separator, for consistency.
    return http_build_query($results, NULL, '&');
  }

  /**
   * This function removes known session ID strings from a URL.
   *
   * These are transient data from a particular browser on a particular day, and
   * definitely do not belong in a links database. The search is not
   * case sensitive. This also works on a bare query string rather than
   * a full URL, and can be used that way.
   *
   * Originally from the links module
   *
   * @param string $url
   *   A URL.
   *
   * @return string
   *   The URL minus a session ID
   */
  static public function stripSessionID($url) {
    $url = trim($url);
    $url = preg_replace('/(^|[&?]|&amp;)[0-9A-Z_]*SESS(ION|_ID|ID)*=[0-9A-Z_]*/i', '\1', $url);
    return self::cleanupParameters($url);
  }

  /**
   * Clean up parameters.
   *
   * The preg_replace() calls some functions may leave some cruft in the URL,
   * such as "&&", "&?", or "?" or "&" as the first or last character. This
   * function clears that problem up.
   *
   * @deprecated
   *
   * Originally from the links module
   *
   * @param string $url
   *   A URL.
   *
   * @return string
   *   The URL minus untidy parameters
   */
  static public function cleanupParameters($url) {
    $patterns = array(
      '/(\&amp;|\&)(\&amp;|\&)/', '/^(\&amp;|\&)/', '/\?(\&amp;|\&)/',
      '/(\&amp;|\&|\?)$/', '/(\&amp;|\&)\?/',
    );
    $replaces = array('\1', '', '?', '', '?');
    return preg_replace($patterns, $replaces, $url);
  }

  /**
   * Build a URL from parts provided from parse_url().
   *
   * This wrapper is used to cover situations where the PECL http_build_url
   * function is not available.
   *
   * @see http://php.net/parse-url#85963
   *
   * @param string|array $url
   *   The existing URL as a string or result from parse_url
   * @param array $parts
   *   Same as $url
   * @param int $flags
   *   URLs are combined based on these
   * @param array|bool $new_url
   *   If set, filled with array version of new url
   *
   * @return string
   *   The URL.
   *
   * @return string
   *   A URL
   */
  static public function buildURL($url, $parts = array(), $flags = NULL, &$new_url = FALSE) {
    if (!is_array($url)) {
      $url = parse_url($url);
    }

    if (function_exists('http_build_url')) {
      return http_build_url($url, $parts, $flags, $new_url);
    }
    else {

      // If only a simple URL is required, use the simple function, which works.
      if (empty($parts) && empty($flags)) {
        return self::buildSimpleUrl($url);
      }

      // Experimental... won't always work.
      return self::httpBuildUrl($url, $parts, $flags, $new_url);
    }
  }

  /**
   * Build a simple URL from parts.
   *
   * @param array $parts
   *   The URL parts to join
   *
   * @return string
   *   A URL.
   */
  public static function buildSimpleUrl($parts) {

    $uri = isset($parts['scheme']) ? $parts['scheme'] . ':' . ((strtolower($parts['scheme']) == 'mailto') ? '' : '//') : '';
    $uri .= isset($parts['user']) ? $parts['user'] . (isset($parts['pass']) ? ':' . $parts['pass'] : '') . '@' : '';
    $uri .= isset($parts['host']) ? $parts['host'] : '';
    $uri .= !empty($parts['port']) ? ':' . $parts['port'] : '';

    if (isset($parts['path'])) {
      $uri .= (substr($parts['path'], 0, 1) == '/') ? $parts['path'] : ((!empty($uri) ? '/' : '') . $parts['path']);
    }

    $uri .= isset($parts['query']) ? '?' . $parts['query'] : '';
    $uri .= isset($parts['fragment']) ? '#' . $parts['fragment'] : '';

    return $uri;
  }

  /**
   * HTTP Build URL.
   *
   * Combines arrays in the form of parse_url() into a new string based on
   * specific options.
   *
   * @param string|array $url
   *   The existing URL as a string or result from parse_url
   * @param array $parts
   *   Same as $url
   * @param int $flags
   *   URLs are combined based on these
   * @param array|bool $new_url
   *   If set, filled with array version of new url
   *
   * @return string
   *   The URL.
   */
  static public function httpBuildUrl($url, $parts = array(), $flags = HTTP_URL_REPLACE, &$new_url = FALSE) {
    drupal_set_message('Link API is using experimental URL builder functions and may not operate correctly. It is recommended to install the PECL HTTP extension instead.');

    // If the $url is a string.
    if (is_string($url)) {
      $url = parse_url($url);
    }

    // If the $parts is a string.
    if (is_string($parts)) {
      $parts = parse_url($parts);
    }

    // Scheme and Host are always replaced.
    if (isset($parts['scheme'])) {
      $url['scheme'] = $parts['scheme'];
    }
    if (isset($parts['host'])) {
      $url['host'] = $parts['host'];
    }

    // (If applicable) Replace the original URL with it's new parts.
    if (HTTP_URL_REPLACE & $flags) {
      $parts_n = array(
        'user',
        'pass',
        'port',
        'path',
        'query',
        'fragment',
      );
      // Go through each possible key.
      foreach ($parts_n as $key) {
        // If it's set in $parts, replace it in $url.
        if (isset($parts[$key])) {
          $url[$key] = $parts[$key];
        }
      }
    }
    else {
      // Join the original URL path with the new path.
      if (isset($parts['path']) && (HTTP_URL_JOIN_PATH & $flags)) {
        if (isset($url['path']) && $url['path'] != '') {
          // If the URL doesn't start with a slash, we need to merge.
          if ($url['path'][0] != '/') {
            // If the path ends with a slash, store as is.
            if ($parts['path'][strlen($parts['path']) - 1] == '/') {
              $s_base_path = $parts['path'];
            }
            // Else trim off the file.
            else {
              // Get just the base directory.
              $s_base_path = dirname($parts['path']);
            }

            // If it's empty.
            if ($s_base_path == '') {
              $s_base_path = '/';
            }

            // Add the two together.
            $url['path'] = $s_base_path . $url['path'];

            // Free memory.
            unset($s_base_path);
          }

          if (strpos($url['path'], './') !== FALSE) {
            // Remove any '../' and their directories.
            while (preg_match('/\w+\/\.\.\//', $url['path'])) {
              $url['path'] = preg_replace('/\w+\/\.\.\//', '', $url['path']);
            }

            // Remove any './'
            $url['path'] = str_replace('./', '', $url['path']);
          }
        }
        else {
          $url['path'] = $parts['path'];
        }
      }

      // Join the original query string with the new query string.
      if (isset($parts['query']) && (HTTP_URL_JOIN_QUERY & $flags)) {
        if (isset($url['query'])) {
          $url['query'] .= '&' . $parts['query'];
        }
        else {
          $url['query'] = $parts['query'];
        }
      }
    }

    // Strips all the applicable sections of the URL.
    if (HTTP_URL_STRIP_USER & $flags) {
      unset($url['user']);
    }
    if (HTTP_URL_STRIP_PASS & $flags) {
      unset($url['pass']);
    }
    if (HTTP_URL_STRIP_PORT & $flags) {
      unset($url['port']);
    }
    if (HTTP_URL_STRIP_PATH & $flags) {
      unset($url['path']);
    }
    if (HTTP_URL_STRIP_QUERY & $flags) {
      unset($url['query']);
    }
    if (HTTP_URL_STRIP_FRAGMENT & $flags) {
      unset($url['fragment']);
    }

    // Store the new associative array in $new_url
    $new_url = $url;

    // Combine the new elements into a string and return it/
    $return = ((isset($url['scheme'])) ? $url['scheme'] . '://' : '')
      . ((isset($url['user'])) ? $url['user'] . ((isset($url['pass'])) ? ':' . $url['pass'] : '') . '@' : '')
      . ((isset($url['host'])) ? $url['host'] : '')
      . ((isset($url['port'])) ? ':' . $url['port'] : '')
      . ((isset($url['path'])) ? trim($url['path']) : '')
      . ((isset($url['query'])) ? '?' . $url['query'] : '')
      . ((isset($url['fragment'])) ? '#' . $url['fragment'] : '');

    return $return;
  }

  /**
   * Sanitize a URL.
   *
   * @param string $url
   *   A URL
   *
   * @return mixed
   *   The sanitized version of the URL.
   */
  static public function sanitizeURL($url) {
    return filter_var($url, FILTER_SANITIZE_URL);
  }

  /**
   * Validate a URL.
   *
   * Note that the function will only find ASCII URLs to be valid;
   * internationalized domain names (containing non-ASCII characters) will fail.
   *
   * Its possible that in PHP prior to 5.2.13/5.3.2, this may return TRUE for
   * some invalid URLs.
   *
   * @param string $url
   *   A URL
   *
   * @return bool
   *   TRUE if the URL validates, and FALSE if it does not
   */
  static public function validateURL($url) {
    $_url = filter_var($url, FILTER_VALIDATE_URL);
    if ($_url == FALSE) {
      return FALSE;
    }
    else {
      return TRUE;
    }
  }

  /**
   * Validate an IP address.
   *
   * @param string $ip
   *   An IP address.
   *
   * @return bool
   *   TRUE if the IP address is valid, or FALSE.
   */
  static public function validateIP($ip) {
    $_ip = filter_var($ip, FILTER_VALIDATE_IP);
    if ($_ip == FALSE) {
      return FALSE;
    }
    else {
      return TRUE;
    }
  }

  /**
   * Compute a hash value for a URL.
   *
   * Useful for lookups on URLs, as a hash is quicker than a full URL.
   *
   * @param string $url
   *   A URL
   *
   * @return string
   *   The MD5 hash for the URL.
   */
  static public function hash($url) {
    return md5($url);
  }

  /**
   * Convert a relative URL to an absolute URL.
   *
   * This function will randomly fail unless the PECL HTTP extension is
   * available.
   *
   * @param string|array $base_url
   *   The base URL or array parts.
   * @param string|array $url_parts
   *   The relative URL.
   * @param int $flags
   *   Any flags. Defaults to HTTP_URL_REPLACE
   *
   * @return string
   *   An absolute URL.
   */
  static public function relativeToAbsolute($base_url, $url_parts, $flags = HTTP_URL_JOIN_PATH) {
    if (function_exists('http_build_url')) {
      return http_build_url($base_url, $url_parts, $flags);
    }
    else {
      return self::httpBuildUrl($base_url, $url_parts, $flags);
    }
  }

  /**
   * Break a path into an array of path parts.
   *
   * @param string $path
   *   A valid path.
   *
   * @return array
   *   An array of parts, possibly empty.
   */
  static public function pathArray($path) {
    $path = trim($path, '/');

    if (!empty($path)) {
      $path_array = explode('/', $path);
      return $path_array;
    }

    return array();
  }
}
