<?php

/**
 * @file
 * This file has functions relating specifically to a board
 */

// Define as a FEN string the standard board starting position
// For FEN, we start with the black side of the board (a8-h8), and finish with the white pieces (a1-h1).
// For FEN, white pieces are stored in UPPER CASE, black pieces in lower case and blank squares as space.
//
// e.g. after 1.e4 the FEN string will be:
// rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR
define("BOARD_DEFAULT", "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
define("BOARD_PROMOTION", "k7/4P3/8/8/8/8/8/K7");

// Constants for diagonal calculation
define("DIAGONAL_UP_RIGHT", 9);
define("DIAGONAL_UP_LEFT", 7);
define("DIAGONAL_DOWN_LEFT", -9);
define("DIAGONAL_DOWN_RIGHT", -7);

// Define the column letters
define("COL_a", 1);
define("COL_b", 2);
define("COL_c", 3);
define("COL_d", 4);
define("COL_e", 5);
define("COL_f", 6);
define("COL_g", 7);
define("COL_h", 8);

/**
 * The Board is designed to take some of the complexity away from the $board array
 * by adding useful functions rather than having the rest of the program need to understand
 * and handle issues like whether a square is blank or not.
 *
 * The Board is basically the static view of what a board looks like (i.e. what pieces are
 * where) and but also includes the following game information:
 * - whether or not a player may castle (queenside and/or kingside)
 * - what the en passant target square is, if any
 *
 * This class acts as the MODEL for the board.
 */
class Board {
  // The board is stored as an array where the key is the coord and the elements
  // are items of type Piece
  // e.g. array(
  //      "a1"->Piece /* white rook */,
  //      "a2"->Piece /* white knight */,
  //      etc.)
  protected $board = array();

  protected $en_passant;

  /**
   * Initialise the board as empty.  We choose later whether to fill with
   * the default board or not.
   */
  function __construct() {

  }

  /**
   * Setup with the standard position
   */
  function setup_as_standard() {
    $this->setup_position(BOARD_DEFAULT);
  }

  /**
   * Setup the board using a FEN (Forsyth–Edwards Notation) string, e.g.
   * rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR
   *
   * See http://en.wikipedia.org/wiki/Forsyth-Edwards_Notation
   */
  function setup_position($fen_string) {
    // Ensure any previous position is cleared
    unset($this->board);

    $chars = str_split($fen_string, 1);

    // The FEN string starts from the black side
    $col = COL_a;
    $row = 8;
    foreach ($chars as $char) {
      if ($char == "/") {
        $col = COL_a;
        $row--;
      }
      elseif (is_numeric($char)) {
        $col += $char;
      }
      else {
        $piece = new Piece();
        if (drupal_strtoupper($char) == $char) {
           // White piece
          $piece->set_type($char);
          $piece->set_color("w");
        }
        else {
          // Black piece
          $piece->set_type($char);
          $piece->set_color("b");
        }

        $coord = vchess_col_row2coord($col, $row);
        $this->board[$coord] = $piece;
        $col++;
      }
    }
  }

  /**
   * Set the piece at a given coordinate
   *
   * @param Piece $piece
   * @param coord $coord e.g. "a1"
   */
  function set_piece($piece, $coord) {
    $this->board[$coord] = $piece;
  }

  /**
   * Get an array of squares for a given color and piece type.
   * e.g. array("a1", "h1") for white rooks
   *
   * @param $search_type 
   *   The type of being searched for, one of 'K', 'Q', 'R', 'B', 'N' or 'P'
   * @param $color 
   *   The color of the pieces being searched for, either 'w' or 'b'
   *   
   * @return
   *   Returns an array of squares for pieces of a given color and piece type
   */
  function piece_type_squares($search_type, $color) {
    $squares = array();
    
    foreach ($this->board as $piece_coord => $piece) {
      if ($piece->type() == $search_type && $piece->color() == $color) {
        $square = new Square;
        $square->set_coord($piece_coord);
        $squares[] = $square;
      }
    }
    
    return $squares;
  }
  
  /**
   * Get an array of squares for a given piece color
   * e.g. array("a1", "a2", ..., "h2") for white at the start of the game
   *
   * @param $color
   *   Player color whose pieces we would like an array of their locations
   */
  function pieces_squares($color) {
    $squares = array();

    foreach ($this->board as $piece_coord => $piece) {
      if ($piece->color() == $color) {
        $square = new Square;
        $square->set_coord($piece_coord);
        $squares[] = $square;
      }
    }

    return $squares;
  }

  /**
   * Get the squares on the diagonals from a given square
   *
   * @from_square
   *   The square which we are starting from
   */
  function diagonal_squares(Square $from_square) {
    $squares = array();

    $squares = array_merge($squares, $this->_single_diagonal_squares($from_square, DIAGONAL_UP_LEFT));
    $squares = array_merge($squares, $this->_single_diagonal_squares($from_square, DIAGONAL_DOWN_LEFT));
    $squares = array_merge($squares, $this->_single_diagonal_squares($from_square, DIAGONAL_UP_RIGHT));
    $squares = array_merge($squares, $this->_single_diagonal_squares($from_square, DIAGONAL_DOWN_RIGHT));

    return $squares;
  }
  
  /**
   * Get the knight moves from a given square
   *
   * @from_square
   *   The square which we are starting from
   */
  function knight_squares(Square $from_square) {
    $squares = array();

    $from_rank = $from_square->rank();  // e.g. "7" in "d7"

    $from_file = $from_square->file();  // e.g. "d" in "d7"
    $from_col = vchess_file2col($file); // e.g. "4" for the "d" in "d7"

    $deltas = array(
        array(-2, -1),
        array(-2, 1),
        array(-1, -2),
        array(-1, 2),
        array(1, -2),
        array(1, 2),
        array(2, -1),
        array(2, 1)
        );
    foreach ($deltas as $delta_pair) {
      $delta_rank = $delta_pair[0];
      $delta_col = $delta_pair[1];
      if ($from_col + $delta_col >= 1 && $from_col + $delta_col <= 8
      && $from_rank + $delta_rank >= 1 && $from_rank + $delta_rank <= 8) {
        $square = new Square;
        $coord = vchess_col2file($from_col + $delta_col) . ($from_rank + $delta_rank);
        $square->set_coord($coord);
        $squares[] = $square;
      }
    }

    return $squares;
  }
  
  /**
   * Get the squares from a given start square based on an index increment
   *
   * @param Square $from_square
   *   Square where diagnonal is starting
   * @param integer $direction
   *   Increment of the diagonal, one of:
   *   - DIAGONAL_UP_LEFT (7)
   *   - DIAGONAL_UP_RIGHT (9)
   *   - DIAGONAL_DOWN_LEFT (-9)
   *   - DIAGONAL_DOWN_RIGHT (-7)
   *  
   * @return
   *   An array of squares
   */
  function _single_diagonal_squares(Square $from_square, $direction) {
    $squares = array();

    $finished = FALSE;

    $new_index = $from_square->index();
    while (!$finished) {
      $square = vchess_i2square($new_index);

      if ($square->file() == 'a' && ($direction == DIAGONAL_UP_LEFT || $direction == DIAGONAL_DOWN_LEFT)) {
        $finished = TRUE;
      }
      if ($square->file() == 'h' && ($direction == DIAGONAL_UP_RIGHT || $direction == DIAGONAL_DOWN_RIGHT)) {
        $finished = TRUE;
      }
      if ($square->rank() == '1' && ($direction == DIAGONAL_DOWN_LEFT || $direction == DIAGONAL_DOWN_RIGHT)) {
        $finished = TRUE;
      }
      if ($square->rank() == '8' && ($direction == DIAGONAL_UP_LEFT || $direction == DIAGONAL_UP_RIGHT)) {
        $finished = TRUE;
      }
      
      if (!$finished) {
        $new_index += $direction;
        $squares[] = vchess_i2square($new_index);
      }
    }
    
    return $squares;
  }
  
  /**
   * Get the squares on a given rank
   * 
   * e.g. given a rank like "8" it will return the 
   * squares "a8" to "h8"
   * 
   * @param $rank The number of the rank (1..8)
   */
  function rank_squares($rank) {
    $squares = array();
    
    for ($col = 1; $col < 8; $col++) {
      $square = new Square;
      $square->set_coord(vchess_col2file($col) . $rank);
      
      $squares[] = $square;
    }
    
    return $squares;
  }
  
  /**
   * Get the squares on the rank and file from a given square
   * 
   * @param $square
   *   Square to start from
   *   
   * @return
   *   An array of squares
   */
  function rank_and_file_squares(Square $square) {
    $squares = array();
    
    $squares = array_merge($squares, $this->rank_squares($square->rank()));
    $squares = array_merge($squares, $this->file_squares($square->file()));
    
    return $squares;
  }
  
  /**
   * Get the squares on a given file
   *
   * e.g. given a file like "a" it will return the
   * squares "a1" to "a8"
   *
   * @param $file The letter of the file, e.g. "a"
   */
  function file_squares($file) {
    $squares = array();
  
    for ($rank = 1; $rank < 8; $rank++) {
      $square = new Square;
      $square->set_coord($file . $rank);
  
      $squares[] = $square;
    }
  
    return $squares;
  }
  
  /**
   * Get the square that the king is on
   */
  function king_square($color) {
    $squares = $this->piece_type_squares("K", $color);
    
    // There should be only 1 square returned 
    return $squares[0]; 
  }

  /**
   * Returns TRUE if the given square is empty
   * 
   * @param coord $coord coordinate, e.g. "a1" 
   */
  function square_is_empty(Square $square) {
    $empty = TRUE;
    if (array_key_exists($square->coord(), $this->board)) {
      $empty = FALSE;
    }

    return $empty;
  }
  
  /**
   * Returns TRUE if the square at the given coord is empty
   */
  function square_at_coord_is_empty($coord) {
    $square = new Square;
    $square->set_coord($coord);
    
    return $this->square_is_empty($square);
  }

  /**
   * Get the player color whose piece is on a given square
   * 
   * @param $square e.g. "a1"
   *
   * @return 'w', 'b' or ''
   */
  function player_on_square(Square $square) {
    $player = "";
    if (!$this->square_is_empty($square)) {
      $player = $this->piece($square)->color();
    }

    return $player;
  }

  /**
   * Convert board in array format (for use in the program) into
   * FEN string (for saving in the database).
   * 
   * e.g. after 1.e4 the FEN string will be:
   * rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR
   *
   * The board is kept internally with one piece per square, starting with a1, a2, ..., b1, b2, ... h8.
   * For FEN, we start with the black side of the board (a8-h8), and finish with the white pieces (a1-h1).
   * For FEN, white pieces are stored in UPPER CASE, black pieces in lower case and blank squares as space.
   *
   */
  function position() {
    $FEN_string = "";
    
    $coord = new Square;
    for ($row = 8; $row >= 1; $row--) {
      $empty_squares = 0;
      for ($col = 1; $col <= 8; $col++) {
        $coord->set_coord(vchess_col2file($col) . $row);
        if ($this->square_is_empty($coord)) {
          $empty_squares++;
        }
        else {
          $piece = $this->piece($coord);
          if ($empty_squares > 0) {
            $FEN_string .= $empty_squares;
            $empty_squares = 0;
          }
          $FEN_string .= $piece->get_FEN_type();
        }
      }
      // The row may end with empty squares
      if ($empty_squares > 0) {
        $FEN_string .= $empty_squares;
        $empty_squares = 0;
      }
      // All rows except the row 1 with a / 
      if ($row > 1) {
        $FEN_string .= "/";
      }
    }
    
    return $FEN_string;
  }
  
  /**
   * Check a number of squares given a start, an end square
   * (which is not included to the check) and a position
   * change for each iteration. Return TRUE if not blocked.
   * All values are given for 1dim board.
   *
   * @param 
   *   $start index of square, 0..63
   *   
   * @param 
   *   $end   index of square, 0..63
   * 
   * @param $change
   *   Number of index change
   */
  function path_is_not_blocked($start, $end, $change) {
    $blocked = FALSE;
  
    for ($pos = $start; $pos != $end; $pos += $change) {
      $square = vchess_i2square($pos);
      if (!$this->square_is_empty($square)) {
        $blocked = TRUE;
      }
    }
  
    return !$blocked;
  }
  
  /**
   * Check all pieces of player whether they attack the given position.
   *
   * @param $to_square
   *   The square which is being checked to see if it is under attack e.g. "d4"
   *   
   * @param string $attacker
   *   Color of player who is doing the attacking, either 'w' or 'b'
   *
   */
  function square_is_under_attack(Square $to_square, $attacker) {
    $under_attack = FALSE;
  
    $pieces_squares = $this->pieces_squares($attacker);
    foreach ($pieces_squares as $from_square) {
      if ($this->piece_attacks($from_square, $to_square)) {
        $under_attack = TRUE;
      }
    }
  
    return $under_attack;
  }
  
  /**
   * Check if given piece attacks the given square
   * 
   * @param $attack_square
   *   Square of piece trying to attack
   * 
   * @param $to_square
   *   Square which is being tested to see if it is attacked or not
   */
  function piece_attacks($attack_square, $to_square) {
    $attacks = FALSE;
    
    $piece = $this->piece($attack_square);
    if ($piece->type() == "P") {
      // For a pawn, we have to check whether it actually attacks
      $attacks = $this->pawn_attacks($attack_square, $to_square);
    }
    else {
      $attacks = $this->square_is_reachable($attack_square, $to_square); 
    }
    
    return $attacks;
  }
  
  /**
   * Check whether player's king is in check.
   */
  function is_check($player) {
    $king_square = $this->king_square($player);
  
    if ($player == 'w') {
      $in_check = $this->square_is_under_attack($king_square, 'b');
    }
    else {
      $in_check = $this->square_is_under_attack($king_square, 'w');
    }
  
    return $in_check;
  }
  
  /**
   * Get the square in front of another square.  This only applies to squares
   * with pawns on, since only they have the concept of in front or behind. 
   * 
   * @param Square $square
   *   The original square for which we want to find the square in front of it
   * 
   */
  function square_in_front(Square $square) {
    if ($this->piece($square)->color() == 'w') {
      $new_square = vchess_i2square($square->index() + 8);
    }
    else {
      $new_square = vchess_i2square($square->index() - 8);
    }
    
    return $new_square;
  }
  
  /**
   * Get the square 2 in front of another square. This only applies to squares
   * with pawns on, since only they have the concept of in front or behind. 
   * 
   * @param Square $square
   *   The original square for which we want to find the square in front of it
   */
  function square_2_in_front(Square $square) {
    if ($this->piece($square)->color() == 'w') {
      $new_square = vchess_i2square($square->index() + 16);
    }
    else {
      $new_square = vchess_i2square($square->index() - 16);
    }
  
    return $new_square;
  }
  
  /**
   * Return the array of adjacent squares (<=8).  
   * 
   * @return 
   *   Returns an array of Squares.
   */
  function get_adjacent_squares(Square $square) {
    $adj_squares = array();
    
    $square_index = $square->index();
    $i = 0;
    $x = $square_index % 8;
    $y = floor($square_index / 8);
  
    if ($x > 0 && $y > 0) {
      $adj_squares[$i++] = vchess_i2square($square_index - 9);
    }
    if ($y > 0) {
      $adj_squares[$i++] = vchess_i2square($square_index - 8);
    }
    if ($x < 7 && $y > 0) {
      $adj_squares[$i++] = vchess_i2square($square_index - 7);
    }
    if ($x < 7) {
      $adj_squares[$i++] = vchess_i2square($square_index + 1);
    }
    if ($x < 7 && $y < 7) {
      $adj_squares[$i++] = vchess_i2square($square_index + 9);
    }
    if ($y < 7) {
      $adj_squares[$i++] = vchess_i2square($square_index + 8);
    }
    if ($x > 0 && $y < 7) {
      $adj_squares[$i++] = vchess_i2square($square_index + 7);
    }
    if ($x > 0) {
      $adj_squares[$i++] = vchess_i2square($square_index - 1);
    }
  
    return $adj_squares;
  }
  
  
  /**
   * Check whether player's king is in checkmate
   * 
   * @param $defender
   *   Player, either 'w' or 'b'
   */
  function is_checkmate($defender) {
    $in_checkmate = TRUE;  // we will look to find a counter-example
    
    if ($defender == 'w') {
      $opponent = 'b';
    }
    else {
      $opponent = 'w';
    }
  
    // Find the position of the player's king
    $king_square = $this->king_square($defender);
  
    // Test adjacent squares
    $adj_squares = $this->get_adjacent_squares($king_square);
    //  $contents = $board[$king_pos];
    //  $board[$king_pos] = '';
    foreach ($adj_squares as $adj_square) {
      // If this adjacent square has a piece of the same color, then
      // we cannot move the king there
      if ($this->piece($adj_square)->color() == $defender) {
        continue;
      }
      // If this adjacent square is under attack, then we cannot
      // move the king there
      if ($this->square_is_under_attack($adj_square, $opponent)) {
        continue;
      }
      //    $board[$king_pos] = $contents;
      // Since this square is neither occupied by one of our own pieces
      // or is under attack then we can move the king there and so it
      // isn't checkmate
      $in_checkmate = FALSE;
    }
  
    // Get all pieces that attack the king
    $attacker_squares = $this->pieces_attacking_square($king_square, $opponent);
  
    // If there is only one attacker, then it might be possible to capture the piece to escape checkmate
    if (count($attacker_squares) == 1) {
      // There is only 1 attacker.  Check whether this attacker can be captured by own defending piece.
      $defender_pieces_squares = $this->pieces_squares($defender);
      foreach ($defender_pieces_squares as $defender_piece_square) {
        if ($this->piece_attacks($defender_piece_square, $attacker_squares[0])) {
          $piece = $this->piece($defender_piece_square);
          if ($piece->type() == 'K') {
            // If the piece which could capture is the king, then we need to check if 
            // the piece it wants to take is defended.  If so, this is not valid
            $defender_squares = $this->pieces_attacking_square($attacker_squares[0], $opponent);
            if (count($defender_squares) == 0) {
              // There is nothing defending the attacking piece, so the king can take it
              $in_checkmate = FALSE;
            }
          }
          else {
            // Attacker is not the king, so OK to use it
            $in_checkmate = FALSE;            
          }
        }
      }
      
      if ($in_checkmate) {
        // Check whether a defending piece can move in the way to block
        $inbetween_squares = vchess_get_inbetween_squares($this->piece($attacker_squares[0])->type(), $attacker_squares[0], $king_square);
        foreach ($inbetween_squares as $inbetween_square) {
          $defending_squares = $this->pieces_squares($defender);
          foreach ($defending_squares as $defending_square) {
            $piece = $this->piece($defending_square);
            $piece_type = $piece->type();
            if ($this->move_is_ok($defending_square, $inbetween_square)) {
              $in_checkmate = FALSE;
            }
          }
        }
      }
    }
    
    return $in_checkmate;
  }

  /**
   * Get an array of all the pieces attacking a particular square
   * 
   * @param Square $square The square which may be attacked
   * @param $attacker The color of the attacker, 'w' or 'b'
   */
  function pieces_attacking_square(Square $square, $attacker) {
    $attacker_squares = array();
    $opponent_pieces_squares = $this->pieces_squares($attacker);
    foreach ($opponent_pieces_squares as $opponent_piece_square) {
      $piece = $this->piece($opponent_piece_square);
      if ($this->piece_attacks($opponent_piece_square, $square)) {
        $attacker_squares[] = $opponent_piece_square;
      }
    }
    
    return $attacker_squares;
  }
  
  /**
   * Check whether $to_square is in reach for piece on the
   * $piece_square. It is not checked whether the square
   * itself is occupied but only the squares in between.
   * 
   * @param $from_square
   *   Square on which piece starts from
   * @param $to_square
   *   Square where piece would like to go to if possible
   * 
   */
  function square_is_reachable(Square $from_square, Square $to_square) {
    $reachable = FALSE;
  
    $piece_type = $this->piece($from_square)->type();
    if ($from_square != $to_square) {
      $piece_pos = $from_square->index();
      $dest_pos = $to_square->index();
  
      $piece_y = floor($piece_pos / 8) + 1;
      $piece_x = $piece_pos % 8;
      $dest_y = floor($dest_pos / 8) + 1;
      $dest_x = $dest_pos % 8;
  
      switch ($piece_type) {
        // Pawn
        case 'P':
          // For a pawn we need to take into account the colour since a pawn is the one
          // piece which cannot go backwards
          $piece_color = $this->piece($from_square)->color();
          if ($piece_color == "w") {
            if (($dest_y - $piece_y) == 1) { // Normal 1-square move
              $reachable = TRUE;
            }
            elseif ($piece_y == 2 && (($dest_y - $piece_y) == 2)) { // Initial 2-square move
              $reachable = TRUE;
            }
          }
          else { // $piece_color == "b"
            if (($dest_y - $piece_y) == -1) {
              $reachable = TRUE;
            }
            else {
              if ($piece_y == 7 && (($dest_y - $piece_y) == -2)) { // Initial 2-square move
                $reachable = TRUE;
              }
            }
          }
          break;
        // Knight
        case 'N':
          if (abs($piece_x - $dest_x) == 1 && abs($piece_y - $dest_y) == 2) {
            $reachable = TRUE;
          }
          if (abs($piece_y - $dest_y) == 1 && abs($piece_x - $dest_x) == 2) {
            $reachable = TRUE;
          }
          break;
        // Bishop
        case 'B':
          if (abs($piece_x - $dest_x) != abs($piece_y - $dest_y)) {
            break;
          }
          if ($dest_y < $piece_y) {
            $change = -8;
          }
          else {
            $change = 8;
          }
          if ($dest_x < $piece_x) {
            $change -= 1;
          }
          else {
            $change += 1;
          }
          if ($this->path_is_not_blocked($piece_pos + $change, $dest_pos, $change)) {
            $reachable = TRUE;
          }
          break;
        // rook
        case 'R':
          if ($piece_x != $dest_x && $piece_y != $dest_y) {
            break;
          }
          if ($piece_x == $dest_x) {
            if ($dest_y < $piece_y) {
              $change = -8;
            }
            else {
              $change = 8;
            }
          }
          else {
            if ($dest_x < $piece_x) {
              $change = -1;
            }
            else {
              $change = 1;
            }
          }
          if ($this->path_is_not_blocked($piece_pos + $change, $dest_pos, $change)) {
            $reachable = TRUE;
          }
          break;
        // queen
        case 'Q':
          if (abs($piece_x - $dest_x) != abs($piece_y - $dest_y) && $piece_x != $dest_x && $piece_y != $dest_y) {
            break;
          }
          // Check if diagonal
          if (abs($piece_x - $dest_x) == abs($piece_y - $dest_y)) {
            if ($dest_y < $piece_y) {
              // diagonal down the board
              $change = -8;
            }
            else {
              // diagonal up the board
              $change = 8;
            }
            if ($dest_x < $piece_x) {
              // diagonal to the left
              $change -= 1;
            }
            else {
              // diagonal to the right
              $change += 1;
            }
          }
          elseif ($piece_x == $dest_x) {
            // vertical
            if ($dest_y < $piece_y) {
              // vertical down the board
              $change = -8;
            }
            else {
              // vertical up the board
              $change = 8;
            }
          }
          else {
            // horizontal
            if ($dest_x < $piece_x) {
              // horizontal to the left
              $change = -1;
            }
            else {
              // horizontal to the right
              $change = 1;
            }
          }
          if ($this->path_is_not_blocked($piece_pos + $change, $dest_pos, $change)) {
            $reachable = TRUE;
          }
          break;
        // king
        case 'K':
          if (abs($piece_x - $dest_x) > 1 || abs($piece_y - $dest_y) > 1) {
            break;
          }
          $kings = 0;
          $adj_squares = $this->get_adjacent_squares($from_square);
          foreach ($adj_squares as $adj_square) {
            if ($this->piece($adj_square)->type() == 'K') {
              $kings++;
            }
          }
          if ($kings == 2) {
            break;
          }
          $reachable = TRUE;
          break;
      }
  
    }
  
    return $reachable;
  }
  
  /**
   * Get the piece on a given square
   *
   * @param $square e.g. "a1"
   */
    function piece(Square $square) {
      $piece = new Piece;
      if (array_key_exists($square->coord(), $this->board)) {
        $piece = $this->board[$square->coord()];
      }
  
      return $piece;
    }

  /**
   * Move a piece from one square to another
   * 
   * No checking is done here as to the validity of the move
   */
  function move_piece(Square $from_square, Square $to_square) {
    $this->board[$to_square->coord()] = $this->board[$from_square->coord()];
    unset($this->board[$from_square->coord()]);
  }
  
  /**
   * Perform en passant pawn capture
   */
  function en_passant_capture(Square $from_square, Square $to_square) {
    // Calculate the square of the pawn which has just moved 2 squares
    if ($from_square->rank() == 4) {
      // Example: 
      // black pawn on e4 ($from_square->coord())
      // white pawn moves d2-d4,
      // black pawn captures with long move Pe4-d3 ($to_square->coord() is d3)
      // algebraic move is exd3
      $enemy_pawn_coord = $to_square->file() . "4";
    }
    elseif ($from_square->rank() == 5) {
      // Example:
      // white pawn on b5 ($from_square->coord())
      // black pawn moves c7-c5,
      // white pawn captures with long move Pb5-c6 ($to_square->coord() is c6)
      // algebraic move is bxc6
      $enemy_pawn_coord = $to_square->file() . "5";
    }
    
    // Move the pawn to the empty square
    $this->move_piece($from_square, $to_square);
    
    // Remove the pawn which has just moved 2 squares
    unset($this->board[$enemy_pawn_coord]);
  }
  
  /**
   * Promote a pawn.  This is effectively a move of a pawn with a change
   * of piece type.
   */
  function promotion(Square $from_square, Square $to_square, Piece $new_piece) {
    $this->move_piece($from_square, $to_square);
    
    $coord = $to_square->coord();
    $this->board[$coord]->set_type($new_piece->type());
  }
  
  /**
   * Check whether pawn at $pawn_square attacks the
   * square $to_square, i.e. the diagonally attacked square
   *
   * Note that it is not necessary for a piece to be on the
   * destination square for that square to be attacked
   *
   * @param $pawn_square 
   *   Square of pawn
   *   
   * @param $to_square 
   *   Square of attacked square
   *   
   * @see piece_attacks()
   */
  function pawn_attacks(Square $pawn_square, Square $to_square) {
    $attacks = FALSE;
  
    $piece_color = $this->piece($pawn_square)->color();
  
    // Convert coord like "d4" into col=4 rank=4
    $piece_col = vchess_file2col($pawn_square->file()); // e.g. d -> 4
    $piece_rank = $pawn_square->rank();
  
    $dest_col = vchess_file2col($to_square->file());  // e.g. e -> 5
    $dest_rank = $to_square->rank();
  
    if ($piece_color == 'w') {
      if ($dest_rank == $piece_rank + 1
          && ($piece_col == ($dest_col - 1) || $piece_col == ($dest_col + 1))) {
        $attacks = TRUE;
      }
    }
    elseif ($piece_color == 'b') {
      if ($dest_rank == $piece_rank - 1
          && ($piece_col == ($dest_col - 1) || $piece_col == ($dest_col + 1))) {
        $attacks = TRUE;
      }
    }
  
    return $attacks;
  }
  
  /**
   * Check whether a given piece may legally move to the given square
   * 
   * @param $from_square
   *   Square where piece is trying to move from
   * @param $to_square
   *   Square where piece is trying to move to
   * 
   */
  function move_is_ok(Square $from_square, Square $to_square) {
    $move_ok = FALSE;
    
    if ($this->piece($from_square)->type() == 'P') {
      $move_ok = $this->_pawn_may_move_to_square($from_square, $to_square);
    }
    else {
      $move_ok = $this->_non_pawn_may_move_to_square($from_square, $to_square);
    }
    
    return $move_ok;
  }
  
  /**
   * Check whether the move is a pawn moving 2 squares
   * 
   * @param Square $from_square
   * @param Square $to_square
   * 
   * @return boolean TRUE if this was a pawn that moved 2 squares
   */
  function pawn_moved_2_squares(Square $from_square, Square $to_square) {
    $pawn_moved_2_squares = FALSE;
    
    $piece = $this->piece($from_square);
    if ($piece->type() == 'P') {
      if ($piece->color() == 'w' 
      && ($from_square->rank() == 1 && $to_square->rank() == 3)) {
        $pawn_moved_2_squares = TRUE;
      }
      elseif ($piece->color() == 'b' 
      && ($from_square->rank() == 7 && $to_square->rank() == 5)) {
        $pawn_moved_2_squares = TRUE;
      }
    }
    
    return $pawn_moved_2_squares;
  }
  
  /**
   * Check whether there is no further move possible.  To do this, we look at each of
   * the opponent pieces and see if any of them have a move which they can make.
   *
   * @param $player The player whose turn it is
   */
  function is_stalemate($player) {
    if ($player == 'w') {
      $opponent = 'b';
    }
    else {
      $opponent = 'w';
    }
  
    // Look at each square to find each of the opponent pieces
    $pieces_squares = $this->pieces_squares($player);
    foreach ($pieces_squares as $piece_square) {
      // Can the piece move theoretically thus is there
      // at least one square free for one piece?
      $valid_moves = $this->valid_moves($piece_square);
      if (count($valid_moves) > 0) {
        return FALSE;
      }
    }
  
    return TRUE;
  }
  
  /**
   * Get the possible valid moves for a particular piece
   *
   * @param $piece_square
   *   The square where the piece is
   */
  function valid_moves(Square $piece_square) {
    $valid_moves = array();
  
    $piece_type = $this->piece($piece_square)->type();
    switch ($piece_type) {
      case 'K':
        $adj_squares = $this->get_adjacent_squares($piece_square);
        foreach ($adj_squares as $to_square) {
          if ($this->move_is_ok($piece_square, $to_square)) {
            $valid_moves[] = $this->long_move($piece_square, $to_square);
          }
        }
        break;
      case 'Q':
        $squares = array_merge(
        $this->rank_and_file_squares($piece_square),
        $this->diagonal_squares($piece_square));
        foreach ($squares as $to_square) {
          if ($this->move_is_ok($piece_square, $to_square)) {
            $valid_moves[] = $this->long_move($piece_square, $to_square);
          }
        }
        break;
      case 'R':
        $squares = $this->rank_and_file_squares($piece_square);
        foreach ($squares as $to_square) {
          if ($this->move_is_ok($piece_square, $to_square)) {
            $valid_moves[] = $this->long_move($piece_square, $to_square);
          }
        }
        break;
      case 'B':
        $squares = $this->diagonal_squares($piece_square);
        foreach ($squares as $to_square) {
          if ($this->move_is_ok($piece_square, $to_square)) {
            $valid_moves[] = $this->long_move($piece_square, $to_square);
          }
        }
      case 'N':
        $squares = $this->diagonal_squares($piece_square);
        foreach ($squares as $to_square) {
          if ($this->move_is_ok($piece_square, $to_square)) {
            $valid_moves[] = $this->long_move($piece_square, $to_square);
          }
        }
        break;
      case 'P':
        // See if the move 1 square in front is possible
        if ($this->move_is_ok($piece_square, $square_in_front = $this->square_in_front($piece_square))) {
          $valid_moves[] = $this->long_move($piece_square, $square_in_front);
        }
        // See if the move 2 squares in front is possible
        if ($this->move_is_ok($piece_square, $square_2_in_front = $this->square_2_in_front($piece_square))) {
          $valid_moves[] = $this->long_move($piece_square, $square_2_in_front);
        }
        // See if an en passant capture is possible
        if ($this->is_en_passant()) {
          if ($this->move_is_ok($piece_square, $en_passant_square = vchess_coord2square($this->en_passant()))) {
            $valid_moves[] = $this->long_move($piece_square, $en_passant_square);
          }
        }
        break;
    }
  
    return $valid_moves;
  }
  
  /**
   * Calculate the long move
   */
  function long_move(Square $from_square, Square $to_square) {
    $long_move = $piece_type = $this->piece($from_square)->type .
      $from_square->coord() . "-" . $to_square->coord();
    
    return $long_move;
  }
  
  
  /**
   * Check whether pawn at $from_square may move to $to_square.
   * First move may be two squares instead of just one.
   *
   * @return
   *   TRUE if the pawn may move to the given square
   *   FALSE if the destination square is occupied or if a square
   *     on the way is occupied for the first 2-square move
   */
  function _pawn_may_move_to_square(Square $from_square, Square $to_square) {
    $move_ok = FALSE;
  
    if ($this->square_is_empty($to_square)) {
      $piece = $this->piece($from_square);
      $piece_file = $from_square->file(); // e.g. e
      $piece_rank = $from_square->rank(); // e.g. 2
      $dest_file = $to_square->file();  // e.g. e
      $dest_rank = $to_square->rank();  // e.g. 4
  
      // Check pawn stays on same file.
      // Captures are checked in pawn_attacks()
      if ($piece_file <> $dest_file) {
        $move_ok = FALSE;
      }
      elseif ($piece->color() == 'w') {
        // white pawn
        if ($piece_rank == 2 && $dest_rank == 4) {
          // Pawn moving 2 squares, so check if intermediate square is empty
          $intermediate_coord = new Square;
          $intermediate_coord->set_coord($piece_file . "3");
          if ($this->square_is_empty($intermediate_coord)) {
            $move_ok = TRUE;
          }
        }
        elseif ($dest_rank == ($piece_rank + 1)) {
          $move_ok = TRUE;
        }
      }
      else {
        // black pawn
        if ($piece_rank == 7 && $dest_rank == 5) {
          // Pawn moving 2 squares, so check if intermediate square is empty
          $intermediate_coord = new Square;
          $intermediate_coord->set_coord($piece_file . "6");
          if ($this->square_is_empty($intermediate_coord)) {
            $move_ok = TRUE;
          }
        }
        elseif ($dest_rank == ($piece_rank - 1)) {
          $move_ok = TRUE;
        }
      }
    }
  
    return $move_ok;
  }
  
  /**
   * Check whether piece at $from_square may move to $to_square.
   *
   * @return
   *   TRUE if the knight may move to the given square
   *   FALSE if the knight may not legally move to the given square
   */
  function _non_pawn_may_move_to_square(Square $from_square, Square $to_square) {
    $move_ok = FALSE;
  
    $color = $this->piece($from_square)->color();
    if ($this->square_is_empty($to_square) && $this->square_is_reachable($from_square, $to_square)) {
      // The only thing which would stop it would be if moving the piece
      // would expose the player to a discovered check.  To test this,
      // make the move and see if they are in check.
      $new_board = new Board();
      $new_board->setup_position($this->position());
      $new_board->move_piece($from_square, $to_square);           
      if (!$new_board->is_check($color)) {
        $move_ok = TRUE;        
      }
    }
  
    return $move_ok;
  }
  
  
  
  /**
   * Convert $row (1..8), $col (1..8) to 1dim index [0..63]
   */
  function _xy2i($row, $col) {
    return ($row * 8) + $col;
  }
  
  /**
   * Set the en_passant square
   * 
   * @param $square_in_front
   *   The square which is just in front of the en_passant.  For
   *   example, if white has moved Pe2-e4, then the square passed will 
   *   be the e4 square and the en_passant will be e3
   */
  function set_en_passant(Square $square_in_front) {
    $file = $square_in_front->file();
    if ($square_in_front->rank() == 4) {
      // White has moved something like "Ph2-h4"
      // so target will be "h3"
      $this->en_passant = $file . "3";
    }
    else {
      // Black has moved something like "Ph7-h5"
      // so target will be "h6"
      $this->en_passant = $file . "6";
    }
  }
  
  /**
   * Set en_passant value
   * 
   * @param $value
   *   Either a coord, e.g. "d3" or "-" 
   */
  function set_en_passant_value($value) {
    $this->en_passant = $value;
  }
  
  /**
   * Reset the en_passant square
   */
  function reset_en_passant() {
    $this->en_passant = "-";
  }
  
  /**
   * Test if there is an en_passant square
   * 
   * @return bool
   *   TRUE if an en_passant square currently exists
   */
  function is_en_passant() {
    $is_en_passant = FALSE;
    if ($this->en_passant() <> "-") {
      $is_en_passant = TRUE;
    }
  
    return $is_en_passant;
  }
  
  /**
   * Get the en_passant
   *
   * The en_passant is the coordinates of the square
   * behind the pawn in the last move to have moved 2 squares.
   *
   * @return
   *   Returns the en_passant coord (e.g. "d3"), if there is one,
   *   otherwise it returns "-"
   */
  function en_passant() {
    return $this->en_passant;
  }
}
  
/**
 * Convert $row (1..8), $col (1..8) to coordinate [a1..h8]
 */
function vchess_col_row2coord($col, $row) {
  $coord = "";
  switch ($col) {
    case 1:
      $coord = 'a';
      break;
    case 2:
      $coord = 'b';
      break;
    case 3:
      $coord = 'c';
      break;
    case 4:
      $coord = 'd';
      break;
    case 5:
      $coord = 'e';
      break;
    case 6:
      $coord = 'f';
      break;
    case 7:
      $coord = 'g';
      break;
    case 8:
      $coord = 'h';
      break;  
  }   
  
  $coord .= $row;
  
  return $coord;
}

 /**
 * Convert index [0..63] to square [a1..h8]
 */
function vchess_i2square($index) {
  $square = new Square();
  
  if ($index < 0 || $index > 63) {
    $square->set_coord('');
  }
  else {
    $y = floor($index / 8) + 1;
    $x = chr(($index % 8) + 97);
    $square->set_coord($x . $y);
  }
  
  return $square;
}

/**
 * Convert a coord [a1..h8] into a square
 */
function vchess_coord2square($coord) {
  $square = new Square;
  
  $square->set_coord($coord);
  
  return $square;
}

/**
 * Convert a file (a..h) into a numerical column (1..8)
 */
function vchess_file2col($file) {
  // "a" = ascii 97
  $col = ord($file) - 96;
  
  return $col;
}

/**
 * Convert column (1..8) as number to a file (a..h) 
 */
function vchess_col2file($col) {
  // "a" = ascii 97
  $file = chr($col + 96);

  return $file;
}

/**
 * Get empty squares between start and end as 1dim array.
 * Whether the path is clear is not checked.
 * 
 * @param $start_square Start square
 * @param $end_square End square
 * @param $change Change in index value on each move, e.g. -8 for vertical down the board
 * 
 * @return 
 *   Returns an array of squares which are between the $start_square and the $end_square,
 *   not including the $start_square and $end_square themselves.
 * 
 */
function vchess_get_inbetween_squares($piece_type, Square $start_square, Square $end_square) {
  $change = vchess_get_inbetween_squares_change($piece_type, $start_square, $end_square);
  
  $start = $start_square->index();
  $end = $end_square->index();
  
  $inbetween_squares = array();
  $i = 0;
  for ($pos = $start + $change; $pos != $end; $pos += $change) {
    $inbetween_squares[$i++] = vchess_i2square($pos);
  }
  return $inbetween_squares;
}


/**
 * Get the change value that must be added to create
 * the 1dim path for piece moving from piece_pos to
 * dest_pos. It is assumed that the move is valid!
 * No additional checks as in tileIsReachable are
 * performed. Rook, queen and bishop are the only
 * units that can have empty tiles in between.
 *
 * @param $piece_type Type of piece, "K", "Q", "R", "N", "B" or "P"
 * @param $from_square Piece square 
 * @param $dest_index Destination square
 */
function vchess_get_inbetween_squares_change($piece_type, Square $from_square, Square $dest_square) {
  
  $piece_index = $from_square->index();
  $dest_index = $dest_square->index();
  
  $change = 0;
  $piece_y = floor($piece_index / 8);
  $piece_x = $piece_index % 8;
  $dest_y = floor($dest_index / 8);
  $dest_x = $dest_index % 8;
  switch ($piece_type) {
    // bishop
    case 'B':
      if ($dest_y < $piece_y) {
        $change = -8;
      }
      else {
        $change = 8;
      }
      if ($dest_x < $piece_x) {
        $change -= 1;
      }
      else {
        $change += 1;
      }
      break;
      // rook
    case 'R':
      if ($piece_x == $dest_x) {
        if ($dest_y < $piece_y) {
          $change = -8;
        }
        else {
          $change = 8;
        }
      }
      else {
        if ($dest_x < $piece_x) {
          $change = -1;
        }
        else {
          $change = 1;
        }
      }
      break;
      // queen
    case 'Q':
      if (abs($piece_x -$dest_x) == abs($piece_y -$dest_y)) {
        if ($dest_y < $piece_y) {
          $change = -8;
        }
        else {
          $change = 8;
        }
        if ($dest_x < $piece_x) {
          $change -= 1;
        }
        else {
          $change += 1;
        }
      }
      elseif ($piece_x == $dest_x) {
        if ($dest_y < $piece_y) {
          $change = -8;
        }
        else {
          $change = 8;
        }
      }
      else {
        if ($dest_x < $piece_x) {
          $change = -1;
        }
        else {
          $change = 1;
        }
      }
      break;
  }
  return $change;
}

