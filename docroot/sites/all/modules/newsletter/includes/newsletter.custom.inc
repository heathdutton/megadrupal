<?php


/**
 * Newsletter class that sends custom newsletters.
 */
class NewsletterCustom extends NewsletterBasic {
  protected $template;
  protected $subscribers;
  protected $newsletter;

  public function __construct($subscribers, $ntid, $nnid = NULL) {
    parent::__construct();
    $this->template = newsletter_template_load($ntid);
    $this->subscribers = $subscribers;
    $this->newsletter = newsletter_newsletter_load($nnid);
  }

  /**
   * Updates current newsletter's number of subscribers sent.
   *
   * @param $times_sent
   *   int The number of subscribers this newsletter was sent this time.
   */
  protected function updateNewsletter($times_sent) {
    $sent_so_far = db_query('SELECT subscribers_sent
      FROM {newsletter_newsletter}
      WHERE nnid = :id',
      array(':id' => $this->newsletter->nnid))
      ->fetchField();
    $subscribers_sent = isset($sent_so_far)
      ? ($sent_so_far + $times_sent)
      : $times_sent;

    $updated_stats = db_update('newsletter_newsletter')
      ->fields(array(
        'last_sent' => REQUEST_TIME,
        'subscribers_sent' => $subscribers_sent,
      ))
      ->condition('nnid', $this->newsletter->nnid)
      ->execute();
  }

 /**
   * Custom newsletter preview for newsletter administrators.
   *
   * @return
   *   an array containing html formatted newsletter's body and subject
   */
  public function preview() {
    $template_body = field_get_items('newsletter_template', $this->template, 'field_newsletter_body');
    if ($template_body) {
      $template_body = $template_body[0];
      $data['body'] = token_replace($template_body['value']);
      $data['body_format'] = $template_body['format'];
    }
    $data['subject'] = token_replace($this->template->subject);
    return $data;
  }

  /**
   * Replaces tokens, sends the custom newsletter and updates stats.
   *
   * @return
   *   Array containing the sent mail status and subscribers left for next cron
   */
  public function send() {
    $i=0;
    $mails_to_send = variable_get('newsletter_cron_number', 500);
    $subscribers_left = array();
    $status = array();
    $params = array(
      'template' => $this->template,
      'subscriber' => '',
      'format' => $this->format,
      'newsletter' => $this->newsletter,
    );
    foreach ($this->subscribers as $subscriber) {
      $language = isset($subscriber->language) ? newsletter_language_list($subscriber->language) : $this->language;
      if ($i>=$mails_to_send) {
        $subscribers_left[] = $subscriber;
        continue;
      }
      $params['subscriber'] = $subscriber;
      $message = drupal_mail('newsletter', 'custom', $subscriber->email, $language, $params, $this->from);

      if (!$message['result']) {
        newsletter_set_watchdog($this->newsletter->nnid, $this->newsletter->title, $subscriber->email, $result);
      }
      $status[] = $message['result'];

      $i++;
    }
    $this->updateNewsletter($i);

    if (empty($subscribers_left) && isset($message)) {
      module_invoke_all('newsletter_list_sent', $message);
    }

    return array(
      'status' => $status,
      'subscribers_left' => $subscribers_left,
    );
  }
}

