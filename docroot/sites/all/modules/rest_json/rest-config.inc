<?php

define('JOIN_TOKEN', '--#JOIN#');
define('WHERE_TOKEN', '--#WHERE#');
define('ORDERBY_TOKEN', '--#ORDERBY#');

global $rest_conf; $rest_conf = array(

	// *** URL ***
	'rest_prefix' => 'rest', // rest prefix, e.g. "rest" to access "rest/event/list"

	// *** CACHE ***
	'cache_bin' => '', //'cache_page', // Drupal bin to store list/* and node/* caching, use '' to disable cache
	'cache_validity' => 1, // Cache validity in hours

	// *** INFINITE SCROLL ***
	'offset_param' => 'offset', // offset parameter name
	'limit_param' => 'limit', // limit parameter name
	'infinite_scroll_size' => 20, // number of items loaded at a time

	// *** IMAGES ***
	'image_style' => 'app_images', // image style for pictures

	// *** NODE FIELDS ***
	'base_node_fields' => array( // base node fields to print, besides custom field_*
		'nid' => 'nid', 
		'tnid' => 'tnid', 
		'type' => 'type', 
		'title_field' => 'display_name',
		'changed' => 'timestamp', 
		'language' => 'language', 
		'comment' => 'comment'
	),

	// *** USER FIELDS ***
	'base_user_fields' => array( // user fields to print
		'uid' => 'uid', 
		'name' => 'name', 
		'mail' => 'mail', 
		'language' => 'language',
	), 

	// *** COMMENTS FIELDS ***
	'base_comment_fields' => array( // comment fields to print
		'cid' => 'cid', 
		'pid' => 'pid', 
		'nid' => 'nid', 
		'uid' => 'uid', 
		'subject' => 'subject', 
		'changed' => 'timestamp', 
		'language' => 'language',
		'name' => 'username',
		'node_type' => 'node_type',
		'comment_body' => 'comment',
	),

);

