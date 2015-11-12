<?php

/**
 * @file
 * Interfaces and classes.
 */

namespace NodeExpireExtras; {

  /**
   * Defines interval enum.
   */
  interface IntervalEnumInterface {
    const UNDEFINED = 0;
    const DAY = 1;
    const WEEK = 2;
    const MONTH = 3;
    const YEAR = 4;
  }

  /**
   * Defines UpdateExpireStart enum.
   */
  interface UpdateExpireStartEnumInterface {
    const EXPIRY_DATE = 0;
    const CURRENT_DATE = 1;
  }

  /**
   * Defines DateStr interface.
   */
  interface IntervalStrInterface {
    /**
     * Makes interval string.
     */
    public function GetIntervalStr();
  }

  /**
   * Defines helper class for expire dates.
   */
  class IntervalStr implements IntervalStrInterface  {

    /**
     * Multiplier.
     *
     * @var int
     */
    protected $multiplier;

    /**
     * Interval.
     *
     * @var int
     */
    protected $interval;

    /**
     * Constructs an IntervalStr object.
     *
     * @param int $multiplier
     *   Interval multiplier.
     * @param int $interval
     *   Interval.
     */
    public function __construct($multiplier, $interval) {
      $this->multiplier = $multiplier;
      $this->interval = $interval;
    }

    /**
     * Makes interval string.
     */
    public function GetIntervalStr() {

      // Sign.
      $date_str = "+ ";
      if ($this->interval < 0) {
        $date_str = "- ";
      }

      // Value.
      $abs_multiplier = abs($this->multiplier);
      $date_str .= $abs_multiplier . " ";

      // Interval.
      switch ($this->interval) {
        case IntervalEnumInterface::DAY:
          $date_str .= 'day';
          break;

        case IntervalEnumInterface::WEEK:
          $date_str .= 'week';
          break;

        case IntervalEnumInterface::MONTH:
          $date_str .= 'month';
          break;

        case IntervalEnumInterface::YEAR:
          $date_str .= 'year';
          break;

        default:
          // Year.
          $date_str .= "year";
          break;
      }

      if ($abs_multiplier > 1) {
        $date_str .= "s";
      }

      return $date_str;

    }

  }

  /**
   * Defines helper class for expire dates.
   */
  class NodeExpireDateHelper {

    /**
     * Checks. whether the input parameter represents integer.
     */
    public static function isInteger($input) {
      return (ctype_digit(strval($input)));
    }

    /**
     * Extends expiry date for particular node type.
     */
    public static function extendExpireDate($node_type, $expire) {

      if (empty($expire)) {
        // Special case, date is not set, keep it.
        return '';
      }

      $config_handler = new NodeTypeConfigHandler($node_type);

      $start = REQUEST_TIME;

      // Period.
      switch ($config_handler->getUpdateExpiryStart()) {
        case UpdateExpireStartEnumInterface::EXPIRY_DATE:
          $start = strtotime($expire);
          break;

        case UpdateExpireStartEnumInterface::CURRENT_DATE:
          // Do nothing.
          break;

        default:
          // Current Date.
          // Do nothing.
          break;
      }

      $date_str = new IntervalStr($config_handler->getUpdateExpiryMultiplier(),
        $config_handler->getUpdateExpiryInterval());
      $str = $date_str->GetIntervalStr();

      $expire_date = date(NODE_EXPIRE_FORMAT, strtotime($str, $start));

      return $expire_date;
    }
  }

  /**
   * Defines config handler for particular node type.
   */
  class NodeTypeConfigHandler  {

    /**
     * Node type.
     *
     * @var string
     */
    protected $nodeType;

    /**
     * Period.
     *
     * @var array
     */
    protected $nodeTypeConfigs;

    /**
     * Constructs a NodeTypeConfigHandler object.
     *
     * @param string $node_type
     *   Node type.
     */
    public function __construct($node_type) {
      $this->nodeType = $node_type;
      $this->loadConfig();
    }

    /**
     * Loads configuration array.
     */
    protected function loadConfig() {
      $this->nodeTypeConfigs = variable_get('node_expire_ntypes', array());
    }

    /**
     * Returns default node type config.
     */
    protected function getNodeTypeConfigDefault() {
      $config_default = array(
        // Current date.
        'update_expiry_start'  => 1,
        // Year.
        'update_expiry_interval' => 4,
        'update_expiry_multiplier' => 1,
      );

      return $config_default;
    }

    /**
     * Returns single node type config.
     */
    public function getNodeTypeConfig() {
      if ((isset($this->nodeTypeConfigs[$this->nodeType]))
        and ($n_type_config = $this->nodeTypeConfigs[$this->nodeType])) {
        return $n_type_config;
      }
      else {
        return $this->getNodeTypeConfigDefault();
      }
    }

    /**
     * Returns update expiry start.
     */
    public function getUpdateExpiryStart() {
      $config = $this->getNodeTypeConfig();
      if (isset($config['update_expiry_start'])) {
        return $config['update_expiry_start'];
      }
      else {
        return 1;
      }
    }

    /**
     * Returns update expiry interval.
     */
    public function getUpdateExpiryInterval() {
      $config = $this->getNodeTypeConfig();
      if (isset($config['update_expiry_interval'])) {
        return $config['update_expiry_interval'];
      }
      else {
        return 4;
      }
    }

    /**
     * Returns update expiry multiplier.
     */
    public function getUpdateExpiryMultiplier() {
      $config = $this->getNodeTypeConfig();
      if (isset($config['update_expiry_multiplier'])) {
        return $config['update_expiry_multiplier'];
      }
      else {
        return 1;
      }
    }

  }

}
