<?php

/**
 * @file
 * Functions concerning a game
 */

// Define game statuses
define('STATUS_WHITE_WIN', '1-0');
define('STATUS_BLACK_WIN', '0-1');
define('STATUS_DRAW', '1/2-1/2');
define('STATUS_IN_PROGRESS', 'in progress');
define('STATUS_AWAITING_PLAYERS', 'awaiting players');

// Define time units
define('TIME_UNITS_DAYS', 'days');
define('TIME_UNITS_HOURS', 'hours');
define('TIME_UNITS_MINS', 'mins');
define('TIME_UNITS_SECS', 'secs');

// Define errors
define('ERROR_CASTLING_SQUARES_BLOCKED', 'ERROR: Castling squares are blocked!');
define('ERROR_CANNOT_ESCAPE_CHECK_BY_CASTLING', 'ERROR: You cannot escape check by castling!');
define('ERROR_CANNOT_CASTLE_ACROSS_CHECK', 'ERROR: You cannot castle across check!');
define('ERROR_CANNOT_CASTLE_SHORT', 'ERROR: You cannot castle short anymore!');
define('ERROR_CANNOT_CASTLE_LONG', 'ERROR: You cannot castle long anymore!');

class Game {
  
  protected $game = array();
  
  protected $scoresheet;
  
  protected $white_uid;
  protected $black_uid;
  
  protected $time_per_move;  // e.g. 3
  protected $time_units;     // e.g. TIME_UNITS_DAYS
  
  // chatter: list of chatter lines (first is newest)
  
  // Dynamic (based on user id) entries:
  // p_maymove: whether it's player's turn (always 0 if user is not playing)
  // p_mayundo: player may undo last move
  // p_mayabort: player may abort game (first move or opponent took too long)
  // p_mayarchive: player may move game to archive
  
  /**
   * Game constructor
   */
  function __construct() {
    $this->scoresheet = new Scoresheet();

    // Setup the board
    $this->board = new Board;
    $this->board->setup_as_standard();
//    $this->board->setup_position("4k3/8/8/2p5/8/8/2P5/4K5");

    // Ensure the game has players defined by default
    $this->white_uid = 0;
    $this->black_uid = 0;
    
    // turn: color of whose turn it is to move (w or b)
    $this->turn = "w";
    
    $this->status = STATUS_AWAITING_PLAYERS;
    
    $this->reset_en_passant();

    // Castling record.  For example, when white king has moved, white may no
    // longer castle.  When rook has moved, player may only castle on other side.
    $this->white_may_castle_short = TRUE;
    $this->white_may_castle_long = TRUE;
    $this->black_may_castle_short = TRUE;
    $this->black_may_castle_long = TRUE;
    
    $this->time_per_move = DEFAULT_TIME_PER_MOVE;
    $this->time_units = DEFAULT_TIME_UNITS;
  }
  
  /**
   * Calculate time left in seconds till next move must be made
   * 
   * @return
   *   Number of seconds till next move must be made
   */
  function time_left() {
    // Convert time_per_move into seconds
    switch ($this->time_units) {
      case TIME_UNITS_DAYS:
        $secs_per_unit = 24 * 60 * 60;
        break;
      case TIME_UNITS_HOURS:
        $secs_per_unit = 60 * 60;
        break;
      case TIME_UNITS_MINS:
        $secs_per_unit = 60;
        break;
      case TIME_UNITS_SECS:
        $secs_per_unit = 1;
        break;
    }
    
    $secs_per_move = $this->time_per_move * $secs_per_unit;

    if ($this->status == STATUS_IN_PROGRESS) {
      $current_time = gmmktime();  // All dates are kept as GMT
//        $other_time = gmdate("Y-m-d H:i:s");
//        $just_time = time();
//        drupal_set_message("current time:" . date("Y-m-d H:i:s", $current_time));
//        drupal_set_message("other time:" . $other_time);
//        drupal_set_message("time: " . date("Y-m-d H:i:s", $just_time));
      if ($this->is_move_made()) {
        $move_time = strtotime($this->last_move()->timestamp());
      }
      else {
        $move_time = strtotime($this->time_started());
      }
      $elapsed = $current_time - $move_time;
      $time_left = $secs_per_move - $elapsed;
    }
    else {
      $time_left = $secs_per_move;
    }
    
    return $time_left;
  }
  
