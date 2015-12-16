<?php

class ShadowIndex extends ShadowStateMachine {
  
  protected $schema;
  
  /**
   * @var array
   *   array(
   *     array('field', size),
   *     ...
   *   )
   */
  protected $fields;
  
  public function __construct($definition, $schema = NULL) {
    $this->fields = array();

    $transitions = array(
      'start' => array(
          array('[a-z][a-z0-9_]*', 'field', 'field', FALSE),
      ),
      'field' => array(
          array('(', 'paren_open', NULL, TRUE),
          array(',', 'comma', NULL, TRUE),
          array('', 'end', NULL, FALSE),
      ),
      'paren_open' => array(
          array('[1-9][0-9]*', 'size', 'size', FALSE),
      ),
      'size' => array(
          array(')', 'paren_close', NULL, TRUE),
      ),
      'paren_close' => array(
          array('', 'end', NULL, FALSE),
          array(',', 'comma', NULL, TRUE),
      ),
      'comma' => array(
          array('[a-z][a-z0-9_]*', 'field', 'field', FALSE),
      ),
    );
    
    $this->regex_allow_whitespace = FALSE;
    $this->regex_case_sensitive = FALSE;
    $this->schema = $schema;
    
    parent::__construct($transitions);
    
    if ($definition) {
      $this->parse($definition);
    }
  }
  
  public function parse($input) {
    parent::parse($input);
    
    $this->fields = array();
    
    $token_count = count($this->state_holder->token_types);
    for ($i = 0; $i < $token_count; ++$i) {
      $type = $this->state_holder->token_types[$i];
      $value = $this->state_holder->token_values[$i];
      switch ($type) {
        case 'field':
          foreach ($this->fields as $field) {
            if (strtolower($field[0]) == strtolower($value)) {
              throw new Exception('Cannot add field "' . $value . '" twice');
            }
          }
          $this->fields[] = array($value, NULL);
          break;
        case 'size':
          $this->fields[count($this->fields) - 1][1] = (int) $value;
          break;
      }
    }
    
    if (!empty($this->schema)) {
      foreach ($this->fields as $field) {
        if (!isset($this->schema['fields'][$field[0]])) {
          throw new Exception('Field "' . $field[0] . '" does not exists in schema');
        }
        else {
          $type = $this->schema['fields'][$field[0]]['type'];
          if ($type == 'text' || $type == 'blob') {
            throw new Exception('The ' . $type . ' field "' . $field[0] . '" cannot be used in an index');
          }
        }
      }
    }
  }
  
  public function getDefinition() {
    $definition = array();
    
    foreach ($this->fields as $field) {
      if ($field[1]) {
        $definition[] = $field;
      }
      else {
        $definition[] = $field[0];
      }
    }
    
    return $definition;
  }
}
