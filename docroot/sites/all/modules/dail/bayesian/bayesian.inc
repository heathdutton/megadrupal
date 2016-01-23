<?php

/**
 * @file: bayesian.inc
 *
 * A class that implements basic bayesian filtering.
 */

/**
 * Load dependencies.
 */
require_once(drupal_get_path('module', 'dail') . "/dail.inc");

class Bayesian extends dail {

  /**
   * All tokens are assigned a probability which is a float value from 0 (FALSE)
   * to 1 (TRUE).  If no probability has been calculated yet for this token in
   * the database, we assign a default probability.
   */
  private $default_probability = .4;

  /**
   * When using bayesian logic to determine the rating of a given string, you
   * may want to only look at the n most "interesting" tokens.  The closer a
   * token's probability is to 0 or 1, the more "interesting" it is.  Tokens
   * that are closer to the median of .5 are less "interesting".  When using
   * bayesian logic to detect spam, filtering the analysis to the n most
   * "interesting" tokens helps prevent spammers from weighting their spam with
   * valid content.
   * 
   * This functionality can be completely disabled by setting
   * $this->interesting to -1.  For example:
   *   <?php
   *   $this->interesting(-1);
   */
  private $interesting = 15;

  private $taught = FALSE;

  /**
   * @string $collection
   *   The name of the collection to work with.
   * @boolean $autoteach
   *   Whether or to store the results of this query
   */
  function __construct($collection = 'default', $autoteach = FALSE) {
    parent::__construct($collection);
    $this->type .= '.Bayesian';
    $this->tokens = array();
    $this->autoteach = $autoteach;
  }

  /**
   * @array $tokens
   *   An array of tokens to prepare
   * @boolean $value
   *   TRUE or FALSE, value stored along with token
   */
  function prepare($tokens = array(), $value = NULL) {
    // Allow a single token to be passed in as a string
    if (is_string($tokens)) {
      $token = $tokens;
      $tokens = array($token);
    }

    // Be sure one or more valid tokens are passed in
    if (is_array($tokens) && !empty($tokens)) {
      $collection = $this->collection;
      foreach ($tokens as $token) {
        if (!isset($this->$collection->tokens[$token][$value])) {
          $this->{$this->collection}->tokens[$token][$value] = 0;
        }
        $this->$collection->tokens[$token][$value]++;
      }
    }
  }

  /**
   * Determine the score for all active tokens.  A score can be any float value
   * from 0 to 1.
   */
  function score() {
    $tokens = $this->tokens();
    if (is_array($tokens) && !empty($tokens)) {
      // construct query
      $query = db_select('bayesian_tokens')
        ->fields('bayesian_tokens', array('probability'))
        ->condition('collection', $this->collection)
        ->condition('token', $tokens, 'IN')
        ->orderBy('interest', 'DESC')
        ->orderBy('total_count', 'DESC')
        ->orderBy('last', 'DESC');
      if ($this->interesting > 0) {
        $query->range(0, $this->interesting);
      }
      $results = $query->execute();
      // determine probability
      foreach ($results as $row) {
        $probabilities[] = $row->probability;
      }
      if (!empty($probabilities)) {
        $current = count($probabilities);
        $available = count($tokens);
        if (($current < $this->interesting) && ($current < $available)) {
          for ($i = $current; $i < $available && $i < $this->interesting; $i++) {
            // Fill in tokens with default score...
            $probabilities[] = $this->default_probability;
          }
        }
        return array_sum($probabilities) / count($probabilities);
      }
    }
    return $this->default_probability;
  }

  /**
   * Update values for prepared tokens.
   */
  function teach($force = FALSE) {
    if ($this->taught === TRUE && $force !== TRUE) {
      return FALSE;
    }
    foreach ($this->tokens() as $token) {
      $values = $this->values($token);
      $previous = $values['values'];
      $true = isset($values[TRUE]) ? $values[TRUE] : 0;
      $all_true = $true + $previous->true_count;
      $false = isset($values[FALSE]) ? $values[FALSE] : 0;
      $all_false = $false + $previous->false_count;
      if ($all_true && $all_false) {
        $probability = $all_true / ($all_true + $all_false);
      }
      else if ($all_true) {
        $probability = 1;
      }
      else if ($all_false) {
        $probability = 0;
      }
      else {
        $probability = $this->default_probability;
      }
      db_merge('bayesian_tokens')
        ->key(array('collection' => $this->collection, 'token' => $token))
        ->fields(array(
          'true_count' => $all_true,
          'false_count' => $all_false,
          'total_count' => $all_true + $all_false,
          'probability' => $probability,
          'interest' => $this->interest($probability),
          'last' => time(),
        ))
        ->expression('true_count', "true_count + $true")
        ->expression('false_count', "false_count + $false")
        ->expression('total_count', "total_count + $true + $false")
        ->execute();
    }
    $this->taught = TRUE;
    return TRUE;
  }

