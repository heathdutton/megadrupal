<?php

/**
 * @file
 * Contains Boxes class.
 */

define('FLIPCARD_UNLIMITED_SIZE', 9999);

define('FLIPCARD_UNSTARTED_LEVEL', -1);
define('FLIPCARD_TOP_LEVEL', 10);

/**
 * A set of boxes of cards.
 *
 * A set of boxes of cards has:
 * - one box called "unstarted" which has all the cards not yet asked
 * - 11 main boxes, labelled 0 to 10, with words/questions gradually progressing
 *   through each box from 0 (start learning) to 10 (fully known)
 *
 * Each card is a node which has a word/question and the answer.
 *
 * Each set of boxes is related to the current user, so different users would
 * have a set of boxes with the same cards (nodes), but with each card in a
 * different box.
 *
 * Example:
 * The boxes set called "Colours" (e.g. tid=20), contains 15 nodes, each of
 * which represent the name of a colour in Thai.  For a particular user, John,
 * the cards are currently in the following boxes:
 * - unstarted (box -1): 100 cards
 * - box 0: 3 cards
 * - box 1: 2 cards
 * - box 2: 0 cards
 * - box 3: 5 cards
 * - box 4: 0 cards
 * - box 5: 10 cards
 * - box 6: 0 cards
 * - box 7: 0 cards
 * - box 8: 0 cards
 * - box 9: 0 cards
 * - box 10: 0 cards
*/
class Boxes {
  // The taxonomy id
  var $tid;

  // The user id
  var $uid;

  // Cards not yet started are stored in box -1 (FLIPCARD_UNSTARTED_LEVEL)
  // Started cards begin in box 0 and move up the boxes as learnt
  var $boxes = array();

  // Max number of cards in each of the different boxes
  // Box -1 is the pool and can always have unlimited cards (represented as 9999)
  var $sizes = array(FLIPCARD_UNSTARTED_LEVEL => FLIPCARD_UNLIMITED_SIZE, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, FLIPCARD_UNLIMITED_SIZE);

  /**
   * Construct a set based on a taxonomy id
   *
   * @param integer $tid
  */
  function __construct($tid, $uid=NULL) {
    if ($uid == NULL) {
      global $user;
      $this->uid = $user->uid;
    }
    else {
      $this->uid = $uid;
    }

    $this->tid = $tid;

    // Sizes of each box
    foreach ($this->sizes as $box_no => $size) {
      $this->boxes[$box_no] = new Box($this->uid, $this->tid, $box_no, $size);
    }

    $this->_normalise_first_started_box();
  }

  /**
   * Set the user id
   */
  function set_uid($uid) {
    $this->uid = $uid;
  }

  /**
   * Ensure box 0 has no more than the maximum allowed cards.
   *
   * It often happens in a revision set (which contains items from
   * more than one started set), that a particular box has more words
   * in it than would normally be allowed.  This is particularly a problem
   * with box 0, where the words are new and have nowhere lower to go. In
   * that case we start the set by returning any extra words above the
   * box 0 size to the pool.
   */
  protected function _normalise_first_started_box() {

    while ($this->boxes[FLIPCARD_UNSTARTED_LEVEL + 1]->count() > 
    $this->boxes[FLIPCARD_UNSTARTED_LEVEL + 1]->size()) {
      $card = $this->boxes[FLIPCARD_UNSTARTED_LEVEL + 1]->get_first_card();

      // drupal_set_message("Moved card '" . $card->title() . "' back to pool.");
      $this->_move_card($card, FLIPCARD_UNSTARTED_LEVEL);
    }
  }

  /**
   * Get a particular box
   */
  function get_box($box_no) {
    return $this->boxes[$box_no];
  }

  /**
   * Get the next card.
   *
   * A card is selected in the following order:
   * 1) Occasionally (randomly 1 time in 10), revise a card from the top box 
   * 2) If any box is full then take the front card from that box.
   * 3) If no box is full, take any card from the pool
   * 4) If there are no boxes full, and no cards in the pool, take the oldest card.
   *
   * @return Card
   */
  function get_next_card() {
  	// Occasionally (randomly 1 time in 10), revise a card from the top box
  	$top_box = $this->get_box(FLIPCARD_TOP_LEVEL);
  	if (rand(1, 10) == 1 
  	&& $top_box->count() >= 3 
  	&& $this->boxes[FLIPCARD_UNSTARTED_LEVEL + 1]->count() < 
  	$this->boxes[FLIPCARD_UNSTARTED_LEVEL + 1]->size()) {
  		$card = $top_box->get_first_card();
  		
  		return $card;
  	}
  	
    // See if any box is full, and if so return the first card
    foreach ($this->boxes as $key => $box) {
      if ($box->count() >= $box->size()) {
        $card = $box->get_first_card();

        return $card;
      }
    }
    
    // See if the pool has any cards, and if so return one of them
    if ($this->boxes[FLIPCARD_UNSTARTED_LEVEL]->count() > 0) {
      $pool_card = $this->boxes[FLIPCARD_UNSTARTED_LEVEL]->get_random_card();

      return $pool_card;
    }

    // In the worst case, just get the oldest card
    //    drupal_set_message("Showing the oldest card");
    return $this->get_oldest_card();
  }

  /**
   * Get the oldest card from the users collection of cards
   */
  function get_oldest_card() {
    global $user;

    $sql = "SELECT cards.nid, cards.box
      FROM {flipcard_cards} AS cards, 
      {taxonomy_index} AS taxonomy
      WHERE tid = :tid
      AND uid = :uid
      AND cards.nid = taxonomy.nid
      ORDER BY cards.time_reviewed ASC "; // We want the oldest first

    $result = db_query_range($sql, 0, 1,
      array(':tid' => $this->tid, ':uid' => $this->uid));
    $row = $result->fetchObject();

    $card = new Card($row->nid, $this->uid);

    return $card;
  }

  /**
   * Move a card into the given box
   */
  protected function _move_card(Card $card, $box_no) {
    $card->setBox($box_no);
    
    $card->save();
  }

  /**
   * Count how many cards there are in a given set.
   */
  function cards_count() {
    $sql = "SELECT count(nid) 
      FROM {taxonomy_index}
      WHERE {taxonomy_index}.tid = :tid";

    $result = db_query($sql, array(':tid' => $this->tid));

    $cards_count = $result->fetchField();

    return $cards_count;
  }

  /**
   * Count how many cards there are started
   */
  function cards_started() {
    $debug_count = array();
     
    $started = 0;
    for ($box_no=FLIPCARD_UNSTARTED_LEVEL + 1; $box_no<=FLIPCARD_TOP_LEVEL; $box_no++) {
      $count = $this->get_box($box_no)->count();
      $started += $count;
       
      $debug_count[$box_no] = $count;
    }

    return $started;
  }

  /**
   * Calculate the average level of the given taxonomy set
   */
  function average_level() {
    $sql = "SELECT nid 
      FROM {taxonomy_index}
      WHERE {taxonomy_index}.tid = :tid
      ORDER by nid";

    $result = db_query($sql, array(':tid' => $this->tid));

    $cards = 0;
    $box_total = 0;
    foreach ($result as $row) {
      $cards++;

      $card = new Card($row->nid, $this->uid);
      $box_total += $card->box();
    }

    $average = 0;
    if ($cards > 0) {
      $average = $box_total / $cards;
    }

    return $average;
  }
}
