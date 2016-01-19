<?php

/**
 * @file
 * Contains the Box class.
 */

/**
 * A box of cards.
 *
 * For performance reasons, we don't load all the cards into the box from
 * the database, but do all the operations by going directly to the database
 * and extracting the information we need each time we need something.
 */
class Box {
  // A box belongs to a particular user
  var $uid;
  
  // A box has an associated taxonomy term
  var $tid;
  
  // A box has a number (e.g. from -1=UNSTARTED to 10
  var $box_no;
  
  // A box has a number of cards in it
  var $size;

  /**
   * Constructor
   */
  function __construct($uid, $tid, $box_no, $size) {
    $this->uid = $uid;
    $this->tid = $tid;
    $this->box_no = $box_no;
    $this->size = $size;
  }

  /**
   * Set the maximum size of the box.
   *
   * It is possible to have more cards temporarily in a particular box.  This
   * happens when a set of boxes is joined with another set of boxes, and a particular
   * box in each set is full (or nearly full).
   */
  function set_size($size) {
    $this->size = $size;
  }

  /**
   * Count the number of cards currently in the box.
   */
  function count() {
    $count = 0;

    if ($this->box_no > FLIPCARD_UNSTARTED_LEVEL) {
      // For boxes other than the pool, look at the number of rows which
      // have a statistics row for that box
      $sql = "SELECT count(fs.nid) AS count
        FROM {flipcard_cards} fs, {taxonomy_index} ti
        WHERE fs.nid = ti.nid
        AND ti.tid = :tid
        AND fs.uid = :uid
        AND fs.box = :box_no";

      $result = db_query_range($sql, 0, 1, array(':tid' => $this->tid,
        ':uid' => $this->uid,
        ':box_no' => $this->box_no));
      $record = $result->fetchObject();
    }
    else {
      // For the pool, we look at how many rows there are which have other
      // box numbers, and then subtract this from total number of rows which this
      // taxonomy set should have. This means we do not have to unnecessarily fill
      // the stats table with hundreds or thousands of rows with a zero count
      // for all those words which each user has not yet started.
      $sql = "SELECT count(fs.nid) AS count
        FROM {flipcard_cards} fs, {taxonomy_index} ti
        WHERE fs.nid = ti.nid
        AND ti.tid = :tid
        AND fs.uid = :uid
        AND fs.box > :pool_box";

      $result = db_query_range($sql, 0, 1, array(':tid' => $this->tid,
        ':uid' => $this->uid,
        ':pool_box' => FLIPCARD_UNSTARTED_LEVEL));
      $record = $result->fetchObject();
    }

    if ($record) {
      $count = $record->count;
    }

    // For the pool, the count is the number of rows in the taxonomy set, minus
    // the number of rows in the other boxes
    if ($this->box_no == FLIPCARD_UNSTARTED_LEVEL) {
      $count = flipcard_cards_tid($this->tid) - $count;
    }

    return $count;
  }

  /**
   * Return the size of the box.
   */
  function size() {
    return $this->size;
  }

  /**
   * Get the first card in the box.
   *
   * @return Card
   */
  function get_first_card() {
    $nid = 0;

    $sql = "SELECT {flipcard_cards}.nid
      FROM {flipcard_cards}, {taxonomy_index}
      WHERE {flipcard_cards}.nid = {taxonomy_index}.nid
      AND {taxonomy_index}.tid = :tid
      AND {flipcard_cards}.uid = :uid
      AND {flipcard_cards}.box = :box_no
      ORDER BY time_reviewed ASC";

    $result = db_query_range($sql, 0, 1, array(
      ':tid' => $this->tid,
      ':uid' =>  $this->uid,
      ':box_no' => $this->box_no
    ));
    $record = $result->fetchObject();

    if ($record) {
      $nid = $record->nid;
    }

    $first_card = new Card($nid, $this->uid);

    return $first_card;
  }

  /**
   * Get a random card from the box.
   * 
   * This function is only needed for selecting a card from the unstarted box
   * and therefore it is an error to call it unless on the unstarted box.
   *
   * @return Card
   */
  function get_random_card() {
    $nid = 0;
    
    if ($this->box_no <> FLIPCARD_UNSTARTED_LEVEL) {
      watchdog("get_random_card",
        "random card is not defined for started levels! uid=%uid, tid=%tid, box=%box",
        array("%tid" => $this->tid, "%uid" => $this->uid, "%box" => $this->box_no),
        WATCHDOG_ERROR);
    }
    else {
      if (module_exists("lp_rank")) {
        $nid = lp_rank_get_unstarted_nid($this->uid, $this->tid);
      }
      else {
        // Get the nids which do not have a flipcard_cards entry
        // for the given user. This will be the nodes in the unstarted box
        $sql = "SELECT ti.nid
          FROM {taxonomy_index} ti
          WHERE ti.tid = :tid
          AND ti.nid NOT IN (SELECT {flipcard_cards}.nid
          	FROM {flipcard_cards}, {taxonomy_index}
          	WHERE {flipcard_cards}.nid = {taxonomy_index}.nid
          	AND {taxonomy_index}.tid = :tid
          	AND {flipcard_cards}.uid = :uid)";
  
        $result = db_query_range($sql, 0, 1, array(':tid' => $this->tid,
          ':uid' => $this->uid));
        $record = $result->fetchObject();
        
        if ($record) {
          $nid = $record->nid;
        }
        else {
          watchdog("get_random_card",
          "No random card found! uid=%uid, tid=%tid, box=%box",
          array("%tid" => $this->tid, "%uid" => $this->uid, "%box" => $this->box_no),
          WATCHDOG_ERROR);
        }
      }
    }

    $random_card = new Card($nid, $this->uid);

    return $random_card;
  }

}