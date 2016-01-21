<?php

/**
* The variables service class.
*/
class Provision_Service_variables extends Provision_Service {
  public $service = 'variables';

  /**
   * Add the properties to the site context.
   */
  static function subscribe_site($context) {
    $context->setProperty('hosting_variables_variables');
  }
}
