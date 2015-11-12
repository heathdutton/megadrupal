<?php

/**
 * @file ReBuildAbstract.php
 *
 * Extends BuildTest class to test rebuilding success.
 * Abstracted for use with make or composer.
 */

/**
 * @class ReBuildAbstract
 */
abstract class ReBuildAbstract extends BuildTest {

  // Store the output from both build and rebuild processes.
  static $build_output;
  static $rebuild_output;

  // Test content for checking re-sourcing of file.
  static $testContent = "Test content";

  /**
   * Implements setUpBeforeClass()
   *
   * We want an existing build, which is then rebuilt, to be the pre-requisite
   * for our work below, so we can run granular tests
   */
  public static function setUpBeforeClass($alias_name = NULL) {
    parent::setUpBeforeClass($alias_name);

    // First build
    self::$build_output = self::_instance_build(self::$default_alias['alias_name'], self::$default_alias['alias_dir']);

    // Make any changes.
    // We want to re-source some_random_toplevel_file.txt, so add some text there.
    // If the text still exists after rebuild, it wasn't re-sourced but moved.
    file_put_contents(self::$default_alias['alias']['root'] . "/some_random_toplevel_file.txt", self::$testContent);

    // Then rebuild
    self::$rebuild_output = self::_instance_build(self::$default_alias['alias_name'], self::$default_alias['alias_dir'],
                          "--rebuild");
  }

  /**
   * Test: paths preserved
   */
  function testPathsPreserved() {
    _drush_instance_build_validate_paths($this->alias);
    foreach($this->alias["instance"]["paths"] as $path => $config) {
      if (file_exists("{$this->site_root}/$path")) {
        $this->assertContains($config['preserve_on_rebuild'], array("keep", "source"), "Path $path should not have been preserved by rebuild, but it's still there! Logs follow.\n\n"
          . join("\n", self::$build_output["output"])
          . "\n\n" . join("\n", self::$rebuild_output["output"]));
      }
      else {
        $this->assertContains($config['preserve_on_rebuild'], array("discard", NULL), "Path $path should have been preserved by rebuild, but it was lost! Logs follow.\n\n"
          . join("\n", self::$build_output["output"])
          . "\n\n" . join("\n", self::$rebuild_output["output"]));
      }
    }
  }

  /**
   * Test: preserve_on_rebuild=source re-sourced.
   */
  function testReSourced() {
    // some_random_toplevel_file.txt should have been re-sourced.
    if (file_get_contents(self::$default_alias['alias']['root'] . "/some_random_toplevel_file.txt") == self::$testContent) {
      $this->fail("Re-sourcing did not work for some_random_toplevel_file.txt: still contains test content.");
    }
  }

  /**
   * Test: symlink still works.
   */
  function testSymlinks() {
    // ./user_module_symlink is a symlink to the user module
    $symlink = self::$default_alias['alias']['root'] . "/symlinks/user";
    $this->assertTrue(file_exists($symlink) && is_link($symlink), "Symlink not found or is not a symlink");
    $this->assertTrue(in_array("$symlink/user.module", glob("$symlink/*")), "Symlink did not work (can't see contents)");
  }

}
