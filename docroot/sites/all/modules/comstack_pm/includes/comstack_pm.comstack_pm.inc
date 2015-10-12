<?php

/**
 * @file
 * comstack_pm.comstack_pm.inc
 */

/**
 * Implements hook_query_TAG_alter().
 */
function comstack_pm_query_comstack_pm_conversation_hide_deleted_alter(QueryAlterableInterface $query) {
  $tables = $query->getTables();
  if (!isset($tables['comstack_conversation'])) {
    return;
  }

  $account = $query->getMetaData('account');
  if (!$account) {
    global $user;
    $account = $user;
  }

  // As the user specific data is held in a different table, we need to add in
  // the condition via the alter as EFQ won't allow it.
  $query->join('comstack_conversation_user', NULL, 'comstack_conversation.conversation_id = comstack_conversation_user.conversation_id');
  $query->condition('comstack_conversation_user.uid', $account->uid);
  $query->condition('comstack_conversation_user.deleted', 0);
}

/**
 * Implements hook_query_TAG_alter().
 *
 * Alter the query to check that users are opted in to private messaging. This
 * will add a join and where condition. Rather that this were done in the
 * RESTful endpoint code but as it's and EntityFieldQuery you can't do joins.
 */
function comstack_pm_query_comstack_pm_preferences__enabled_alter(QueryAlterableInterface $query) {
  // Check we're working with a proper table, not count query.
  $tables = $query->getTables();
  if (!isset($tables['users'])) {
    return;
  }

  // Join to the preferences table.
  $query->leftJoin('user_preferences', 'upr', 'users.uid = upr.uid');

  // If we're treating NULL as true then we need to do so or'in. This is a
  // reduced version of...
  // https://github.com/lohi-io/user_preferences/blob/7.x-1.x/views/handlers/user_preferences_handler_filter_boolean.inc
  if (variable_get('comstack_pm_preferences__enabled__null_as_true', TRUE)) {
    $where = db_and()
      ->condition(db_or()
        ->condition('upr.name', 'comstack_pm_enabled', '=')
        ->isNull('upr.name')
      )
      ->condition(db_or()
        ->condition('upr.value', '1', '=')
        ->isNull('upr.value')
      );
  }
  else {
    $where = db_and()
      ->condition('upr.name', 'comstack_pm_enabled', '=')
      ->condition('upr.value', '1', '=');
  }

  $query->condition($where);
}

/**
 * Implements hook_query_TAG_alter().
 */
function comstack_pm_query_comstack_pm_separate_transcripts_alter(QueryAlterableInterface $query) {
  if (variable_get('comstack_pm_record_separate_transcripts', FALSE)) {
    // This alter run operation can and will be run against a query that gets
    // used as a countQuery. Because of this we need to check that the
    // necessary tables are present before trying to change things. We'll check
    // for {message}. This tag will still be applied as tags can apply to a
    // query and any subqueries within.
    $tables = $query->getTables();
    if (!isset($tables['message'])) {
      return;
    }

    $account = $query->getMetaData('account');
    if (!$account) {
      global $user;
      $account = $user;
    }

    $query->join('comstack_conversation_message', 'ccm', 'message.mid = ccm.mid');
    $query->condition('ccm.uid', $account->uid);

    if (!user_access('view deleted comstack messages')) {
      $query->condition('ccm.deleted', 0);
    }
  }
}

/**
 * Implements hook_entity_presave().
 *
 * When a message is updated record edits and changes to text, if not new.
 */
function comstack_pm_message_presave($message) {
  if ($message->type === 'cs_pm') {
    $wrapper = entity_metadata_wrapper('message', $message);
    // Set the updated time.
    $message->updated = REQUEST_TIME;

    if (!$message->is_new && variable_get('comstack_pm_record_message_edits', FALSE)) {
      // Compare the original text to the new text, if altered record the old
      // text and timestamp and increment the "edits" property on this message
      // entity.
    }
  }
}

/**
 * Add an entry to the message edits table.
 */
function comstack_pm_message_record_text($message) {
  $wrapper = entity_metadata_wrapper('message', $message);
  extract($wrapper->cs_pm_text->value());

  db_insert('comstack_message_edits')
    ->fields(array(
      'mid' => $message->mid,
      'timestamp' => REQUEST_TIME,
      'text' => $value,
      'format' => $format,
    ))
    ->execute();
}

/**
 * Implements hook_entity_insert().
 *
 * When a message is created add it to conversation message transcripts if
 * keeping separate version per participant.
 */
