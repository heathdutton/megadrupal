<?php

/**
 * @file
 * Main Route class
 */

class Route {
  public $active_step;
  public $active_substep;
  public $processed_steps;
  public $total_steps; 
  public $steps;  
  public $current_subtask;
  public $messages;
  
  /**
   * Provides storage container for forms and applications that use this route.
   * Because this object persists between requests and is associated with current user's session
   * this attribute can be used to store temporary data (i.e. search criteria) between requests.
   * Other forms can later read and modify this data.
   */
  private $data = array();

  public function __construct($steps = array()) {
    $this->steps = array();
    $this->reset($steps);
  }

  public function __set($name, $value) {
    $this->data[$name] = $value;
  }

  public function __get($name) {
    if (array_key_exists($name, $this->data)) {
      return $this->data[$name];
    }

    return null;
  }

  public function __isset($name) {
    return isset($this->data[$name]);
  }

  public function __unset($name)
  {
    unset($this->data[$name]);
  }

  /**
   * Sets data array associated with this route
   */
  public function set_data($data) {
    $this->data = $data;
  }

  /**
   * Returns data associated with this route
   */
  public function get_data() {
    return $this->data;
  }

  /**
   * Returns formatted, display safe property value.
   *
   * @param $property
   */
  public function format($property) {
    // Allow all modules to react to format attempt
    $format_proposals = module_invoke_all('route_format_property', $property, $this->$property);
    if (array_filter($format_proposals)) {
      // If any module returned the value that is not FALSE please spit out the last proposal, which is the proposal returned by the module with the lowest weight.
      return array_pop($format_proposals);
    }

    return self::sanitize($this->$property);
  }

  /**
   * Sanitizes the value
   *
   * @static
   * @param $value
   */
  private static function sanitize($value) {
    // If no module is keen to handle the formatting, process the property yourself.
    // If property is an array, iterate over it an format each element separately
    if (is_array($value)) {
      $formatted_elements = array();
      foreach($value as $element) {
        $formatted_elements[] = self::sanitize($element);
      }

      return implode(', ', array_filter($formatted_elements));
    }

    // If value is an object, attempt to call magic __toString method.
    // It's toString responsibility to make sure that the user data stored in an object is safe.
    else if (is_object($value)) {
      return (string) $value;
    }

    // If value is of a simple type, just check_plain it.
    else {
      return check_plain($value);
    }
  }

  /**
   * Resets steps associated with the context.
   * This also resets all attributes and processes the first step (Makes it accessible).
   */
  public function set_steps($steps = array()) {
    // If there are any steps available, please set first one as active and automatically process it
    $this->steps = $steps;
    if($this->total_steps = count($this->steps)) {
      $this->active_step = NULL;
      $this->process_step($this->get_first_step());
    }
  }

  /**
   * Reset the route information to default data. Storage is wiped in the process.
   */
  public function reset($steps = array()) {
    if (empty($steps)) {
      $steps = $this->steps;
    } 
    $this->data = array();
    $this->current_subtask = '';
    $this->processed_steps = array();
    $this->active_step = NULL;
    $this->active_substep = 0;
    $this->set_steps($steps);
  }

  /**
   * Mark step as processed.
   * @return Boolean
   */  
  public function process_step($step = NULL) {
    if ($step === NULL) {
      $step = $this->active_step;
    }
    
    if (!in_array($step, $this->processed_steps)) {
      $this->processed_steps[] = $step;
      return TRUE;
    }
    
    return FALSE;
  }
  
  /**
   * Mark step as deprocessed.
   * @return Boolean
   */  
  public function deprocess_step($step = NULL) {
    if ($step === NULL) {
      $step = $this->active_step;
    }
    
    if ($key = array_search($step, $this->processed_steps)) {
      unset($this->processed_steps[$key]);
      return TRUE;
    }
    
    return FALSE;
  }
  
  /**
   * Checks if current or given step has indicator link
   * @return Boolean
   */  
  public function has_indicator($step = NULL) {
    if ($step === NULL) {
      $step = $this->active_step;
    }
    
    return $this->steps[$step]['#indicator'];
  }
  
  
  /**
   * Checks if current or given step is processed
   * @return Boolean
   */  
  public function is_processed($step = NULL) {
    if ($step === NULL) {
      $step = $this->active_step;
    }
    
    return in_array($step, $this->processed_steps);
  }
  
