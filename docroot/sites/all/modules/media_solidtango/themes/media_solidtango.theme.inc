<?php

/**
 * @file
 * Theme and preprocess functions for Media: Solidtango.
 */

/**
 * Preprocess function for theme('media_solidtango_video').
 */
function media_solidtango_preprocess_media_solidtango_video(&$variables) {
  // Build the URI.
  $wrapper = file_stream_wrapper_get_instance_by_uri($variables['uri']);
  $parts = $wrapper->get_parameters();
  $variables['video_id'] = $parts['slug'];
  $variables['host'] = $parts['host'];

  // Make the file object available.
  $file_object = file_uri_to_object($variables['uri']);

  // Parse options and build the query string. Only add the option to the query
  // array if the option value is not default. Be careful, depending on the
  // wording in media_solidtango.formatters.inc, TRUE may be query=0.
  $query = array();

  // Non-query options.
  if ($variables['options']['protocol_specify']) {
    $protocol = $variables['options']['protocol'];
  }
  else {
    $protocol = '//';
  }
  
  if ($variables['options']['enablejsapi']) {
    $query['api'] = 'true';
    $id = drupal_html_id('media-solidtango-' . $variables['video_id']);
    //THIS NEEDS WORK
    // setting var to_origin and iframe_id in $(document).ready( function is not working
    
    $js = '
    
    	// Send/recieve messages from/to
	   var to_origin = "'. $variables['options']['origin'] . '"

	   // iframe ids
	   var iframe1 = "' . $id . '";
	
  /* Variables */

	// Player ready for commands
	var ready = false;

	// Player playing
	var playing = false;

	// Current position in seconds
	var position = 0;

	// Check if player is ready for actions
	var status_timer;

	// Runtime timer
	var runtime;

	// Status update interval
	var status_update_interval = 1000;

	// UI update interval
	var ui_update_interval = 1000;
  
  // Recieve message from iframe
function receiveMessage(event){
  var data = JSON.parse(event.data);

  switch(data.func){
    case "ready":
      ready = data.args;
    case "position":
      position = data.args;
    break;
    case "playing":
      playing = data.args;
    break;
    default:
      console.log(event);
    break;
  }
}

// Send message to iframe
function sendMessage(iframe_id, func, args){
  
  var message = {
    "event" : "message",
    "func"  : func
  };
  if(args){ message[\'args\'] = args }
  var message = JSON.stringify(message);

  document.getElementById(iframe_id).contentWindow.postMessage(
    message, to_origin
  );
}

// this should be the only js added inline so why can dynamically set the to_origin and iframe1
  (function ($) {
    $(document).ready(function(){
			// Send/recieve messages from/to
			var to_origin = "'. $variables['options']['origin'] . '"

			// iframe ids
			var iframe1 = "' . $id . '";

			// Recieve message event listener
			window.addEventListener(
				"message", 
				receiveMessage,
				false
			);
    });
  }(jQuery));';

  drupal_add_js($js, 'inline');
  }
  // Add some options as their own template variables.
  foreach (array('width', 'height') as $themevar) {
    $variables[$themevar] = $variables['options'][$themevar];
  }

  // Do something useful with the overridden attributes from the file
  // object. We ignore alt and style for now.
  if (isset($variables['options']['attributes']['class'])) {
    if (is_array($variables['options']['attributes']['class'])) {
      $variables['classes_array'] = array_merge($variables['classes_array'], $variables['options']['attributes']['class']);
    }
    else {
      // Provide nominal support for Media 1.x.
      $variables['classes_array'][] = $variables['options']['attributes']['class'];
    }
  }
  
  // Add template variables for accessibility.
  $variables['title'] = check_plain($file_object->filename);
  // @todo Find an easy and not too expensive way to get the Solidtango description
  //   to use for the alternative content.
  $variables['alternative_content'] = t('Video of @title', array('@title' => $variables['title']));

  // Build the iframe URL with options query string.
  $variables['url'] = url($protocol . $variables['host'] . '/widgets/embed/' . $variables['video_id'], array('query' => $query, 'external' => TRUE));
}
