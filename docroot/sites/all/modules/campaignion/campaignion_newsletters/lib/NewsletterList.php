<?php

namespace Drupal\campaignion_newsletters;

class NewsletterList extends \Drupal\little_helpers\DB\Model {
  public $list_id;
  public $source;
  public $identifier;
  public $language;
  public $title;
  public $data;

  protected static $table = 'campaignion_newsletters_lists';
  protected static $key = array('list_id');
  protected static $values = array('source', 'identifier', 'title', 'language', 'data');
  protected static $serial = TRUE;
  protected static $serialize = array('data' => TRUE);

  public static function listAll() {
    $result = db_query('SELECT * FROM {campaignion_newsletters_lists} ORDER BY title');
    $lists = array();
    foreach ($result as $row) {
      $lists[$row->list_id] = new static($row);
    }
    return $lists;
  }

  public static function load($id) {
    $result = db_query('SELECT * FROM {campaignion_newsletters_lists} WHERE list_id=:id', array(':id' => $id));
    if ($row = $result->fetch()) {
      return new static($row);
    }
  }

  public static function byIdentifier($source, $identifier) {
    $result = db_query('SELECT * FROM {campaignion_newsletters_lists} WHERE source=:source AND identifier=:identifier', array(
      ':source' => $source,
      ':identifier' => $identifier,
    ));
    if ($row = $result->fetch()) {
      return new static($row);
    }
  }

  public static function fromData($data) {
    $adata = array();
    foreach ($data as $k => $v) {
      $adata[$k] = $v;
    }
    if ($item = self::byIdentifier($data['source'], $data['identifier'])) {
      unset($adata['list_id']);
      $item->__construct($adata);
      return $item;
    } else {
      return new static($data, TRUE);
    }
  }

  public function __construct($data = array(), $new = FALSE) {
    parent::__construct($data, $new);
    foreach ($data as $k => $v) {
      $this->$k = (is_string($v) && !empty(self::$serialize[$k])) ? unserialize($v) : $v;
    }
    if (!isset($this->language)) {
      $this->language = language_default('language');
    }
  }

  public function provider() {
    return ProviderFactory::getInstance()->providerByKey($this->source);
  }

  /**
   * Subscribe a single email-address to this newsletter.
   */
  public function subscribe($email, $fromProvider = FALSE) {
    $fields = array(
      'list_id' => $this->list_id,
      'email' => $email,
    );
    // MySQL supports multi-value merge queries, drupal does not so far,
    // so we could replace the following by a direct call to db_query().
    db_merge('campaignion_newsletters_subscriptions')
      ->key($fields)
      ->fields($fields)
      ->execute();

    if (!$fromProvider) {
      QueueItem::byData(array(
        'list_id' => $this->list_id,
        'email' => $email,
        'action' => QueueItem::SUBSCRIBE,
      ))->save();
    }
  }

  public function unsubscribe($email, $fromProvider = FALSE) {
    db_delete('campaignion_newsletters_subscriptions')
      ->condition('list_id', $this->list_id)
      ->condition('email', $email)
      ->execute();

    if (!$fromProvider) {
      QueueItem::byData(array(
        'list_id' => $this->list_id,
        'email' => $email,
        'action' => QueueItem::UNSUBSCRIBE,
      ))->save();
    }
  }
}
