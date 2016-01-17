<?php
/**
 * @file
 * Association support for comment module relationships
 */

/**
 * Implements hook_topic_map_export_association_info().
 */
function comment_topic_map_export_association_info() {

  $return = array(
    array(
      'description' => t('Parent comment'),
      'requirements' => array(
        'entities' => array('comment'),
      ),
      'definitions' => array(
        'class' => array('comment-parent_comment' => t('Comment - Parent Comment')),
        'roles' => array(
          'parent_comment' => t('Parent comment'),
        ),
      ),
      'handler' => '_topic_map_export_comment_process_comment_parent_relation',
      'handler_entity_type' => 'comment',
    ),

    array(
      'description' => t('Creator of comment'),
      'requirements' => array(
        'entities' => array('comment', 'user'),
      ),
      'definitions' => array(
        'class' => array('comment-commentor' => t('Commentor - Comment')),
        'roles' => array(
          'commentor' => t('Commentor'),
        ),
      ),
      'handler' => '_topic_map_export_comment_process_comment_commentor_relation',
      'handler_entity_type' => 'comment',
    ),

    array(
      'description' => t('Target of comment'),
      'requirements' => array(
        'entities' => array('comment', 'node'),
      ),
      'definitions' => array(
        'class' => array('comment-comment_target' => t('Comment - Target of comment')),
        'roles' => array(
          'comment_target' => t('Comment target'),
        ),
      ),
      'handler' => '_topic_map_export_comment_process_comment_target_relation',
      'handler_entity_type' => 'comment',
    ),
  );

  return $return;

}

/**
 * Creates the comment parent association.
 *
 * @see _topic_map_export_process_taxonomy_term_reference()
 * @see topic_map_export.api.php
 */
function _topic_map_export_comment_process_comment_parent_relation($topic, $comment) {

  if ($comment->pid > 0) {

    $members = array();
    $members[] = array('type' => 'comment', 'id' => $topic->entity_id);
    $members[] = array('type' => 'comment', 'id' => $comment->pid);

    return array($members);
  }
}

/**
 * Creates association between comment and the user that created the comment.
 *
 * @see _topic_map_export_sample_handler_process_taxonomy_term()
 * @see topic_map_export.api.php
 */
function _topic_map_export_comment_process_comment_commentor_relation($topic, $comment) {

  $members = array();
  $members[] = array('type' => 'comment', 'id' => $topic->entity_id);
  $members[] = array('type' => 'user', 'id' => $comment->uid);

  return array($members);

}

/**
 * Creates the association between comment and entity being commented.
 *
 * @see _topic_map_export_sample_handler_process_taxonomy_term()
 * @see topic_map_export.api.php
 */
function _topic_map_export_comment_process_comment_target_relation($topic, $comment) {

  $members = array();
  $members[] = array('type' => 'comment', 'id' => $topic->entity_id);
  $members[] = array('type' => 'node', 'id' => $comment->nid);

  return array($members);

}
