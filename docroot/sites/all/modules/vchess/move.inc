<?php

/**
 * @file
 * Functions concerning a move
 */

class Move {
  protected $long_format; // e.g. "Pd2-d4", "Nb1xBc3", "Ke1-g1"
  protected $algebraic; // e.g. "e4"
  protected $timestamp; // the GMT timestamp of when the move was made
  
  /**
   * Initialise a move with a given string
   * 
   * A move is one of the following formats:
   * - "Bf5xPe4" i.e. a capture which includes the type of the piece being captured 
   * - "Rh4-d4" i.e. a move to a square
   */
  function initialise($long_format) {
    $this->long_format = $long_format;
  }
  
  /**
   * Get the long move notation
   */
  function long_format() {
    return $this->long_format;
  }
  
  /**
   * Get the destination square from a given move
   *
   * e.g. in a move like "Rh4-d4" the destination square is d4
   */
  function to_square() {
    $to_square = new Square();
    
    if ($this->long_format[3] == "x") {
      // In a move like "Bf5xPe4"
      //   $move[0] = source piece
      //   $move[1-2] = source coord
      //   $move[3] = move type, "x"
      //   $move[4] = dest piece
      //   $move[5-6] = dest square
      $to_square->set_coord(drupal_substr($this->long_format, 5, 2));
    }
    else { // Move type = "-"
      // In a move like "Rh4-d4":
      //   $move[0] = source piece
      //   $move[1-2] = source coord
      //   $move[3] = move type, "-"
      //   $move[4-5] = dest square
      $to_square->set_coord(drupal_substr($this->long_format, 4, 2));
    }
  
    return $to_square;
  }
  
  /**
   * Get the from square for a given move
   *
   * e.g.
   * In a move like "Bf5xPe4" return "f5"
   * In a move like "Rh4-d4" return "h4"
   */
  function from_square() {
    $from_square = new Square();
    $from_square->set_coord(drupal_substr($this->long_format, 1, 2));
  
    return $from_square;
  }
  
  /**
   * Get the move type
   * e.g. "-" for a move like "Ra1-a4"
   *   or "x" for a move like "Ra1xNa6"
   */
  function type() {
    return $this->long_format[3];
  }
  
  /**
   * Set the $long_format
   */
  function set_long_format($long_format) {
    $this->long_format = $long_format;
  }
  
  /**
   * Set the algebraic format
   */
  function set_algebraic($algebraic) {
    $this->algebraic = $algebraic;
  }
  
  /**
   * Get the source piece from a given move
   * 
   * e.g. "Ra1-a4" returns "R"
   * 
   * @return
   *   A piece type, one of: 'K', 'Q', 'R', 'B', 'N', 'P'
   */
  function source_piece_type() {
    return $this->long_format[0];
  }
  
  /**
   * Get the destination piece from a given move
   *
   * If there is no destination piece, return ""
   *
   * e.g.
   * "Qd1xBd7" returns "B"
   * "Ra1-a4" returns ""
   */
  function dest_piece_type() {
    if ($this->long_format[3] == "x") {
      $dest_piece_type = $this->long_format[4];
    }
    else {
      $dest_piece_type = "";
    }
  
    return $dest_piece_type;
  }
  
  /** 
   * Get the piece type for pawn promotion
   *  e.g. Ph7-h8=Q returns "Q"
   *  
   * @return 
   *   the piece type which is selected, one of: Q, R, B or N
   * 
   * If the move is not a valid promotion move then "" is returned.
   * 
   */
  function promotion_piece_type() {
    $piece_type = "";
    
    // Check that a pawn promotion is happening
    if ($this->from_square()->rank() == 7 && $this->to_square()->rank() == 8) {
      $white_promotion = TRUE;
    }
    else {
      $white_promotion = FALSE;
      
      if ($this->from_square()->rank() == 2 && $this->to_square()->rank() == 1) {
        $black_promotion = TRUE;
      }
      else {
        $black_promotion = FALSE;
      }
    }
    
    if (($this->source_piece_type() == "P")
    && ($white_promotion || $black_promotion)) {
      if ($this->type() == "-") {
        // e.g. In "Pb7-b8=Q" the "Q" is the 7th element
        $piece_type = drupal_substr($this->long_format, 7, 1);
      }
      else {
        // e.g. In "Pb7xRa8=Q" the "Q" is the 8th element
        $piece_type = drupal_substr($this->long_format, 8, 1);
      } 
    }
    
    return $piece_type;
  } 
  
