<?php

/**
 * @file
 * Rules integration by Bot Rules (botrules) module.
 */

/**
 * Implements hook_rules_action_info().
 */
function botrules_rules_action_info() {
  $actions = array(
    'botrules_action_message' => array(
      'label' => t('Send a message to IRC channel or user'),
      'group' => t('Bot'),
      'parameter' => array(
        'target' => array(
          'type' => 'text',
          'label' => t('Channel/nickname'),
        ),
        'text' => array(
          'type' => 'text',
          'label' => t('Message text'),
        ),
      ),
    ),
    'botrules_action_action' => array(
      'label' => t('Send an action to IRC channel or user'),
      'group' => t('Bot'),
      'parameter' => array(
        'target' => array(
          'type' => 'text',
          'label' => t('Channel/nickname'),
          'description' => t('Keep in mind that not all IRC clients are capable of sensibly displaying action to a user'),
        ),
        'text' => array(
          'type' => 'text',
          'label' => t('Action text'),
        ),
      ),
    ),
    'botrules_action_queued' => array(
      'label' => t('Queue a message to IRC user until they show signs of activity'),
      'group' => t('Bot'),
      'parameter' => array(
        'target' => array(
          'type' => 'text',
          'label' => t('Nickname'),
        ),
        'text' => array(
          'type' => 'text',
          'label' => t('Message text'),
        ),
      ),
    ),
    'botrules_action_get_usercount' => array(
      'label' => t('Get user count of a channel'),
      'group' => t('Bot'),
      'parameter' => array(
        'channel' => array(
          'type' => 'text',
          'label' => t('Channel'),
        ),
      ),
      'provides' => array(
        'user_count' => array(
          'type' => 'integer',
          'label' => t('User count'),
          'description' => t('Note: returns 0 if the bot is not on the channel.'),
        ),
      ),
    ),
    'botrules_action_change_nick' => array(
      'label' => t('Make bot change its nickname'),
      'group' => t('Bot'),
      'parameter' => array(
        'nickname' => array(
          'type' => 'text',
          'label' => t('Nickname'),
        ),
      ),
    ),
    'botrules_action_join_channel' => array(
      'label' => t('Make bot join a channel'),
      'group' => t('Bot'),
      'parameter' => array(
        'channel' => array(
          'type' => 'text',
          'label' => t('Channel'),
        ),
      ),
    ),
    'botrules_action_part_channel' => array(
      'label' => t('Make bot part (leave) a channel'),
      'group' => t('Bot'),
      'parameter' => array(
        'channel' => array(
          'type' => 'text',
          'label' => t('Channel'),
        ),
      ),
    ),
  );
  if (module_exists('bot_agotchi')) {
    $actions['botrules_action_modify_botagotchi_greetings'] = array(
      'label' => t('Modify Botagotchi greeting responses'),
      'group' => t('Bot'),
      'parameter' => array(
        'responses' => array(
          'type' => 'text',
          'label' => t('Greeting responses (one per line)'),
        ),
      ),
    );
    $actions['botrules_action_modify_botagotchi_feeding'] = array(
      'label' => t('Modify Botagotchi feeding responses'),
      'group' => t('Bot'),
      'parameter' => array(
        'responses' => array(
          'type' => 'text',
          'label' => t('Feeding responses (one per line)'),
        ),
      ),
    );
  }
  if (module_exists('bot_log')) {
    $actions['botrules_log_highlight'] = array(
      'label' => t('Log a highlight'),
      'group' => t('Bot'),
      'parameter' => array(
        'type' => array(
          'type' => 'list<integer>',
          'label' => t('Message type'),
          'options list' => 'botrules_log_entry_types',
        ),
        'channel' => array(
          'type' => 'text',
          'label' => t('Highlights pseudochannel'),
        ),
        'nick' => array(
          'type' => 'text',
          'label' => t('Nickname'),
        ),
        'message' => array(
          'type' => 'text',
          'label' => t('Message'),
        ),
      ),
    );
  }
  return $actions;
}

