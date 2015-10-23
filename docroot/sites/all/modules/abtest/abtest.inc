<?php

/**
 * @file
 * Implements the ABTest class for actually doing the real work.
 */

class ABTest {
  public $test_id;
  public $test_name;
  protected $alternatives;
  protected $statistics;

  protected function __construct($obj) {
    $this->test_id = $obj->test_id;
    $this->test_name = $obj->test_name;
    $this->alternatives = array();
    $this->statistics = NULL;
  }

  /**
   * Get or create the test.
   */
  static public function getTest($test_name, $alternatives = NULL, $options = array()) {
    $cache =& drupal_static(__FUNCTION__);

    if (!isset($cache[$test_name])) {
      $test = self::_getTest($test_name, $alternatives, $options);
      $cache[$test->test_name] = $test;
      $cache[$test->test_id]   = $test;
    }

    return $cache[$test_name];
  }

  static protected function _getTest($test_name, $alternatives, $options) {
    $create = isset($options['create']) ? $options['create'] : TRUE;

    // get the test if it already exists
    $obj = db_select('abtest', 't')
      ->fields('t')
      // TODO: too much magic!
      ->condition(is_numeric($test_name) && !$create ? 't.test_id' : 't.test_name', $test_name)
      ->execute()
      ->fetch();
    if (!empty($obj)) {
      return new ABTest($obj);
    }

    if (!$create) {
      // if we aren't creating on demand, we bail!
      return NULL;
    }

    $alternatives = self::parseAlternatives($alternatives);

    $conversion_name = isset($options['conversion']) ?
      $options['conversion'] : $test_name;

    // create a new test
    $obj = (object)array(
      'test_name'       => $test_name,
      'conversion_name' => $conversion_name,
      'created'         => time(),
    );
    drupal_write_record('abtest', $obj);
    $test = new ABTest($obj);

    // create the alternatives
    foreach ($alternatives as $alternative_value) {
      // TODO: support weight!
      $alternative = (object)array(
        'test_id' => $test->test_id,
        'value'   => $alternative_value,
        'lookup'  => $test->calculateAlternativeLookup($alternative_value),
      );
      drupal_write_record('abtest_alternatives', $alternative);
      $test->alternatives[$alternative->alternative_id] = $alternative;
    }

    return $test;
  }

