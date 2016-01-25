<?php

/**
 * @file
 * Contains DropboxAccountInterface
 */

namespace Drupal\fluxcontextio\Plugin\Service;

/**
 * Interface for Contextio accounts.
 */
interface ContextioAccountInterface {

  /**
   * Gets the Contextio client object.
   * The web service client for the Contextio API.
   *
   * @return \Contextio\API
   */
  public function client();
}
