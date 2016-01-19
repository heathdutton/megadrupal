<?php

abstract class DcycleCheck {
  private $metrics;
  private $artifacts;
  private $errors;
  private $preflight;
  // Arguments used when creating the class, see the contructor
  private $args;

  /**
   * Constructor
   *
   * @param $args
   *   Arguments in the format:
   *     Array
   *     (
   *         [0] => arg1
   *         [1] => arg2
   *         [2] => arg3
   *         [3] => arg4
   *     )
   *   For example, if this is called via drush dcycle-check Class arg1 arg2 you will get
   *     Array
   *     (
   *         [0] => arg1
   *         [1] => arg2
   *     )
   */
  public function __construct($args) {
    $this->args = $args;
  }

  /**
   * Getter for the args
   *
   * The args are passed on construction and can be used when running or
   * preflighting the command. Subclasses will typically call this. See DcycleCoder
   * for example usage.
   *
   * @return
   *   Arguments in the format:
   *     Array
   *     (
   *         [0] => arg1
   *         [1] => arg2
   *         [2] => arg3
   *         [3] => arg4
   *     )
   *   For example, if this is called via drush dcycle-check Class arg1 arg2 you will get
   *     Array
   *     (
   *         [0] => arg1
   *         [1] => arg2
   *     )
   */
  public function GetArgs() {
    return $this->args;
  }

  private function Preflight() {
    return $this->_Preflight_();
  }

  /**
   * Run the check if the preflight is OK.
   */
  function Run() {
    if ($this->Preflight()) {
      return $this->_Run_();
    }
  }

  static function loadXML($uri) {
    $return = simplexml_load_file($uri);
    if (!$return) {
      throw new Exception('Could not load XML file');
    }
    return $return;
  }

  function GetArtifacts() {
    self::MakeArray($this->artifacts);
    return $this->artifacts;
  }
  function GetMetrics() {
    self::MakeArray($this->metrics);
    return $this->metrics;
  }
  function GetErrors() {
    self::MakeArray($this->errors);
    return $this->errors;
  }
  protected function AddArtifact($artifact) {
    $this->AddInfo($this->artifacts, $artifact);
  }
  protected function AddMetric($metric) {
    $this->AddInfo($this->metrics, $metric);
  }
  protected function AddError($error) {
    $this->AddInfo($this->errors, $error);
  }
  private function MakeArray(&$variable) {
    if (!is_array($variable)) {
      $variable = array();
    }
  }
  private function AddInfo(&$variable, $info) {
    self::MakeArray($variable);
    if (is_array($info)) {
      $variable = array_merge($variable, $info);
    }
    else {
      $variable[] = $info;
    }
  }

  abstract function _Preflight_();

  /**
   * The actual code to run this check.
   */
  abstract function _Run_();
}
