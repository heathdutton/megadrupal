<?php

namespace Drupal\campaignion_newsletters;

class Subscription extends \Drupal\little_helpers\DB\Model {
  public $list_id;
  public $email;
  public $fingerprint = '';
  public $delete = FALSE;
  public $source = NULL;
  public $needs_opt_in = FALSE;

  public static $lists = array();

  protected static $table = 'campaignion_newsletters_subscriptions';
  protected static $key = array('list_id', 'email');
  protected static $values = array('fingerprint');
  protected static $serial = FALSE;

  public static function byData($list_id, $email) {
    $result = db_select(static::$table, 's')
      ->fields('s')
      ->condition('list_id', $list_id)
      ->condition('email', $email)
      ->execute();
    if ($row = $result->fetch()) {
      return new static($row, FALSE);
    } else {
      return static::fromData($list_id, $email);
    }
  }

  public static function fromData($list_id, $email) {
    return new static(array(
      'list_id' => $list_id,
      'email' => $email,
    ), TRUE);
  }

  public static function byEmail($email) {
    $subscriptions = array();
    $result = db_select(static::$table, 's')
      ->fields('s')
      ->condition('email', $email)
      ->execute();
    foreach ($result as $row) {
      $subscriptions[] = new static($row, FALSE);
    }
    return $subscriptions;
  }

  public function newsletterList() {
    if (!isset(self::$lists[$this->list_id])) {
      self::$lists[$this->list_id] = NewsletterList::load($this->list_id);
    }
    return self::$lists[$this->list_id];
  }

  public function save($fromProvider = FALSE) {
    if ($this->delete) {
      return $this->delete($fromProvider);
    }
    list($data, $fingerprint) = $this->providerData();
    if ($fingerprint != $this->fingerprint) {
      $this->fingerprint = $fingerprint;
      if (!$fromProvider) {
        $action = QueueItem::SUBSCRIBE;
        if ($this->new) {
          $action |= QueueItem::WELCOME;
          if ($this->needs_opt_in) {
            $action |= QueueItem::OPTIN;
          }
        }
        QueueItem::byData(array(
          'list_id' => $this->list_id,
          'email' => $this->email,
          'action' => $action,
          'data' => $data,
        ))->save();
      }
    }
    db_merge(static::$table)
      ->key($this->values(static::$key))
      ->fields($this->values(static::$values))
      ->execute();
    $this->new = FALSE;
  }

  public function providerData() {
    if (($l = $this->newsletterList()) && ($p = $l->provider())) {
      return $p->data($this);
    }
    else {
      return array(array(), '');
    }
  }

  public function delete($fromProvider = FALSE) {
    if (!$this->isNew() && !$fromProvider) {
      QueueItem::byData(array(
        'list_id' => $this->list_id,
        'email' => $this->email,
        'action' => QueueItem::UNSUBSCRIBE,
      ))->save();
    }
    parent::delete();
  }
}
