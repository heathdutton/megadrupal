<?php

/**
 * Basic Chat class
 */
class Chat {

  /**
   * Chat Name. Human name to be displayed as markup (e.g the chat Title).
   */
  protected $name;

  /**
   * A chat needs at least a channel to (send / receive) messages (to / from).
   *
   * The channel should be generated within the class, instead of passing it
   * as an argument, to avoid calls to the channel creation function from
   * different places in different ways. That will ensure unique channels for
   * different chats, which will ultimately affect the chat behavior on the
   * browser as well, given the channels are used as CSS IDs for each possible
   * chat on the page.
   */
  protected $channel;

  /**
   * Javascript settings to be sent to the browser.
   */
  protected $settings;

  /**
   * Users on the chat, stored as User IDs
   */
  protected $user_ids;

  /**
   * Chat render array
   */
  protected $form;

  /**
   * Basic constructor, just with the socket.io channel assigned.
   */
  function __construct($channel, $name = 'Node JS Chat', $settings = array()) {
    // Make sure the channel name generated is unique within the page request.
    $unique_id = &drupal_static(__METHOD__);
    if (!isset($unique_id)) {
      $unique_id = 0;
    }
    $unique_id++;
    $unique_channel = __CLASS__ . '__' . $channel . '_' . $unique_id;

    // Some filtering to avoid unwanted characters on the channel_name.
    $this->channel = preg_replace('/[^(_|a-zA-Z0-9)]/', '_', $unique_channel);


    // Check if chat channel exists
    if (!nodejs_get_check_channel($this->channel)) {
      // It doesn't, so try to add it.
      if (nodejs_get_add_channel($this->channel)) {
        drupal_set_message(t('Chat channel created successfully'));
      }
      // In case of error, let the caller know.
      else {
        drupal_set_message(t('The Chat channel couldn\'t be created'), 'error');
        return FALSE;
      }
    }

    // Set the chat name.
    $this->name = $name;

    // Store any settings passed on to the constructor.
    $this->settings = $settings;
  }

  /**
   * Wrapper function to call sequential steps to render a functional chat.
   */
  function render() {
    // Create socket.io channel
    $this->createSocketChannel();

    // Set the list of present users.
    $this->setUserList();

    // Generate form
    $this->form = $this->renderArray();
    // Add assets to the page
    $this->loadAssets();

    // Render the form.
    return drupal_render($this->form);
  }

  /**
   * Generate render array for the chat form.
   */
  function renderArray() {
    // If the render array has already been generated, return it.
    if (isset($this->form)) {
      return $this->form;
    }
    // Otherwise, generate it.
    $form = array();

    // Connected users info. TODO: Still need improvements and better placement.
    // This shouldn't be on the renderArray(() method.
    $users_markup = theme('nodejs_chat_users', array('user_ids' => $this->user_ids));
    $form['nodejs_chat'] = array(
      '#type' => 'fieldset',
      '#title' => $this->name,
      '#attributes' => array(
         // Assuming channel comes transliterated as a valid CSS ID.
        'id' => 'nodejs_chat_' . $this->channel,
        'class' => array(
          'nodejs-chat',
        ),
      ),
    );
    $form['nodejs_chat']['user_list'] = array(
      '#markup' => '<div class="user-list">' . 'Users on the chat:' . $users_markup .  '</div>',
    );
    $form['nodejs_chat']['chat_log'] = array(
      '#markup' => '<div class="chat-log">Chat log is empty. Start writing! =): </div>',
    );
    $form['nodejs_chat']['chat_entry_box'] = array(
      '#type' => 'textarea',
      '#title' => t('Enter your message here: '),
      '#value' => '',
      '#rows' => 2,
      '#attributes' => array(
        'id' => 'chat-entry-box',
      ),
    );
    $form['nodejs_chat']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Send Message'),
    );

    return $form;
  }

  function loadAssets() {
    // Javascript Assets.
    drupal_add_js(drupal_get_path('module', 'nodejs_chat') . '/nodejs_chat.js');

    // This is not the best place to do this, but for now it does the job to
    // allow a dev to choose whether the chat messages will go through drupal,
    // or through the nodejs server extension (directly through sockets).
    if (isset($this->settings['messageTransmission']) && $this->settings['messageTransmission'] == 'drupal') {
      $this->settings['postMessagePath'] = url('nodejs-chat/message/post', array('absolute' => TRUE));
    }

    // Add general chat settings.
    $chat_settings = array(
      $this->channel => array(
        'channel' => $this->channel,
        'settings' => $this->settings,
      ),
    );
    drupal_add_js(array('nodejs_chat' => array('chats' => $chat_settings)), 'setting');

    // Add global settings. TODO: This should be overridable.
    $global_settings = array(
      'defaultAvatar' => '/' . variable_get('user_picture_default', ''),
    );
    drupal_add_js(array('nodejs_chat' => array('globalSettings' => $global_settings)), 'setting');

    // Current user settings.
    global $user;
    $current_user = array(
      'uid' => $user->uid,
      'name' => $user->name,
      'picture' => $user->picture,
    );
    drupal_add_js(array('nodejs_chat' => array('currentUser' => $current_user)), 'setting');

    // CSS Assets TODO: Make this optional, through a setting option maybe?.
    drupal_add_css(drupal_get_path('module', 'nodejs_chat') . '/css/nodejs_chat.css');
  }

  function createSocketChannel() {
    global $user;

    $message = (object) array(
      'broadcast' => FALSE,
      'channel' => $this->channel,
      'callback' => 'nodejsChatUserOnlineHandler',
      'data' => array(
        'chatroom' => $this->channel,
        'name' => $user->name,
        'subject' => 'New user connected',
        'body' => 'User ' . $user->name . ' has entered the chat',
        'user' => array(
          'uid' => $user->uid,
          'name' => $user->name,
          'picture' => $user->picture,
        ),
      ),
    );
    nodejs_send_content_channel_token($this->channel, TRUE);
    nodejs_send_content_channel_message($message);
  }

  /**
   * Sets the list of users present on the chat, making sure the current user
   * is on the list.
   */
  function setUserList() {
    global $user;
    nodejs_add_user_to_channel($user->uid, $this->channel);
    $this->user_ids = $this->getUids();

    // Make sure the current user is as added as well to the channel uids, as it
    // might not have been added by node js if the server hasn't authenticated
    // him previously.
    if (empty($this->user_ids) || !in_array($user->uid, $this->user_ids)) {
      $this->user_ids[] = $user->uid;
      sort($this->user_ids);
    }
  }

  /**
   * Returns an array with the UIDs of the users present on the channel.
   */
  public function getUids() {
    $uids_on_channel = array();
    // Check socket.io channel and return the uids in the desired format.
    if ($users_on_channel = nodejs_get_content_channel_users($this->channel)) {
      // Check how data is returned, for compatibility with all nodejs verions.
      $uids_on_channel = is_object($users_on_channel) ? $users_on_channel->uids : $users_on_channel['uids'];
    }
    return $uids_on_channel;
  }

}