  /**
   * Get the time_per_move
   */
  function time_per_move() {
    return $this->time_per_move;
  }
  
  /**
   * Get the time units
   */
  function time_units() {
    return $this->time_units;
  }
  
  
  /**
   * Get the timestamp of when the game started
   * 
   * @return
   *   A timestamp, e.g. "2012-05-03 12:01:29"
   */
  function time_started() {
    if ($this->status == STATUS_IN_PROGRESS) {
      $time_started = $this->time_started;
    }
    else {
      $time_started = FALSE;
    }
    
    return $time_started;
  }
  
  /**
   * Get the game speed, which is the combination of the time_per_move
   * and the time_units, e.g. "3 days"
   *
   * @return
   *   Returns the speed per move, e.g. "3 days"
   */
  function speed() {
    return $this->time_per_move . " " . $this->time_units;
  }
  
  /**
   * Set the time per move
   * 
   * This just sets the value of the time per move (e.g. 1 or 3).  The units of time 
   * would be set in set_time_units(), which isn't currently needed so does not exist. 
   * 
   * @parm $time_per_move
   *   Time per move, e.g. "3".  
   */
  function set_time_per_move($time_per_move) {
    $this->time_per_move = $time_per_move;

    $this->save();
  }
  
  /**
   * Check if the game has been lost on time
   * 
   * This checks if the time since the last move was made is 
   * now more than the time allowed for the game 
   * 
   * @return
   *   TRUE if the game has been lost on time
   */
  function is_lost_on_time() {
    if ($this->time_left() <= 0) {
      $lost_on_time = TRUE;

      if ($this->status() == STATUS_IN_PROGRESS) {
        $this->_handle_lost_on_time();
      }
    }
    else {
      $lost_on_time = FALSE;
    }
    
    return $lost_on_time;
  }
  
  /**
   * Deal with the case that the player has lost on time
   */
  function _handle_lost_on_time() {
    if ($this->turn() == 'w') {
      $this->set_status(STATUS_BLACK_WIN);
    }
    else {
      $this->set_status(STATUS_WHITE_WIN);
    }
    
    $this->save();
  }
  
  /**
   * Set the players for a new game.
   * 
   * It is at this stage that the game really begins playing.
   * 
   * @param $white_uid
   *   Userid of white player
   * @param $black_uid
   *   Userid of black player
   *
   * @return
   *   Game id, $gid.   
   */
  function set_players($white_uid, $black_uid) {
    // Build new game context 
    $this->white_uid = $white_uid;
    $this->black_uid = $black_uid;

    $this->status = STATUS_IN_PROGRESS;
    
    $this->save();
  }
  
  /**
   * Set a single player
   * 
   * Note that it is an error to set a player when there are already 2 players
   * assigned to the game
   */
  function set_player($uid) {
//     watchdog("VChess", "In game.inc for game %gid, at start of set_player() setting player uid=%uid." .  
//         " Currently white_uid=%white_uid and black_uid=%black_uid", 
//         array('%gid' => $this->gid(), 
//             '%uid' => $uid, 
//             '%white_uid' => $this->white_uid, 
//             '%black_uid' => $this->black_uid));
    
    if ($this->white_uid == 0 && $this->black_uid == 0) {
      if (rand(1,100) < 50) {
        $this->white_uid = $uid;
      }
      else {
        $this->black_uid = $uid;
      }
    }
    else {
      if ($this->white_uid == 0) {
        $this->white_uid = $uid;
        $this->status = STATUS_IN_PROGRESS;
      }
      elseif ($this->black_uid == 0) {
        $this->black_uid = $uid;
        $this->status = STATUS_IN_PROGRESS;
      }
      else {
        watchdog("VChess", "Attempt to set a player when both players are already assigned", 
            array(), WATCHDOG_ERROR);
      }
    }
    
//     watchdog("VChess", "in game.inc for game %gid, at end of set_player() setting " . 
//         " player uid=%uid.  Now white_uid=%white_uid and black_uid=%black_uid", 
//         array('%gid' => $this->gid(), 
//             '%uid' => $uid, 
//             '%white_uid' => $this->white_uid, 
//             '%black_uid' => $this->black_uid));
    
    $this->save();
  }
   
