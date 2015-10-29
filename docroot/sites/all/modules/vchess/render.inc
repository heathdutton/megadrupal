<?php

/**
 * @file
 * Functions for creating renderable (HTML) versions of different things
 */

/**
 * Create a visible view of the game
 *
 * @param $gid
 *   Game id
 *   
 * @param $player
 *   This indicates the color of the player who is playing, and thus the 
 *   viewing perspective of the board.  It is either 'w' (white) or 'b' (black).  
 *   If it is empty then the game is being observed
 *   by someone who is not the player.  Note that player is not necessarily the
 *   same as the person who is to move.  It may be the player is white but it is
 *   black to move.
 *   
 * @param $active
 *   This indicates whether or not the board allows moves or not (i.e. whether the
 *   board is in a view-only state or not) 
 *   
 * 
 */
function vchess_render_game($gid, $player, $active) {
  global $user;
  
  $uid = $user->uid;

  $game = new Game();
  $game->load($gid);
  
  if (isset($_POST['cmd']) && !empty($_POST['cmd'])) {
    $cmd = $_POST['cmd'];  // e.g. Pe2-e4

    $cmdres = '';

    // Save the values for when we need to save to database
    $player_with_turn = $game->turn();
    $move_no = $game->move_no();
    
    if ($cmd == 'abort') {
      $cmdres = $game->abort($uid);
    }
    elseif ($cmd == 'acceptdraw') {
      $cmdres = $game->make_move($uid, 'accept_draw');
    }
    elseif ($cmd == 'refusedraw') {
      $cmdres = $game->make_move($uid, 'refuse_draw');
    }
    else { // try as chess move
      $cmdres = $game->make_move($uid, $cmd);
    }
    
    // Only save move and game if a move has actually been made
    if ($player_with_turn <> $game->turn()) {
      // Save move
      $game->save_move($move_no, $player_with_turn, $game->last_move());
    
      // Save game
      $game->save();
      
      // Ensure that the other player is informed
//      rules_invoke_event('vchess_move_made', $game->white_player(), $game->black_player());
      $opponent = $game->opponent($uid);
      $opponent_user = user_load($opponent->uid());
      
      rules_invoke_event('vchess_move_made', $opponent_user, 
          $gid, $game->last_move()->algebraic());
      
      if ($game->status() <> STATUS_IN_PROGRESS) {
        switch ($game->status()) {
          case STATUS_WHITE_WIN:
            $score = GAMER_WHITE_WIN;
            break;
          case STATUS_BLACK_WIN:
            $score = GAMER_BLACK_WIN;
            break;
          case STATUS_DRAW:
            $score = GAMER_DRAW;
            break;
        }
        
        gamer_update_stats($game->white_uid(), $game->black_uid(), $score);
      }
    
      // Send a notification if email address was supplied.
      //    if ($opponent == 'b') {
      //       $oid = $game['black'];
      //     }
      //     else {
      //       $oid = $game['white'];
      //     }
      //  $email=ioLoadUserEmailAddress($oid);
      $email = FALSE; // Hugh - force following condition to be FALSE
      if ($email) {
        $prot = ($GLOBALS['_SERVER']['HTTPS'] == 'on') ? 'https' : 'http';
        $url = $prot . '://' . $GLOBALS['_SERVER']['HTTP_HOST'] . $GLOBALS['_SERVER']['SCRIPT_NAME'] . '?gid=' . $gid;
        $message = "Dear $oid\n\n$uid has just moved.\n\nMove:\n$move\n\nIt is your turn now!\n\nEnter the game:\n$url";
        mail($email, "[OCC] " . $game['white'] . "vs" . $game['black'] . ": $move->long_format()",
            $message, 'From: ' . $mail_from);
      }
    }
    
    drupal_set_message(check_plain(t($cmdres)));
  }

  // show white at bottom if not playing
  if (empty($player)) {
    $player = 'w';
  }

  // Check if board needs to be shown flipped
  if (isset($_SESSION['flipped' . $gid])) {
    $flipped = $_SESSION['flipped' . $gid];
  }
  else {
    $flipped = FALSE;
  }
  
  // build the actual frame and the board of chessboard squares
  $content = "";
  $content .= '<table class="boardFrame"><tr><td>';  // table to frame board of squares
  $content .= vchess_render_board_squares($game->board(), $player, $active, $flipped);
  $content .= "</td></tr></table>"; // end of table framing board
  
  $content .= "<br />";
  
  return $content;
}

/**
 * Build the table of board squares
 * 
 * @param char $player
 *   This indicates the color of the player who is playing, and thus the 
 *   viewing perspective of the board.  It is either 'w' (white) or 'b' (black).  
 *   If it is empty then the game is being observed
 *   by someone who is not the player.  Note that player is not necessarily the
 *   same as the person who is to move.  It may be the player is white but it is
 *   black to move.
 */
