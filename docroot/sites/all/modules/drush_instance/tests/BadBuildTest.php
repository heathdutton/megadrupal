<?php

/**
 * @file BadBuildTest.php
 *
 * Extends BuildTest class to test what happens when bad builds are attempted
 */

/**
 * @class BadBuildTest
 */
class BadBuildTest extends BuildTest {
  /**
   * Test: build off a branch with no makefile
   */
  function testNoMakefile() {
    $result = $this->_instance_build($this->alias_name, $this->alias_dir, "--branch=master");

    // Command should fail
    $this->assertFalse($result['success'],
                       "Was able to build off branch master; but there should be no makefile on that branch");
    // It should fail based on not finding the makefile
    $output = join("\n", $result['output']);
    $this->assertTrue(strpos($output,
                      "Makefile not present in source.") !== FALSE, "Was able to find a makefile in branch master. Output follows:\n\n$output");
  }

  /**
   * Test: build off a tag that doesn't exist
   */
  function testNonexistentTag() {
    $result = $this->_instance_build($this->alias_name, $this->alias_dir, "--tag=not_a_tag");
    // Command should fail
    $this->assertFalse($result['success'],
                       "Was able to build off a tag that does not exist");
    // It should fail based on not finding the makefile
    $this->assertTrue(strpos(join("", $result['output']),
                      "Unable to check out tag not_a_tag") !== FALSE, "Was able to find a makefile in a tag that does not exist");
  }
}
