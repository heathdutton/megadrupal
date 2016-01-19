<?php

/**
 * @file
 *  Class for the CI Server entity.
 */

/**
 * The CIServerEntity is the base entity for all CI Servers.
 *
 * To extend this module with a new CI Server Type, a new entity 
 * that is a bundle of this entity is created. @see update_trigger_build_jenkins.
 * 
 * No entities of this type are created, it acts as a gateway for retrieving all 
 * of it's bundles, which are specific implementations of CI Servers, e.g. Jenkins.
 */
class CIServerEntity extends Entity {

  /**
   * Change the default URI from default/id to ci_server/id.
   * 
   * @see Entity::defaultURI()
   */
  protected function defaultURI() {
    return array('path' => 'ci_server/' . $this->identifier());
  }
}