function vchess_render_board_squares($board, $player, $active, $flipped) {
  global $base_url;
  
  $module_path = drupal_get_path('module', 'vchess');
  
  drupal_add_css($module_path . '/vchessstyle.css');
  drupal_add_js($module_path . '/board.js');
  
  $theme = 'default'; // later add global theme
//  $theme = 'wikipedia';  // does not work because pieces include their own background
  
  $content = "";
  $content .= PHP_EOL;  
  
  //  $path = "http://localhost" . base_path() . drupal_get_path('module', 'vchess');
  // e.g.
  // $base_url = http://localhost/chess_drupal-7.14
  // base_root() = http://localhost
  // base_path() = /chess_drupal-7.14
  $path = $base_url . "/" . drupal_get_path('module', 'vchess');
  
  $content .= '<table class="boardMain">';
  
  if (($player == 'w' && !$flipped) || ($player == 'b' && $flipped)) {
    $orientation = 'w';
    $index = 56;
    $pos_change = 1;
    $line_change = -16;
  }
  else {
    $orientation = 'b';
    $index = 7;
    $pos_change = -1;
    $line_change = 16;
  }
  // row [1..8] are normal board rows
  // row 0 contains column labels "a" to "h"
  for ($row = 8; $row >= 0; $row--) {
    $content .= '<tr>' . PHP_EOL;
    // column 0 contains the row labels "1" to "8"
    // columns [1..8] are normal board columns
    for ($col = 0; $col <= 8; $col++) {
      if ($row == 0) {
        // letters a-h at bottom
        if ($col > 0) {
          // ascii:
          // 97 => a
          // 98 => b
          // ...
          // 104 => h
          if ($orientation == 'w') {
            $c = chr(96 + $col);
          }
          else {
            $c = chr(105 - $col);
          }
//          $cell = '<img height=4 src="' . $path . '/images/spacer.gif"><br /><b class="boardSquare">' . $c . '</b>';
          $cell = "<img src='" . $path . "/images/spacer.gif'>$c";
          $content .= '<td align="center" class="fileLetters">' . $cell . '</td>' . PHP_EOL;
        }
        else {
          $table_rows[$row][$col] = "This is col $col";
          $content .= "<td></td>" . PHP_EOL; // Bottom left
        }
      }
      elseif ($col == 0) {
        // number on the left
        if ($orientation == 'w') {
          $i = $row;
        }
        else {
          $i = 9 - $row;
        }
        $cell = '<b class="boardCoord">' . $i . '</B>';
        $cell2 = '<img width=4 src="' . $path . '/images/spacer.gif">';
        $content .= '<td>' . $cell . '</td>' . PHP_EOL;
      }
      else {
        // normal square
        $coord = new Square();
        if ($orientation == 'w') {
          $coord->set_coord(vchess_col2file($col) . $row);
        }
        else {
          $coord->set_coord(vchess_col2file(9 - $col) . (9 - $row));
        }
        $piece = $board->piece($coord);
        $piece_name = strtolower($piece->name());
        $piece_color = $piece->color();
  
        if ((($row + 1) + ($col)) % 2 == 0) {
          $class = 'boardSquareWhite';
        }
        else {
          $class = 'boardSquareBlack';
        }
  
        if ($piece_name != BLANK) {
          // square with a piece and player has the ability to move
          if ($active) {
            // Build the first part of the move e.g. "Nb1"
            $cmdpart = sprintf('%s%s', $board->piece($coord)->type(), $coord->coord());
            // if the piece is the opposition then the cmdpart becomes e.g. "xNb1"
            if ($piece_color <> $player) {
              $cmdpart = "x" . $cmdpart;
            }
            $cell = '<a href="" onClick="return assembleCmd(\'' . $cmdpart . '\');"><img border=0 src="' . $path . '/images/' . $theme . '/' . $piece_color . $piece_name . '.gif"></a>';
            $content .= '<td id="' . $coord->coord() . '" class="' . $class . '">' . $cell . '</td>' . PHP_EOL;
          }
          else {
            // Square with piece which is not movable
            $cell = '<img src="' . $path . '/images/' . $theme . '/' . $piece_color . $piece_name . '.gif">';
            $content .= '<td class="' . $class . '">' . $cell . '</td>' . PHP_EOL;
          }
        }
        else {
          if ($active) {
            // Build a blank square on active board
            // For this we build the target part of the move e.g. "-c3", so the move will end up as "Nb1-c3"
            $cmdpart = sprintf('-%s', vchess_i2square($index)->coord());
            $cell = '<a href="" onClick="return assembleCmd(\'' . $cmdpart . '\');"><img border=0 src="' . $path . '/images/' . $theme . '/empty.gif"></a>';
            $content .= '<td id="' . $coord->coord() . '" class="' . $class . '">' . $cell . '</td>' . PHP_EOL;
          }
          else {
            $cell = '<img src="' . $path . '/images/' . $theme . '/empty.gif">';
            $content .= '<td class="' . $class . '">' . $cell . '</td>' . PHP_EOL;
          }
        }
        $index += $pos_change;
      }
    }
    $index += $line_change;
    $content .= "</tr>" . PHP_EOL;
  }
  $content .= "</table>" . PHP_EOL; // end of table with squares including coordinates
  
  return $content;
}


