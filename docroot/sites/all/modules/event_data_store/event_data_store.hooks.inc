<?php
/**
 * @file
 * Implementation of hooks to store data.
 */

/**
 * Implements hook_user_insert().
 */
function event_data_store_user_insert(&$edit, $account, $category) {
  if (in_array('user_registrations', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('user_registrations', 'user', $account->uid, array('name' => $account->name));
  }
}

/**
 * Implements hook_user_login().
 */
function event_data_store_user_login(&$edit, $account) {
  if (in_array('user_logins', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('user_logins', 'user', $account->uid, array('name' => $account->name));
  }
}

/**
 * Implements hook_user_logout().
 */
function event_data_store_user_logout($account) {
  if (in_array('user_logouts', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('user_logouts', 'user', $account->uid, array('name' => $account->name));
  }
}

/**
 * Implements hook_user_update().
 */
function event_data_store_user_update(&$edit, $account, $category) {
  if (in_array('users_blocked', variable_get('event_data_store_in_use', array()), TRUE) || in_array('users_activated', variable_get('event_data_store_in_use', array()), TRUE)) {
    // Check if the user was blocked.
    if ($account->original->status != $account->status && $account->status == 0) {
      event_data_store_log('users_blocked', 'user', $account->uid, array('name' => $account->name));
    }
    elseif ($account->original->status != $account->status && $account->status == 1) {
      event_data_store_log('users_activated', 'user', $account->uid, array('name' => $account->name));
    }
  }
}

/**
 * Implements hook_user_cancel().
 */
function event_data_store_user_cancel($edit, $account, $method) {
  if (in_array('users_cancelled', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('users_cancelled', 'user', $account->uid, array('name' => $account->name, 'method' => $method));
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function event_data_store_form_user_pass_alter(&$form, &$form_state, $form_id) {
  if (in_array('user_password_resets', variable_get('event_data_store_in_use', array()), TRUE)) {
    $form['#submit'][] = 'event_data_store_user_password_reset_submit';
  }
}

/**
 * React to a user having successfully requested a new password.
 */
function event_data_store_user_password_reset_submit($form, &$form_state) {
  $account = $form_state['values']['account'];
  event_data_store_log('user_password_resets', 'user', $account->uid, array('name' => $account->name));
}

/**
 * Implements hook_node_insert().
 */
function event_data_store_node_insert($node) {
  if (in_array('nodes_created', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('nodes_created', 'node', $node->nid, array('type' => $node->type));
  }
  if (in_array('nodes_created_' . $node->type, variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('nodes_created_' . $node->type, 'node', $node->nid);
  }
}

/**
 * Implements hook_node_update().
 */
function event_data_store_node_update($node) {
  if (in_array('nodes_updated', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('nodes_updated', 'node', $node->nid, array('type' => $node->type));
  }
  if (in_array('nodes_updated_' . $node->type, variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('nodes_updated_' . $node->type, 'node', $node->nid);
  }
}

/**
 * Implements hook_node_delete().
 */
function event_data_store_node_delete($node) {
  if (in_array('nodes_deleted', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('nodes_deleted', 'node', $node->nid, array('type' => $node->type));
  }
  if (in_array('nodes_deleted_' . $node->type, variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('nodes_deleted_' . $node->type, 'node', $node->nid);
  }
}

/**
 * Implements hook_comment_insert().
 */
function event_data_store_comment_insert($comment) {
  if (in_array('comments_created', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('comments_created', 'comment', $comment->cid);
  }
}

/**
 * Implements hook_comment_delete().
 */
function event_data_store_comment_delete($comment) {
  if (in_array('comments_deleted', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('comments_deleted', 'comment', $comment->cid);
  }
}

/**
 * Implements hook_file_insert().
 */
function event_data_store_file_insert($file) {
  if (in_array('files_uploaded', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('files_uploaded');
  }
}

/**
 * Implements hook_file_delete().
 */
function event_data_store_file_delete($file) {
  if (in_array('files_deleted', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('files_deleted');
  }
}

/**
 * Implements hook_search_execute().
 */
function event_data_store_search_execute($keys = NULL, $conditions = NULL) {
  if (in_array('search_executed', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('search_executed', array('keys' => $keys));
  }
}

/**
 * Implements hook_harmony_thread_insert().
 */
function event_data_store_harmony_thread_insert($thread) {
  if (in_array('harmony_threads_created', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('harmony_threads_created', 'harmony_thread', $thread->thread_id, array('type' => $thread->type, 'uid' => $thread->uid));
  }

  if (in_array('harmony_threads_created_' . $thread->type, variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('harmony_threads_created_' . $thread->type, 'harmony_thread', $thread->thread_id, array('uid' => $thread->uid));

    $wrapper = entity_metadata_wrapper('harmony_thread', $thread);
    $category_field = variable_get('harmony_core_category_field', 'field_harmony_category');
    // Check for a category field.
    if ($wrapper->__isset($category_field) && $wrapper->{$category_field}->value()) {
      $tid = $wrapper->{$category_field}->tid->value();
      event_data_store_log('harmony_threads_created_category', 'taxonomy_term', $tid, array('uid' => $thread->uid, 'thread_id' => $thread->thread_id));
    }
  }
}

/**
 * Implements hook_harmony_thread_update().
 */
function event_data_store_harmony_thread_update($thread) {
  if (in_array('harmony_threads_updated', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('harmony_threads_updated', 'harmony_thread', $thread->thread_id, array('type' => $thread->type, 'uid' => $thread->uid));
  }
  if (in_array('harmony_threads_updated_' . $thread->type, variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('harmony_threads_updated_' . $thread->type, 'harmony_thread', $thread->thread_id, array('uid' => $thread->uid));
  }
}

/**
 * Implements hook_harmony_thread_delete().
 */
function event_data_store_harmony_thread_delete($thread) {
  if (in_array('harmony_threads_deleted', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('harmony_threads_deleted', 'harmony_thread', $thread->thread_id, array('type' => $thread->type, 'uid' => $thread->uid));
  }
  if (in_array('harmony_threads_deleted_' . $thread->type, variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('harmony_threads_deleted_' . $thread->type, 'harmony_thread', $thread->thread_id, array('uid' => $thread->uid));
  }
}

/**
 * Implements hook_harmony_post_insert().
 */
function event_data_store_harmony_post_insert($post) {
  if (in_array('harmony_posts_created', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('harmony_posts_created', 'harmony_post', $post->post_id, array('uid' => $post->uid));
  }
}

/**
 * Implements hook_harmony_post_update().
 */
function event_data_store_harmony_post_update($post) {
  if (in_array('harmony_posts_updated', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('harmony_posts_updated', 'harmony_post', $post->post_id, array('uid' => $post->uid));
  }
}

/**
 * Implements hook_harmony_post_delete().
 */
function event_data_store_harmony_post_delete($post) {
  if (in_array('harmony_posts_deleted', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('harmony_posts_deleted', 'harmony_post', $post->post_id, array('uid' => $post->uid));
  }
}

/**
 * Implements hook_harmony_thread_view_tracked().
 */
function event_data_store_harmony_thread_view_tracked($thread_id) {
  global $user;
  if (in_array('harmony_thread_view_tracked', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('harmony_thread_view_tracked', 'harmony_thread', $thread_id, array('uid' => $user->uid));
  }

  if ($user->uid) {
    if (in_array('harmony_thread_view_tracked_auth', variable_get('event_data_store_in_use', array()), TRUE)) {
      event_data_store_log('harmony_thread_view_tracked_auth', 'user', $user->uid, array('thread_id' => $thread_id));
    }
  }
  else {
    if (in_array('harmony_thread_view_tracked_anon', variable_get('event_data_store_in_use', array()), TRUE)) {
      event_data_store_log('harmony_thread_view_tracked_anon', 'harmony_thread', $thread_id);
    }
  }
}

/**
 * Implements hook_comstack_conversation_insert().
 */
function event_data_store_comstack_conversation_insert($conversation) {
  if (in_array('comstack_conversations_created', variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log('comstack_conversations_created', 'comstack_conversation', $conversation->conversation_id, array('uid' => $conversation->uid));
  }
}

/**
 * Implements hook_message_insert().
 */
function event_data_store_message_insert($message) {
  $type = $message->type;
  if (in_array("message_created_$type", variable_get('event_data_store_in_use', array()), TRUE)) {
    event_data_store_log("message_created_$type", 'message', $message->mid, array('uid' => $message->uid));
  }
}