  /**
   * Regresses to the given step (exclusive), so all steps that are after specified one become
   * @return Boolean
   */  
  public function regress($step = NULL) {
    if ($step === NULL) {
      $step = $this->active_step;
    }
    
    foreach(array_reverse($this->steps) as $step_to => $settings) {
      if ($step == $step_to) {
        break;
      }
      $this->deprocess_step($step_to);
    }
  }
  
  /**
   * Checks if the given step exists.
   * 
   * @param string $step
   * @return Boolean 
   */
  public function step_exists($step = NULL) {
    if ($step === NULL) {
      $step = $this->active_step;
    }
    
    $available_steps = array_keys($this->steps);  
    
    return in_array($step, $available_steps);
  }
  
  /**
   * Checks if the given step is preceding in reference to reference_step.
   * 
   * @param string $step
   * @param string $reference_step
   * @return Boolean 
   */
  public function is_preceding($step, $reference_step = NULL) {
    if ($reference_step === NULL) {
      $reference_step = $this->active_step;
    }
    
    return $this->get_step_index($step) < $this->get_step_index($reference_step);
  }
  
  /**
   * Checks if the given step is succeeding in reference to reference_step.
   * @param string $step
   * @param string $reference_step
   * @return Boolean 
   */
  public function is_succeeding($step, $reference_step = NULL) {
    if ($reference_step === NULL) {
      $reference_step = $this->active_step;
    }
    
    return $this->get_step_index($step) > $this->get_step_index($reference_step);
  } 
  
  /**
   * Returns the configuration of active or specified step.
   * @return Array
   */  
  public function get_step_configuration($step = NULL, $substep = NULL) {
    if ($step === NULL) {
      $step = $this->active_step;
    }
    if ($substep === NULL) {
      $substep = $this->active_substep;
    }

    return $this->steps[$step][$substep];
  }
    
  /**
   * Returns order index of the step specified
   * @param String $step
   * @return Mixed 
   */
  public function get_step_index($step = NULL) {
    if ($step === NULL) {
      $step = $this->active_step;
    }
    
    $steps_ordered = array_keys($this->steps);    
    return array_search($step, $steps_ordered);
  }
  
  /**
   * Returns name of the first step
   * 
   * @return
   *   - name of the first step 
   */
  public function get_first_step() {
    return current(array_keys($this->steps));
  }
  
  /**
   * Returns next available step name. FALSE if there are no steps to return
   * @param String $step
   * @return Boolean 
   */
  public function get_next_step($step = NULL) {
    if ($step === NULL) {
      $step = $this->active_step;
    }
    
    if ($steps_ordered = array_keys($this->steps)) {
      $index = $this->get_step_index($step);

      // If we're trying to get next step from step that doesn't exists, please always return the first step.
      // Otherwise return name of next step in the flow.
      if($index !== FALSE) {
        if (isset($steps_ordered[$index+1])) {
          return $steps_ordered[$index+1];
        }
      }
      else {
        return $steps_ordered[0];
      }
    }
    
    return FALSE;
  }
  
  /**
   * Returns previous step name. FALSE if there are no steps to return
   * @param in $step
   * @return Boolean 
   */
  public function get_previous_step($step = NULL) {
    if ($step === NULL) {
      $step = $this->active_step;
    }
    
    if ($steps_ordered = array_keys($this->steps)) {
      $index = $this->get_step_index($step);
      if($index !== FALSE) {
        if (isset($steps_ordered[$index-1])) {
          return $steps_ordered[$index-1];
        }
      }
    }
    
    return FALSE;
  }

  /**
   * Sets a message to display to the user in a perticular step.
   *
   * @param string $step
   * @param string $message
   *   (optional) The translated message to be displayed to the user. For
   *   consistency with other messages, it should begin with a capital letter and
   *   end with a period.
   * @param string $type
   *   (optional) The message's type. Defaults to 'status'. These values are
   *   supported:
   *   - 'status'
   *   - 'warning'
   *   - 'error'
   */
  public function set_message($step, $message, $type = 'status') {
    $this->messages[$step][] = array(
      'message' => $message,
      'type' => $type,
    );
  }

  /**
   * Get messages to display to user in a step.
   *
   * @param string $step
   */
  public function get_messages($step = NULL, $repeat = FALSE) {
    $messages = array();
    if (!$step) {
      $step = $this->active_step;
    }
    if (isset($this->messages[$step])) {
      $messages =  $this->messages[$step];
      if (!$repeat) {
        unset($this->messages[$step]);
      }
    }
    return $messages;
  }
}