/**
 * Implements hook_rules_condition_info().
 */
function botrules_rules_condition_info() {
  $conditions = array(
    'botrules_condition_bot_on_channel' => array(
      'label' => t('Bot is on a channel'),
      'group' => t('Bot'),
      'parameter' => array(
        'channel' => array(
          'type' => 'text',
          'label' => t('Channel'),
        ),
      ),
    ),
    'botrules_condition_nick_on_channel' => array(
      'label' => t('Nickname is on a channel'),
      'group' => t('Bot'),
      'parameter' => array(
        'nickname' => array(
          'type' => 'text',
          'label' => t('Nickname'),
        ),
        'channel' => array(
          'type' => 'text',
          'label' => t('Channel'),
        ),
      ),
      'description' => t('Returns TRUE only if the bot is on the channel, too.'),
    ),
    'botrules_condition_nick_on_any_channel' => array(
      'label' => t("Nickname is on any of bot's channels"),
      'group' => t('Bot'),
      'parameter' => array(
        'nickname' => array(
          'type' => 'text',
          'label' => t('Nickname'),
        ),
      ),
    ),
  );
  return $conditions;
}

/**
 * Implements hook_rules_event_info().
 */
function botrules_rules_event_info() {
  $events = array(
    'botrules_event_got_channel_message' => array(
      'label' => t('Received a channel message'),
      'group' => t('Bot'),
      'variables' => array(
        'channel' => array(
          'type' => 'text',
          'label' => t('Channel'),
        ),
        'nickname' => array(
          'type' => 'text',
          'label' => t('Nickname'),
        ),
        'message' => array(
          'type' => 'text',
          'label' => t('Message'),
        ),
      ),
    ),
    'botrules_event_got_private_message' => array(
      'label' => t('Received a private message'),
      'group' => t('Bot'),
      'variables' => array(
        'nickname' => array(
          'type' => 'text',
          'label' => t('Nickname'),
        ),
        'message' => array(
          'type' => 'text',
          'label' => t('Message'),
        ),
      ),
    ),
    'botrules_event_user_joined_channel' => array(
      'label' => t('User joined channel'),
      'group' => t('Bot'),
      'variables' => array(
        'nickname' => array(
          'type' => 'text',
          'label' => t('Nickname'),
        ),
        'channel' => array(
          'type' => 'text',
          'label' => t('Channel'),
        ),
      ),
    ),
  );
  return $events;
}

/**
 * The action function for 'botrules_action_message'.
 *
 * @param string $target
 *   Target of the action (nickname or channel).
 * @param string $text
 *   Body of the message.
 */
function botrules_action_message($target, $text) {
  // Typecast and check data received.
  $target = (string) $target;
  $text = (string) $text;
  if ($target == '' || $text == '') {
    if (variable_get('botrules_log_invalid', TRUE) == TRUE) {
      watchdog(
        'botrules',
        "Attempt to send invalid IRC message '%message' to target '%target'",
        array(
          '%message' => $text,
          '%target' => $target,
        )
      );
    }
    return;
  }
  global $irc;
  // Rule fired from a bot event, $irc is directly available.
  if (is_object($irc)) {
    bot_message($target, $text);
  }
  // Rule fired from elsewhere, queue the message to be released during
  // cron_fastest run (every 15 seconds).
  else {
    botrules_queue($target, $text);
  }
}

/**
 * The action function for 'botrules_action_action'.
 *
 * @param string $target
 *   Target of the action (nickname or channel).
 * @param string $text
 *   Text of the action.
 */
