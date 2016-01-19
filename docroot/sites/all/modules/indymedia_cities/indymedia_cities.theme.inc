<?php
// $Id: indymedia_cities.theme.inc,v 1.1.4.2 2010/08/14 07:38:58 mfb Exp $

/**
 * @file
 * Themeable functions for Indymedia cities module.
 */

/**
 * Given a SimpleXML object, render the HTML cities list.
 *
 * In many cases, you should be able to theme your cities list via CSS
 * without overriding these functions.  For example, to render the block
 * as a lower-case, justified paragraph in the footer, you could try:
 *   #footer {
 *     text-align: justify;
 *   }
 *   #footer .block-indymedia_cities {
 *     text-transform: lowercase;
 *   }
 *   #footer .block-indymedia_cities ul, #footer .block-indymedia_cities li, #footer .block-indymedia_cities div {
 *     padding: 0;
 *     margin: 0;
 *     display: inline;
 *   }
 *   #footer .block-indymedia_cities li:after, #footer .block-indymedia_cities li:before {
 *     content: " ";
 *   }
 */
function theme_indymedia_cities_list($variables) {
  return $variables['simplexml'] ? theme('indymedia_cities_' . $variables['style'], array('items' => indymedia_cities_parse($variables['simplexml']))) : t('To be downloaded');
}

function theme_indymedia_cities_item_list($variables) {
  return theme('item_list', array('items' => $variables['items'], 'title' => NULL, 'type' => 'ul', 'attributes' => array()));
}

function theme_indymedia_cities_fieldset($variables) {
  $output = '';
  foreach ($variables['items'] as $item) {
    if (is_array($item)) {
      $fieldset = array(
        '#attributes' => array('class' => array('collapsible', 'collapsed')),
        '#title' => $item['data'],
        '#children' => theme('item_list', array('items' => $item['children'], 'title' => NULL, 'type' => 'ul', 'attributes' => array())),
      );
      $output .= theme('fieldset', array('element' => $fieldset));
    }
    else {
      $output .= theme('item_list', array('items' => array($item), 'title' => NULL, 'type' => 'ul', 'attributes' => array()));
    }
  }
  return $output;
}

function theme_indymedia_cities_accordion($variables) {
  foreach ($variables['items'] as $key => $item) {
    if (is_array($item)) {
      $variables['items'][$key]['data'] = '<a href="#" class="indymedia_cities-header">' . $variables['items'][$key]['data'] . '</a>';
    }
  }
  return theme('item_list', array('items' => $variables['items'], 'title' => NULL, 'type' => 'ul', 'attributes' => array('class' => array('indymedia_cities-accordion'))));
}

/**
 * Cities list XML parser.
 */
function indymedia_cities_parse($simplexml) {
  $types = array();
  $regions = array();
  $items = array();
  $type_id = 0;
  $region_id = 0;
  $site_id = 0;

  // Set up the various types of IMC sites.
  foreach ($simplexml->dict->key as $type) {
    if ($type_id > 0) {
      $types[$type_id]['data'] = strval($type);
      $types[$type_id]['children'] = array();
    } 
    $type_id++;
  }

  // Set up the regions
  foreach ($simplexml->dict->array[0]->dict->key as $region) {
    if ($region_id > 0) {
      $regions[$region_id]['data'] = strval($region);
      $regions[$region_id]['children'] = array();
    } 
    $region_id++;
  }

  // Load all the "normal" local IMC sites.
  $region_id = 0;
  foreach ($simplexml->dict->array[0]->dict->array as $region) {
    $site_id = 0;
    if ($region_id == 0) {
      $items[] = l($region->dict->key[$site_id], $region->dict->string[$site_id]); 
    }
    else {
      foreach ($region->dict->key as $site) {
        $regions[$region_id]['children'][] = l($site, $region->dict->string[$site_id]); 
        $site_id++;
      }
    }
    $region_id++;
  }

  // Process sites.
  indymedia_cities_render_type($simplexml, $types, 1);

  // Project sites.
  indymedia_cities_render_type($simplexml, $types, 2);

  // Regional sites.
  $type_id = 3;
  $site_id = 0;
  foreach ($simplexml->dict->array[$type_id]->dict->array as $site) {
    $types[$type_id]['children'][] = l($site->dict->key, $site->dict->string);
    $site_id++;
  }

  // Topical sites.
  indymedia_cities_render_type($simplexml, $types, 4);

  $items = array_merge($items, $regions, $types);
  return $items;
}

/**
 * Helper function for parsing IMC types.
 */
function indymedia_cities_render_type($simplexml, &$types, $type_id) {
  $site_id = 0;
  foreach ($simplexml->dict->array[$type_id]->dict->array->dict->key as $site) {
    $types[$type_id]['children'][] = l($site, $simplexml->dict->array[$type_id]->dict->array->dict->string[$site_id]);
    $site_id++;
  }
}
