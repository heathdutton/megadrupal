<?php

/**
 * Add a new comment on give node.
 *
 * Endpoint: /<rest_prefix>/add/comment/<nid>
 */
function page_rest_json_add_comment($nid=0) {
	global $user, $language;
	
	// Check auth
	check_user_auth('post comments');
	
	// Check params
	$subject = request_param('subject');
	$body = request_param('body');
	if (!$nid) rest_json_response(FALSE, 'parameter', NULL, 'Missing node ID');
	
	// Load host node
	$node = node_load($nid);
	if (!$node) {
		rest_json_response(FALSE, 'comment', NULL, "Target node $nid does not exist");
	}
	if ($node->status == 0 && $node->uid != $user->uid) {
		rest_json_response(FALSE, 'comment', NULL, "Target node $nid is not published");
	}
	if ($node->comment != COMMENT_NODE_OPEN) {
		rest_json_response(FALSE, 'comment', NULL, "Target node $nid does not allow comments");
	}
	
	// Create comment
	$comment = entity_create('comment', array(
		'node_type' => 'comment_node_'.$node->type,
		'nid' => $nid,
		'uid' => $user->uid,
		'status' => 1,
		'language' => $language->language,
	));
	$commentw = entity_metadata_wrapper('comment', $comment);
	$commentw->subject = $subject;
#	$commentw->comment_body = array('value'=>$body, 'format'=>'plain_text');
	$commentw->comment_body = $body;
	
	// Let other modules attach more content
	drupal_alter('rest_json_comment_add', $commentw);
	
	// Save comment
	$commentw->save();
	
	rest_json_response(TRUE, 'comment', rest_json_comment($commentw->value()));
} // page_rest_json_add_comment

/**
 * Get JSON of single comment.
 */
function rest_json_comment($comment_or_cid, $load_node=FALSE) {
	global $rest_conf;
	
	// Load comment, if needed
	$comment = is_numeric($comment_or_cid) ? comment_load($comment_or_cid) : $comment_or_cid;
	$commentw = entity_metadata_wrapper('comment', $comment);
	
	$json = new stdClass;
	
	// Base fields
	foreach ($rest_conf['base_comment_fields'] as $field => $name) {
		$json->{$name} = rest_json_value($commentw, $field);
	}
	
	// Custom fields
	$prefix = 'field_';
	foreach (array_keys(get_object_vars($comment)) as $field) {
		// Filter custom fields only
		if (strrpos($field, $prefix, -strlen($field)) !== FALSE && isset($commentw->{$field})) {
			$name = substr($field, strlen($prefix));
			$json->{$name} = rest_json_value($commentw, $field);
		}
	}
	
	// Target node
	if ($load_node) {
		$json->node = rest_json_node($commentw->value()->nid);
	}
	
	// Let other modules attach more content
	drupal_alter('rest_json_comment', $json);
	
	return $json;
} // rest_json_comment

