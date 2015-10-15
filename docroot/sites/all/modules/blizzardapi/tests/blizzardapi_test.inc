<?php

/**
 * @file
 * Testing classes that extend the actual API classes in order to redirect
 * requests to this testing module.
 */

/**
 * Provides information on the most recent batch of current auction data.
 */
class BlizzardApiWowAuctionTest extends BlizzardApiWowAuction {
  /**
   * Overrides BlizzardApi::getApiHost().
   */
  protected function getApiHost() {
    // Redirect to the testing module.
    return url('blizzardapi', array('absolute' => TRUE));
  }
  
  /**
   * Returns a fully formed URL to the requested API.
   */
  public function getUrl() {
    $this->buildUrl();
    return $this->url;
  }
}
