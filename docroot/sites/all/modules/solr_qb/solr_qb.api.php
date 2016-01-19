<?php
/**
 * @file
 * Hooks provided by Solr Query Builder module.
 */

/**
 * Defines one or more drivers.
 *
 * @return array
 *   An associative array whose keys are driver machine names and whose values
 *   identify driver properties.:
 *   - name: The human-readable name of the driver.
 *   - driver class: The name of the class that is used to send query to Solr
 *   and return response.
 *   - parameters callback: (optional) The name of function which will be used
 *   for providing parameters for driver class constructor.
 */
function hook_solr_qb_driver() {
  return array(
    'custom' => array(
      'name' => t('Custom Solr driver'),
      'driver class' => 'SolrQbCustomDriver',
      'parameters callback' => 'solr_qb_get_custom_parameters',
    ),
  );
}
