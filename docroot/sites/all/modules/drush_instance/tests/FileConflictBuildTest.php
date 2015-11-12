<?php

/**
 * @file
 * Test a broken build broken owing to file conflict.
 */

/**
 * @class FileConflictBuildTest
 */
class FileConflictBuildTest extends BuildTestWithBuild {

  // Store the output from the pre-class build process.
  // Has to be defined in this class, not (just) GoodBuildAbstract.
  static $instance_build_output;

  /**
   * Implements setUpBeforeClass().
   *
   * Set the build alias to the one with a file conflict, and run.
   */
  public static function setUpBeforeClass() {
    self::$instance_build_output = parent::setUpBeforeClass("", "instance.fileconflict");
  }

  /**
   * Test: sites/all/modules/views
   *
   * Should be added by composer.
   */
  public function testBuildBroken() {
    $output = join("\n", self::$instance_build_output['output']);
    $this->assertFalse(self::$instance_build_output['success'], "Build broken by file conflict appears to have worked after all.\n\n$output");
    $this->assertTrue(
      strpos($output, "Could not move into new build: CHANGELOG.txt/test_conflict.txt") !== FALSE,
      "File conflict build error messages do not include warning of conflict:\n\n$output"
    );
  }
}