function botrules_action_action($target, $text) {
  // Typecast and check data received.
  $target = (string) $target;
  $text = (string) $text;
  if ($target == '' || $text == '') {
    if (variable_get('botrules_log_invalid', TRUE) == TRUE) {
      watchdog(
        'botrules',
        "Attempt to send invalid IRC action '%message' to target '%target'",
        array(
          '%message' => $text,
          '%target' => $target,
        )
      );
    }
    return;
  }
  global $irc;
  // Rule fired from a bot event, $irc is directly available.
  if (is_object($irc)) {
    bot_action($target, $text);
  }
  // Rule fired from elsewhere, queue the message to be released during
  // cron_fastest run (every 15 seconds).
  else {
    botrules_queue($target, $text, 'action');
  }
}

/**
 * The action function for 'botrules_action_get_usercount'.
 *
 * @param string $channel
 *   Channel name.
 */
function botrules_action_get_usercount($channel) {
  global $_botrules_user_data;
  // Rule fired from a bot event, data is directly available.
  if (is_array($_botrules_user_data)) {
    if (isset($_botrules_user_data[$channel])) {
      $user_count = count($_botrules_user_data[$channel]);
    }
    else {
      $user_count = 0;
    }
  }
  else {
    // Rule fired from elsewhere, look up cached bot data.
    $bot = cache_get('botrules_user_data');
    if (isset($bot->data[$channel])) {
      $user_count = count($bot->data[$channel]);
    }
    else {
      $user_count = 0;
    }
  }
  return array('user_count' => $user_count);
}

/**
 * The action function for 'botrules_action_queued'.
 *
 * @param string $target
 *   Target of the action (nickname or channel).
 * @param string $text
 *   Body of the message.
 */
function botrules_action_queued($target, $text) {
  // Typecast and check data received.
  $target = (string) $target;
  $text = (string) $text;
  if ($target == '' || $text == '') {
    if (variable_get('botrules_log_invalid', TRUE) == TRUE) {
      watchdog(
        'botrules',
        "Attempt to send invalid IRC message '%message' to target '%target'",
        array(
          '%message' => $text,
          '%target' => $target,
        )
      );
    }
    return;
  }
  $timestamp = '';
  $timestamp_format = variable_get('botrules_timestamp_format', 'none');
  if ($timestamp_format != 'none') {
    // Here we explicitly use time() instead of REQUEST_TIME as there is a fair
    // chance it may be called by a rule triggered by a bot event.
    $timestamp = '[' . format_date(time(), $timestamp_format) . '] ';
  }
  botrules_queue($target, $timestamp . $text, 'message', 1);
}

/**
 * The action function for 'botrules_action_change_nick'.
 *
 * @param string $nickname
 *   New nickname.
 */
function botrules_action_change_nick($nickname) {
  global $irc;
  // Bail out if we already have that nickname.
  $current_nick = variable_get('bot_nickname', 'bot_module');
  if ($current_nick == $nickname) {
    return;
  }
  // Else, proceed with changing it.
  variable_set('bot_nickname', $nickname);
  // Rule fired from a bot event, $irc is directly available and we can change
  // nickname immediately.
  if (is_object($irc)) {
    $irc->changeNick($nickname);
  }
}

/**
 * The action function for 'botrules_action_join_channel'.
 *
 * @param string $channel
 *   Channel to join.
 */
function botrules_action_join_channel($channel) {
  // Make sure channel name passed starts with #.
  if (drupal_substr($channel, 0, 1) != '#') {
    $channel = '#' . $channel;
  }
  $channels = variable_get('bot_channels', '#botwar');
  // Do not attempt to join a channel we're already on.
  if (!stripos($channels, $channel)) {
    global $irc;
    variable_set('bot_channels', $channels . ',' . $channel);
    // Rule fired from a bot event, $irc is directly available and we can join
    // channel immediately.
    if (is_object($irc)) {
      $irc->join($channel, NULL);
    }
  }
}

/**
 * The action function for 'botrules_action_part_channel'.
 *
 * @param string $channel
 *   Channel to part.
 */
