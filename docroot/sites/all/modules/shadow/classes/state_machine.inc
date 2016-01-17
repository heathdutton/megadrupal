<?php

class ShadowStateMachine {
  protected $transitions;
  protected $state_holder;
  protected $stack_state_holder;

  protected $regex_allow_whitespace;
  protected $regex_case_sensitive;

  /**
   * Constructor.
   *
   * @param array $transitions
   *   array(
   *     'start' => array(
   *       array('/./', 'source'),
   *     ),
   *     'source' => array(
   *       array('/[a-z]/', 'destination', 'token name', TRUE),
   *     ),
   *     'destination' => array(
   *       array('.', 'end'),
   *     ),
   *   )
   */
  public function __construct($transitions) {
    $this->transitions = $transitions;
    $this->state_holder = new ShadowStateHolder();
    $this->stack_state_holder = array();
    $this->regex_allow_whitespace = FALSE;
    $this->regex_case_sensitive = TRUE;
  }

  /**
   * Parse input.
   *
   * @param string $input
   */
  public function parse($input) {
    $this->state_holder = new ShadowStateHolder();
    $this->stack_state_holder = array();
    $this->state_holder->state = 'start';
    $length = strlen($input);
    array_push($this->stack_state_holder, $this->state_holder);
    while (!empty($this->stack_state_holder)) {
      $this->state_holder = array_pop($this->stack_state_holder);
      // Check for end of input.
      if ($this->state_holder->position == $length) {
        if ($this->state_holder->state == 'end') {
          // Done.
          return;
        }
        else {
          // Check if there is a possible next end state.
          foreach ($this->transitions[$this->state_holder->state] as $transition) {
            $value = array_shift($transition);
            if (empty($value) && $transition[0] == 'end') {
              // Do a transition.
              if (!empty($transition[1])) {
                $this->state_holder->token_values[] = $this->state_holder->buffer;
                $this->state_holder->token_types[] = $transition[1];
              }
              $this->state_holder->state = $transition[0];
              // Done.
              return;
            }
          }
        }
        if (!empty($this->stack_state_holder)) {
          // Make another choice previously by popping from stack.
          continue;
        }
        else {
          throw new Exception("Unexpected end in state \"{$this->state_holder->state}\"", 2);
        }
      }
      $char = $input{$this->state_holder->position};
      $context = substr($input, $this->state_holder->position);
      // Validate possible transitions for this state.
      foreach ($this->transitions[$this->state_holder->state] as $transition) {
        $value = array_shift($transition);
        $match = NULL;

        // Build pattern.
        $allow_ws_begin = $this->regex_allow_whitespace;
        $allow_ws_end = $this->regex_allow_whitespace;
        if (substr($value, 0, 1) == '^') {
          $value = substr($value, 1);
          $allow_ws_begin = FALSE;
        }
        if (substr($value, -1, 1) == '$') {
          $value = substr($value, 0, -1);
          $allow_ws_end = FALSE;
        }

        $pattern = '/^';
        if ($allow_ws_begin) {
          $pattern .= '[\\s]*';
        }
        $pattern .= $value;
        if ($allow_ws_end) {
          $pattern .= '[\\s]*';
        }
        $pattern .= '/s';
        if (!$this->regex_case_sensitive) {
          $pattern .= 'i';
        }

        // Check if we may do this transition.
        if (strcmp($value, $char) == 0 || (strlen($value) > 1 && preg_match($pattern, $context, $match))) {
          $match = $match ? $match[0] : $char;

          // Add possible next state to stack.
          $new_state = clone $this->state_holder;

          if (empty($transition[2])) {
            // Add the match to the possible next state.
            $new_state->buffer .= $match;
          }

          if (!empty($transition[1])) {
            // Save token.
            $new_state->token_values[] = $new_state->buffer;
            $new_state->token_types[] = $transition[1];
            $new_state->buffer = '';
          }
          
          $new_state->state = $transition[0];
          if (!isset($this->transitions[$new_state->state])) {
            throw new Exception("Trying to switch to unknown state \"{$new_state->state}\"");
          }
          $new_state->position += strlen($match);
          array_push($this->stack_state_holder, $new_state);
        }
      }
    }
    if (empty($this->stack_state_holder)) {
      // No possible transition found!
      $near = substr($input, $this->state_holder->position, 10);
      throw new Exception("Parse error in state \"{$this->state_holder->state}\" at offset {$this->state_holder->position} (near \"$near\")", 1);
    }
  }

  public function get_state_holder() {
    return $this->state_holder;
  }
}
