<?php

/**
 * @file
 * Definition of WoW\RemoteEntityController.
 */

/**
 * This extends the EntityAPIController class, adding required special handling
 * for remote entity objects.
 *
 * To extends this controller, an entity must have the following requirements:
 *   - a 'lastFetched' field, corresponding to the timestamp it was last fetched
 *   from the service, not necessary updated, but at least fetched.
 *   - a refresh function of the form $entity_type . '_refresh' which is responsible
 *   to refresh an existing entity. This function should honor If-Modified-Since
 *   headers if needed, and saves the entity if updated.
 *   Important note: you need to set explicitely the original field before saving
 *   the entity, doing that will prevent an infinite loop between save and load
 *   mechanics.
 *   @see wow_character_refresh()
 *   @see wow_guild_refresh()
 */
abstract class WoWRemoteEntityController extends EntityAPIController implements WoWRemoteEntityControllerInterface {

  /**
   * The wow entity refresh method.
   *
   * @var string
   */
  protected $refreshMethod;

  /**
   * The wow entity refresh threshold.
   *
   * @var string
   */
  protected $refreshThreshold;

  public function __construct($entityType) {
    parent::__construct($entityType);

    $this->refreshMethod = wow_entity_refresh_method($entityType);
    $this->refreshThreshold = wow_entity_refresh_threshold($entityType);
  }

  /**
   * Load an entity from the database.
   *
   * If the refresh method is set to loading time, calls the corresponding hook
   * responsible for refreshing an entity.
   */
  protected function attachLoad(&$queried_entities, $revision_id = FALSE) {
    // Get the entity refresh method.
    if ($this->refreshMethod == WOW_REFRESH_LOAD) {

      foreach ($queried_entities as $entity) {
        // Check the lastFetched value before proceding to an update.
        if ($entity->lastFetched + $this->refreshThreshold < REQUEST_TIME) {
          // Refresh the entity by calling the service.
          module_invoke($this->entityInfo['load hook'], 'refresh', $entity);
        }
      }
    }

    parent::attachLoad($queried_entities, $revision_id);
  }

  /**
   * (non-PHPdoc)
   * @see WoWRemoteEntityControllerInterface::fetch()
   */
  public function fetch($entity, array $fields = array(), $locale = NULL, $catch = TRUE) {
    // Build the query.
    $query = array();
    $query += empty($fields) ? array() : array('fields' => implode(',', $fields));
    $query += empty($locale) ? array() : array('locale' => $locale);

    // Before adding the If-Modified-Since header, first check that all the
    // fields requested are already defined in the entity. In this case we can
    // set the header.
    $options = array();
    if (isset($entity->lastModified) && $this->requireCacheValidation($entity, $fields)) {
      // Remote entities uses a cache by validation.
      $options['headers']['If-Modified-Since'] = gmdate("D, d M Y H:i:s T", $entity->lastModified);
    }

    // Call the API with the corresponding parameters for the requested entity.
    $response = $this->request($entity->region, $entity->remotePath(), $query, $options);

    // Updates the lastFetched timestamp after a request.
    $entity->lastFetched = $response->getDate()->format('U');

    if ($response instanceof WoWHttpResult) {
      // Merges the entity with the data from service.
      $this->merge($entity, $response);
      // Let the controller hook the result.
      $this->processResult($entity, $response);
    }
    elseif (FALSE === $catch) {
      throw new WoWHttpException($response);
    }
    else {
      // Let the controller hook the status.
      $this->processStatus($entity, $response);
    }

    return $entity;
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
   * Merge a result with an entity.
   *
   * @param WoWRemoteEntity $entity
   * @param WoWHttpResult $result
   */
  protected function merge($entity, $result) {
    foreach ($result->getArray() as $key => $value) {
      $entity->{$key} = $value;
    }
  }

  /**
   * Check the entity needs a cache validation header or not.
   *
   * More importantly, if a requested field is not present, the cache validation
   * header is not set.
   *
   * @param WoWRemoteEntity $entity
   * @param array $fields
   *
   * @return boolean
   *   Whether to add a cache validation header or not.
   */
  protected function requireCacheValidation($entity, $fields) {
    foreach ($fields as $field) {
      // If there is a requested field which is not present in the entity,
      // do not add the If-Modified-Since header. We'll need a full update.
      if (!isset($entity->{$field})) {
        return FALSE;
      }
    }

    return TRUE;
  }

  /**
   * Processes a result from the service.
   *
   * @param WoWRemoteEntity $entity
   *   The entity being working on.
   * @param WoWHttpResult $result
   *   The http result.
   *
   * @see WoWGuildController, WoWCharacterController
   */
  protected abstract function processResult($entity, $result);

  /**
   * Processes a status from the service.
   *
   * @param WoWRemoteEntity $entity
   *   The entity being working on.
   * @param WoWHttpStatus $status
   *   The http status.
   */
  protected function processStatus($entity, $status) {
    // Updates the lastFetched timestamp. This will avoid the trigger of a
    // refresh when deleting an entity for instance if the refresh method is
    // set at load time.
    db_update($this->entityInfo['base table'])
      ->condition($this->idKey, $entity->{$this->idKey})
      ->fields(array('lastFetched' => $entity->lastFetched))
      ->execute();

    switch ($status->getCode()) {
      case 304:
        // The status returned is 304 Not Modified.
        // The entity was not modified since the lastModified timestamp.
        break;

      case 404:
        // The status returned is 404 Not Found.
        // The entity was existing but can't be found anymore.
        // Deletes the local entity.
        $entity->delete();

      default:
        // The status code could not be handled here.
        watchdog_exception($entity->entityType(), new WoWHttpException($status));
        break;
    }
  }
}