function botrules_action_part_channel($channel) {
  // Make sure channel name passed starts with #.
  if (drupal_substr($channel, 0, 1) != '#') {
    $channel = '#' . $channel;
  }
  $channels = variable_get('bot_channels', '#botwar');
  // Do not attempt to part a channel we are not on.
  if (stripos($channels, $channel)) {
    global $irc;
    $channel_array = explode(',', $channels);
    $channel_array = array_diff($channel_array, array($channel));
    variable_set('bot_channels', implode(',', $channel_array));
    // Rule fired from a bot event, $irc is directly available and we can part
    // channel immediately.
    if (is_object($irc)) {
      $irc->part($channel, NULL);
    }
  }
}

/**
 * The action function for 'botrules_action_modify_botagotchi_greetings'.
 *
 * @param string $responses
 *   Response strings (newline-delimited).
 */
function botrules_action_modify_botagotchi_greetings($responses) {
  variable_set('bot_agotchi_greeting_responses', $responses);
}

/**
 * The action function for 'botrules_action_modify_botagotchi_feeding'.
 *
 * @param string $responses
 *   Response strings (newline-delimited).
 */
function botrules_action_modify_botagotchi_feeding($responses) {
  variable_set('bot_agotchi_feeding_responses', $responses);
}

/**
 * The action function for 'botrules_log_highlight'.
 *
 * @param int $type
 *   Log entry type.
 * @param string $channel
 *   Highlight pseudochannel.
 * @param string $nick
 *   Nickname this message refers to.
 * @param string $message
 *   Message body.
 */
function botrules_log_highlight($type, $channel, $nick, $message) {
  $log = new stdClass();
  $log->type      = current($type);
  $log->timestamp = time();
  $log->channel   = '#' . $channel;
  $log->nick      = $nick;
  $log->message   = $message;
  drupal_write_record('bot_log', $log);
}

/**
 * The condition function for 'botrules_condition_bot_on_channel'.
 *
 * @param string $channel
 *   Channel name.
 */
function botrules_condition_bot_on_channel($channel) {
  global $_botrules_user_data;
  // Rule fired from a bot event, data is directly available.
  if (is_array($_botrules_user_data)) {
    if (isset($_botrules_user_data[$channel])) {
      return TRUE;
    }
  }
  // Rule fired from elsewhere, look up cached bot data.
  else {
    $bot = cache_get('botrules_user_data');
    if (isset($bot->data[$channel])) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * The condition function for 'botrules_condition_nick_on_channel'.
 *
 * @param string $nick
 *   Nickname.
 * @param string $channel
 *   Channel name.
 */
function botrules_condition_nick_on_channel($nick, $channel) {
  $nick = drupal_strtolower($nick);
  global $_botrules_user_data;
  // Rule fired from a bot event, data is directly available.
  if (is_array($_botrules_user_data)) {
    if (in_array($nick, $_botrules_user_data[$channel])) {
      return TRUE;
    }
  }
  // Rule fired from elsewhere, look up cached bot data.
  else {
    $bot = cache_get('botrules_user_data');
    if (in_array($nick, $bot->data[$channel])) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * The condition function for 'botrules_condition_nick_on_any_channel'.
 *
 * @param string $nick
 *   Nickname.
 */
function botrules_condition_nick_on_any_channel($nick) {
  $nick = drupal_strtolower($nick);
  $nicks = array();
  global $_botrules_user_data;
  // Rule fired from a bot event, data is directly available.
  if (is_array($_botrules_user_data)) {
    foreach ($_botrules_user_data as $channel) {
      $nicks = array_merge($nicks, array_keys($channel));
    }
    if (in_array($nick, $nicks)) {
      return TRUE;
    }
  }
  // Rule fired from elsewhere, look up cached bot data.
  else {
    $bot = cache_get('botrules_user_data');
    foreach ($bot->data as $channel) {
      $nicks = array_merge($nicks, $channel);
    }
    if (in_array($nick, $nicks)) {
      return TRUE;
    }
  }
  return FALSE;
}
