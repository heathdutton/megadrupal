<?php

/**
 * @file
 * Apple News DB class.
 *
 * This is a helper class to populate various Apple News
 * related database tables.
 */

/**
 * Interface ApplenewsDatabaseInterface.
 */
interface ApplenewsDatabaseInterface {

  /**
   * Get article object.
   */
  public function getArticle();

  /**
   * Set entity ID.
   */
  public function setEntityId($id);

  /**
   * Set entity revision ID.
   */
  public function setEntityRevisionId($id);

  /**
   * Set entity type.
   */
  public function setEntityType($type);

  /**
   * Set channel ID.
   */
  public function setChannelId($id);

  /**
   * Get entity channels.
   */
  public function getChannels();

  /**
   * Set post ID (primary key value).
   *
   * Returns post ID by entity type, entity ID, entity revision ID.
   */
  public function setPostId($entity_type, $entity_id, $revision_id);

  /**
   * Get article by article ID.
   */
  public function getArticleById($article_id);

  /**
   * Save new Apple News post information.
   */
  public function insert($record);

  /**
   * Update Apple News related tables.
   */
  public function update($record);

  /**
   * Delete Apple News related records.
   */
  public function delete();

  /**
   * Delete Apple News related records by Entity ID and Entity Type.
   */
  public function deleteByEntity(EntityDrupalWrapper $entity_id, $entity_type);

}

/**
 * Main class to work with AppleNewsAPI local database tables.
 */
class ApplenewsDatabase implements ApplenewsDatabaseInterface {

  private $postId = NULL;
  private $entityType = '';
  private $entityId = NULL;
  private $revisionId = NULL;
  private $channelId = NULL;
  private $exid = NULL;

  /**
   * Save Apple News post information.
   */
  private function saveEntities(array $entity = [], $primary_key = '') {
    if (!empty($primary_key)) {
      $entity['post_id'] = $this
        ->setPostId($this->entityType, $this->entityId, $this->revisionId);
    }
    $entity['entity_type'] = $this->entityType;
    $entity['entity_id'] = $this->entityId;
    $entity['revision_id'] = $this->revisionId;
    $entity['exid'] = $this->exid;
    if (!empty($primary_key)) {
      drupal_write_record('applenews_entities', $entity, $primary_key);
    }
    else {
      drupal_write_record('applenews_entities', $entity);
    }
    $this->postId = !empty($entity['post_id']) ? $entity['post_id'] : NULL;
    return $this->postId;
  }

  /**
   * Save export for an entity.
   */
  public function setExport($exid) {
    $this->exid = $exid;
    $entity = [
      'post_id' => $this->postId,
      'exid' => $exid,
    ];
    drupal_write_record('applenews_entities', $entity, 'post_id');
  }

  /**
   * Save Apple News channels.
   */
  private function saveChannels(array $channels = []) {
    $this->deleteChannels();
    foreach ($channels as $channel) {
      $channel['post_id'] = $this->postId;
      drupal_write_record('applenews_channels', $channel);
    }
  }

  /**
   * Save Apple News sections.
   */
  private function saveSections(array $sections = []) {
    $this->deleteSections();
    foreach ($sections as $section) {
      $section['post_id'] = $this->postId;
      drupal_write_record('applenews_sections', $section);
    }
  }

  /**
   * Save Apple News metadata.
   */
  private function saveMetadata(array $metadata = []) {
    $this->deleteMetadata();
    $metadata['post_id'] = $this->postId;
    drupal_write_record('applenews_metadata', $metadata);
  }

  /**
   * Delete entities.
   */
  private function deleteEntities() {
    db_delete('applenews_entities')
      ->condition('post_id', $this->postId)
      ->execute();
  }

  /**
   * Delete channels.
   */
  private function deleteChannels() {
    db_delete('applenews_channels')
      ->condition('post_id', $this->postId)
      ->execute();
  }

  /**
   * Delete section information.
   */
  private function deleteSections() {
    db_delete('applenews_sections')
      ->condition('post_id', $this->postId)
      ->execute();
  }

  /**
   * Delete post metadata.
   */
  private function deleteMetadata() {
    db_delete('applenews_metadata')
      ->condition('post_id', $this->postId)
      ->execute();
  }

  /**
   * Get article information.
   */
  public function getArticle() {
    $article = [];
    $query = db_select('applenews_entities', 'e');
    $query->join('applenews_channels', 'c', 'c.post_id = e.post_id');
    $query->join('applenews_sections', 's', 's.post_id = e.post_id');
    $query->join('applenews_metadata', 'm', 'm.post_id = e.post_id');
    $query->fields('e');
    $query->fields('c');
    $query->fields('s');
    $query->fields('m');
    $query->condition('e.post_id', $this->postId);
    if (!empty($this->channelId)) {
      if (is_array($this->channelId)) {
        $query->condition('c.channel_id', $this->channelId, 'IN');
      }
      else {
        $query->condition('c.channel_id', $this->channelId);
      }
    }
    $query->orderBy('e.postdate', 'DESC');
    foreach ($query->execute() as $row) {
      $article = [
        'article_id'     => $row->article_id,
        'share_url'      => $row->share_url,
        'exid'           => $row->exid,
        'entity_id'      => $row->entity_id,
        'revision_id'    => $row->revision_id,
        'entity_type'    => $row->entity_type,
        'postdate'       => $row->postdate,
        'sections'       => [],
      ];
    }
    foreach ($query->execute() as $row) {
      $article['sections'][$row->section_id] = $row->channel_id;
    }
    return $article;
  }

