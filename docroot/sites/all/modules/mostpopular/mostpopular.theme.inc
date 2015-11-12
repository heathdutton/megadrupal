<?php
// $Id$
/*
 * Drupal Most Popular - Showcase the most popular content across your Drupal website and engage your audience.
 * Copyright © 2009-2012 New Signature
 * 
 * This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * You can contact New Signature by electronic mail at labs@newsignature.com �or- by U.S. Postal Service at 1100 H St. NW, Suite 940, Washington, DC 20005.
 */
/**
 * @file
 * Defines all the pages, blocks and themes for rendering the most popular
 * data to general users.
 */

function template_preprocess_mostpopular_block(&$variables) {
  $variables['attributes_array']['data-bid'] = $variables['bid'];
  
  $variables['services'] = theme('mostpopular_services', array(
    'services' => $variables['services'],
  ));
  $variables['intervals'] = theme('mostpopular_intervals', array(
    'intervals' => $variables['intervals']
  ));
}

function template_preprocess_mostpopular_services(&$variables) {
  $variables['theme_hook_suggestions'][] = 'item_list__mostpopular_services';
  $variables['theme_hook_suggestions'][] = 'item_list';
  
  $variables += array(
  	'attributes' => array(),
    'title' => NULL,
    'type' => 'ul',
    'items' => array(),
  );
  $variables['attributes']['class'][] = 'mostpopular--services';
  
  foreach ($variables['services'] as $sid => $service) {
    $variables['items'][$sid] = array(
      'data' => theme('mostpopular_service', array( 'service' => $service )),
      'data-sid' => $sid,
    );
  }
}

function theme_mostpopular_service($variables) {
  $service = $variables['service'];
  return $service->title;
}

function template_preprocess_mostpopular_intervals(&$variables) {
  $variables['theme_hook_suggestions'][] = 'item_list__mostpopular_intervals';
  $variables['theme_hook_suggestions'][] = 'item_list';

  $variables += array(
  	'attributes' => array(),
    'title' => NULL,
    'type' => 'ul',
    'items' => array(),
  );
  $variables['attributes']['class'][] = 'mostpopular--intervals';
  
  $variables['items'][] = array(
    'data' => t('Past:'),
    'class' => array('mostpopular--label'),
  );
  
  foreach ($variables['intervals'] as $iid => $interval) {
    $variables['items'][$iid] = array(
      'data' => theme('mostpopular_interval', array( 'interval' => $interval )),
      'data-iid' => $iid,
    );
  }
}

function theme_mostpopular_interval($variables) {
  $interval = $variables['interval'];
  return $interval->title;
}

function template_preprocess_mostpopular_items(&$variables) {
  $variables['theme_hook_suggestions'][] = 'item_list__mostpopular_items';
  $variables['theme_hook_suggestions'][] = 'item_list';
  
  $variables += array(
  	'attributes' => array(),
    'title' => NULL,
    'type' => 'ul',
  );
  $variables['attributes']['class'][] = 'mostpopular--items';
  
  foreach ($variables['items'] as $key => $item) {
    $variables['items'][$key] = array(
      'data' => theme('mostpopular_item', array( 'item' => $item )),
      'data-count' => $item->count,
    );
  }
  if (empty($variables['items'])) {
    $variables['theme_hook_suggestions'][] = 'mostpopular_items__none';
  }
}

/**
 * Themes a list with no most popular items.
 *
 * @ingroup themeable
 */
function theme_mostpopular_items__none($variables) {
  return '<p class="mostpopular--no-results">'.t('No results found').'</p>';
}

/**
 * Themes an individual entry in the most popular results.  The $item includes
 * a title, url and a count of the number of times it appears within the interval.
 *
 * By default, this theme function returns HTML of the form:
 *
 * <a href='$item->url'>
 *   <span class='title'>$item->title</span>
 *   <span class='count'>($item->count times)</span>
 * </a>
 *
 * If variable mostpopular_show_count is set to false, the
 *   <span class='count'>...</span> part will not appear.
 *
 * * @param array An array containing:
 *   - item: A single most popular item to show.  It will have at least the following:
 *     - title: The title of the page
 *     - url: The URL of the page
 *     - count: The number of times the page was viewed.
 *   - sid: The service ID of the currently-selected service.
 *   - iid: The interval ID of the currently-selected interval.
 *
 * @ingroup themeable
 */
function template_preprocess_mostpopular_item(&$variables) {
  $item = $variables['item'];
  
  if (!empty($item->entity_type) && !empty($item->entity_id)) {
    $variables['entity'] = reset(entity_load($item->entity_type, array($item->entity_id)));
  }
  
  if (variable_get('mostpopular_show_count', 1)) {
    $variables['show_count'] = true;
  }
}
