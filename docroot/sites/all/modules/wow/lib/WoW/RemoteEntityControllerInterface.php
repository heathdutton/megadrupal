<?php

/**
 * @file
 * Definition of WoW\RemoteEntityControllerInterface.
 */

interface WoWRemoteEntityControllerInterface {

  /**
   * Fetch a remote resource from the service.
   *
   * @param WoWRemoteEntity $entity
   *   The entity to fetch.
   * @param array $fields
   *   (Optional) An array of fields to request.
   * @param string $locale
   *   (Optional) The locale to fetch the resource with.
   *     It it the responsibility of the caller to pass a valid locale.
   *     @see wow_api_locale()
   * @param Boolean $catch
   *   Whether to catch exceptions or not.
   *
   * @return WoWRemoteEntity
   *   An entity object.
   *
   * @throws WoWHttpException
   */
  public function fetch($entity, array $fields = array(), $locale = NULL, $catch = TRUE);

}
