<?php
/**
 * @file
 * Storypal's batch related functions.
 */

/**
 * Check if the type of node to create is valid.
 */
function storypal_type_check($type, $url, &$context) {
  $context['message'] = 'Checking entity type validity...';

  $types = node_type_get_types();
  if (!isset($types[$type])) {
    $context['results'][] = 'Bad entity type.';
  }

  $context['finished'] = 1;
}

/**
 * Check if the URL is a valid Storify URL.
 */
function storypal_url_check($type, $url, &$context) {
  $context['message'] = 'Checking URL validity';

  $Storypal = new Storypal($url);
  if ($Storypal->isValid()) {
    $context['results']['url'] = $url;
  }
  else {
    $context['results'][] = 'Bad url from Storify.';
  }

  $context['finished'] = 1;
}

/**
 * Import data from Storified URL.
 */
function storypal_url_import($type, $url, &$context) {
  if (empty($context['results']['url'])) {
    return;
  }

  $context['message'] = 'Importing Story content...';

  $Storypal = new Storypal($context['results']['url']);
  $metadata = $Storypal->getStoryMetadata();

  if (!is_array($metadata)) {
    $context['results'][] = 'Error importing content.';
  }
  else {
    $context['results']['story'] = $metadata;
  }

  $context['finished'] = 1;
}

/**
 * Create the node from the data retrieved.
 */
function storypal_create_node($type, $url, &$context) {
  if (!is_array($context['results']['story'])) {
    return;
  }

  $context['message'] = 'Creating node...';

  $Storypal = new Storypal();
  $story = $context['results']['story'];

  $node = new stdClass();
  $node->type = $type;
  $node->title = $story['title'];
  $node->language = LANGUAGE_NONE;

  $field_name = $Storypal->getFieldName($type);
  $node->$field_name = array($node->language => array(0 => array('user' => $story['user'], 'slug' => $story['slug'], 'template' => 'default', 'noscript' => 0)));

  $node->status = 1;

  node_object_prepare($node);
  node_save($node);

  $context['finished'] = 1;
  $context['results']['processed']++;
  $context['results']['redirect'] = 'node/' . $node->nid;
}

/**
 * Last function of the batch, when it's finished.
 */
function storypal_import_finished($success, $results, $operations) {
  if ($success) {
    drupal_goto($results['redirect']);
  }
}
