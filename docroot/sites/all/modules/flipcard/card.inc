<?php

/**
 * @file
 * Contains the Card class.
 */

/**
 * A Card represents a node for a particular user in a box.
 */
class Card {
  protected $uid;
  protected $nid;

  // The box represents the level which this card has been
  // learnt and starts at 0 (for the pool) and then goes from 1-10
  protected $box;
  
  protected $shown;
  protected $correct;

  /**
   * Constructor.
   */
  function __construct($nid, $uid=NULL) {
    if ($uid == NULL) {
      global $user;
      $this->uid = $user->uid;
    }
    else {
      $this->uid = $uid;
    }

    $this->nid = $nid;
    
    $this->_load_stats();
  }

  /**
   * Return the box which this card is currently in
   *
   * The box represents the level which this card has been
   * learnt and starts at 0 (for the pool) and then goes from 1-10
   */
  function box() {
    if (!isset($this->box)) {
      $this->_load_stats();
    }

    return $this->box;
  }

  /**
   * Save the current card.
   */
  function save() {
    $sql = "SELECT * FROM {flipcard_cards}
      WHERE uid = :uid
      AND nid = :nid";
    $result = db_query($sql, array(':uid' => $this->uid, ':nid' => $this->nid));
    
    if ($result->rowCount() == 1) {
    	if ($this->box <> FLIPCARD_UNSTARTED_LEVEL) {
      	$result = db_update('flipcard_cards')->fields(array(
		      'shown' => $this->shown,
		      'correct' => $this->correct,
		      'box' => $this->box,
		      'time_reviewed' => time()
		      ))
		      ->condition('uid', $this->uid)
		      ->condition('nid', $this->nid)
		      ->execute();
    	}
    	else {
    		// We don't save any card which is in the unstarted level. Instead it
    		// is implied. Hence, if a card needs to be in the unstarted box, then
    		// we delete the card for this user.
    		db_delete('flipcard_cards')
    			->condition('uid', $this->uid)
	    		->condition('nid', $this->nid)
  	  		->execute();
    	}
    }
    else {
    	// We only save cards in the started levels.
    	if ($this->box > FLIPCARD_UNSTARTED_LEVEL) {
      	$insert_query = db_insert('flipcard_cards')
        	->fields(array(
          	'uid' => $this->uid,
          	'nid' => $this->nid,
          	'shown' => $this->shown,
          	'correct' => $this->correct,
          	'box' => $this->box,
          	'time_reviewed' => time()
        	)
      	);
        $result = $insert_query->execute();
    	}
    }
  }
  
  /**
   * Set the box number.
   */
  function setBox($box) {
    if ($box <= 10) {
      $this->box = $box;
    }
    else {
      $this->box = 10;
    }
  }
  
  /**
   * Set the number of times the card has been shown.
   */
  function setShown($shown) {
    $this->shown = $shown;
  }
  
  /**
   * Set the number of times this card has been answered correctly.
   */
  function setCorrect($correct) {
    $this->correct = $correct;
  }
  
  /**
   * Load the stats
   */
  function _load_stats() {
    $sql = "SELECT * FROM {flipcard_cards} WHERE uid = :uid AND nid = :nid";
    
    $result = db_query($sql, 
      array(':uid' => $this->uid, ':nid' => $this->nid));
    
    if ($result->rowCount() > 0) {
      $stats = $result->fetchAssoc();
      
      $this->shown = $stats['shown'];
      $this->correct = $stats['correct'];
      $this->box = $stats['box'];
    }
    else {
      $this->shown = 0;
      $this->correct = 0;
      $this->box = 0;
    }
  }

  /**
   * Reset the stats
   */
  function reset_stats() {
    db_delete('flipcard_cards')
    ->condition('uid', $this->uid)
    ->condition('nid', $this->nid)
    ->execute();
  }

  /**
   * Get the node id of the card
   */
  function nid() {
    return $this->nid;
  }

  /**
   * Get the user id of the card
   */
  function uid() {
    return $this->uid;
  }

  /**
   * Get the number of times this card has been shown
   */
  function shown() {
    if (!isset($this->shown)) {
      $this->_load_stats();
    }
    	
    return $this->shown;
  }

  /**
   * Get the number of times this card has been answered correctly.
   */
  function correct() {
    if (!isset($this->correct)) {
      $this->_load_stats();
    }

    return $this->correct;
  }

  /**
   * Get the title of the node
   */
  function title() {
    $node = node_load($this->nid);

    return $node->title;
  }
}