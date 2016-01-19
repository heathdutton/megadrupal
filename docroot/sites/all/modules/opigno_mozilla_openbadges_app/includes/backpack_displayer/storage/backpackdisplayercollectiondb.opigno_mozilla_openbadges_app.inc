<?php

class BackpackDisplayerCollectionDb
{

  const TABLE_NAME = 'opigno_openbadges_backpack_displayer_collections';
  const FIELD_UID = 'uid';
  const FIELD_COLLECTION_ID = 'collection_id';

  public static function save($collectionId, $uid)
  {
    db_merge(self::TABLE_NAME)
      ->fields(array(
        self::FIELD_COLLECTION_ID => $collectionId,
        self::FIELD_UID => $uid
      ))
      ->key(array(
        self::FIELD_COLLECTION_ID => $collectionId
      ))
      ->execute();
  }

  public static function getEntries($uid)
  {
    return db_select(self::TABLE_NAME, 't')
      ->fields('t')
      ->condition(self::FIELD_UID, $uid, '=')
      ->execute()->fetchAllAssoc(self::FIELD_COLLECTION_ID, PDO::FETCH_ASSOC);
  }

  public static function clear($uid)
  {
    db_delete(self::TABLE_NAME)
      ->condition(self::FIELD_UID, $uid, '=')
      ->execute();
  }

  public static function getPrimaryKey()
  {
    return array(self::FIELD_COLLECTION_ID);
  }

}