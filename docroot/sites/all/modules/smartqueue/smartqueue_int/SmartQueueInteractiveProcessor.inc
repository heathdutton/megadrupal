<?php

class SmartQueueInteractiveProcessor extends SmartQueueProcessorDefault {

  /**
   * {@inherit}
   */
  public $maxProcessTime = 3;

  /**
   * {@inherit}
   */
  protected function query($min = NULL) {
    global $user;

    $args = array();

    if ($min) {
      $query = 'SELECT qid FROM {smartqueue} WHERE qid > ? AND ';
      $args[] = $min;
    } else {
      $query = 'SELECT qid FROM {smartqueue} WHERE';
    }

    $query .= " status = 0 AND uid = ? LIMIT 1";
    $args[] = $user->uid;

    return db_query($query, $args)->fetchField();
  }

}
