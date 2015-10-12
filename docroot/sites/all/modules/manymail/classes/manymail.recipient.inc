<?php
/**
 * @file
 * Definition of ManyMailRecipient.
 */

class ManyMailRecipient {

  /**
   * The user id of the recipient or false in case the instance contains
   * custom data (for instance, in the Lists submodule).
   *
   * @var int|false
   */
  public $uid = FALSE;

  /**
   * The e-mail address of the recipient.
   *
   * @var string
   */
  public $mail;

  /**
   * The name of the recipient.
   *
   * @var string
   */
  public $name;

  /**
   * Constructor-like function.
   *
   * Creates and returns a ManyMailRecipient filled with
   * the data supplied in the parameters.
   *
   * @param string $mail
   *   The e-mail address of the recipient.
   * @param string $name
   *   The name of the recipient.
   *
   * @return ManyMailRecipient
   *   A new instance of this class
   */
  public static function fromData($mail, $name) {
    $ret = new self();

    $ret->mail = $mail;
    $ret->name = $name;

    return $ret;
  }

  /**
   * Constructor-like function.
   *
   * Creates and returns a ManyMailRecipient filled with
   * the data retrieved from the supplied user id.
   *
   * @param int $uid
   *   The user id of the recipient.
   *
   * @return ManyMailRecipient|false
   *   A new instance of this class or FALSE on failure
   */
  public static function fromUid($uid) {
    $query = db_select('users', 'u');

    $query->addField('u', 'mail');
    $query->addField('u', 'name');

    $query->condition('u.uid', $uid);
    $query->condition('u.status', 1);

    if ($user = $query->execute()) {
      $ret = new self();

      $ret->uid  = $uid;
      $ret->mail = $user->mail;
      $ret->name = $user->name;

      return $ret;
    }

    return FALSE;
  }

}
