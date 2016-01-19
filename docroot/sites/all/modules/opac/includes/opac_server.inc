<?php
/**
 * @file
 * Contains OpacServer.
 */

class OpacServer extends Entity {

  /**
   * Override parent::defaultUri.
   *
   * @return array
   *   An uri callback for an opac server.
   */
  protected function defaultUri() {
    return array('path' => 'admin/config/opac/servers/manage/' . $this->identifier() . '/view');
  }
}
