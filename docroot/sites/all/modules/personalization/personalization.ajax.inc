<?php
/**
 * @file
 * This file contains the AJAX end points for the module
 */

include_once "personalization.tracker.inc";

/**
 * Receives the AJAX call that runs from the dom ready event of the page.
 */
function personalization_ajax_init() {
  $response = array(
    'success' => FALSE,
    'track' => personalization_trackable(),
    'get_location' => FALSE,
  );

  if ($response['track']) {
    // Do we need the location?
    $location = personalization_get_user_latlon();
    if (!$location) {
      $response['get_location'] = TRUE;
    }

    // Track the page.
    if (isset($_POST['nid']) && is_numeric($_POST['nid'])) {
      personalization_track_page((int) $_POST['nid']);
    }

    // Track internal site search.
    if (isset($_POST['path'])) {
      $url = parse_url($_POST['path']);
      if (isset($url['path'])) {
        // Trim the slashes and get the path sections as an array.
        $url['path'] = explode('/', trim($url['path'], '/'));
      }
      $keywords = array();

      // Get the search keywords from rewrite rule.
      if (isset($url['path']) && $url['path'][0] == 'search' && isset($url['path'][1]) && $url['path'][1] == 'node' && isset($url['path'][2])) {
        $keywords = explode(' ', trim($url['path'][2]));
      }
      // Get the search keywords from GET param.
      elseif (isset($url['query'])) {
        parse_str($url['query'], $query);
        if (isset($query['q'])) {
          $query = explode('/', trim($query['q'], '/'));
          if ($query[0] == 'search' && isset($query[1]) && $query[1] == 'node' && isset($query[2])) {
            $keywords = explode(' ', trim($query[2]));
          }
        }
      }

      // Score the keywords.
      foreach ($keywords as $key) {
        personalization_track_search($key);
      }
    }

    // Track Google and Bing searches.
    personalization_track_external_search();

    $response['success'] = TRUE;
  }

  return drupal_json_output($response);
}

/**
 * Autocomplete endpoint for the term field on the "Add mapping" form.
 *
 * Results are limited to the vocabularies set under "General settings"
 *
 * @param string $str
 *   The search string
 */
function personalization_ajax_admin_search_terms($str = NULL) {
  $matches = array();
  $vocabularies = array();
  $settings = variable_get('personalization_settings', array());

  foreach ($settings['pz_vocabularies'] as $v) {
    if ($v) {
      $vocabularies[] = $v;
    }
  }

  if ($vocabularies) {
    $result = db_select('taxonomy_term_data', 't')->condition('vid', $vocabularies, 'IN')->fields('t', array(
      'tid',
      'name',
      'vid',
    ))->condition('name', '%' . $str . '%', 'LIKE')->range(0, 10)->execute();

    foreach ($result as $term) {
      $matches[check_plain($term->name) . ' (tid=' . $term->tid . ')'] = check_plain($term->name) . ' (tid=' . $term->tid . ')';
    }
  }

  return drupal_json_output($matches);
}

/**
 * Records the users location into the database when sent by the AJAX request.
 */
function personalization_ajax_user_location() {
  $uid = personalization_get_user();

  if (isset($_POST['latitude']) && $_POST['latitude'] && isset($_POST['longitude']) && $_POST['longitude']) {
    db_delete('personalization_user_locations')->condition('user', $uid, '=')->execute();

    // Add the record to the database.
    db_insert('personalization_user_locations')->fields(array(
      'user' => $uid,
      'latitude' => $_POST['latitude'],
      'longitude' => $_POST['longitude'],
      'changed' => REQUEST_TIME,
    ))->execute();

    personalization_user_location_score(personalization_get_user_location());

    return drupal_json_output(array(
      'success' => TRUE,
    ));
  }

  return drupal_json_output(array(
    'success' => FALSE,
  ));
}

/**
 * Returns the content listing to the user.
 *
 * @param string $type
 *   What we're rendering, e.g. block.
 * @param Int $limit
 *   The maximum nuber of items to return.
 */
function personalization_ajax_get_list($type, $limit = 10) {
  switch ($type) {
    case 'block':
      // Get the rendered block with this user's personalized content.
      $block = block_load('personalization', 'personalized_content');
      $block_content = _block_render_blocks(array(
        $block,
      ));
      $build = _block_get_renderable_array($block_content);
      die(drupal_render($build));
      break;

    case 'page':
      // Get the rendered HTML list with this user's personalized content.
      if (isset($_GET['page']) && is_numeric($_GET['$page'])) {
        $active_page = (int) $_GET['page'];
      }
      else {
        $active_page = 0;
      }

      // Grab the paginated listing.
      $page = personalization_paginated_list($limit, ($active_page * $limit), FALSE, TRUE);

      // Get the correct url without the page param. We're doing this because
      // the Drupal pager takes the current URL, this means the pager links
      // would have the AJAX url.
      preg_match('|(.*?)(&?)page=([0-9])|', $_SERVER['HTTP_REFERER'], $matches);
      if (isset($matches[1])) {
        $path = $matches[1];
      }
      else {
        $path = $_SERVER['HTTP_REFERER'];
      }

      // Add the correct query character.
      if (strpos($path, '?') !== FALSE) {
        $path .= '&';
      }
      else {
        $path .= '?';
      }

      // Replace the url apart from page number.
      $pattern = '|href="(.*?)page=([0-9])"|';
      $replace = 'href="' . $path . 'page=$2"';

      $page = preg_replace($pattern, $replace, $page);

      // Replace the url without page number.
      $url = url($_GET['q']);
      $new_url = rtrim(preg_replace('/([?&])page=[^&]+(&|$)/', '$1', $_SERVER['HTTP_REFERER']), '&');

      $page = str_replace($url, $new_url, $page);

      echo $page;
      drupal_exit();
      break;
  }
}
