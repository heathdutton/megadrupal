<?php

/**
 * @file
 *   Contains NewsletterBasic class and
 *   newsletter's module implementation of drupal mail system.
 */

/**
 * Basic Newsletter class.
 */
class NewsletterBasic {

  protected $from;
  protected $format;
  protected $language;

  public function __construct() {
    $this->from = variable_get('newsletter_from');
    $this->format = variable_get('newsletter_format', 'html');
    $this->language = language_default()->language;
  }

  /**
   * Sends a test newsletter to an email-address.
   * Called from administration Settings tab.
   *
   * @param $to
   *   The e-mail to send the test newsletter to.
   */
  public function sendTest($to) {
    $message = drupal_mail('newsletter', 'test' , $to , $this->language , array('format' => $this->format) , $this->from);
    return $message['result'];
  }

  /**
   * Sends email to a subscriber using a basic template (confirmation/welcome/unsubscribe emails).
   *
   * @param $id
   *   The the basic template id of this email.
   * @param $to
   *   The e-mail to send the newsletter confirmation to.
   */
  public function sendBasic($id, $to) {
    $template = newsletter_template_load($id);
    $subscriber = newsletter_subscriber_load(array(), array('email' => $to));

    $params = array(
      'template' => $template,
      'subscriber' => array_pop($subscriber),
      'format' => $this->format,
    );
    $language = isset($subscriber->language) ? newsletter_language_list($subscriber->language) : $this->language;
    $message = drupal_mail('newsletter', 'basic', $to, $language , $params , $this->from);
    return $message['result'];
  }
}