  /**
   * Set entity id.
   */
  public function setEntityId($id) {
    $this->entityId = $id;
  }

  /**
   * Set revision id.
   */
  public function setEntityRevisionId($id) {
    $this->revisionId = $id;
  }

  /**
   * Set entity type.
   */
  public function setEntityType($type) {
    $this->entityType = $type;
  }

  /**
   * Set channel id.
   */
  public function setChannelId($id) {
    $this->channelId = $id;
  }

  /**
   * Get channels.
   */
  public function getChannels() {
    $channel_ids = [];

    $query = db_select('applenews_entities', 'e');
    $query->join('applenews_channels', 'c', 'c.post_id = e.post_id');
    $query->fields('c', ['channel_id']);
    $query->condition('e.entity_type', $this->entityType);
    $query->condition('e.entity_id', $this->entityId);
    $query->condition('e.revision_id', $this->revisionId);

    if (!empty($this->channelId)) {
      if (is_array($this->channelId)) {
        $query->condition('c.channel_id', $this->channelId, 'IN');
      }
      else {
        $query->condition('c.channel_id', $this->channelId);
      }
    }
    $result = $query->execute();
    foreach ($result as $row) {
      $channel_ids[] = $row->channel_id;
    }
    return $channel_ids;
  }

  /**
   * Get primary key by entity information.
   */
  public function setPostId($entity_type, $entity_id, $revision_id) {
    $this->setEntityId($entity_id);
    $this->setEntityRevisionId($revision_id);
    $this->setEntityType($entity_type);

    $query = db_select('applenews_entities', 'e');
    $query->join('applenews_channels', 'c', 'c.post_id = e.post_id');
    $query->fields('e', ['post_id', 'exid']);
    $query->condition('e.entity_type', $entity_type);
    $query->condition('e.entity_id', $entity_id);
    $query->condition('e.revision_id', $revision_id);
    if (!empty($this->channelId)) {
      if (is_array($this->channelId)) {
        $query->condition('c.channel_id', $this->channelId, 'IN');
      }
      else {
        $query->condition('c.channel_id', $this->channelId);
      }
    }
    if ($result = $query->execute()->fetchAssoc()) {
      $this->postId = $result['post_id'];
      $this->exid = $result['exid'];
    }
    return $this->postId;
  }

  /**
   * Get article by article ID.
   */
  public function getArticleById($article_id) {
    $this->postId = db_select('applenews_entities', 'e')
      ->fields('e', ['post_id'])
      ->condition('article_id', $article_id)
      ->execute()
      ->fetchField();
    return $this->getArticle();
  }

  /**
   * Save new Apple News post information.
   */
  public function insert($record) {
    $transaction = db_transaction();
    try {
      $this->saveEntities($record['entity']);
      $this->saveChannels($record['channels']);
      $this->saveSections($record['sections']);
      $this->saveMetadata($record['metadata']);
    }
    catch (Exception $e) {
      $transaction->rollback();
      $msg = "Couldn't write post data for entity ID @id";
      $var = [
        '@id' => !empty($record['entity']['entity_id'])
                   ? $record['entity']['entity_id']
                   : 'N/A'
      ];
      watchdog_exception('applenews', $e, $msg, $var, WATCHDOG_ERROR);
    }
  }

  /**
   * Update Apple News related tables.
   */
  public function update($record) {
    $transaction = db_transaction();
    try {
      $this->saveEntities($record['entity'], 'post_id');
      $this->saveChannels($record['channels']);
      $this->saveSections($record['sections']);
      $this->saveMetadata($record['metadata']);
    }
    catch (Exception $e) {
      $transaction->rollback();
      $msg = "Couldn't write post ID @id";
      $var = [
        '@id' => $this->postId
      ];
      watchdog_exception('applenews', $e, $msg, $var, WATCHDOG_ERROR);
    }
  }

  /**
   * Delete Apple News related records.
   */
  public function delete() {
    $transaction = db_transaction();
    try {
      $this->deleteEntities();
      $this->deleteChannels();
      $this->deleteSections();
      $this->deleteMetadata();
    }
    catch (Exception $e) {
      $transaction->rollback();
      $msg = "Couldn't delete post ID @id";
      $var = [
        '@id' => $this->postId
      ];
      watchdog_exception('applenews', $e, $msg, $var, WATCHDOG_ERROR);
    }
  }

  /**
   * Delete Apple News related records by Entity ID and Entity Type.
   */
  public function deleteByEntity(EntityDrupalWrapper $entity_wrapper, $entity_type) {
    $transaction = db_transaction();
    try {
      $entity_id = $entity_wrapper->getIdentifier();

      $result = db_select('applenews_entities', 'e')
        ->fields('e', ['post_id'])
        ->condition('e.entity_id', $entity_id)
        ->condition('e.entity_type', $entity_type)
        ->execute();
      foreach ($result as $row) {
        $this->postId = $row->post_id;
        $this->deleteEntities();
        $this->deleteChannels();
        $this->deleteSections();
        $this->deleteMetadata();
      }
    }
    catch (Exception $e) {
      $transaction->rollback();
      $msg = "Couldn't delete post ID @id";
      $var = [
        '@id' => $this->postId
      ];
      watchdog_exception('applenews', $e, $msg, $var, WATCHDOG_ERROR);
    }
  }

}