  /**
   * @int $value
   *  Optionally set the $interesting value.  Can be -1, or any integer > 0.
   *
   * @return $int
   *  Returns the current $interesting value
   */
  function interesting($value = NULL) {
    if ($value != NULL && is_int($value) && ($value == -1 || $value > 0)) {
      $this->interesting = $value;
    }
    return $this->interesting;
  }

  /**
   * Helper function for calculating interest, which is the drift from the
   * median value of .5.
   */
  private function interest($value) {
    return abs($value - .5);
  }

  /**
   * Function to change the default_probability.  Must be a value from 0 to 1.
   */
  function default_probability($probability = NULL) {
    if (isset($probability) && (is_int($probability) || is_float($probability)) && $probability >= 0 && $probability <= 1) {
      $this->default_probability = $probability;
    }
    else if (isset($probability)) {
      drupal_set_message(t('Invalid default_probability of %probability, ignored.', array('%probability' => $probability)));
    }
    return $this->default_probability;
  }

  /**
   * Returns an array of all tokens.
   */
  function tokens() {
    $collection = $this->collection;
    $tokens = array();
    if (isset($this->$collection) && is_array($this->$collection->tokens)) {
      foreach ($this->$collection->tokens as $token => $values) {
        $tokens[] = $token;
      }
    }
    return $tokens;
  }

  /**
   * Returns an array of all values.
   */
  function values($token = NULL) {
    $collection = $this->collection;
    if (isset($this->$collection) && is_array($this->$collection->tokens)) {
      if (isset($token)) {
        if (is_array($this->$collection->tokens[$token])) {
          if (!isset($this->$collection->tokens[$token]['values'])) {
            $this->$collection->tokens[$token]['values'] = $this->load($token);
          }
          return $this->$collection->tokens[$token];
        }
      }
      else {
        return $this->$collection->tokens;
      }
    }
  }

  /**
   * Dump the current values of all tokens in memory to the screen.
   */
  function debug_echo() {
    $collection = $this->collection;
    if (isset($this->$collection) && is_array($this->$collection->tokens)) {
      echo '<pre>';
      print_r($this->$collection->tokens);
      echo '</pre>';
    }
    else {
      echo "empty\n";
    }
  }

  /**
   * @string $token
   *   Optionally load stored values for a given token.  Specify no token to get
   *   the stored values for all tokens.
   */
  function load($token = NULL) {
    if (isset($token)) {
      $query = db_select('bayesian_tokens')
        ->fields('bayesian_tokens', array('true_count', 'false_count', 'total_count', 'probability', 'interest'))
        ->condition('collection', $this->collection)
        ->condition('token', $token);
      $values = $query->execute();
      foreach ($values as $value) {
        return $value;
      }
      // Fell through, return a default object.
      return $this->create();
    }
    else {
      $tokens = $this->tokens();
      $collection = $this->collection;
      foreach ($tokens as $token) {
        $query = db_select('bayesian_tokens')
          ->fields('bayesian_tokens', array('true_count', 'false_count', 'total_count', 'probability', 'interest'))
          ->condition('collection', $this->collection)
          ->condition('token', $token);
        $values = $query->execute();
        foreach ($values as $value) {
          $this->$collection->tokens[$token]['values'] = $value;
        }
        if (empty($this->$collection->tokens[$token]['values'])) {
          $this->$collection->tokens[$token]['values'] = $this->create();
        }
      }
    }
  }

  /**
   * Returns a default bayesian object.
   */
  function create() {
    return (object)array(
      'true_count' => 0,
      'false_count' => 0,
      'total_count' => 0,
      'probability' => $this->default_probability,
      'interest' => $this->interest($this->default_probability));
  }

  /**
   * If specified, store values.
   */
  function __destruct() {
    if ($this->autoteach && !$this->taught) {
      $this->teach();
    }
  }

}

function bayesian_dail_class() {
  return (object)array(
    'name' => 'bayesian',
    'class' => t('Bayesian'),
    'description' => t('A basic bayesian filtering class.'),
  );
}
