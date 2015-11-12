<?php

/**
 * @file
 * Definition of Scoresheet (list of moves).
 * 
 * The scoresheet is the table of moves in the game.
 */

/**
 * Scoresheet class
 */
class Scoresheet {
  protected $scoresheet = array();
  
  /**
   * Create a new scoresheet
   * 
   * @param unknown_type $gid Game id
   */
  function __construct() {
    
  }
  
  /**
   * Load the moves for a scoresheet
   */
  function load($gid) {
    $sql = "SELECT move_no, color, long_move, algebraic, datetime FROM {vchess_moves} WHERE gid = :gid";
    $result = db_query($sql, array('gid' => $gid));
    
    foreach ($result as $row) {
      $move = new Move;
      $move->set_long_format($row->long_move);
      $move->set_algebraic($row->algebraic);
      $move->set_timestamp($row->datetime);
    
      $this->scoresheet[$row->move_no][$row->color] = $move;
    }
  }
  
  /**
   * Get the move number
   */
  function move_no() {
    $move_no = count($this->scoresheet);
    // If the scoresheet is empty then we are on move 1
    if ($move_no == 0) {
      $move_no = 1;
    }
    else {
//     if black has not yet moved, then the move number is the length of the array, 
//       otherwise it is the length plus one
      
//       e.g. if 3. Nc3
//       then scoresheet has:
//       $scoresheet[3]['w'] = "Nc3"
//       and so => move number = 3
      
//       e.g. if 3. Nc3 Nc6 
//       then scoresheet has:
//       $scoresheet[3]['w'] = "Nc3"
//       $scoresheet[3]['b'] = "Nc6"
//       and so => move number = 4
      if (array_key_exists("b", $this->scoresheet[count($this->scoresheet)])) {
        $move_no = $move_no + 1;
      }
    }
  
    return $move_no;
  }
  
  /**
   * Write the latest move down.  The move is added to the end
   * of the scoresheet.
   */
  function write_move_down(Move $move) {
    $move_no = $this->move_no();
    if (!array_key_exists($move_no, $this->scoresheet)) {
      $this->scoresheet[$move_no] = array();  
    }
        
    if (array_key_exists('w', $this->scoresheet[$move_no])) {
      $this->scoresheet[$move_no]['b'] = $move;
    }
    else {
      $this->scoresheet[$move_no]['w'] = $move;
    }
  }
  

  /**
   * Get the white move of a particular number
   * 
   * @param int $move_no
   */
  function white_move($move_no) {
    return $this->_get_move($move_no, "w");
  }
  
  /**
   * Get the black move of a particular number
   * 
   * @param int $move_no
   */
  function black_move($move_no) {
    return $this->_get_move($move_no, "b");
  }
  
  /**
   * Get the move of a given color
   * 
   * Outside this class, use instead one of:
   * - white_move()
   * - black_move()
   */
  function _get_move($move_no, $color) {
    $move = new Move;
  
    if (array_key_exists($move_no, $this->scoresheet)) {
      if (array_key_exists($color, $this->scoresheet[$move_no])) {
        $move = $this->scoresheet[$move_no][$color];
      }
    }
    
    return $move;
  }
  
  /**
   * Get the last move
   */
  function last_move() {
    $move_no = $this->move_no();
    
    $move = $this->black_move($move_no);
    if ($move->algebraic() == "") {
      // Looks like black hasn't moved yet for this move no
      $move = $this->white_move($move_no);
      
      if ($move->algebraic() == "" && $move_no > 1) {
        // Looks like black has just moved, so we need to look at the previous move number
        $move = $this->black_move($move_no - 1); 
      }
    }
    
    return $move;
  }
  
  /** 
   * Get the scoresheet as a table
   * 
   * @return
   *   Returns a themed table of moves.
   */
  function get_table() {
    $rows = array();
    foreach ($this->scoresheet as $move_no => $moves) {
      if (array_key_exists('b', $moves)) {
        $rows[] = array($move_no, $moves['w']->algebraic(), $moves['b']->algebraic());        
      }
      else {
        $rows[] = array($move_no, $moves['w']->algebraic(), "");
      }
    }
    $table['header'] = array('Move #', 'White', 'Black');
    $table['rows'] = $rows;
    $table['empty'] = "There are no moves played yet.";
    $table['attributes'] = array();  // I don't care, but I'm forced to define this
    $table['caption'] = "";   // I don't care about this either
    $table['colgroups'] = array();  // I just don't care about this either
    $table['sticky'] = "";  // Nope, don't care
    
    return theme_table($table);
  }
}