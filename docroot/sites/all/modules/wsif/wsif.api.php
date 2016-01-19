<?php

/**
 * @file
 * Describe hooks provided by the wsif module.
 */

/**
 * Describe your Web Service to WSIF.
 *
 * @return array
 *   An array with each element an array describing a web service
 *   with the following keys.
 *    'name' string the name of the service
 *    'description' string a short description
 *    'machine_name' string shortname identifying the service
 *    'extra' string any addition details to appear on the report status screen.
 */
function hook_wsif_info() {
  return array(
    'myservice' => array(
      'name' => t('My Service'),
      'description' => t('My Service integration'),
      'machine_name' => 'myservice',
      'extra' => '',
    ),
  );
}

/**
 * Factory function for constructing the service object.
 *
 * @return object
 *   The wrappered service.
 */
function hook_wsif() {
  return new MyServiceWrapper();
}
