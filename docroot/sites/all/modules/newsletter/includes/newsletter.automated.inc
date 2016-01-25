<?php

/**
 * @file
 *   Contains NewsletterMail and NewsletterCustom that extend NewsletterBasic.
 */

/**
 * Newsletter class that sends automated, non-custom newsletters
 * with dynamic content based on taxonomy terms.
 */
class NewsletterAutomated extends NewsletterBasic {

  protected $list;
  protected $subscribers;

  protected $template;
  protected $nodes;

  protected $newsletter;

  protected $customSendRate;
  protected $manualSendRate;
  protected $left;


  public function __construct($nlid) {
    parent::__construct();

    $this->left = variable_get('newsletter_for_next_cron', array());

    $this->list = newsletter_list_load($nlid);

    $template = field_get_items('newsletter_list', $this->list, 'field_newsletter_template');
    $this->template = newsletter_template_load($template[0]['target_id']);

    $this->subscribers = $this->getSubscribers();

    $this->manualSendRate = (bool) ($this->list->send_rate=='Manual');
    $this->customSendRate = (bool) is_numeric($this->list->send_rate);

    $this->nodes = $this->getNodes();

    $this->newsletter = $this->getNewsletterData();

    $this->left[$this->list->nlid] = array();
  }

  /**
   * Loads up current lists's subscribers.
   */
  protected function getSubscribers() {
    if (!empty($this->left[$this->list->nlid])) {
      return $this->left[$this->list->nlid];
    }
    else {
      return newsletter_subscribers_by_list($this->list->nlid);
    }
  }

  /**
   * Loads up current newsletter, or creates a new one.
   *
   * @return
   *   The newsletter object.
   */
  protected function getNewsletterData() {

    $newsletter_current = db_query('SELECT * FROM {newsletter_newsletter}
      WHERE title = :title AND send_id = (
        SELECT MAX(send_id) as send_id
        FROM {newsletter_newsletter}
        WHERE title = :title
      )', array(
        ':title' => $this->list->title,
      ))->fetchObject();

    $last_sent = @($newsletter_current->last_sent !== '0' || !isset($newsletter_current->last_sent));

    if (empty($this->left[$this->list->nlid]) && $last_sent) {
      $newsletter = entity_get_controller('newsletter_newsletter')
        ->create($this->list->title, $this->template->ntid);
    }
    else {
      $newsletter = $newsletter_current;
    }

