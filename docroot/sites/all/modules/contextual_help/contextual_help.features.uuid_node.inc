<?php
/**
 * @file
 * contextual_help.features.uuid_node.inc
 */

/**
 * Implements hook_uuid_features_default_content().
 */
function contextual_help_uuid_features_default_content() {
  $nodes = array();

  $nodes[] = array(
  'uid' => 1,
  'title' => 'Adding contextual help',
  'log' => '',
  'status' => 1,
  'comment' => 0,
  'promote' => 0,
  'sticky' => 0,
  'vuuid' => 'f9d65f70-c9b1-471d-9ce2-4dc9135b60ea',
  'type' => 'contextual_help',
  'language' => 'und',
  'created' => 1381095510,
  'tnid' => 0,
  'translate' => 0,
  'uuid' => '3e602083-347c-4197-b908-a351c9e19498',
  'revision_uid' => 1,
  'field_context_path' => array(
    'und' => array(
      0 => array(
        'url' => 'node/add/contextual-help',
        'title' => NULL,
        'attributes' => array(),
      ),
    ),
  ),
  'field_contextual_help_text' => array(
    'und' => array(
      0 => array(
        'value' => 'Adding new contextual help is as simple as creating a new node of the \'Contextual help\' content type.

The \'context path\' field specifies the path on which a given piece of contextual help should be listed. It will then automatically appear in the \'Contextual help\' block.',
        'summary' => '',
        'format' => 'plain_text',
        'safe_value' => '<p>Adding new contextual help is as simple as creating a new node of the &#039;Contextual help&#039; content type.</p>
<p>The &#039;context path&#039; field specifies the path on which a given piece of contextual help should be listed. It will then automatically appear in the &#039;Contextual help&#039; block.</p>
',
        'safe_summary' => '',
      ),
    ),
  ),
  'path' => array(
    'pathauto' => 1,
  ),
  'name' => 'admin',
  'picture' => 0,
  'data' => 'b:0;',
  'pathauto_perform_alias' => FALSE,
  'date' => '2013-10-06 23:38:30 +0200',
);
  return $nodes;
}
