<?php
/**
 * @file
 * Separate include file to house lyris_member entity functionality.
 */

/**
 * Instantiate a new lyris member object.
 */
function lyris_member() {
  return lyris_entity_create('lyris_member');
}

/**
 * Access control for Lyris members
 */
function lyris_member_access($op, $list, $account = NULL) {

}

/**
 * Load a Lyris Member entity.
 */
function lyris_member_load($eid, $reset = FALSE) {
  return lyris_entity_load_entity('lyris_member', $eid, $reset);
}

/**
 * Load a single Lyris Member entity given an array of criteria.
 */
function lyris_member_load_entity($params = array()) {
  $select = db_select(LYRIS_MEMBER_TABLE, 'lm')->fields('lm', array('eid'));

  foreach ($params as $field => $value) {
    $select->condition(llf($field), $value);
  }
  $res = $select->execute()->fetchAssoc();

  return (is_numeric($res['eid']) ? lyris_member_load($res['eid']) : FALSE);
}

/**
 * Load multiple Lyris Member entities given an array of criteria.
 */
function lyris_member_load_entities($params = array()) {
  $select = db_select(LYRIS_MEMBER_TABLE, 'lm')->fields('lm', array('eid'));

  foreach ($params as $field => $value) {
    $select->condition(llf($field), $value);
  }
  $res = $select->execute()->fetchAllAssoc('eid');

  return lyris_member_load_multiple(array_keys($res));
}

/**
 * Load multiple Lyris Member entities.
 */
function lyris_member_load_multiple($eids = array(), $conditions = array(), $reset = FALSE) {
  return lyris_entity_load_multiple('lyris_member', $eids, $conditions, $reset);
}

/**
 * Inserts or updates a Lyris member object into the database.
 *
 * @param $bean
 *   The bean object to be inserted.
 *
 * @return
 *   Failure to write a record will return FALSE. Otherwise SAVED_NEW or
 *   SAVED_UPDATED is returned depending on the operation performed.
 */
function lyris_member_save(LyrisMember $member) {
  return $member->save();
}

/**
 * Entity URI callback.
 */
function lyris_member_uri($member) {
  return array('path' => 'lyris-member/' . $member->eid);
}

/**
 * Entity label callback.
 * @param $entity_type
 * @param $entity
 */
function lyris_member_label($entity_type, $entity) {
  return t('Subscription info for ') . $entity->EmailAddress;
}

/**
 * Implements hook_entity_delete().
 */
function lyris_member_entity_delete($entity, $type) {
  switch ($type) {
    case 'lyris_list':
      // Delete members when a list is deleted.
      $ListName = $entity->localVal('ListName');

      // Get all members from this list
      $cond = array(
        llf('ListName') => $ListName,
      );
      $members = lyris_member_load_multiple(array(), $cond);

      break;
  }
}

/**
 * Entity class for Lyris Members.
 */
class LyrisMember extends LyrisEntity implements LyrisEntityInterface {
  /**
   * Constructor
   */
  public function __construct(array $values = array()) {
    // If this is new then set the type first.
    if (isset($values['is_new'])) {
      $this->type = isset($values['type']) ? $values['type'] : '';
    }

    // Set the default mode.
    $this->server = variable_get('lyris_mode');

    parent::__construct('MemberStruct', 'lyris_member', $values);
  }

  /**
   * Create a FullName value for the member, derriving it from a Drupal user
   * if possible.
   */
  public function createMemberName() {
    $mail = $this->localVal('EmailAddress');

    // Try to look up a user by email.
    $account = user_load_by_mail($mail);

    if ($account) {
      $token_name = variable_get('lyris_member_name_pattern', '[user:name]');
      $name = token_replace($token_name, array('user' => $account));
    }
    else {
      // Attempt to intelligently decipher a name from an email address.
      $name = LyrisMember::namifyEmail($mail);
    }

    $this->setLocalVal('FullName', $name);
  }

  /**
   * Implements LyrisEntityInterface::getDisplayName().
   */
  public function getDisplayName() {
    return $this->localVal('EmailAddress');
  }

  /**
   * Load the membership from Lyris in the struct array.
   */
  public function loadMembership() {
    $lyris = lyris(NULL, $this->server);
    $membership = $lyris->loadListMembers($this->localVal('ListName'), array(), array('EmailAddress' => $this->localVal('EmailAddress')));

    if ($membership) {
      $this->struct = $membership;
      return TRUE;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Implements LyrisEntityInterface::pushToLyris().
   */
  public function pushToLyris() {
    $lyris = lyris('admin', $this->server);

    if ($this->localVal('MemberID')) {
      if ($lyris->updateMember($this)) {
        return SAVED_UPDATED;
      }
    }
    else {
      if ($lyris->createMember($this)) {
        $this->setSynced(SAVED_NEW);
        return SAVED_NEW;
      }
    }

    return FALSE;
  }

  /**
   * Build a SimpleMemberStruct from the local values to give to Lyris.
   */
  public function simpleMemberStruct() {
    // We need to be careful that the email address wasn't changed in this push.
    // Check the old_values field and use that email address if it exists so it
    // will match with Lyris's record when we push.

    $struct = array(
      'MemberID'     => $this->localVal('MemberID'),
      'EmailAddress' => (isset($this->old_values['EmailAddress']) ? $this->old_values['EmailAddress'] : $this->localVal('EmailAddress')),
      'ListName'     => $this->localVal('ListName'),
    );

    return $struct;
  }

  /**
   * Update the MemberStatus and save the member.
   */
  public function updateStatus($status, $push = TRUE) {
    // Only log a change if the status is actually changing.  This will help
    // to save on processing and connection resources.
    if ($this->localVal('MemberStatus') == $status) {
      return TRUE;
    }

    $this->setLocalVal('MemberStatus', $status);

    if ($status == 'unsub') {
      $this->setLocalVal('DateUnsubscribed', REQUEST_TIME);
    }

    $save_status = $this->save();

    if ($push && $save_status && lyris_threshold_allow()) {
      $this->populateStruct();
      return $this->pushToLyris();
    }
    else {
      return $status;
    }
  }

  /**
   * Given an email address, attempt to intelligently derrive a name from it.
   */
  static function namifyEmail($mail) {
    // Get the prefix from the email address.
    $name = substr($mail, 0, stripos($mail, '@'));

    // Replace any special characters with spaces.
    $name = preg_replace('/[^a-zA-Z0-9\-]/', ' ', $name);

    // Capitalize words
    $name = ucwords($name);

    return $name;
  }
}

/**
 * Entity API Controller for Lyris Members.
 */
class LyrisMemberEntityAPIController extends LyrisEntityAPIController {}
