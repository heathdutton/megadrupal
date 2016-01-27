<?php
/**
 * @file
 * search_api_stats_block.theme.inc
 */

function theme_search_api_stats_block($vars) {
  $links = array();
  $attributes = array(
    'class' => array('search-api-stats-top-phrases'),
  );

  foreach ($vars['phrases'] as $key => $phrase) {
    $links[$key] = array(
      'title' => $key,
      'href' => $vars['path'],
      'query' => array(
        $vars['param_name'] => $key,
      ),
    );
  }

  return theme('links', array('links' => $links, 'attributes' => $attributes));
}