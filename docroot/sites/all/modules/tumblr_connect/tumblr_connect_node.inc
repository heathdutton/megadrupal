<?php

/**
 * @file
 * handles relationship between local nodes and remote tumblr objects
 */
/*
 * Class implements relationship between drupal node and remote tumbler post.
 */
class TumblrConnectNode {

  /**
   * Unique identifier for relationship.
   *
   * @var integer.
   */
  protected $id;

  /**
   * Nid identifier for node.
   *
   * @var integer.
   */
  protected $nid;

  /**
   * Vid identifier for node revision.
   *
   * @var integer.
   */
  protected $vid;

  /**
   * Unique identifier for tumblr remote service content.
   *
   * @var integer.
   */
  protected $post_id;

  function __construct($nid = NULL, $vid = NULL) {

    if (isset($nid)) {
      $this->nid = $nid;
    }
    if (isset($vid)) {
      $this->vid = $vid;
    }
  }

  public function init($nid, $vid) {
    $this->nid = $nid;
    $this->vid = $vid;
  }

  public function getId() {
    return $this->id;
  }

  public function setId($id) {
    $this->id = $id;
  }

  public function getNid() {
    return $this->nid;
  }

  public function setNid($nid) {
    $this->nid = $nid;
  }

  public function getVid() {
    return $this->vid;
  }

  public function setVid($vid) {
    $this->vid = $vid;
  }

  public function getPostId() {
    return $this->post_id;
  }

  public function setPostId($post_id) {
    $this->post_id = $post_id;
  }

  /**
   * Gets all properties as an array.
   *
   * @return array
   */
  public function toArray() {
    return array('id' => $this->id,
        'nid' => $this->nid,
        'vid' => $this->vid,
        'post_id' => $this->post_id);
  }

}

/*
 * Data Access Object for our relationship between Drupal node and tumblr post.
 */

class DbTumblrConnectNode {

  function __construct() {

  }

  /**
   * Adds a new relationship to the database.
   *
   * @param tumblr_connect_node
   *   A releationship instance.
   *
   * @returns
   *   Upon success, the relationship instance (tumblr_connect_node) with a id assigned by the database.
   *
   * @throws TumblrConnectDatabaseException
   */

  public function add(TumblrConnectNode $tumblr_connect_node) {
    // drupal write record must have direct access to all object fields so
    // let's pass an array representation instead
    try {
      $result = db_insert('tumblr_connect_node')
        ->fields($tumblr_connect_node->toArray())
        ->execute();
    } catch (Exception $e) {
      throw new TumblrConnectDatabaseException(t("Unable to Add Tumblr Connect Record"));
    }
    return $result;
  }

  /**
   * Modifies a relationship record in the database.
   *
   * @param tumblr_connect_node
   *   A releationship instance.
   *
   * @returns
   *   TRUE (1) upon sucess.
   *
   * @throws TumblrConnectDatabaseException
   */

  public function modify(TumblrConnectNode $tumblr_connect_node) {
    try {
      $fields = array(
          'nid' => $tumblr_connect_node->getNid(),
          'vid' => $tumblr_connect_node->getVid(),
          'post_id' => $tumblr_connect_node->getPostId()
      );
      $update_query = db_update('tumblr_connect_node')->fields($fields);
      $update_query->condition('id', $tumblr_connect_node->getId());
      $result = $update_query->execute();
    } catch (Exception $e) {
      throw new TumblrConnectDatabaseException(t("Unable to Modify Tumblr Connect Record"));
    }
    return $result;
  }

  /**
   * Deletes a relationship from the database.
   *
   * @param $id
   *   The unique identifier for relationship.
   *
   * @returns
   *   TRUE (1) upon sucess.
   *
   * @throws TumblrConnectDatabaseException
   */

  public function delete($id) {
    try {
      $result = db_delete('tumblr_connect_node')
        ->condition('id', $id)
        ->execute();
    } catch (Exception $e) {
      throw new TumblrConnectDatabaseException(t("Unable to Delete Tumblr Connect Record"));
    }
    return $result;
  }

  /**
   * Gets a relationship record post id value by node nid and vid.
   *
   * @param $nid
   *   Node id for the corresponding node.
   * @param $vid
   *   Node revision id for the corresponding node revision.
   *
   * @returns
   *   The relationship object between node and tumblr post TumblrConnectNode.
   *
   * @throws TumblrConnectDatabaseNoRecordException
   */

  public function getByNode($nid, $vid) {
    $sql = 'SELECT id, post_id from {tumblr_connect_node} where nid = :nid and vid = :vid';
    $result = db_query($sql, array(':nid' => $nid, ':vid' => $vid))->fetchObject();
    $tumblr_node = new TumblrConnectNode();
    $tumblr_node->setNid($nid);
    $tumblr_node->setVid($vid);
    // If this node was created without a tumblr_connect relationship
    // there will be no record found. 
    if ($result) {
      $tumblr_node->setPostId($result->post_id);
      $tumblr_node->setId($result->id);
    }

    if (!$result) {
      throw new TumblrConnectDatabaseNoRecordException(t('No Record Found'));
    }

    return $tumblr_node;
  }

}
