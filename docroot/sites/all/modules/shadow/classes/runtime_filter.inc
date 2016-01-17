<?php

class ShadowRuntimeFilter extends ShadowStateMachine {
  /**
   * @var array
   *   array(
   *     array('left_table', 'left_field', 'right_table', 'right_field', array(
   *       array('field', array(values)),
   *       ...
   *     ), optional),
   *     ...
   *   )
   */
  protected $joins;

  /**
   * @var string
   */
  protected $table;

  /**
   * @var string
   */
  protected $field;

  /**
   * @var array Array with allowed values, mixed strings and numeric types.
   */
  protected $values;

  /**
   * @var int
   */
  protected $join_index;

  public function __construct($definition = NULL) {
    $this->joins = array();
    $this->table = NULL;
    $this->field = NULL;
    $this->values = array();

    $transitions = array(
      'start' => array(
         array('/[a-z]/i', 'fieldname'),
      ),

      'fieldname' => array(
         array('/[a-z0-9_]/i', 'fieldname'),
         array(':', 'start_value', 'fieldname', TRUE),
      ),

      'start_value' => array(
         array('\'', 'string', NULL, TRUE),
         array('/[0-9\\.\\-]/', 'numeric'),
      ),

      'string' => array(
         array('/[^\'\\\\]/', 'string'),
         array('\\', 'backslashed', NULL, TRUE),
         array('\'', 'after_value', 'string', TRUE),
      ),

      'backslashed' => array(
         array('/./', 'string'),
      ),

      'numeric' => array(
         array('/[0-9\\.\\-]/', 'numeric'),
         array('', 'end', 'numeric'),
      ),

      'after_value' => array(
         array('', 'end'),
      ),
    );

    $this->regex_allow_whitespace = FALSE;
    $this->regex_case_sensitive = FALSE;
    
    parent::__construct($transitions);
    
    if ($definition) {
      $this->parse($definition);
    }
  }

  public function parse($input) {
    parent::parse($input);

    $this->field = NULL;
    $this->value = NULL;
    
    $token_count = count($this->state_holder->token_types);
    for ($i = 0; $i < $token_count; ++$i) {
      $type = $this->state_holder->token_types[$i];
      $value = $this->state_holder->token_values[$i];
      switch ($type) {
        case 'fieldname':
          $this->field = $value;
          break;
        case 'string':
          $this->value = $value;
          break;
        case 'numeric':
          $this->value = (int) $value;
          break;
      }
    }
  }

  /**
   * Get the fieldname.
   *
   * @return string
   */
  public function getFieldName() {
    return $this->field;
  }

  /**
   * Get the value used for filtering.
   *
   * @return array
   */
  public function getValue() {
    return $this->value;
  }
}