function comstack_pm_message_insert($message) {
  if ($message->type === 'cs_pm') {
    $wrapper = entity_metadata_wrapper('message', $message);
    $conversation_id = $wrapper->cs_pm_conversation->getIdentifier();

    // Chuck in the first message history entry.
    if (variable_get('comstack_pm_record_message_edits', FALSE)) {
      comstack_pm_message_record_text($message);
    }

    // Load the conversation this message belongs to.
    if (!$conversation = comstack_conversation_load($conversation_id, $message->uid)) {
      return;
    }

    // Check we're meant to record separate histories.
    if (variable_get('comstack_pm_record_separate_transcripts', FALSE)) {
      $participants = $conversation->getParticipants();

      // Add an entry to each users conversation history.
      foreach ($participants as $uid) {
        db_insert('comstack_conversation_message')
          ->fields(array(
            'conversation_id' => $conversation_id,
            'uid' => $uid,
            'mid' => $message->mid,
          ))
          ->execute();
      }
    }
  }
}

/**
 * Implements hook_entity_update().
 *
 */
function comstack_pm_message_update($message) {
  if ($message->type === 'cs_pm') {
    // If we're recording edits, save stuff.
    if (variable_get('comstack_pm_record_message_edits', FALSE)) {
      $wrapper = entity_metadata_wrapper('message', $message);
      $old_wrapper = entity_metadata_wrapper('message', $message->original);

      // Check that the text has changed.
      $array_diff = array_diff($wrapper->cs_pm_text->value(), $old_wrapper->cs_pm_text->value());
      if (!empty($array_diff)) {
        comstack_pm_message_record_text($message);
        db_query('UPDATE {message} SET edits = edits + 1 WHERE mid = :mid', array(':mid' => $message->mid));
      }
    }
  }
}

/**
 * Create a conversation.
 *
 * @param $data
 *   The data to start a new conversation with.
 *   - participants (required) An array of user IDs.
 *   - title (optional) A string to title the conversation with.
 *   - text (required) The text of the first message in the conversation.
 *   - uid (optional) The ID of the user who started this conversation.
 *
 * @return boolean
 */
function comstack_pm_new_conversation($data) {
  if (empty($data['participants']) || !is_array($data['participants']) || empty($data['text']) || !is_string($data['text'])) {
    throw new \ComstackInvalidParameterException(t('Attempted to create a new conversation with either incomplete or incorrect data.'));
  }

  // Create the conversation.
  global $user;
  $uid = isset($data['uid']) && ctype_digit((string) $data['uid']) ? (integer) $data['uid'] : $user->uid;
  $conversation = entity_create('comstack_conversation', array());

  // Wrap the entity and start setting data.
  $conversation->uid = $uid;
  $conversation->started = REQUEST_TIME;
  $conversation->updated = REQUEST_TIME;
  if (!empty($data['title'])) {
    $conversation->title = $data['title'];
  }

  $wrapper = entity_metadata_wrapper('comstack_conversation', $conversation);
  $wrapper->cs_pm_participants->set($data['participants']);
  $wrapper->cs_pm_historical_participants->set($data['participants']);
  $wrapper->cs_pm_started_by->set($uid);
  $wrapper->cs_pm_last_updated_by->set($uid);

  // Save the conversation.
  $conversation->save();

  // Create the first message.
  $conversation->reply($data['text'], $uid);

  return $conversation;
}

/**
 * Return a list of user ids who are available to a user.
 */
function comstack_pm_get_available_users($uid = NULL, $reset = FALSE) {
  $available_users = &drupal_static(__FUNCTION__, array());

  if (!isset($available_users[$uid]) || $reset) {
    if (!$uid) {
      global $user;
      $uid = $user->uid;
    }

    $account = user_load($uid);

    $query = new EntityFieldQuery();
    $query->entityCondition('entity_type', 'user')
      ->propertyCondition('status', 1)
      ->addTag('comstack_recipients')
      ->addMetaData('account', $account);

    $result = $query->execute();
    $available_users[$uid] = isset($result['user']) ? array_keys($result['user']) : array();
  }

  return $available_users[$uid];
}

/**
 * Build an EntityFieldQuery object for getting a list of conversations.
 */
function comstack_pm_conversations_query($account) {
  if (!$account) {
    global $user;
    $account = user_load($user->uid);
  }

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'comstack_conversation')
    ->fieldCondition('cs_pm_participants', 'target_id', $account->uid)
    ->addTag('comstack_conversations')
    ->addMetaData('account', $account);

  if (!user_access('view deleted comstack conversations', $account)) {
    $query->addTag('comstack_pm_conversation_hide_deleted');
  }

  return $query;
}

/**
 * Load a users conversations.
 */
function comstack_pm_load_conversations($account = NULL, $limit = 10) {
  if (!$account) {
    global $user;
    $account = user_load($user->uid);
  }

  $query = comstack_pm_conversations_query($account);
  $query->range(0, $limit);
  $result = $query->execute();
  $conversation_ids = isset($result['comstack_conversation']) ? array_keys($result['comstack_conversation']) : array();

  return !empty($conversation_ids) ? entity_load('comstack_conversation', $conversation_ids) : array();
}