  /**
   * Get the player who is the current challenger
   */
  function challenger() {
    $uid = 0;

    if ($this->white_uid != 0 && $this->black_uid == 0) {
      $uid = $this->white_uid;
    }
    elseif ($this->black_uid != 0 && $this->white_uid == 0) {
      $uid = $this->black_uid;
    }
  
    $challenger = new Player($uid);
    
    return $challenger;
  }
  
  /**
   * Load an existing game (try active games first, then archived games) and set various
   * user-based variables, too. Return game as array or NULL on error.
   *
   * @param $gid: Game id
   *
   */
  function load($gid) {
    $sql = "SELECT gid, turn, white_uid, black_uid, status, board, " . 
      "castling, en_passant, time_per_move, time_units, time_started " .
      " FROM {vchess_games} WHERE gid = '" .
      $gid . "'";
    // Converted to the D7 database API syntax.
    $result = db_query($sql);
    $row = $result->fetchAssoc();

    $this->gid = $row['gid'];
    $this->turn = $row['turn'];
  
    // Fill chess board
    $this->board->setup_position($row['board']);
    
    $this->white_uid = $row['white_uid'];
    $this->black_uid = $row['black_uid'];
    $this->status = $row['status'];
    $this->time_per_move = $row['time_per_move'];
    $this->time_units = $row['time_units'];
    $this->time_started = $row['time_started'];
  
    $this->board->set_en_passant_value($row['en_passant']);
  
    // Handle castling availability
    $this->initialise_castling($row['castling']);
    
    $this->scoresheet->load($gid); 
  }
  
  /**
   * Get the last move
   */
  function last_move() {
    return $this->scoresheet->last_move();
  }
  
 
  /**
   * This sets up the castling state
   * 
   * If neither side can castle, this is "-". 
   * Otherwise, this has one or more letters: 
   * - "K" (White can castle kingside), 
   * - "Q" (White can castle queenside), 
   * - "k" (Black can castle kingside), and/or 
   * - "q" (Black can castle queenside)
   * 
   * @see http://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation 
   */
  function initialise_castling($castling) {
    if (strpos($castling, "K") !== FALSE) {
      $this->white_may_castle_short = TRUE;
    }
    else {
      $this->white_may_castle_short = FALSE;
    }
    if (strpos($castling, "Q") !== FALSE) {
      $this->white_may_castle_long = TRUE;
    }
    else {
      $this->white_may_castle_long = FALSE;
    }
    if (strpos($castling, "k") !== FALSE) {
      $this->black_may_castle_short = TRUE;
    }
    else {
      $this->black_may_castle_short = FALSE;
    }
    if (strpos($castling, "q") !== FALSE) {
      $this->black_may_castle_long = TRUE;
    }
    else {
      $this->black_may_castle_long = FALSE;
    }
  }
  
  /** 
   * Create $castling string.
   * 
   * If neither side can castle, this is "-". 
   * Otherwise, this has one or more letters: 
   * - "K" (White can castle kingside), 
   * - "Q" (White can castle queenside), 
   * - "k" (Black can castle kingside), and/or 
   * - "q" (Black can castle queenside)
   * e.g. "KQkq"
   * 
   * @see http://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation 
   */
  function castling() {
    $castling = "";
    
    if ($this->white_may_castle_short) {
      $castling .= "K";
    }
    if ($this->white_may_castle_long) {
      $castling .= "Q";
    }
    if ($this->black_may_castle_short) {
      $castling .= "k";
    }
    if ($this->black_may_castle_long) {
      $castling .= "q";
    }
    if ($castling == "") {
      $castling = "-";
    }
    
    return $castling;
  }
  
  /**
   * Get the white player
   */
  function white_player() {
    return new Player($this->white_uid);
  }
  
  /**
   * Get the the black player
   */
  function black_player() {
    return new Player($this->black_uid);
  }
  
  /**
   * Get the number of the current move.  
   * 
   * The move number will be the number of the move which is currently not yet complete.  
   * Each move has a white move and a black move.
   * 
   * i.e.
   * No moves, i.e.
   * 1. ... ...
   * move_no = 1 (i.e. waiting for move 1 of white)
   * After 1.e4 ... 
   * move_no = 1 (i.e. waiting for move 1 of black)
   * After 1. e4 Nf6 
   * move_no = 2 (i.e. waiting for move 2) 
   */
  function move_no() {
    return $this->scoresheet->move_no();
  }
  
