<?php

/**
 * @file
 * This is the WSIF extension wrapper for the Responsys PHP API.
 */

class ResponsysWrapper extends ResponsysAPI implements WSIFInterface {

  /**
   * Check the connection to the api.
   *
   * @return bool
   *   TRUE if connection is OK or FALSE otherwise.
   *   Note that for slow connections this may time
   *   out and throw an Exception which should also
   *   be accounted for.
   *
   * @see wsif_cron()
   */
  public function wsifCheckConnection() {
    try {
      $response = $this->listFolders();
      return TRUE;
    }
    catch (Exception $e) {
      watchdog_exception('responsys_autoconnect', $e);
    }

    return FALSE;
  }

  /**
   * Provide the current version of your API here.
   *
   * @return string
   *   Version information.
   */
  public function wsifGetVersion() {
    // There is no API call for this, the docs this was
    // created from show the API version to be 6.10.
    return 'Interact version 6.10';
  }


}
