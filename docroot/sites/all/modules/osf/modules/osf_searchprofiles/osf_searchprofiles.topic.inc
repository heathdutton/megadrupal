<?php

/**
 * @file Maintains a statically cached list of page topics. Uses context static cache.
 *
 * Topic lists are arrays mapping topic to priority, where a higher number is higher priority:
 *   Array(
 *     'Cancer' => 1,
 *     'Breast cancer' => 0,
 *   )
 */

/**
 * Get the topics for this page.
 */
function osf_searchprofiles_get_page_topics($rebuild = TRUE) {
  if ($rebuild) {
    $topics = osf_searchprofiles_build_topics_list();
  }
  else {
    $topics = context_get('osf_searchprofiles', 'page_topics');
  }
  return $topics ? $topics : array();
}

/**
 * Build the topics list
 */
function osf_searchprofiles_build_topics_list() {
  // Get topics from hook implementations.
  $topics = module_invoke_all('osf_searchprofiles_page_topics');
  
  // Fix priorities and order by priority.
  $topics = osf_searchprofiles_topic_priorities($topics);
  
  context_set('osf_searchprofiles', 'page_topics', $topics);
  return $topics;
}

/**
 * Implements hook_osf_searchprofiles_page_topics().
 */
function osf_searchprofiles_osf_searchprofiles_page_topics() {
  $topics = array();
  $node = menu_get_object();
  
  if (arg(0) == 'lookup') {
    // This is a search results page so just inherit the search query.
    $topics[arg(1)] = 2;
  } else if (!empty($node)) {
    // This is a node, use the terms in the Thesaurus terms field.
    $terms = $node->field_thesaurus_term[LANGUAGE_NONE];
    if (!empty($terms)) {
      foreach($terms as $term) {
        $parts = explode("#", $term['value']);
        $topics[] = str_replace("_", " ", $parts[1]);
      }
    }
  }
  return $topics;
}

/**
 * Make sure all items have priorities and arrange them in order.
 */
function osf_searchprofiles_topic_priorities($topics) {
  $fixed_topics = array();
  foreach ($topics as $key => $value) {
    if (is_numeric($key)) {
      // Means the value is the topic, and there's no priority given
      $fixed_topics[$value] = 0;
    }
    elseif (is_numeric($value)) {
      // The key is the topic and the value is the priority
      $fixed_topics[$key] = $value;
    }
  }
  
  // Arrange by priority
  asort($fixed_topics);
  return $fixed_topics;
}

