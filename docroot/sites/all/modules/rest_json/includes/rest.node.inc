<?php

/**
 * Prints a JSON list of content, filtered by node type.
 *
 * Endpoint: /<rest_prefix>/list/<content-type>
 */
function page_rest_json_list($type, $filter_nid=0) {
	global $rest_conf, $language;

	// Check auth
	check_user_auth('access content');

	// Check cached requests
	if ($nodes = rest_json_cache_get()) {
		rest_json_response(TRUE, 'node', $nodes, '', TRUE);
	}

	// Infinite scroll
	$offset = intval(request_param($rest_conf['offset_param'], FALSE, 0));
	$limit = intval(request_param($rest_conf['limit_param'], FALSE, $rest_conf['infinite_scroll_size']));

	// Set filters
	$filter_sql = '';
	$tokens = array(':type'=>$type, ':lang'=>$language->language);
	if ($filter_nid) {
		$filter_sql = "AND node.nid = :nid";
		$tokens[':nid'] = $filter_nid;
	}

	// Select elements
	// TODO: use db_select()
	$sql = "SELECT node.nid"
		." FROM {node} node"
		." LEFT JOIN (SELECT tnode.nid AS nid, tnode.tnid AS tnid, tnode.status AS status, tnode.language AS language FROM {node} tnode WHERE tnode.language = 'en' AND tnode.status = 1) tnode"
		." ON node.tnid = tnode.tnid AND node.tnid != 0"
		." ".JOIN_TOKEN
		." WHERE ( node.status = 1 AND (node.tnid = 0 OR node.language = :lang OR (node.nid = node.tnid AND tnode.nid IS NULL)) AND node.type = :type $filter_sql )"
		." ".WHERE_TOKEN
		." ".ORDERBY_TOKEN
		." LIMIT $limit OFFSET $offset";
	
	// Let other modules edit the raw query
	// FIXME: better using db_select and query_TAG_alter
	$context = array('type'=>$type, 'filter_nid'=>$filter_nid, 'filter_sql'=>$filter_sql, 'limit'=>$limit, 'offset'=>$offset);
	drupal_alter('rest_json_query_node_list', $sql, $context);
	
	// Remove unused placeholders
	$sql = str_replace(JOIN_TOKEN, '', $sql);
	$sql = str_replace(ORDERBY_TOKEN, '', $sql);
	$sql = str_replace(WHERE_TOKEN, '', $sql);
	
	// All nodes by type
	$nodes = array(); $result = db_query($sql, $tokens);
	foreach ($result as $r) {
		// Get node
		$node = rest_json_node($r->nid);
		
		// Let other modules attach more content
		drupal_alter('rest_json_node_list', $node);
	
		$nodes[] = $node;
	}
	
	// Nodes list
	rest_json_cache_set($nodes);
	rest_json_response(TRUE, 'node', $nodes);
} // rest_json_list

/**
 * Prints a JSON node, selected by nid.
 *
 * Endpoint: /<rest_prefix>/node/<nid>
 */
function page_rest_json_node($nid) {
	// Check auth
	check_user_auth('access content');

	// Check cached requests
	if ($node = rest_json_cache_get()) {
		rest_json_response(TRUE, 'node', $node, '', TRUE);
	}

	// Single node
	$node = rest_json_node($nid);
	
	// Check that node exists and is published
	if (!$node) rest_json_response(FALSE, 'node', NULL, "Node $nid does not exist or is unpublished");
	
	// Let other modules attach more content
	drupal_alter('rest_json_node', $node);

	// Node
	rest_json_cache_set($node);
	rest_json_response(TRUE, 'node', $node);
} // rest_json_node

/**
 * Return the best translation, current language or default language or original.
 */
function rest_json_get_node_translation($node) {
	global $language;
	
	// Get translations
	$translations = translation_node_get_translations($node->tnid);
	if (!$translations) return $node;
	
	// Choose translation
	$cur = $def = NULL;
	$deflang = language_default();
	foreach ($translations as $lang => $t) {
		if ($lang == $language->language) { $cur = $t; break; }
		elseif ($lang == $deflang->language) $def = $t;
	}
	
	if ($cur) $node = node_load($cur->nid);
	elseif ($def) $node = node_load($def->nid);
	
	return $node;
} // rest_json_get_node_translation

/**
 * Get JSON of single node.
 */
function rest_json_node($node_or_nid) {
	global $rest_conf, $language;
	
	$j = new stdClass;
	
	$node = is_numeric($node_or_nid) ? node_load($node_or_nid) : $node_or_nid;
	if (empty($node) || !$node->status) return NULL;
	
	// Choose translation
	$node = rest_json_get_node_translation($node);
	
	// See https://www.drupal.org/documentation/entity-metadata-wrappers
	$nodew = entity_metadata_wrapper('node', $node);
	
	// Base fields
	foreach ($rest_conf['base_node_fields'] as $field => $name) {
		$j->{$name} = rest_json_value($nodew, $field);
	}
	
	// Custom fields
	$prefix = 'field_';
	foreach (array_keys(get_object_vars($node)) as $field) {
		// Filter custom fields only
		if (strrpos($field, $prefix, -strlen($field)) !== FALSE && isset($nodew->{$field})) {
			$name = substr($field, strlen($prefix));
			$j->{$name} = rest_json_value($nodew, $field);
		}
	}
	
	return $j;
} // rest_json_node

/**
 * Add a new node.
 *
 * Endpoint: /<rest_prefix>/add/node/<content-type>
 */
function page_rest_json_add_node($type) {
	global $user, $language;
	
	// Check auth
	check_user_auth('create ' . $type . ' content');
	
	// Check params
	// FIXME: how to check mandatory params?
	$title = request_param('title');
	$body = request_param('body');
	$fields = request_param('fields');
	
	// Create node
	$node = entity_create('node', array(
		'type' => $type,
		'uid' => $user->uid,
		'language' => $language->language,
	));
	$nodew = entity_metadata_wrapper('node', $node);
	if ($title) $nodew->title = $title;
	if ($body) $nodew->body = array('value'=>$body, 'format'=>'plain_text');
	
	// Add custom fields
	foreach ($fields as $k => $v) rest_json_value_set($nodew, 'field_'.$k, $v);
	
	// Let other modules attach more content
	drupal_alter('rest_json_node_add', $nodew, $title, $body, $fields);
	
	// Save node
#	d($nodew->value());
	$nodew->save();
	
	rest_json_response(TRUE, 'node', rest_json_node($nodew->value()));
} // page_rest_json_add_node