/** 
 * Render command form which contains information about players and last
 * moves and all main buttons (shown when necessary). The final command
 * is mapped to hidden field 'cmd' on submission. Show previous command
 * result $cmdres if set or last move if any. Fill move edit with $move
 * if set (to restore move when notes were saved).
 */
function vchess_command_form($form, &$form_state, Game $game) {
  global $user;   
  
  $move = "";
  
  $form['cmd'] = array(
    '#type' => 'hidden',
    '#value' => '',
  );

  $form['comment'] = array(
    '#type' => 'value',
    '#value' => '',
  );

  $form['privnotes'] = array(
    '#type' => 'value',
    '#value' => '',
  );
  
  $form['moveButton'] = array(
    '#type' => 'submit',
    '#value' => 'Move',
//    '#attributes' => array('class' => 'invisible'),
      '#attributes' => array('style' => array('visibility:hidden;')),
//    '#attributes' => array('class=invisible'),
    '#validate' => array('vchess_move_button_clicked')
  );

  $form['flipBoardButton'] = array(
    '#type' => 'submit', // For now!
    '#value' => 'Flip board',
    '#validate' => array('vchess_flip_board_clicked')
  );
  
  if ($game->is_move_made() 
  && !$game->is_over()
  && ($game->white_uid() == $user->uid || $game->black_uid() == $user->uid)) {
    $form['resignButton'] = array(
      '#type' => 'submit',
      '#value' => 'Resign',
      '#validate' => array('vchess_resign_clicked')
    );
  }
  
  $form['gid'] = array(
    '#type' => 'hidden',
    '#value' => $game->gid()
  );
  
  $form['white'] = array(
    '#type' => 'hidden',
    '#value' => $game->white_player()->name()
  );
  
  $form['black'] = array(
    '#type' => 'hidden',
    '#value' => $game->black_player()->name()
  );
  
  $form['turn'] = array(
    '#type' => 'hidden',
    '#value' => $game->turn()
  );

  $form['move'] = array(
    '#type' => 'hidden',
    '#value' => $move,
  );
  
  // abort options
  return $form;
}

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function vchess_render_command_form($game) {
  //$move = ""; // Hugh - removed this as function parm, since used to get this parm but was always empty!
  
  $out = drupal_get_form('vchess_command_form', $game);

  $out = drupal_render($out);
  
  return $out;
}

/**
 * Resign from a particular game.  This is the form handler for the resignation button.
 */
function vchess_resign_clicked($form, &$form_state) {
  global $user;

  $turn = $form_state['values']['turn'];
  $gid = $form_state['values']['gid'];

  $game = new Game();
  $game->load($gid);
  $game->resign($user->uid);
  $game->save();

  if ($game->status() == STATUS_BLACK_WIN) {
    $score = GAMER_BLACK_WIN; // white resigned
  }
  else {
    $score = GAMER_WHITE_WIN; // black resigned
  }

  gamer_update_stats($game->white_uid(), $game->black_uid(), $score);

  drupal_set_message(t('You have now resigned.'));
}

/**
 * This is the form handler for the flip board button.
 */
function vchess_flip_board_clicked($form, &$form_state) {
  global $user;

  $turn = $form_state['values']['turn'];
  $gid = $form_state['values']['gid'];

  if (!isset($_SESSION['flipped'. $gid])) {
    $_SESSION['flipped' . $gid] = FALSE;
  }
  
  if ($_SESSION['flipped' . $gid]) {
    drupal_set_message(t('Flip now OFF!'));
    $_SESSION['flipped' . $gid] = FALSE;
  }
  else {
    drupal_set_message(t('Flip now ON!'));
    $_SESSION['flipped' . $gid] = TRUE;
  }
}

/**
 * Form handler for the move button
 */
function vchess_move_button_clicked($form, &$form_state) {
  // This is called artifically to avoid the Flip or Resign
  // buttons being called as submit buttons.  So, even though
  // it is empty it still needs to exist
  
  // The real move is handled in vchess_render_game()
}

/**
 * Render a FEN string as an HTML board
 */
function vchess_render_FEN($fen_string) {
  $game = new Game();
  
  $game->setup_position($fen_string);
  
  $html = vchess_render_board_squares($game->board(), "w", FALSE, FALSE);
  
  return $html;
}

