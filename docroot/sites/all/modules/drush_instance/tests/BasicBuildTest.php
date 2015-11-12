<?php

/**
 * @file BasicBuildTest.php
 *
 * Extends BuildTest class to test basics of building and test suite setup
 */

/**
 * @class BasicBuildTest
 */
class BasicBuildTest extends BuildTest {

  /**
   * Test: our setup, but especially the class setup, which is a bit complex
   */
  function testSetUp() {
    $record = $this->alias;
    $this->assertEquals($record['#name'], 'instance.local');
  }

  /**
   * Test: the build process
   */
  function testBuild() {
    // Invoke instance-build as a separate process
    $result = $this->_instance_build();
    $this->assertTrue($result['success'], "Test build failed.");
  }
}
