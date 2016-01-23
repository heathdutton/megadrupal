<?php

/**
 * @file
 * Hooks provided by the Shared Content module.
 */

/**
 * @addtogroup sharedcontent
 * @{
 */

/**
 * Allow to alter index records when they get created or updated.
 *
 * @param $index
 *   The index record.
 * @param $wrapper
 *   An entity wrapper object containing the entity the index is created for.
 * @param $op
 *   'create' if a new index record was created, 'update' otherwise.
 */
function hook_sharedcontent_index_alter($index, $wrapper, $op) {
  if ($wrapper->type() == 'node') {
    $node = $wrapper->value();
    $index->keywords = sharedcontent_rules_get_keywords($node, 'node');
    $index->tags = sharedcontent_rules_get_tags($node, 'node');
    switch ($node->status) {
      case NODE_PUBLISHED:
        $index->status = SHAREDCONTENT_INDEX_STATUS_VISIBLE;
        break;
      default:
        $index->status = SHAREDCONTENT_INDEX_STATUS_LINKABLE;
    }

    $terms = taxonomy_get_term_by_name($op, 'sharedcontent_reason');
    if (empty($terms)) {
      $vocabulary = taxonomy_vocabulary_machine_name_load('sharedcontent_reason');
      if (!$vocabulary) {
        $vocabulary = entity_create('taxonomy_vocabulary', array(
          'name' => 'Sharedcontent reason',
          'machine_name' => 'sharedcontent_reason',
          'description' => 'Reasons for why a shared Content Index record exists.',
          'module' => 'sharedcontent',
        ));
        taxonomy_vocabulary_save($vocabulary);
      }
      $term = entity_create('taxonomy_term', array(
        'name' => drupal_ucfirst($op),
        'description' => '',
        'parent' => array(0),
        'vid' => $vocabulary->vid,
      ));
      taxonomy_term_save($term);
    }
    else {
      $term = reset($terms);
    }

    $index->field_sharedcontent_reason = array(
      LANGUAGE_NONE => array(
        array(
          'tid' => $term->tid,
        )
      )
    );
  }
}

/**
 * Allow to alter the keywords stored in the sharedcontent_index.
 *
 * @param $keywords
 *   Array of keywords.
 * @param $entity
 *  The entity for which the keywords are built for.
 * @param $entity_type
 *  The type of the entity as string.
 */
function hook_sharedcontent_index_keywords_alter(array &$keywords, $entity, $entity_type) {
  $stop_words = array('a', 'I', 'and', 'the');
  foreach ($keywords as $index => $words) {
    if (in_array($words, $stop_words)) {
      unset($keywords[$index]);
    }
  }
}

/**
 * Allow to alter the tags stored in the sharedcontent_index.
 *
 * @param $tags
 *   Array of tags.
 * @param $entity
 *  The entity for which the tags are built for.
 * @param $entity_type
 *  The type of the entity as string.
 */
function hook_sharedcontent_index_tags_alter(array &$tags, $entity, $entity_type) {
  $stop_words = array('a', 'I', 'and', 'the');
  foreach ($tags as $index => $tag) {
    if (in_array($tag, $stop_words)) {
      unset($tags[$index]);
    }
  }
}

/**
 * @} End of "addtogroup sharedcontent".
 */