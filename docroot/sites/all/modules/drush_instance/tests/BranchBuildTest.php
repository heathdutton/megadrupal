<?php

/**
 * @file BranchBuildTest.php
 *
 * Extends BuildTest class to test that a "good" build works on a branch
 */

/**
 * @class BranchBuildTest
 */
class BranchBuildTest extends BuildTestWithBuild {

  // Store the output from the pre-class build process
  static $instance_build_output;

  /**
   * Implements setUpBeforeClass()
   *
   * We want an existing build to be the pre-requisite for our work below, so we can run granular tests
   */
  public static function setUpBeforeClass() {
    self::$instance_build_output = parent::setUpBeforeClass('--branch=examples');
  }

  /**
   * Test: build
   */
  function testBuild() {
    $this->assertTrue(self::$instance_build_output["success"], 
      "Initial build for this test class failed: error report follows.\n"
      . join("\n", self::$instance_build_output["output"]));
  }
}
