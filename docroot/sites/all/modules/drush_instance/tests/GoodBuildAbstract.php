<?php

/**
 * @file GoodBuildAbstract.php
 *
 * Extends BuildTest class to test that a "good" build works as expected.
 * Abstracted for use with make or composer.
 */

/**
 * @class GoodBuildAbstract.
 */
abstract class GoodBuildAbstract extends BuildTestWithBuild {

  // Store the output from the pre-class build process
  static $instance_build_output;

  /**
   * Implements setUpBeforeClass()
   *
   * We want an existing build to be the pre-requisite for our work below, so we can run granular tests
   */
  public static function setUpBeforeClass($alias_name = NULL) {
    self::$instance_build_output = parent::setUpBeforeClass("", $alias_name);
  }

  /**
   * Test: build
   */
  function testBuild() {
    $this->assertTrue(self::$instance_build_output["success"],
      "Initial build for this test class failed: error report follows.\n"
      . join("\n", self::$instance_build_output["output"]));
  }

  /**
   * Test: Drupal core
   */
  function testDrupalCore() {
    $this->assertTrue(file_exists("{$this->site_root}/CHANGELOG.txt"), "Can't find Drupal core CHANGELOG");
    $this->assertTrue(strpos(file_get_contents("{$this->site_root}/CHANGELOG.txt"),"Drupal 7.21, 2013-03-06") !== false,
                      "CHANGELOG does not specify Drupal 7.21");
  }

  /**
   * Test: sites/default
   */
  function testSitesDefault() {
    // sites/default contains something we expect
    $this->assertTrue(count(glob("{$this->sites_default}/instance.drush.inc")) > 0, "Can't find sample contents in sites/default");

    // sites/default has the correct git origin repository
    drush_shell_cd_and_exec($this->sites_default, 'git remote show origin 2>' . drush_bit_bucket());
    $output = drush_shell_exec_output();
    $found_origin = FALSE;
    foreach($output as $line) {
      if(trim($line) == "Fetch URL: http://git.drupal.org/project/drush_instance.git") {
        $found_origin = TRUE;
      }
    }
    $this->assertTrue($found_origin, "Can't find correct remote origin in sites/default git repository");
    return;

    // sites/default is on the right branch
    drush_shell_cd_and_exec($this->sites_default, 'git branch 2>' . drush_bit_bucket());
    $output = drush_shell_exec_output();
    $this->assertTrue(!empty($output) && in_array("* 7.x-1.x-dev", $output), "Could not detect sites/default git repository to be on 7.x-1.x-dev");
  }

  /**
   * Test: symlinks
   */
  function testSymlinks() {
    // ./user_module_symlink is a symlink to the user module
    $symlink = "{$this->site_root}/symlinks/user";
    $this->assertTrue(file_exists($symlink) && is_link($symlink), "Symlink not found or is not a symlink");
    $this->assertTrue(in_array("$symlink/user.module", glob("$symlink/*")), "Symlink did not work (can't see contents)");
  }

  /**
   * Test: rsync
   */
  function testRsync() {
    $file = "{$this->site_root}/sites/includes_rsynced/date.inc";
    $this->assertTrue(file_exists($file), "Rsynced includes/date.inc not found in sites/includes_rsynced: {$this->site_root}");
  }

  /**
   * Test: copy
   */
  function testCopy() {
    $file = "{$this->site_root}/CHANGELOG_copy.txt";
    $this->assertTrue(file_exists($file), "Copied CHANGELOG not found: {$this->site_root}");
    $file = "{$this->site_root}/sites/includes_copied/date.inc";
    $this->assertTrue(file_exists($file), "Copied includes/ contents not found: {$this->site_root}");
  }

  /**
   * Test: file permissions
   */
  function testPermissions() {
    // sites/files/core - chmod_group
    $this->assertTrue((fileperms("{$this->site_root}/sites/files/core") & 0x0010) != 0, "sites/files/core has not had permission change chmod_group");
    $this->assertTrue((fileperms("{$this->site_root}/some_random_toplevel_file.txt") & 0x0002) != 0, "some_random_toplevel_file.txt has not had permission change chmod_group");
  }

  /**
   * Test: template files
   */
  function testTemplatefiles() {
    foreach(array('vhost', 'crontab') as $template_key) {
      $this->assertTrue(file_exists("{$this->site_root}/sites/$template_key"), "Can't find template $template_key", "error");
    }
  }
}
