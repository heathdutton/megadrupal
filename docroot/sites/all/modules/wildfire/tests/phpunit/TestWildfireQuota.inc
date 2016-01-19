<?php
/**
 * @file
 * Test the WildfireQuota class.
 */

class TestWildfireQuota extends PHPUnit_Framework_TestCase {
  /**
   * Overrides parent setUp().
   */
  public function setUp() {
    require_once '../../WildfireQuota.inc';

    parent::setUp();
  }

  /**
   * Test the constructor.
   *
   * Make sure it sets up the defaults correctly when refresh is turned off.
   */
  public function testConstructor() {
    $quota = new WildfireQuota(FALSE);

    $this->assertEquals(
      0,
      $quota->getTopup()
    );
    $this->assertEquals(
      0,
      $quota->getMonthly()
    );
    $this->assertEquals(
      0,
      $quota->getTotal()
    );
    $this->assertEquals(
      0,
      $quota->getLastChecked()
    );
  }
}
