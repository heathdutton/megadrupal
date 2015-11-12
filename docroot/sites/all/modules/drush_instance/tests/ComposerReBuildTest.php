<?php

/**
 * @file
 * Extends ReBuildAbstract class to test a composer-based build.
 */

// Ideally autoload would pick up on this.
if (!class_exists("ReBuildAbstract")) {
  require_once(__DIR__ . "/ReBuildAbstract.php");
}

/**
 * @class ComposerReBuildTest
 */
class ComposerReBuildTest extends ReBuildAbstract {
  // Define this as a property of this class, not its parent class.
  static $instance_build_output;

  /**
   * Implements setUpBeforeClass().
   *
   * Set the build alias to the composer-driven one and run.
   */
  public static function setUpBeforeClass() {
    self::$instance_build_output = parent::setUpBeforeClass("instance.composer");
  }

  /**
   * Test: sites/all/modules/views
   *
   * Should be added by composer, and preserved in rebuild.
   */
  public function testViewsModule() {
     $this->assertTrue(file_exists("{$this->sites_all_modules}/views"), "Can't find Views module that Composer should have added.");
  }

}
