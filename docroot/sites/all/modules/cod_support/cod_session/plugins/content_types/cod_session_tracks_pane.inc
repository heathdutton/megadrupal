<?php

/**
 * This plugin array is more or less self documenting
 */
$plugin = array(
  // the title in the admin
  'title' => t('Sessions Track Filter'),
  // no one knows if "single" defaults to FALSE...
  'single' => TRUE,
  // oh joy, I get my own section of panel panes
  'category' => array(t('Conference Organizing'), -9),
  //'edit form' => 'cod_events_pane_content_type_edit_form',
  'render callback' => 'cod_session_tracks_pane_render'
);

/**
* Run-time rendering of the body of the block (content type)
* See ctools_plugin_examples for more advanced info
*/
function cod_session_tracks_pane_render($subtype, $conf, $context = NULL) {
  // our output is generate by js. Any markup or theme functions
  // could go here.
  // that private js function is so bad that fixing it will be the
  // basis of the next tutorial
  $block = new stdClass();

  if($node = node_load(array_shift($context))) {
    $block->content = cod_session_tracks($node);
  } else {
    $block->content = t('Invalid Event Type');
  }
  return $block;
}

function cod_session_tracks($event) {
  // Get the tracks view
  $view = views_get_view('cod_session_tracks');
  $view->set_display('panel_pane_2');


  $content = '';

  return $content;
}
