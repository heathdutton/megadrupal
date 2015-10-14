<?php

namespace Drupal\campaignion_newsletters;

class Subscriptions {
  protected $subscriptions;

  protected static $lists = NULL;

  public static function byContact($contact) {
    $lists = static::lists();
    $addresses = array();
    foreach ($contact->allEmail() as $address) {
      $addresses[] = $address['value'];
    }
    $storedSubscriptions = Subscription::byEmail($addresses);
    return new static($lists, $addresses, $storedSubscriptions);
  }

  public function __construct($lists, $addresses, $storedSubscriptions) {
    $subscriptions = array();
    foreach ($addresses as $email) {
      $subscriptions[$email] = array();
      foreach ($lists as $list_id => $list) {
        $subscriptions[$email][$list_id] = NULL;
      }
    }

    foreach ($storedSubscriptions as $s) {
      $subscriptions[$s->email][$s->list_id] = $s;
    }
    $this->subscriptions = $subscriptions;
  }

  public static function lists() {
    if (!isset(static::$lists)) {
      static::$lists = NewsletterList::listAll();
    }
    return static::$lists;
  }

  public function update($values) {
    foreach ($values as $email => $lists) {
      foreach ($lists as $list_id => $subscribed) {
        if ($subscription = &$this->subscriptions[$email][$list_id]) {
          $subscription->delete = !$subscribed;
        }
        elseif ($subscribed) {
          $subscription = Subscription::fromData($list_id, $email);
        }
      }
    }
  }

  public function save() {
    foreach ($this->subscriptions as $email => $lists) {
      foreach ($lists as $list_id => $subscription) {
        if ($subscription) {
          $subscription->save();
        }
      }
    }
  }

  public function unsubscribeAll() {
    foreach ($this->subscriptions as $email => $lists) {
      foreach ($lists as $list_id => $subscription) {
        $subscription->delete = TRUE;
      }
    }
  }

  public function optionsArray() {
    $options = array();
    foreach (static::lists() as $list_id => $list) {
      $options[$list_id] = $list->title;
    }
    return $options;
  }

  public function values($email) {
    $values = array();
    foreach ($this->subscriptions[$email] as $list_id => $subscription) {
      $values[$list_id] = ($subscription && !$subscription->delete) ? $list_id : 0;
    }
    return $values;
  }
}