  /**
   * See if it's the given players move
   */
  function is_players_move($uid) {
    if (($this->turn == 'w' && $this->white_uid == $uid) 
    || ($this->turn == 'b' && $this->black_uid == $uid)) {
      $players_move = TRUE;
    }
    else {
      $players_move = FALSE;
    }
    
    return $players_move;
  }
  
  /**
   * See if the game is started yet
   * 
   * @return
   *   TRUE if a move has already been made
   */
  function is_move_made() {
    if ($this->move_no() == 1 && $this->turn() == "w") {
      $is_move_made = FALSE;
    }
    else {
      $is_move_made = TRUE;
    }
    
    return $is_move_made;
  }
  
  /**
   * See if the given user is one of the players
   * 
   * @return
   *   TRUE if the given user is one of the players
   */
  function is_user_playing($uid) {
    if ($this->black_uid == $uid || $this->white_uid == $uid) {
      $playing = TRUE;
    }
    else {
      $playing = FALSE;
    }
    
    return $playing;
  }
  
  /** 
   * Get the game id
   */
  function gid() {
    // This game may never have been saved
    if (!isset($this->gid)) {
      $this->save();
    }
    return $this->gid;
  }
  
  /**
   * Get the game board
   */
  function board() {
    return $this->board;
  }
  
  /**
   * Set the board
   */
  function set_board(Board $board) {
    $this->board = $board;
  }
  
  /**
   * Setup a postion
   * 
   * @param $fen_string
   *   A FEN string, e.g. after 1.e4 the FEN string will be:
   *   "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR"
   */
  function setup_position($fen_string) {
    $this->board->setup_position($fen_string);
  }
  
  /**
   * Get the current position as a FEN string
   * 
   * @return the current position as a FEN string
   *   e.g. after 1.e4 the FEN string will be:
   *   "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR"
   */
  function position() {
    return $this->board->position();
  }

  /**
   * Get the uid of the white player
   */
  function white_uid() {
    return $this->white_uid;
  }
  
  /**
   * Get the uid of the black player
   */
  function black_uid() {
    return $this->black_uid;
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
    return $this->board->en_passant();
  }
  
  /**
   * Get the player whose turn it is, either 'w' or 'b'
   * 
   * @return
   *   Whose turn it is, 'w' or 'b'
   */
  function turn() {
    return $this->turn;
  }
  
  /**
   * Get the status
   * 
   * Status can be one of:
   * - "awaiting players"
   * - "in progress"
   * - "1-0"
   * - "0-1"
   * - "1/2-1/2"
   */
  function status() {
    return $this->status;
  }
  
  /**
   * Set the player whose turn it is to move to be 'w'
   */
  function set_turn_white() {
    $this->turn = 'w';
  }
  
  /**
   * Set the player whose turn it is to move to be 'b'
   */
  function set_turn_black() {
    $this->turn = 'b';
  }
  
  /**
   * Say whether the game is over or not
   * 
   * @return TRUE if the game is over
   */
  function is_over() {
    if ($this->status() == STATUS_IN_PROGRESS) {
      $is_over = FALSE;
    }
    else {
      $is_over = TRUE;
    }
    
    return $is_over;
  }
  
  /**
   * Say whether the king is in checkmate
   * 
   * @param $player
   *   Player, either 'w' or 'b'
   */
  function is_checkmate($player) {
    return $this->board->is_checkmate($player);
  }
  
  /**
   * Say whether the king is in check
   * 
   * @param $player
   *   Player, either 'w' or 'b'
   */
  function is_check($player) {
    return $this->board->is_check($player);
  }
  
  /**
   * Find for a particular player who the opponent is.
   *
   * @param $uid
   *   User id of one of the players
   *
   * @return Player $player
   *   The opposing player
   */
  function opponent($uid) {
    if ($this->white_uid == $uid) {
      $opponent = $this->black_player();
    }
    else {
      $opponent = $this->white_player();
    }
  
    return $opponent;
  }
  
  /**
   * Resign a particular game
   */
  function resign($uid) {
    $winner = $this->opponent($uid);
  
    if ($this->player_color($uid) == 'w') {
      $this->set_status(STATUS_BLACK_WIN);
    }
    else {
      $this->set_status(STATUS_WHITE_WIN);
    }
  }
  
