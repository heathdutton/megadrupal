<?php

/**
 * @file
 * Tic Tac Toe
 */

require 'tic_tac_toe_game_search.php';

class TicTacToe extends TicTacToeGameSearch {

  /**
   * Indicates if the game is being drawn or not.
   */
  protected function isDrawn(TicTicTacBoard $board) {
    $ret = TRUE;
    for ($i = 0; $i < count($board); $i++) {
      if ($board->get($i) == TicTicTacBoard::BLANK) {
        $ret = FALSE;
        break;
      }
    }
    return $ret;
  }

  /**
   * Checks all the win states for player.
   */
  protected function isWon(TicTicTacBoard $board, $player) {
    switch (TRUE) {
      case ($this->winChecker(0, 1, 2, $player, $board)):
      case ($this->winChecker(3, 4, 5, $player, $board)):
      case ($this->winChecker(6, 7, 8, $player, $board)):
      case ($this->winChecker(0, 3, 6, $player, $board)):
      case ($this->winChecker(1, 4, 7, $player, $board)):
      case ($this->winChecker(2, 5, 8, $player, $board)):
      case ($this->winChecker(0, 4, 8, $player, $board)):
      case ($this->winChecker(2, 4, 6, $player, $board)):
        $rt = TRUE;
        break;

      default:
        $rt = FALSE;
        break;
    }
    return $rt;
  }

  /**
   * Checks if the user is going to be winner with this board situation.
   */
  protected function winChecker($c1, $c2, $c3, $player, TicTicTacBoard $board) {
    $b = TicTicTacBoard::BLANK;
    if ($player) {
      $b = TicTicTacBoard::HUMAN;
    }
    else {
      $b = TicTicTacBoard::COM;
    }
    if ($board->get($c1) == $b && $board->get($c2) == $b && $board->get($c3) == $b) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Evaluetes the board position.
   */
  protected function boardEvaluation(TicTicTacBoard $position, $player) {
    $count = 0;
    for ($i = 0; $i < 9; $i++) {
      if ($position->get($i) == TicTicTacBoard::BLANK) {
        $count++;
      }
    }
    $count = 10 - $count;

    // Prefer the center cell.
    $base = 1;
    if ($position->get(4) == TicTicTacBoard::HUMAN && $player) {
      $base += 0.4;
    }
    if ($position->get(4) == TicTicTacBoard::COM && !$player) {
      $base -= 0.4;
    }
    $ret = ($base - 1);
    if ($this->isWon($position, $player)) {
      return $base + (1 / $count);
    }
    if ($this->isWon($position, !$player)) {
      return -($base + (1 / $count));
    }
    $this->debug("Board Eval: $ret");
    return $ret;
  }

  /**
   * Returns all of the possible moves from current position.
   */
  protected function possibleMoves(TicTicTacBoard $position, $player) {
    $count = 0;
    for ($i = 0; $i < 9; $i++) {
      if ($position->get($i) == TicTicTacBoard::BLANK) {
        $count++;
      }
    }
    if ($count == 0) {
      return NULL;
    }
    $ret = array();
    $count = 0;
    for ($i = 0; $i < 9; $i++) {
      if ($position->get($i) == TicTicTacBoard::BLANK) {
        $pos = new TicTicTacBoard();
        for ($j = 0; $j < 9; $j++) {
          $pos->set($j, $position->get($j));
        }
        if ($player) {
          $pos->set($i, TicTicTacBoard::HUMAN);
        }
        else {
          $pos->set($i, TicTicTacBoard::COM);
        }
        $ret[$count++] = $pos;
      }
    }
    return $ret;
  }

  /**
   * Indicates if the search has reached the maximum depth or not.
   */
  protected function isMaxDepth(TicTicTacBoard $board) {
    $ret = FALSE;
    if ($this->isWon($board, FALSE)) {
      $ret = TRUE;
    }
    elseif ($this->isWon($board, TRUE)) {
      $ret = TRUE;
    }
    elseif ($this->isDrawn($board)) {
      $ret = TRUE;
    }
    return $ret;
  }

}
