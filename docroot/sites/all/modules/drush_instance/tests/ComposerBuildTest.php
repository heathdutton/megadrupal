<?php

/**
 * @file
 * Extends GoodBuildAbstract class to test a composer-based build.
 */

// Ideally autoload would pick up on this.
if (!class_exists("GoodBuildAbstract")) {
  require_once(__DIR__ . "/GoodBuildAbstract.php");
}

/**
 * @class ComposerBuildTest
 */
class ComposerBuildTest extends GoodBuildAbstract {

  // Store the output from the pre-class build process.
  // Has to be defined in this class, not (just) GoodBuildAbstract.
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
   * Should be added by composer.
   */
  public function testViewsModule() {
     $this->assertTrue(file_exists("{$this->sites_all_modules}/views"), "Can't find Views module that Composer should have added.");
  }
}