  /**
   * Find out what color a particular player is
   * 
   * In the case where a player is playing against themself (!), which we allow
   * at least for testing purposes, the color is the color of whoever's turn it 
   * is to move.
   * 
   * @param $uid
   *   The user id of a player
   *   
   * @return
   *   'w' if the player is playing white
   *   'b' if the player is playing black
   *   '' if the player is not a player of this game
   */
  function player_color($uid) {
    $color = "";
    
    if ($this->white_uid == $this->black_uid) {
      $color = $this->turn();
    }
    elseif ($this->white_uid == $uid) {
      $color = 'w';
    }
    elseif ($this->black_uid == $uid) {
      $color = 'b';
    }
  
    return $color;
  }
  
  /**
   * Set status
   */
  function set_status($status) {
    $this->status = $status;
  }
  
  /**
   * Set last move
   */
  function set_last_move($last_move) {
//  $this->game['last_move'] = $last_move;
  }
  
  function white_may_not_castle() {
    $this->white_may_castle_short = FALSE;
    $this->white_may_castle_long = FALSE;
  }
  
  function black_may_not_castle() {
    $this->black_may_castle_short = FALSE;
    $this->black_may_castle_long = FALSE;
  }
  
  function set_white_may_castle_short($may_castle_short) {
    $this->white_may_castle_short = $may_castle_short;  
  }
  
  function set_white_may_castle_long($may_castle_long) {
    $this->white_may_castle_long = $may_castle_long;
  }
  
  function set_black_may_castle_short($may_castle_short) {
    $this->black_may_castle_short = $may_castle_short;
  }
  
  function set_black_may_castle_long($may_castle_long) {
    $this->black_may_castle_long = $may_castle_long;
  }
  
  function black_may_castle_long() {
    return $this->black_may_castle_long;
  }
  
  function black_may_castle_short() {
    return $this->black_may_castle_short;
  }

  function white_may_castle_long() {
    return $this->white_may_castle_long;
  }
  
  function white_may_castle_short() {
    return $this->white_may_castle_short;
  }
  
  /**
   * Find out if the given player may castle short
   * 
   * @param unknown_type $player
   * @return TRUE if player may castle short
   */
  function may_castle_short($player) {
    if ($player == 'w') {
      $may_castle_short = $this->white_may_castle_short();
    }
    else {
      $may_castle_short = $this->black_may_castle_short();
    }
    
    return $may_castle_short;
  }
  
  function may_castle_long($player) {
    if ($player == 'w') {
      $may_castle_short = $this->white_may_castle_long();
    }
    else {
      $may_castle_short = $this->black_may_castle_long();
    }
  
    return $may_castle_short;
  }
  
