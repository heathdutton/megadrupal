<?php
/**
 * @file Twitter DB plugin. 
 */
$plugin = array(
  'single' => TRUE,
  'title' => t('Twitter DB Pane'),
  'description' => t('Recent tweets.'),
  'category' => t('Miscellaneous'),
  'defaults' => array(
    'variable' => 1,
  ),
);

function twitter_db_twitter_pane_content_type_render($subtype, $conf, $panel_args, $context) {
  $return = new stdClass();
  $return->title = '';
  $content = _twitter_db_generate_content();
  $return->content = $content;
  return $return;
}