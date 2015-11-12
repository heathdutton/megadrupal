<?php

class BackpackDisplayerDb
{

  const TABLE_NAME = 'opigno_openbadges_backpack_displayer';
  const FIELD_UID = 'uid';
  const FIELD_BACKPACK_USER_ID = 'backpack_user_id';


  public static function save($uid, $backpackUserId)
  {
    db_merge(self::TABLE_NAME)
      ->fields(array(
        self::FIELD_UID => $uid,
        self::FIELD_BACKPACK_USER_ID => $backpackUserId
      ))
      ->key(array(
        self::FIELD_UID => $uid
      ))
      ->execute();
  }

  public static function getEntries($uid)
  {
    return db_select(self::TABLE_NAME, 't')
      ->fields('t')
      ->condition(self::FIELD_UID, $uid, '=')
      ->execute()->fetchAllAssoc(self::FIELD_BACKPACK_USER_ID, PDO::FETCH_ASSOC);
  }

  public static function clear($uid)
  {
    db_delete(self::TABLE_NAME)
      ->condition(self::FIELD_UID, $uid, '=')
      ->execute();
  }

  public static function getPrimaryKey()
  {
    return array(self::FIELD_UID);
  }


}