  /**
   * Verify move, execute it and modify game.
   *
   * @param $uid 
   *   User id of current player
   *   
   * @param $move_string 
   *   String of current move in long format, e.g. "Pe2-e4"
   *   
   * @param $comment
   *   @todo Please document this parameter
   */
  function make_move($uid, $move_string) {
    $mate_type = "";
  
    $move = new Move();
    $move->initialise($move_string);
    
    $board = $this->board();
    
    // We will use this board at the end to get the algebraic move
    $board_clone = clone $board;
  
    $result = "";
    $pawn_promoted = FALSE;
    $en_passant_set = FALSE;
    $move_ok = TRUE;
    
    $piece_square = $move->from_square();
    $to_square = $move->to_square();
    
//    $result .= "move_string = $move_string. ";
  
    if (!$this->is_players_move($uid)) {
      $result .= 'It is not your turn!';
    }
    else {
      if ($this->turn() == 'w') {
        $opponent = 'b';
      }
      else {
        $opponent = 'w';
      }

      // HANDLE MOVES:
      if ($move->type() == 'draw?' && $this->status() == '?') {
        // Offer draw
        $this->set_status('D');
        $result .= 'You have offered a draw.';
        $draw_handled = 1;
        $game['lastmove'] = 'DrawOffered';
      }
      elseif ($move->type() == 'refuse_draw' && $this->status() == 'D') {
        // Refuse draw
        $this->set_status('?');
        $draw_handled = 1;
        $result .= 'You refused the draw.';
        $game['lastmove'] = 'DrawRefused';
      }
      elseif ($move->type() == 'accept_draw' && $this->status() == 'D') {
        // Accept draw
        $this->set_status('-');
        $draw_handled = 1;
        $result .= 'You accepted the draw.';
        $this->set_last_move('DrawAccepted');
        if ($game['curplyr'] == 'b') {
          $game['curmove']++; // new move as white offered
        }
        $game['mhistory'][count($game['mhistory'])] = 'draw';
      }
      elseif ($move->long_format() == 'Ke1-g1' 
      || $move->long_format() == 'Ke8-g8' 
      || $move->long_format() == 'Ke1-c1' 
      || $move->long_format() == 'Ke8-c8') {
        switch ($move->long_format()) {
          case 'Ke1-g1':
            $error = $this->castle('w', 'e1', 'g1', 'h1', 'f1', array('f1', 'g1'), $board);
            break;
          case 'Ke1-c1':  
            $error = $this->castle('w', 'e1', 'c1', 'a1', 'd1', array('b1', 'c1', 'd1'), $board);
            break;
          case 'Ke8-g8':
            $error = $this->castle('b', 'e8', 'g8', 'h8', 'f8', array('f8', 'g8'), $board);
            break;
          case 'Ke8-c8':
            $error = $this->castle('b', 'e8', 'c8', 'a8', 'd8', array('b8', 'c8', 'd8'), $board);
            break;
          default:
            break;  
        }
        if ($error <> "") {
          return $error;
        }
      }
      elseif ($move->type() == "-") {
        // Validate piece and position.
        // Move is e.g. "Nb1-c3"
        $piece = new Piece();
        $piece->set_type($move->source_piece_type());
        $piece->set_color($this->turn());

        if ($piece->type() == 'P' && $to_square->coord() == $this->en_passant()) {
          // Perform en passant pawn capture
          $board->en_passant_capture($piece_square, $to_square);
        }
        elseif (!$board->move_is_ok($piece_square, $to_square)) {
          $move_ok = FALSE;
        }
        else {
          // If pawn moved 2 squares, then record the en_passant square
          // (the square behind the pawn which has just moved)
          if ($board->pawn_moved_2_squares($piece_square, $to_square)) {
            $this->set_en_passant($to_square);

            $en_passant_set = TRUE;
          } 
          
          // If pawn reached last rank, promote it
          $pawn_promoted = $this->handle_pawn_promotion($move, $board);
        
          if (!$pawn_promoted) {
            // Perform normal move
            $board->move_piece($piece_square, $to_square);
          }
        }
      }
      elseif ($move->type() == "x") {
        if ($board->square_is_empty($to_square)) {
          // En passant of pawn?
          if ($piece_type == 'P') {

          }
          else {
            $result .= 'ERROR: ' . $to_square->coord() . ' is empty!';
            $move_ok = FALSE;
          }
        }
        elseif ($board->piece($to_square)->color() == $this->turn()) {
          $result .= 'ERROR: You cannot attack own chessman at ' . $to_square->coord() . '!';
          $move_ok = FALSE;
        }
        elseif (!$board->piece_attacks($piece_square, $to_square)) {
          $result .= 'ERROR: You cannot take the piece on ' . $to_square->coord() . '!';
          $move_ok = FALSE;
        }
        
        // If pawn reached last rank, promote it
        $pawn_promoted = $this->handle_pawn_promotion($move, $board);
        
        if ($move_ok && !$pawn_promoted) {
          // Perform normal capture
          $board->move_piece($piece_square, $to_square);
        }
      }
    
      // If OWN king is still in check, then invalid move
      if ($board->is_check($this->turn())) {
        $result .= 'ERROR: King is in check. ';
        $move_ok = FALSE;        
      }

      $move->calculate_algebraic($this->turn(), $board_clone);
      
      // If move was executed update game state.
      if ($move_ok) {
        $result .= 'Your last move: ' . $move->algebraic();
        
        // If this wasn't a 2-square pawn move, we need to reset 
        // the en_passant square
        if (!$en_passant_set) {
          $this->reset_en_passant();
        }
        
        // Check checkmate/stalemate
        if ($board->is_check($opponent)) {
          // If this is check mate finish the game otherwise
          // add '+' to the move.
          if ($board->is_checkmate($opponent)) {
            if ($this->turn() == 'w') {
              $this->set_status(STATUS_WHITE_WIN);
            }
            else {
              $this->set_status(STATUS_BLACK_WIN);
            }
            $result .= '... CHECKMATE!';
          }
        }
        elseif ($board->is_stalemate($opponent)) {
          $this->set_status(STATUS_DRAW);
          $result .= '... STALEMATE!';
        }
        
        // Update whose turn it is.  Even if mate has occured, it
        // is still logically the opponents move, even if they have
        // no valid move that they can make  
        if ($this->turn() == 'b') {
          $this->set_turn_white();
        }
        else {
          $this->set_turn_black();
        }
      
        // Add comment to head of chatter. Right now we have only two
        // chatter items. Strip backslashes and replace newlines to get
        // a single line.
        if (empty($comment)) {
          $comment = '(silence)';
        }
        else {
          $comment = str_replace("\\", '', strip_tags($comment));
          $comment = str_replace("\n", '<br />', $comment);
        }
        $comment = '<u>' . $uid . '</u>: ' . $comment;
        //    $game['chatter'][1] = $game['chatter'][0];
        //    $game['chatter'][1] = "Hugh hard coding for now";
        //    $game['chatter'][0] = $comment;
      
        // Store changed board
        $this->set_board($board);
        
        $this->scoresheet->write_move_down($move);
      }
      else {
        $result .= 'ERROR: ' . $move->algebraic() . ' is not a legal move!  ';
      }
    }
    return $result;
  }
  
