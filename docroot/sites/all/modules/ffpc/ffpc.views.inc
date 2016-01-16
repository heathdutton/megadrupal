<?php

/*
 * Much of the output here is based on the Apple Podcast standard.  Details
 * can be found here:
 * http://www.apple.com/itunes/store/podcaststechspecs.html
 * 
 * Note: We have a custom style plugin because we want to add custom
 * elements (e.g., cover art) as options on the feed itself.
 *
 * @todo add additional podcast elements.
*/

function ffpc_views_plugins() {
  return array(
    'module' => 'ffpc',
    'style'   => array(
      'podcast' => array(
        'title' => t('Podcast Feed'),
        'help' => t('Generates Podcast Feed from a view.'),
        'handler' => 'ffpc_plugin_style_podcast',
        'theme' => 'views_view_rss',
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'type' => 'feed',
      ),
    ),
    'row'   => array(
      'podcast' => array(
        'title' => t('Podcast Episode'),
        'help' => t('Display the node as a Podcast Episode.'),
        'handler' => 'ffpc_plugin_row_podcast',
        'uses fields' => TRUE,
        'theme' => 'views_view_row_rss',
        'base' => array('node'), // only works with 'node' as base.
        'uses options' => TRUE,
        'type' => 'feed',
      ),
    ),
  );
}