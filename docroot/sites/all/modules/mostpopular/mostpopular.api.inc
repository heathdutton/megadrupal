<?php
// $Id$
/*
 * Drupal Most Popular - Showcase the most popular content across your Drupal website and engage your audience.
 * Copyright © 2009-2012 New Signature
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * You can contact New Signature by electronic mail at labs@newsignature.com -or- by U.S. Postal Service at 1100 H St. NW, Suite 940, Washington, DC 20005.
 */

/**
 * @file
 * Provides examples of the hooks and callbacks that can be implemented to add
 * new services to the Most Popular framework.
 */

/**
 * Defines one or more Most Popular services provided by this module.
 * 
 * @return array
 *   A keyed array of most popular service definitions, each of which can contain:
 *   - name: The name of the service, to display to admins.
 *   - title: The default title to display on the tab for this service.  This can be overridden in the admin interface.
 *   - entity_types: If this is set, admins can choose which types of entities and bundles can be returned by this service.
 *       If set to TRUE, admins can choose from among all the entity types.  If set to an array of entity types, admins can
 *       only choose from amongst the bundles of those types.
 *       
 *   - file: Optionally, the full path of a file to load before attempting to invoke callbacks. 
 *   - callbacks: Optionally, an array of callback functions to use for this service.  If a callback function is not specified
 *       in this array, the system will attempt to find one by prepending the module name to the callback name and if possible
 *       appending the key (delta) of this service.
 *       If no callback could be found and a default exists, the default will be used.
 *       In other words, callbacks are found in the following order:
 *       - $info['callbacks'][$callback]
 *       - $module_$callback_$delta
 *       - $module_$callback
 *       - mostpopular_default_$callback
 *       
 *       The available callbacks are:
 *       - refresh: Refreshes the most popular data for this service.  This callback must exist and is where all the interesting
 *          things happen for this module.
 *       - config_form: Provides additional elements to the array for configuring a service.
 *       - next_run: Returns the timestamp of the next time the service should run.  If not provided, a default will be used.
 */
function hook_mostpopular_service_info() {
  $info = array();
  
  $info['viewed'] = array(
    'name' => t('Drupal Most Viewed Nodes'),
    'title' => t('Viewed'),
		'entity_types' => array('node'), // This service can only return nodes.
    'callbacks' => array(
      'refresh_viewed' => 'callback_mostpopular_refresh_viewed',
      'next_run' => 'callback_mostpopular_next_run',
    ),
  );
  
  $info['commented'] = array(
    'name' => t('Drupal Most Commented Nodes'),
    'title' => t('Commented'),
		'entity_types' => TRUE, // This service can return any type of entity.
  );
  return $info;
}

/**
 * Implements the 'refresh_$delta' callback.
 * 
 * @param object $service 
 *   The service definition.
 * @param object $block 
 *   The block definition. 
 * @param integer $span 
 *   The number of seconds over which to search.
 * @param integer $last_run 
 *   The timestamp of the last time this service was run.
 * 
 * @return boolean|array
 *   If there was a failure, this function should return FALSE.
 *   Otherwise, return an array of mostpopular_item objects with the following keys:
 *     - entity_type: the type of entity, if applicable.
 *     - entity_id: the ID of the entity, if applicable.
 *     - title: the title of the entity.
 *     - url: the external URL of the entity.
 *     - path: the internal Drupal path of the entity.
 *     - count: the number of times the entity was referenced.
 */
function callback_mostpopular_refresh_viewed($service, $block, $span, $last_run) {
  $ts = time() - $span;
  $limit = $block->count;
  
  // Get a set of URLs and counts
  try {
    $data = mymodule_get_data($ts);
  }
  catch (Exception $ex) {
    // Ther was an 
    return FALSE;
  }
  $out = array();
  if (!empty($data)) {
    foreach ($data as $v) {
      $count = $v['shares'];
      $url = $v['url'];
  
      // Match the URL to an existing entity
      $obj = mostpopular_match_result_nodes($url, $count, $service->data);
      if (isset($obj)) {
        $out[] = $obj;
      }
  
      // Return only the first N results
      if (count($out) >= $limit) {
        break;
      }
    }
  }
  return $out;
}

/**
 * Implements the 'next_run' callback.
 *
 * Returns the timestamp at which to next refresh the data for this interval.
 *
 * @param object $service The service definition
 * @param integer $span The number of seconds representing the current interval.
 * @param integer $last_run The timestamp at which this service was last run for this interval.
 */
function callback_mostpopular_next_run($service, $span, $last_run) {
  // If the interval is 2 days or less, refresh once per hour
  if ($span <= 60 * 60 * 24 * 2) {
    return strtotime('1 hour', $last_run);
  }
  // If the interval is 1 year or more, refresh once per week
  elseif ($span >= 60 * 60 * 24 * 365) {
    return strtotime('1 week', $last_run);
  }
  // Otherwise, refresh once per day.
  return strtotime('1 day', $last_run);
}