  /**
   * Set the en_passant square
   */
  function set_en_passant(Square $square_in_front) {
    $this->board()->set_en_passant($square_in_front);
  }
  
  /**
   * Reset the en_passant square
   */
  function reset_en_passant() {
    $this->board->reset_en_passant();
  }
  
  /**
   * Test if there is an en_passant square
   */
  function is_en_passant() {
    return $this->board->is_en_passant();
  }
  
  /**
   * Handle pawn promotion
   */
  function handle_pawn_promotion(Move $move, Board $board) {
    $pawn_promoted = FALSE;
    
    $piece = $board->piece($move->from_square());
    if ($piece->type() == 'P') {
      if (($this->turn() == 'w' && $move->to_square()->rank() == 8) ||
          ($this->turn() == 'b' && $move->to_square()->rank() == 1)) {
        $promote_to = new Piece();
        $promote_to->set_type($move->promotion_piece_type());
        $promote_to->set_color($this->turn());
    
        $board->promotion($move->from_square(), $move->to_square(), $promote_to);
        
        $pawn_promoted = TRUE;
      }
    }
    
    return $pawn_promoted;
  }
  
  /**
   * Handle castling
   * 
   * @param $turn either 'w' or 'b'
   * @param $king_coord the coord of the king, e.g. "e1"
   * @param $castling_coords array of the coords of all the squares involved, from
   * left to right, e.g. array("e1", "f1", "g1", "h1"
   *       
   */
  function castle($turn, $king_from, $king_to, $rook_from, $rook_to, $gap_coords, $board) {
    $error = "";  
    
    if ($turn == 'w') {
      $opponent = 'b';
    }
    else {
      $opponent = 'w';
    }
  
    if (count($gap_coords) == 2) {
      if (!$this->may_castle_short($turn)) {
        $error = ERROR_CANNOT_CASTLE_SHORT;
      }
    }
    else { // count == 3
      if (!$this->may_castle_long($turn)) {
        $error = ERROR_CANNOT_CASTLE_LONG;
      }
    }
    
    if ($error == "") {
      foreach ($gap_coords as $gap_coord) {    
        if (!$board->square_at_coord_is_empty($gap_coord)) {
          $error = ERROR_CASTLING_SQUARES_BLOCKED;
        }
      }
    }
    
    if ($error == "") {      
      if ($board->is_check($turn)) {
        $error = ERROR_CANNOT_ESCAPE_CHECK_BY_CASTLING;
      }
    }
    // Check the squares between the king's current position and where he will move to are not attacked  
    if ($error == "") {
      foreach ($gap_coords as $gap_coord) {
        $square = new Square;
        $square->set_coord($gap_coord);
        if ($board->square_is_under_attack($square, $opponent)) {
          $error = ERROR_CANNOT_CASTLE_ACROSS_CHECK;
        }
      }
    }

    if ($error == "") {
      $board->move_piece(vchess_coord2square($king_from), vchess_coord2square($king_to));  // White King
      $board->move_piece(vchess_coord2square($rook_from), vchess_coord2square($rook_to));  // Rook
      
      $this->may_not_castle($turn);
      $this->set_last_move('Ke1-g1');
    }
    
    return $error;
  }
  