    return newsletter_newsletter_load($newsletter->nnid);
  }

  /**
   * Builds a dynamic query
   * that gets the current nodes to be sent with the current newsletter.
   */
  protected function getQuery() {
    $tids = newsletter_get_template_terms($this->template->ntid);
    $tids = array_keys($tids) ? array_keys($tids) : array(0);

    $query = db_select('taxonomy_index', 'tax');
    $query->fields('tax', array('nid'));
    $query->join('newsletter_list', 'list', 'list.nlid = ' . (int) $this->list->nlid);
    $query->join('node', 'node', 'tax.nid = node.nid');
    $query->addField('node', 'created', 'created');
    $query->condition('tax.tid', $tids, 'IN');
    $query->where('(list.last_sent <= node.created) OR (list.last_sent IS NULL )');
    if (!$this->manualSendRate && !$this->customSendRate) {
      $query->where('list.send_again = CURDATE() OR list.send_again IS NULL');
    }
    if ($this->customSendRate) {
      $query->range(0, (int) $this->list->send_rate);
    }
    else {
      $query->range(0, (int) variable_get('newsletter_node_limit', 50));
    }
    $query->orderBy('created', 'DESC');
    $query->distinct();

    return $query;
  }

  /**
   * Get this newsletter nodes if list is not exposed.
   *
   * @return
   *   Array containing node objects.
   */
  protected function getNodes() {
    $check_exposed = db_query('SELECT *
      FROM {field_data_field_newsletter_list}
      WHERE field_newsletter_list_target_id = :lid AND target_id_tids IS NOT NULL',
      array(':lid' => $this->list->nlid))
      ->fetchAll();
    // If we get even one subscriber with custom terms
    // or left nodes from previous cron run
    // then no need to continue here.
    if (!empty($check_exposed) || isset($this->left[$this->list->nlid][0]->nodes)) {
      return 'exposed';
    }
    $nodes = $this->getQuery()->execute()->fetchAll();
    foreach ($nodes as $node) {
      $newsletter_nodes[] = node_load($node->nid);
    }
    return isset($newsletter_nodes) ? $newsletter_nodes : array();
  }

  /**
   * Get this newsletter nodes if list is exposed.
   *
   * @param $subscriber
   *   The subscriber's object.
   *
   * @return
   *   Array containing node objects.
   */
  protected function getSubscriberNodes($subscriber) {
    if (isset($subscriber->nodes)) {
      return $subscriber->nodes;
    }
    $nodes = array();
    $newsletter_nodes = array();
    $tids = db_query('SELECT target_id_tids
      FROM {field_data_field_newsletter_list}
      WHERE entity_id = :sid AND field_newsletter_list_target_id = :lid',
      array(
        ':sid' => $subscriber->nsid,
        ':lid' => $this->list->nlid,
      ))->fetchField();
    $tids = @unserialize($tids);
    if (is_array($tids)) {
      foreach ($tids as $tid) {
        $query = $this->getQuery();
        $query->condition('tax.tid', $tid);
        $nodes = array_merge($nodes, $query->execute()->fetchAll());
      }
    }
    foreach ($nodes as $node) {
      $newsletter_nodes[] = node_load($node->nid);
    }
    return $newsletter_nodes;
  }

  /**
   * Updates newsletter list after it is sent.
   *
   * @param $times_sent
   *   The number of subscribers the newsletter was sent to.
   *
   * @return
   *   Boolean depending on whether the update succeeded or not.
   */
  protected function updateStats($times_sent) {
    switch ($this->list->send_rate) {
      case 'Daily':
        $send_again = date('Y-m-d', strtotime('+1 day'));
        break;
      case 'Weekly':
        $send_again = date('Y-m-d', strtotime('+1 week'));
        break;
      case 'Monthly':
        $send_again = date('Y-m-d', strtotime('+1 month'));
        break;
      default:
        $send_again = NULL;
    }
    $updated_list = db_update('newsletter_list')
                    ->fields(array(
                      'last_sent' => REQUEST_TIME,
                      'send_again' => $send_again,
                    ))
                    ->condition('nlid', $this->list->nlid)
                    ->execute();

    $sent_so_far = db_query('SELECT subscribers_sent
      FROM {newsletter_newsletter} WHERE nnid = :id',
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
    return (isset($updated_list) && isset($updated_stats)) ? TRUE : FALSE;
  }

  /**
   * Checkes whether current custom list reached its send rate.
   */
  public function checkCustom() {
    $result = $this->getQuery()->execute()->fetchAll();
    $count = count($result);
    return (bool) ($count >= $this->list->send_rate);
  }

  /**
   * Replaces tokens and sends the current newsletter.
   */
  public function send() {
    $i=0;
    $mails_to_send = variable_get('newsletter_cron_number', 500);
    $params = array(
      'template' => $this->template,
      'subscriber' => '',
      'list' => $this->list,
      'format' => $this->format,
      'newsletter' => $this->newsletter,
    );

    if (empty($this->subscribers)) {
      return array('No subscribers');
    }

    foreach ($this->subscribers as $subscriber) {
      $language = isset($subscriber->language) ? newsletter_language_list($subscriber->language) : $this->language;
      $nodes = ($this->nodes == 'exposed')
        ? $this->getSubscriberNodes($subscriber)
        : $this->nodes;

      if (empty($nodes)) {
        continue;
      }
      $params['nodes'] = $nodes;
      if ($i>=$mails_to_send) {
        $subscriber->nodes = $nodes;
        $this->left[$this->list->nlid][] = $subscriber;
        continue;
      }
      $params['subscriber'] = $subscriber;
      $message = drupal_mail('newsletter', 'automated', $subscriber->email, $language, $params, $this->from);
      if (!$message['result']) {
        newsletter_set_watchdog($this->newsletter->nnid, $this->list->title, $subscriber->email, $result);
      }
      $status[] = $message['result'];

      $i++;
    }

    variable_set('newsletter_for_next_cron', $this->left);

    if (empty($this->left[$this->list->nlid]) && isset($message)) {
      module_invoke_all('newsletter_list_sent', $message);
    }

    $this->updateStats($i);

    return isset($status) ? $status : array('No items');
  }
}
