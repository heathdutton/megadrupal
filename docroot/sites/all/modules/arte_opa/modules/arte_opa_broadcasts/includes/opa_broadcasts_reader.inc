<?php

/**
 * @file
 * OPA Broadcasts reader
 */

/**
 * An iterator over an OPA broadcast resource.
 */
class OPABroadcastsReader extends OPAReader {

  /**
   * Initialize the members.
   */
  public function __construct($opa, $params) {
    parent::__construct($opa, $params, 'id', 'broadcastProgrammings');
  }

}
