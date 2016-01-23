<?php

/**
 * @file
 * Abstract class that should be extended by modules
 * wishing to implement a robust api based on the wsif
 * interface.
 */

/**
 * An abstract interface for the creation of a robust
 * API via the wsif module.
 */
interface WSIFInterface {

  /**
   * Check the connection to the api.
   *
   * @return bool
   *   FALSE if the connection failed, otherwise the service is assumed to be
   *   up. If this function throws a WSIFUnavailableException then FALSE is also
   *   assumed.
   *
   * @see wsif_cron()
   */
  public function wsifCheckConnection();

  /**
   * Provide the current version of your API here.
   *
   * @return string
   *   Version information.
   */
  public function wsifGetVersion();

}
