<?php

/**
 * @file
 * Hooks and callback functions for rules.module integration.
 *
 * @Inspiration
 * Based heavily on privatemsg_rules.module
 */

/**
 * Implements hook_rules_action_info().
 *
 * @ingroup rules
 */
function user_notifications_rules_action_info() {
  return array(
    'rules_action_user_notifications_new' => array(
      'label' => t('Send a new notification to a user'),
      'group' => t('User Notifications'),
      'parameter' => array(
        'recipient' => array(
          'type' => 'user',
          'label' => t('Recipient'),
          'description' => t("The user who will recieve the notfication. Use PHP evaluation or tokens to find the recipient UID."),
        ),
        'actor' => array(
          'type' => 'user',
          'label' => t('Actor'),
          'description' => t("The user who triggered the notfication, the actor. Use PHP evaluation or tokens to find the actor UID.")
        ),
        'subject' => array(
          'type' => 'text',
          'label' => t('Subject')
        ),
        'body' => array(
          'type' => 'text',
          'label' => t('Body text'),
        ),
        'tag' => array('type' => 'string',
          'type' => 'text',
          'label' => t('Tag'),
          'description' => t('Unique identifier tag for this message. ie. status_like or node_comment')
        ),
      ),
    ),
  );
}

/**
 * Helper function for sending a new message.
 * NOTE This function will send to itself so rules condition is need to check that
 *
 * TODO: Need to implement formatting. Stripped it out for now as the system has
 * changed in D7.
 */
function rules_action_user_notifications_new($recipient, $actor, $subject, $body, $tag, $settings = array()) {
  //TODO: pass body to theme function so footer or anything can be added
  //$body = theme('user_notifications_message_body', $body, $recipient);

  //TODO: NEED TO IMPLEMENT BAIL OUT
  //don't send if recipient unless setting is forcing it
  //if ($recipient->uid == $actor->uid && empty($author_force)) {
  //  rules_log(t('Bailed out of notification as recipient is the same as the author.'));
  //  return;
  //}

  //fire PM notification unless disabled
  rules_log(t('Writing new notification with subject %subject to %user', array('%subject' => $subject, '%user' => $recipient->name)));
  if (!module_exists('messaging')) {
    //defaults PM
    $result = user_notifications_new_notification(array($recipient), $actor->uid, $subject, $body, NULL, $tag);
    if ($result['success']) {
      rules_log(t('New default notification sucessfully sent.'));
    }
    else {
      rules_log(t('Default notification not sent.'));
    }
  }

  //fire additional send methods
  if (module_exists('messaging') && !empty($send_methods)) {
    //loop through all send_methods and invoke message API
    $message = array(
      'subject' => $subject,
      'body'    => $body,
    );
    foreach ($send_methods as $method) {
      //TODO: check if user has disabled method
      $message['type'] = 'user_notifications_'. $method;
      messaging_message_send_user($recipient, $message, $method);
      if ($message->success) {
        rules_log(t('New notification sucessfully sent via @method method', array('@method' => $method)));
      }
      else {
        rules_log(t('Notification failed via @method method', array('@method' => $method)));
      }
    }
  }
}

/**
function rules_action_user_notifications_new_form($settings = array(), &$form) {
  $form['settings']['#format'] = isset($settings['#format']) ? $settings['#format'] : NULL;
  $form['settings']['author_force'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force notification even if recipient is the author.'),
    '#default_value' => isset($settings['author_force']) ? $settings['author_force'] : 0,
    '#description' => t("By default the module will not send notifications if the recipient is the author, check this box to override that."),
  );

  $form['settings']['send_methods_default'] = array(
    '#value' => '<strong>'. t("By default the module will create and save notifications you it's own system.") .'</strong>'
  );
  if ($methods = user_notifications_get_send_methods()) {
    $form['settings']['disable_default_notifications'] = array(
      '#type' => 'checkbox',
      '#title' => t('Disable default notifications'),
      '#default_value' => $settings['disable_default_notifications'],
      '#description' => t("Check this box to disable the default notification."),
    );
    $form['settings']['send_methods'] = array(
      '#type' => 'select',
      '#options' => $methods,
      '#title' => t('Additional Sending Methods'),
      '#multiple' => TRUE,
      '#default_value' => $settings['send_methods'],
      '#description' => t("Select additional method to send this notification with."),
    );
  }
  else {
    $form['settings']['send_methods_default']['#value'] .= '<br />'. t('To enable additional sending methods you need to install the <a href="@messaging">Messaging module</a>.', array('@messaging' => 'http://drupal.org/project/messaging'));
  }
  return $form;
}