  /**
   * Returns an array of tests which respond to the given conversion event.
   */
  static public function getTestsForConversionEvent($event_name) {
    $tests = array();

    $res = db_query("SELECT * FROM {abtest} WHERE conversion_name =
      :event_name", array(':event_name' => $event_name));
    foreach ($res as $obj) {
      $tests[] = new ABTest($obj);
    }

    return $tests;
  }

  /**
   * Gets an object representing the alternative for the current user.
   */
  public function getAlternativeForParticipant($identity = NULL, $create = TRUE) {
    if (is_null($identity)) {
      $identity = abtest_get_identity();
    }

    // if this user already participated in this test, return the same
    // alternative that they always get!
    $obj = db_query("SELECT a.*, p.converted FROM {abtest_participants} p JOIN
      {abtest_alternatives} a ON a.alternative_id = p.alternative_id WHERE
      p.test_id = :test_id AND p.abtest_identity = :identity",
      array(':test_id' => $this->test_id, ':identity' => $identity))->fetch();
    if (!empty($obj)) {
      $obj->value = unserialize($obj->value);
      return $obj;
    }

    if ($create) {
      // chosse the correct alternative
      // TODO: support weight!
      $alternative = clone $this->moduloChoice($identity, $this->getAlternatives());
      $alternative->converted = 0;

      // record this choice for this user
      $obj = (object)array(
        'test_id'         => $this->test_id,
        'abtest_identity' => $identity,
        'alternative_id'  => $alternative->alternative_id,
      );
      drupal_write_record('abtest_participants', $obj);

      // increment the participant count on this alternative
      db_query("UPDATE {abtest_alternatives} SET participants = (participants + 1)
        WHERE alternative_id = :alternative_id",
        array('alternative_id' => $alternative->alternative_id));

      return $alternative;
    }
  }

  /**
   * Tracks a conversion for the given user.
   */
  public function trackConversionForParticipant($identity = NULL) {
    if (is_null($identity)) {
      $identity = abtest_get_identity();
    }

    $alternative = $this->getAlternativeForParticipant($identity, FALSE);
    if (!is_null($alternative) && !$alternative->converted) {
      // mark this participant as converted
      db_query("UPDATE {abtest_participants} SET converted = 1
        WHERE test_id = :test_id AND abtest_identity = :identity",
        array(':test_id' => $this->test_id, ':identity' => $identity));

      // increment the conversion count on this alternative
      db_query("UPDATE {abtest_alternatives} SET conversions = (conversions + 1)
        WHERE alternative_id = :alternative_id",
        array('alternative_id' => $alternative->alternative_id));
    }
  }

  /**
   * Returns all the alternatives.
   */
  public function getAlternatives() {
    if (count($this->alternatives) == 0) {
      $res = db_query("SELECT * FROM {abtest_alternatives} WHERE test_id = :test_id",
        array(':test_id' => $this->test_id));
      foreach ($res as $obj) {
        $obj->value = unserialize($obj->value);
        $this->alternatives[$obj->alternative_id] = $obj;
      }
    }

    return $this->alternatives;
  }

  /**
   * Returns an ABTestStatistics object to query stats data from.
   */
  public function getStatistics() {
    if (is_null($this->statistics)) {
      $this->statistics = new ABTestStatistics($this);
    }
    return $this->statistics;
  }

  //
  // Internal functions:
  //

  // gets the full alternatives array from an alternatives argument
  static protected function parseAlternatives($alternatives) {
    if (is_null($alternatives)) {
      $alternatives = array(1, 0);
    }
    elseif (is_numeric($alternatives)) {
      $alternatives = range('a', chr($alternatives + 97));
    }

    return $alternatives;
  }

  protected function calculateAlternativeLookup($value) {
    return md5($this->test_name . serialize($value));
  }

  // Copied from ABingo - seems weird but I'll trust Patrick...
  // If I had to guess, the choice for a given participant for a given test has
  // to be deterministic (to avoid race conditions) but still reasonably random.
  protected function moduloChoice($identity, $items) {
    $items = array_values($items);
    $count = count($items);

    $md5 = md5($this->test_name . $identity);
    // PHP can't modulo numbers as big as an MD5 hash, so we limit to the
    // first 2 bytes.
    $part = substr($md5, 0, 4);
    $index = hexdec($part) % $count;
    // Egads! Sometimes modulo returns a negative number. :-/
    $index = ($index > 0 && $index < $count) ? $index : 0;

    return $items[$index];
  }
}

class ABTestStatistics {
  protected $test;

  protected $total_participants;
  protected $total_conversions;
  protected $confidence_score;
  protected $alternatives;

  public function __construct($test) {
    $this->test = $test;
    $this->alternatives = array();
  }

  public function getTotalParticipants() {
    if (is_null($this->total_participants)) {
      $this->total_participants = 0;
      foreach ($this->test->getAlternatives() as $alternative) {
        $this->total_participants += $alternative->participants;
      }
    }
    return $this->total_participants;
  }

  public function getTotalConversions() {
    if (is_null($this->total_conversions)) {
      $this->total_conversions = 0;
      foreach ($this->test->getAlternatives() as $alternative) {
        $this->total_conversions += $alternative->conversions;
      }
    }
    return $this->total_conversions;
  }

  public function getAlternative($alternative_id) {
    if (!isset($this->alternatives[$alternative_id])) {
      $alternative_list = $this->test->getAlternatives();
      $alternative = clone $alternative_list[$alternative_id];

      // Do some simple calculations
      $alternative->total_participants_percent = 100 *
        ($alternative->participants / $this->getTotalParticipants());
      $alternative->total_conversions_percent = 100 *
        ($alternative->conversions / $this->getTotalParticipants());
      $alternative->conversions_percent = 100 *
        ($alternative->conversions / $alternative->participants);

      $this->alternatives[$alternative_id] = $alternative;
    }

    return $this->alternatives[$alternative_id];
  }
}

