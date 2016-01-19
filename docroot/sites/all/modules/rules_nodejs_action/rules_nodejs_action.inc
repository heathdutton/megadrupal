<?php

/**
 * @file
 * Main class for rules node.js integration.
 */

class RulesNodejsAction {

  /**
   * The Action subject.
   *
   * @var string
   */
  public $subject;

  /**
   * The Action body.
   *
   * @var string
   */
  public $body;

  /**
   * Uid of user picture which should be shown in action message.
   *
   * @var int
   */
  public $picture;

  /**
   * The roles which are able to see this action messasge.
   *
   * @var array
   */
  public $roles;

  /**
   * The user uids which are able to see this action message.
   *
   * @var array
   */
  public $users;

  /**
   * The unique id for message.
   *
   * @var string
   */
  public $uniqid;

  /**
   * Class constructor.
   */
  public function __construct($subject, $body, $picture, $roles, $users) {

    // Add action message properties to object properties.
    $this->subject = $subject;
    $this->body    = $body;
    $this->picture = $picture;
    $this->roles   = $roles;

    // Generate unique id for action message.
    $this->uniqid = uniqid();

    // Generate array with uids from string.
    $this->getUids($users);

  }

  /**
   * Generate array with uids from comma separated string.
   *
   * @param string $users
   *   comma separated string with users uids.
   */
  public function getUids($users) {
    $uids = explode(',', $users);
    $this->users = array_filter(array_map('trim', $uids), 'is_numeric');
    $this->users = array_unique($this->users);
  }

  /**
   * Generate message and send to node.js.
   */
  public function send() {
    global $user;
    // Send message to node.js
    $message = (object) array(
      'uniqid' => $this->uniqid,
      'markup' => theme('rules_nodejs_view',
        array(
          'settings' => array(
            'subject' => $this->subject,
            'body' => $this->body,
            'picture' => $this->picture,
          ),
        )
      ),
      'callback' => 'nodejsAction',
      'messageAuthorUid' => $user->uid,
    );

    // If some roles are selected then the message is going to be sent to this
    // roles.
    if ($this->roles) {
      $this->sendForRoles($message);
    }
    // If some users are added then the message is going to be sent to this
    // users.
    if ($this->users) {
      $this->sendForUsers($message);
    }

  }

  /**
   * Send action message to each role or to all.
   *
   * @param object $message
   *   object with all message properties.
   */
  public function sendForRoles($message) {
    foreach ($this->roles as $rid) {
      $message->channel = 'rules_nodejs_role_' . $rid;
      nodejs_send_content_channel_message($message);

    }
  }

  /**
   * Send action message for each user.
   *
   * @param object $message
   *   object with all message properties.
   */
  public function sendForUsers($message) {
    foreach ($this->users as $uid) {
      $message->channel = 'rules_nodejs_user_' . $uid;
      nodejs_send_content_channel_message($message);
    }
  }

  /**
   * Enable channels for user.
   */
  public static function enableChannels() {
    global $user;
    if (!user_is_anonymous()) {
      nodejs_send_content_channel_token('rules_nodejs_user_' . $user->uid);
      foreach ($user->roles as $rid => $name) {
        nodejs_send_content_channel_token('rules_nodejs_role_' . $rid);
      }
    }
  }
}
