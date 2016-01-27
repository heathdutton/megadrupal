<?php

/**
 * @file
 * Abstract class to perform alpha beta search in two-player zero-sum games
 */

abstract class TicTacToeGameSearch {

  const DEBUG = FALSE;
  const HUMAN = TRUE;
  const COM = FALSE;
  /**
   * Function to check board is drawn.
   */
  protected abstract function isDrawn(TicTicTacBoard $board);
  /**
   * Function to check board is won.
   */
  protected abstract function isWon(TicTicTacBoard $board, $player);
  /**
   * Function to do board evaluation.
   */
  protected abstract function boardEvaluation(TicTicTacBoard $board, $player);
  /**
   * Function to check board possible move.
   */
  protected abstract function possibleMoves(TicTicTacBoard $board, $player);
  /**
   * Function to check board max depth.
   */
  protected abstract function isMaxDepth(TicTicTacBoard $board);
  /**
   * Function to check debug.
   */
  protected function debug($msg) {
    if (self::DEBUG) {
      echo $msg . "\n\r";
    }
  }

  /**
   * Starts searching with initial alpha and beta to create minimax tree.
   */
  public function alphaBeta(TicTicTacBoard $board, $player) {
    return $this->alphaBetaHelper($board, $player, 1000000, -1000000);
  }

  /**
   * Alpha-Beta Search.
   */
  protected function alphaBetaHelper(TicTicTacBoard $board, $player, $alpha, $beta) {
    if ($this->isMaxDepth($board)) {
      $val = $this->boardEvaluation($board, $player);
      $this->debug("alphaBetaHelper mx depth -> alpha : $alpha | beta : $beta | player : $player | board : $board");
      return array($val, NULL);
    }
    $best = array();
    $moves = $this->possibleMoves($board, $player);
    for ($i = 0; $i < count($moves); $i++) {
      $avalues = $this->alphaBetaHelper($moves[$i], !$player, -$beta, -$alpha);
      $value = -floatval($avalues[0]);
      if ($value > $beta) {
        $beta = $value;
        $best = array();
        array_push($best, $moves[$i]);
        for ($j = 1; $j < count($avalues); $j++) {
          if ($avalues[$j] != NULL) {
            $best[] = $avalues[$j];
          }
        }
      }
      if ($beta >= $alpha) {
        break;
      }
    }
    $ret = array();
    $ret[0] = floatval($beta);
    for ($i = 0; $i < count($best); $i++) {
      $ret[] = $best[$i];
    }
    return $ret;
  }

  /**
   * Starts the game with initial board position.
   */
  public function play(array $boardposition) {
    $board = new TicTicTacBoard($boardposition);

    $status = NULL;
    if ($this->isWon($board, TRUE)) {
      $status = TicTicTacBoard::HUMAN;
    }
    elseif ($this->isWon($board, FALSE)) {
      $status = TicTicTacBoard::COM;
    }
    elseif ($this->isDrawn($board)) {
      $status = 0;
    }
    if (!is_null($status)) {
      return array('board' => $boardposition, 'status' => $status);
    }

    $value = $this->alphaBeta($board, self::COM);
    $boardposition = $value[1]->get();
    $board = new TicTicTacBoard($boardposition);

    if ($this->isWon($board, TRUE)) {
      $status = TicTicTacBoard::HUMAN;
    }
    elseif ($this->isWon($board, FALSE)) {
      $status = TicTicTacBoard::COM;
    }
    elseif ($this->isDrawn($board)) {
      $status = 0;
    }
    return array('board' => $boardposition, 'status' => $status);
  }
}
