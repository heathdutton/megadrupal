<?php

/**
 * @file
 * Main class for Private messages with node.js.
 */

class PrivatemsgNodejs {

  /**
   * The private message thread.
   *
   * We need this information for the later usage in mini chats.
   *
   * @var array
   */
  public $thread;

  /**
   * The mini chat position.
   *
   * We need this information for save position of mini chat after reloading the
   * page.
   *
   * @var array
   */
  public $options;


  /**
   * Class constructor.
   */
  public function __construct($thread, $options, $action) {

    // Save the thread to the property of the class.
    $this->thread = $thread;

    // Check if the options are valid.
    $this->checkOptions($options, $action);
  }


  /**
   * Build HTML structure of the mini chat.
   *
   * @return json
   *   callback json.
   */
  public function buildMiniChat() {

    // Check if the user is using the current mini chat.
    if (!$this->checkMiniChat()) {
      $this->addMiniChat();
    }
    // Open channels for the mini chat.
    $channel_name = 'privatemsg_nodejs_thread_' . $this->thread['thread_id'];
    $content_tokens = array(
      $channel_name => nodejs_send_content_channel_token($channel_name)->token,
    );

    // Open channels for statuses.
    $channel_name = 'privatemsg_nodejs_status_' . $this->thread['thread_id'];
    if (variable_get('privatemsg_nodejs_message_status', TRUE)) {
      $content_tokens[$channel_name] = nodejs_send_content_channel_token($channel_name)->token;
    }

    // Take the latest messages from the thread into the mini chat.
    $this->takeLatestMessages();

    // The construction of the mini chat.
    $content = theme('privatemsg_nodejs_mini_chat', array(
      'recipient' => $this->takeParticipantsNames(),
      'thread' => $this->thread,
      'form' => drupal_get_form('privatemsg_nodejs_mini_form_' . $this->thread['thread_id'], $this->thread),
    ));

    // Output json with HTML structure of the mini chat.
    drupal_json_output(
      array(
        'data' =>
        array(
          'status' => 'success',
          'content' => $content,
          'options' => $this->options,
          'recipient' => $this->takeParticipantsNames(),
          'contentTokens' => $content_tokens,
        ),
      )
    );
  }

  /**
   * The message is sent to the private message.
   */
  public function sendMessage() {

    if ($_POST['form_id'] == 'privatemsg_nodejs_mini_form_' . $this->thread['thread_id']) {
      if (!empty($_POST['privatemsg_nodejs_mini_entry_box_' . $this->thread['thread_id']])) {

        privatemsg_new_thread(
          $this->thread['participants'],
          $this->thread['subject'],
          check_plain($_POST['privatemsg_nodejs_mini_entry_box_' . $this->thread['thread_id']]),
          array('thread_id' => $this->thread['thread_id'])
        );

        drupal_json_output(array('data' => array('status' => 'success')));
      }
    }

  }

  /**
   * Add mini chat and options to the user SESSION.
   */
  public function addMiniChat() {
    global $user;
    $_SESSION['privatemsg_nodejs']['mini_chats'][$this->thread['thread_id']] = array(
      'thread_id' => $this->thread['thread_id'],
      'token' => self::getUserToken('mini_chat', $user->uid, $this->thread['thread_id']),
      'x' => $this->options['x'],
      'y' => $this->options['y'],
      'w' => $this->options['w'],
      'h' => $this->options['h'],
    );
  }


  /**
   * Call when user changed position of mini chat.
   *
   * Resaving options in user session.
   *
   * @return json
   *   callback json with status.
   */
  public function moveMiniChat() {
    $this->addMiniChat('move');
    drupal_json_output(array('data' => array('status' => 'success')));
  }


  /**
   * Call when user changed size of mini chat.
   *
   * Resaving options in user session.
   *
   * @return json
   *   callback json with status.
   */
  public function resizeMiniChat() {
    $this->addMiniChat('resize');
    drupal_json_output(array('data' => array('status' => 'success')));
  }


  /**
   * Call when user put close button.
   *
   * Delete mini chat from user session.
   *
   * @return json
   *   callback json with status.
   */
  public function deleteMiniChat() {
    if ($this->checkMiniChat()) {
      unset($_SESSION['privatemsg_nodejs']['mini_chats'][$this->thread['thread_id']]);
    }
    drupal_json_output(array('data' => array('status' => 'success')));
  }


  /**
   * Check if user has opened mini chat.
   *
   * @return Boolean
   *   Boolean TRUE or FALSE.
   */
  public function checkMiniChat() {
    $thread_id = $this->thread['thread_id'];

    if (!empty($_SESSION['privatemsg_nodejs']['mini_chats'][$thread_id])) {
      return TRUE;
    }
    return FALSE;
  }


  /**
   * Get link that opens mini chat.
   *
   * @param int $thread_id
   *   privatemsg thread id as int.
   *
   * @param array $options
   *   mini chat options as array
   *
   * @param int $recipient_uid
   *   as user uid.
   * 
   * @return string
   *   string url.
   */
  public static function getMiniChatLink($thread_id, $options, $recipient_uid) {

    if (!$options) {
      // Default options.
      $options = array('x' => 50, 'y' => 50, 'w' => 260, 'h' => 290);
    }

    $token = self::getUserToken('mini_chat', $recipient_uid, $thread_id);
    return array(
      'path' => implode('/',
        array(
          'privatemsg-nodejs',
          'mini',
          $thread_id,
          $token,
          'add',
        )
      ),
      'thread_id' => $thread_id,
      'token' => $token,
      'query' => $options,
    );
  }


  /**
   * Get token for user.
   *
   * @param int $uid
   *   as int user uid.
   * 
   * @return string
   *   hash as string.
   */
  public static function getUserToken($value, $uid, $thread_id) {
    return drupal_hmac_base64($value, $uid . $thread_id . drupal_get_private_key() . drupal_get_hash_salt());
  }


  /**
   * Check if options are valid.
   *
   * @return json
   *   if not valid - callback json with status.
   *   if valid - save options to object property.
   */
  public function checkOptions($options, $action) {

    $properties = array();
    $actions = array('add', 'move', 'resize');

    if (in_array($action, $actions)) {
      $properties = array('y', 'x', 'w', 'h');
    }
    foreach ($properties as $property) {
      if (!isset($options[$property]) || !is_numeric($options[$property])) {
        drupal_json_output(array('data' => array('accessDenied' => 'invalid options')));
        drupal_exit();
      }
    }

    $this->options = $options;
  }


  /**
   * Take latest messages from the thread for the mini chat.
   */
  public function takeLatestMessages() {

    // Count of messages can be configured on admin page.
    $count = variable_get('privatemsg_nodejs_count_messages', 5);
    if (count($this->thread['messages']) > $count) {
      $this->thread['messages'] = array_slice($this->thread['messages'], -$count);
    }
  }


  /**
   * Take participants' names for the current thread.
   *
   * @return string
   *   participants' names as string.  
   */
  public function takeParticipantsNames() {
    global $user;
    $participants = array();
    foreach ($this->thread['participants'] as $account) {
      if ($user->uid != $account->uid) {
        $participants[] = $account->name;
      }
    }
    return implode(', ', $participants);
  }

}
