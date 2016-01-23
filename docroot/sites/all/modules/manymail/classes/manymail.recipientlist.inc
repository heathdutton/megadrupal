<?php
/**
 * @file
 * Definition of ManyMailRecipientList.
 */

class ManyMailRecipientList {

  /**
   * All ManyMailRecipients for this list.
   *
   * @var array
   */
  protected $recipients = array();

  /**
   * Adds a result set obtained through Database API execute().
   *
   * Please note that the result set -must- contain a 'uid', 'mail'
   * and 'name' parameter.
   *
   * @param array $result
   *   The result set of a query.
   */
  protected function fromResult($result) {
    foreach ($result as $record) {
      $recipient = new ManyMailRecipient();

      $recipient->uid  = $record->uid;
      $recipient->mail = $record->mail;
      $recipient->name = $record->name;

      $this->recipients[] = $recipient;
    }
  }

  /**
   * Get all recipients gathered so far.
   *
   * Filters out all duplicates before returning the
   * ManyMailRecipient objects.
   *
   * @return array
   *   An array of ManyMailRecipient objects.
   */
  public function getAll() {
    $recipients = array();

    // Drupal can store e-mail addresses in upper case, so we
    // convert them to lower case to check for duplicates.
    foreach ($this->recipients as $key => $recipient) {
      $recipients[$key] = strtolower($recipient->mail);
    }

    return array_intersect_key($this->recipients, array_unique($recipients));
  }

  /**
   * Adds this list to {manymail_log_recipients}.
   *
   * Calls ManyMailRecipientList::getAll() to get unique
   * e-mail address results.
   *
   * @param int $mlog
   *   The {manymail_log}.mlog to add to.
   *
   * @return int
   *   The amount of recipients in this list.
   */
  public function attachToMailing($mlog) {
    $query = db_insert('manymail_log_recipients');
    $query->fields(array('mlog', 'mail', 'recipient'));

    foreach ($this->getAll() as $recipient) {
      $query->values(array(
        'mlog' => $mlog,
        'mail' => $recipient->mail,
        'recipient' => serialize($recipient),
      ));
    }
    $query->execute();

    return count($this->recipients);
  }

  /**
   * Adds (a) recipient(s) without a UID to the list.
   *
   * @param string|array $mail
   *   The e-mail address of the recipient or an array of associative arrays
   *   containing:
   *    - mail: The e-mail address of the recipient
   *    - name: The name of the recipient
   * @param string|null $name
   *   (optional) The name of the recipient or NULL if $mail was an array.
   */
  public function addData($mail, $name = NULL) {
    if (isset($name)) {
      $this->recipients[] = ManyMailRecipient::fromData($mail, $name);
    }
    elseif (is_array($mail)) {
      foreach ($mail as $data) {
        $this->recipients[] = ManyMailRecipient::fromData($data['mail'], $data['name']);
      }
    }
  }

  /**
   * Adds (a) recipient(s) from (a) UID(s) to the list.
   *
   * @param array|int $uid
   *   The UID or an array of UID(s) of the recipient(s).
   */
  public function addUser($uid) {
    if (is_array($uid)) {
      $query = db_select('users', 'u');

      $query->addField('u', 'mail');
      $query->addField('u', 'name');

      $query->condition('u.uid', $uid, 'IN');
      $query->condition('u.status', 1);

      $this->fromResult($query->execute());
    }
    else {
      $this->recipients[] = ManyMailRecipient::fromUid($uid);
    }
  }

  /**
   * Adds recipients from (a) user role(s) to the list.
   *
   * @param array|int $rid
   *   The RID or an array of RID(s) of the roles to add recipients from.
   */
  public function addRole($rid) {
    $query = db_select('users', 'u');
    $query->join('users_roles', 'ur', 'u.uid=ur.uid');

    $query->addField('u', 'uid');
    $query->addField('u', 'mail');
    $query->addField('u', 'name');

    $query->condition('ur.rid', (array) $rid, 'IN');
    $query->condition('u.status', 1);
    $query->distinct();

    $this->fromResult($query->execute());
  }

  /**
   * Adds all unblocked users to the list.
   */
  public function addAll() {
    $query = db_select('users', 'u');

    $query->addField('u', 'uid');
    $query->addField('u', 'mail');
    $query->addField('u', 'name');
    $query->condition('u.status', 1);

    $this->fromResult($query->execute());
  }

}