  /**
   * Get short notation of move
   * e.g. Pe2-e4 -> e4
   *      Re1xNa6 -> Rxa6
   *
   * When two (or more) identical pieces can move to the same square,
   * the moving piece is uniquely identified by specifying the piece's letter,
   * followed by (in descending order of preference):
   * - the file of departure (if they differ); or
   * - the rank of departure (if the files are the same but the ranks differ);
   * - or both the rank and file (if neither alone is sufficient to identify the piece—which occurs only in rare cases where one or more pawns have promoted, resulting
   *   in a player having three or more identical pieces able to reach the same square).
   *
   * For example, with knights on g1 and d2, either of which might move to f3,
   * the move is specified as Ngf3 or Ndf3, as appropriate. With knights on g5 and g1,
   * the moves are N5f3 or N1f3. As above, an x can be inserted to indicate a capture,
   * for example: N5xf3.
   *
   * Occasionally, it may be possible to disambiguate in two different ways -
   * for example, two rooks on d3 and h5, either one of which may move to d5.
   * If the rook on d3 moves to d5, it is possible to disambiguate with either Rdd5
   * or R3d5. In cases such as these the file takes precedence over the rank,
   * so Rdd5 is correct.
   *
   * See http://en.wikipedia.org/wiki/Algebraic_notation_(chess)
   * 
   * @param $player 
   *   The color of the player making the move, either "b" or "w"
   * @param $board
   *   The board on which the move is being made
   */
  function calculate_algebraic($player, Board $board) {
    // If all else fails, just return the long move
    $this->algebraic = $this->long_format;

    $from_square = $this->from_square();
    $to_square = $this->to_square();
    
    $source_piece_type = $this->source_piece_type();
  
    if ($player == 'w') {
      $opponent = 'b';
    }
    else {
      $opponent = 'w';
    }
  
    // Castling short
    if ($this->long_format == "Ke1-g1" || $this->long_format == "Ke8-g8") {
      $this->algebraic = "O-O";
    }
    // Castling long
    elseif ($this->long_format == "Ke1-c1" || $this->long_format == "Ke8-c8") {
      $this->algebraic = "O-O-O";
    }
    // Pawn moves
    elseif ($source_piece_type == 'P') {
      // P moves are always unambiguous. For attacks skip source digit
      // and for moves skip source pos and "-"
      if ($this->type() == '-') {
        if ($from_square->file() == $to_square->file()) {
          // e.g. e4
          $this->algebraic = $to_square->coord();
        }
        else {
          // A pawn move to another file which is not a capture (e.g. Pa5-b6)
          // must be an en passant capture, e.g. "axb5"
          $this->algebraic = $from_square->file() . "x" . $to_square->coord();
        }
      }
      else {
        // e.g. cxd4
        $this->algebraic = $this->long_format[1] . "x" . $to_square->coord();
      }
      
      // Check if pawn promotion, e.g. e8=Q
      if ($this->to_square()->rank() == 1 || $this->to_square()->rank() == 8) {
        $this->algebraic .= "=" . $this->promotion_piece_type();
      }      
    }
    // All other moves
    else {
      // First find out where all possible pieces of this type are
      $pieces_squares = $board->piece_type_squares($source_piece_type, $player);
  
      // If there is only 1 piece of this type, then move is unambiguous
      if (count($pieces_squares) == 1) {
        if ($this->type() == '-') {
          // e.g. Ne4
          $this->algebraic = $source_piece_type . $to_square->coord();
        }
        else {
          // e.g. Nxd4
          $this->algebraic = $source_piece_type . "x" . $to_square->coord();
        }
      }
      else {
        // Find how many other pieces of this type may move to the dest square
        $trouble_squares = array();
        foreach ($pieces_squares as $piece_square) {
          // Only look at the other pieces
          if ($piece_square != $from_square) {
            if ($board->move_is_ok($piece_square, $to_square)) {
              $trouble_squares[] = $piece_square;
            }
          }
        }
  
        if (count($trouble_squares) == 0) {
          // No other piece of this type can reach the square, so unambiguous
          if ($this->type() == '-') {
            // e.g. Ne4
            $this->algebraic = $source_piece_type . $to_square->coord();
          }
          else {
            // e.g. Nxd4
            $this->algebraic = $source_piece_type . "x" . $to_square->coord();
          }
        }
        else {
          // First try to disambiguate by looking at the file, e.g. Ngf3
          $source_file = $from_square->file();
          $file_unique = TRUE;
          foreach ($trouble_squares as $trouble_coord) {
            if ($trouble_coord->file() == $source_file) {
              $file_unique = FALSE;
            }
          }
  
          // In this case the file is enough to make the move unique, e.g. Ngf3
          if ($file_unique) {
            if ($this->type() == '-') {
              $this->algebraic = $source_piece_type . $source_file . $to_square->coord();
            }
            else {
              // e.g. Nxd4
              $this->algebraic = $source_piece_type . $source_file . "x" . $to_square->coord();
            }
          }
          else {
            // Try to disambiguate by looking at the rank, e.g. N1f3
            $source_rank = $from_square->rank();
            $rank_unique = TRUE;
            foreach ($trouble_squares as $trouble_coord) {
              if ($trouble_coord->rank() == $source_rank) {
                $rank_unique = FALSE;
              }
            }
            
            // In this case the rank is enough to make the move unique, e.g. N1f3
            if ($rank_unique) {
              if ($this->type() == '-') {
                // e.g. N1f3
                $this->algebraic = $source_piece_type . $source_rank . $to_square->coord();
              }
              else {
                // e.g. N1xf3
                $this->algebraic = $source_piece_type . $source_rank . "x" . $to_square->coord();
              } 
            }
            else {
              // File is not unique, rank is not unique, so we need full source square, e.g. Ng1f3  
              // This can only ever happen when promotion to a third piece has occured.
              $prefix = $source_piece_type . $source_rank . $source_file;
              if ($this->type() == '-') {
                // e.g. Ng1f3
                $this->algebraic = $prefix . $to_square->coord();
              }
              else {
                // e.g. Ng1xf3
                $this->algebraic = $prefix . "x" . $to_square->coord();
              }
            }
          }
        }
      }
    }
    
    // Finally we need to see if the move results in check or checkmate.  We make the move on 
    // a copy of the board to not muck up the existing board 
    $clone_board = clone $board;
    
    $clone_board->move_piece($from_square, $to_square);
    if ($clone_board->is_check($opponent)) {
      if ($clone_board->is_checkmate($opponent)) {
        $this->algebraic .= "#";
        if ($player == 'w') {
          $this->algebraic .= " 1-0";
        }
        else {
          $this->algebraic .= " 0-1";
        }
      }
      else {
        $this->algebraic .= "+";
      }
    }
    elseif ($clone_board->is_stalemate($opponent)) {
      $this->algebraic .= " 1/2-1/2";
    }

    return $this->algebraic;
  }
  
  /**
   * Get the algebraic version of the move
   * 
   * @return
   *   The algebraic version of the move, e.g. "Nc3"
   */
  function algebraic() {
    return $this->algebraic;
  }
  
  /**
   * Get the timestamp of the move
   */
  function timestamp() {
    return $this->timestamp;
  }
  
  /**
   * Set the timestamp of the move
   * 
   * @param $timestamp
   *   The timestamp to be set
   */
  function set_timestamp($timestamp) {
    $this->timestamp = $timestamp;
  }
}