  /**
   * Set the fact that player may not castle
   * 
   * @param $turn 
   *   $turn is 'w' or 'b'
   */
  function may_not_castle($turn) {
    if ($turn == 'w') {
      $this->white_may_not_castle();
    }
    else {
      $this->black_may_not_castle();
    }
  }
  
  /**
   * Save a game move
   *
   * @param int $gid Game id
   * @param int $move_no Number of move, e.g. 3
   * @param $turn Whose turn it is, either "w" or "b"
   * @param $move e.g. "Nb1-c3" or "Bc1xNf4"
   */
  function save_move($move_no, $turn, Move $move) {
    db_insert('vchess_moves')
    ->fields(array(
        'gid' => $this->gid,
        'move_no' => $move_no,
        'color' => $turn,
        'long_move' => $move->long_format(),
        'algebraic' => $move->algebraic(),
        // In order to avoid timestamp problems, we always store
        // the timestamp as GMT, which does not change for summer time 
        'datetime' => gmdate("Y-m-d H:i:s"),
        ))
        ->execute();
  }
  
  /**
   * Save an open game
   */
  function save() {

    if (!isset($this->gid)) {
      $this->gid = db_insert('vchess_games')
      ->fields(array(
          'turn' => 'w',
          'status' => $this->status(),
          'white_uid' => $this->white_uid,
          'black_uid' => $this->black_uid,
          'board' => $this->board->position(),
          'castling' => $this->castling(),
          'en_passant' => $this->en_passant(),
          'time_per_move' => $this->time_per_move,
          'time_units' => $this->time_units,
          // In order to avoid timestamp problems, we always store
          // the timestamp as GMT, which does not change for summer time
          'time_started' => gmdate("Y-m-d H:i:s"),
          )
      )
      ->execute();
    }
    else {
      $sql = "SELECT gid FROM {vchess_games} WHERE gid = '" . $this->gid . "'";
  
      $result = db_query($sql);
      $exists = $result->fetchField();
    
      if ($exists) {
        db_update('vchess_games')->fields(array(
            'turn' => $this->turn(),
            'status' => $this->status(),
            'white_uid' => $this->white_uid(),
            'black_uid' => $this->black_uid(),
            'board' => $this->board()->position(),
            'castling' => $this->castling(),
            'en_passant' => $this->en_passant(),
            'time_per_move' => $this->time_per_move,
            'time_units' => $this->time_units,
        ))
        ->condition('gid', $this->gid)
        ->execute();
      }
    }
  }

  /**
   * Abort an open game. This is only possible if your opponent did not move
   * at all yet or did not move for more than four weeks. Aborting a game will
   * have NO influence on the game statistics. Return a status message.
   */
  function abort($uid) {
    //  global $res_games;
  
    $gamefolder =  variable_get('vchess_game_files_folder', 'vchess-data');
    $res_games = $gamefolder;
  
    //  ioLock();
  
    $game = new Game($gid);
    if ($game == NULL) {
      return 'ERROR: Game "' . $gid . '" does not exist!';
    }
    if (!$game['p_mayabort']) {
      return 'ERROR: You cannot abort the game!';
    }
    unlink("$res_games/$gid");
  
    //  ioUnlock();
  
    return 'Game "' . $gid . '" deleted.';
  }
  
  /**
   * Undo last move (only possible if game is not over yet).
   */
  function handle_undo($uid) {

  }
  
}

/**
 * HACK: this function checks whether en-passant is possible
 */
function vchess_en_passant_is_ok($player, $pos, $dest, $opp_ep_flag) {
  if ($opp_ep_flag != 'x') {
    if ($dest % 8 == $opp_ep_flag) {
      // if (pawn_attacks($pos,$dest)) right now
      // this is not required as we only use this
      // function in isStaleMate which uses correct dests
      if (($player == 'w' && floor($dest / 8) == 5)
          || ($player == 'b' && floor($dest / 8) == 2)) {
        return 1;
      }
    }
  }
  return 0;
}