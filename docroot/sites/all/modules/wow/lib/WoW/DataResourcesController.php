<?php

/**
 * @file
 * Definition of DataResourcesController.
 */
abstract class WoWDataResourcesController extends EntityAPIController {

  public function fetchAll($region, $locale = NULL) {
    // Call the API with the corresponding parameters for the requested data resource.
    $query['locale'] = empty($locale) ? wow_service_default()->locale : $locale;
    return $this->request($region, $this->remotePath(), $query);
    // TODO: RemoteEntityController, CacheValidationEntityController, CacheExpirationEntityController.
  }

  /**
   * Perform an HTTP GET request.
   *
   * Adds automatically region settings according to website configuration.
   *
   * @param string $region
   *   The region to use for API call.
   * @param string $path
   *   Resource URL being linked to. It is the responsibility of the caller to url
   *   encode the path: http://$host/api/wow/$path.
   * @param array $query
   *   (Optional) An array of query key/value-pairs (without any URL-encoding) to
   *   append to the URL.
   *   - locale: You can specify your own locale here.
   *     It it the responsibility of the caller to pass a valid locale.
   *     Default to the global $language_content->language.
   *     @see wow_api_locale()
   * @param array $options
   *   (Optional) An array that can have one or more of the following elements:
   *   - headers: An array containing request headers to send as name/value pairs.
   *   - method: A string containing the request method. Defaults to 'GET'.
   *   - data: A string containing the request body, formatted as
   *     'param=value&param=value&...'. Defaults to NULL.
   *   - max_redirects: An integer representing how many times a redirect
   *     may be followed. Defaults to 3.
   *   - timeout: A float representing the maximum number of seconds the function
   *     call may take. The default is 30 seconds. If a timeout occurs, the error
   *     code is set to the HTTP_REQUEST_TIMEOUT constant.
   *   - context: A context resource created with stream_context_create().
   *
   *  @return WoWHttpResponse
   *    The Service response in the form of a WoWHttpResponse object.
   *
   * @see wow_http_request().
   */
  protected function request($region, $path, array $query = array(), array $options = array()) {
    return wow_http_request($region, $path, $query, $options);
  }

  /**
   * Return the entity remote path relative to wow/api/ in the service.
   */
  abstract public function remotePath();

}
