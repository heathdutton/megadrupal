<?php

/**
 * @file
 * A class used for entity email.
 */

/**
 * The class used for Entity Email entities.
 */
class EntityEmail extends Entity {

  /**
   * The email entity type.
   *
   * @var string
   */
  public $type;

  public $mail_from = '';

  public $mail_to = '';

  public $mail_replyto = '';

  public $mail_bcc = '';

  public $mail_cc = '';

  public $cuid;

  public $tuid;

  public $fuid;

  public $nid;

  public $subject = '';

  public $mail_body = '';

  public $mail_body_plain = '';

  public $dynamic = array();

  /**
   * The email entity timestamp.
   *
   * @var string
   */
  public $timestamp = NULL;
  
  public function __construct($values = array()) {

    // Make sure we have the uid of who receive the email.
    if (isset($values['tuid']) && is_object($values['tuid'])) {
      $values['tuid'] = $values['tuid']->uid;
    }

    // Make sure we have the uid of who send the email.
    if (isset($values['fuid']) && is_object($values['fuid'])) {
      $values['fuid'] = $values['fuid']->uid;
    }

    if (isset($values['type']) && is_object($values['type'])) {
      $values['type'] = $values['type']->name;
    }

    parent::__construct($values, 'entity_email');

    if (!isset($this->timestamp)) {
      $this->timestamp = time();
    }

    // By default the mail will be sent to the user (tuid) unless specify otherwise in the mail_to.
    if (empty($this->mail_to) && is_numeric($values['tuid'])) {
      $account = user_load($values['tuid']);
      $this->mail_to = $account->mail;
    }

    // Load email type.
    $entity_email_type = $this->getType();

    // Make sure we found the template and we have a new entity email.
    if ($entity_email_type && isset($this->is_new) && $this->is_new) {
      // Load token substitution.
      $substitutions = entity_email_substitutions($values);
      $this->subject = token_replace($entity_email_type->getSubject(), $substitutions);
      $this->mail_body = token_replace($entity_email_type->getMailBody(), $substitutions);
      $this->mail_body_plain = token_replace($entity_email_type->getMailBodyPlain(), $substitutions);
      $this->mail_bcc = token_replace($entity_email_type->getBcc(), $substitutions);
      $this->mail_cc = token_replace($entity_email_type->getCc(), $substitutions);
    }
    else {
      watchdog('entity_email', 'Could not retrieve the email template', array(), WATCHDOG_ERROR);
    }

  }

  /**
   * Returns the user associated with the entity_email.
   */
  public function getCurrentUser() {
    return user_load($this->cuid);
  }

  /**
   * Sets a new user associated with the entity_email.
   *
   * @param $account
   *   The user account object or the user account id (uid).
   */
  public function setCurrentUser($account) {
    $this->cuid = is_object($account) ? $account->uid : $account;
  }

  /**
   * Gets the associated entity email type.
   *
   * @return EntityEmailType
   */
  public function getType() {
    return entity_email_type_load($this->type);
  }

  /**
   * Generate an array for rendering the entity's content.
   *
   * Iterate over the extra field settings, and show the visible partials.
   *
   * @see entity_build_content()
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    // Get all the entity_email text fields.
    $content = array();
    foreach (field_extra_fields_get_display('entity_email', $this->type, $view_mode) as $key => $value) {
      if (!$value['visible']) {
        // Partial is hidden.
        continue;
      }

      // Field name might have double underscore as-well, so we need to
      // make sure we get it right. For this we inverse the string, and
      // exlpode the first double-underscores.
      // e.g. entity_email__field-name__0
      $inverse = strrev($key);
      $argument = explode('__', $inverse, 2);
      if (count($argument) != 2) {
        continue;
      }
      $delta = strrev($argument[0]);
      // "entity_email__" is 9 chars.
      $field_name = substr(strrev($argument[1]), 9);

      if (!is_numeric($delta)) {
        continue;
      }

      $field = field_info_field($field_name);
      if (empty($field['settings']['entity_email_text'])) {
        continue;
      }

      $options = array(
        'partials' => TRUE,
        'partial delta' => $delta,
        'field name' => $field_name,
      );

      $content['entity_email__' . $field_name .'__' . $delta] = array(
        '#markup' => $this->getText($langcode, $options),
        '#weight' => $value['weight'],
      );
    }
    return entity_get_controller($this->entityType)->buildContent($this, $view_mode, $langcode, $content);
  }
  
  /**
   * Send
   *
   * @return Message
   */
  public function send() {
    $entity_email_type = $this->getType();

    if ($entity_email_type) {

      $params['subject'] = $this->subject;
      $params['body'] = $this->mail_body;
      $params['plaintext'] = $this->mail_body_plain;
      $params['headers']['Bcc'] = $this->mail_bcc;
      $params['headers']['Cc'] = $this->mail_cc;

      // Add attachments.
      $params['attachments'] = $entity_email_type->getAttachments();

      // @todo add key.
      $message = drupal_mail('entity_email', 'entity_email', $this->mail_to, language_default(), $params, $this->mail_from);

      if ($message['send']) {
        watchdog('entity_email', 'Successfully sent email to %recipient', array('%recipient' => $this->mail_to));
      }
    }
    else {
      $message = NULL;
      watchdog('entity_email', 'Could not retrieve the email template', array(), WATCHDOG_ERROR);
    }

    return $message;
  }

}
