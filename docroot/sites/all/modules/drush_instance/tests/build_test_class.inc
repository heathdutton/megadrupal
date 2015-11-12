<?php

/**
 * @file build_test_class.inc
 *
 * Bootstrapped by phpunit using phpunit.xml, to be
 * available for all tests
 */

/**
 * @class BuildTest
 *
 * Abstract class with no tests of its own
 */
abstract class BuildTest extends PHPUnit_Framework_TestCase {

  // All the alias details, as a class variable (see below)
  static $alias_details;
  // Location of drush_instance files
  static $drush_instance_root;
  // Default alias, extracted from ::$alias_details.
  static $default_alias = NULL;

  /**
   * We only want to assemble alias details the once, so use phpunit's
   * setUpBeforeClass class method and store in a static variable
   */
  public static function setUpBeforeClass($alias_name = NULL) {
    if ($alias_name === NULL) {
      $alias_name = "instance.local";
    }

    // Run parent's own method
    parent::setUpBeforeClass();

    // Where's Drush? Unlike Drush's own test cases, we have to find it!
    require_once(dirname(exec("readlink -f `which drush`")) . '/includes/bootstrap.inc');
    drush_bootstrap_prepare();

    // Need to set umask for all child processes, so that we can detect
    // that permissions-wrangling has really worked
    umask(022);

    // Now start setting up our custom alias(es): need utility functions
    // for this (ideally we'd use drush_boostrap())
    self::$drush_instance_root = dirname(__FILE__) . "/..";
    require_once(self::$drush_instance_root . "/instance.drush.inc");

    // Include our test aliases
    drush_set_context('ALIAS_PATH', array(dirname(__FILE__)));

    // Convert each alias in turn to a unique namespace.
    $alias_old_to_new = array();
    $unique_alias = uniqid();
    $alias_suffixes = array("local", "composer", "fileconflict");
    foreach ($alias_suffixes as $alias_suffix) {
      $alias_old_name = "instance.$alias_suffix";
      // Instantiate the alias.
      drush_sitealias_get_record("@$alias_old_name");
      $alias_old_to_new[$alias_old_name] = "instance$unique_alias.$alias_suffix";
    }
    // Then retrieve them all and loop over the ones we want.
    $aliases =& drush_get_context('site-aliases');

    // Begin aliases file
    $aliases_file = "<?php ";
    // All aliases go in the same directory.
    // Add a random numeric suffix to alias name, to avoid clash with existing aliases
    drush_mkdir($this_alias_dir = _drush_instance_tmp_unique());

    foreach ($alias_old_to_new as $old_name => $new_name) {
      $this_alias =& $aliases["@$old_name"];
      // Site root needs to be a unique temporary directory
      $this_alias['root'] = _drush_instance_tmp_unique();

      // Propagate this unique name into the rsync folder definition
      if (isset($this_alias['instance']['paths']['sites/includes_rsynced'])) {
        foreach ($alias_suffixes as $alias_suffix) {
          $this_alias['instance']['paths']['sites/includes_rsynced']['source']['from']
            = preg_replace("/instance[0-9a-z]*.$alias_suffix/", $new_name,
              $this_alias['instance']['paths']['sites/includes_rsynced']['source']['from']);
        }
      }

      // Add to aliases file
      $aliases_file .= "\$aliases['$new_name'] = " . var_export($this_alias, TRUE) . ";\n\n";

      // And add to this class.
      self::$alias_details[$old_name] = array(
        'alias' => $this_alias,
        'alias_dir' => $this_alias_dir,
        'alias_name' => $new_name,
      );
    }

    // Now write whole aliases file with all aliases.
    file_put_contents("$this_alias_dir/aliases.drushrc.php", $aliases_file);

    // And switch to a single default alias.
    self::$default_alias = self::$alias_details[$alias_name];
  }

  /**
   * Implements tearDownAfterClass()
   */
  public static function tearDownAfterClass() {
    parent::tearDownAfterClass();
    drush_bootstrap_finish();
  }

  /**
   * Implements $this->setup()
   * Moves class variables onto the object, for convenience
   */
  function setUp() {
    parent::setUp();
    foreach(self::$default_alias as $k => $v) {
      $this->{$k} = $v;
    }

    // Some other special variables
    $this->site_root = $this->alias['root'];
    $this->sites_default = $this->site_root . "/sites/default";
    $this->sites_all_modules = $this->site_root . "/sites/all/modules";
  }

  /**
   * Function to build an instance based on alias name and directory
   * Defaults to obtaining these two arguments from the current object
   * Optional third parameter can provide extra arguments
   */
  protected function _instance_build($alias_name = NULL, $alias_dir = NULL, $other_arguments = '') {
    // If we're not given good arguments, we must be in object context,
    // not static context. Test for this and raise nicer exception if not.
    if (!$alias_name || !$alias_dir) {
      if (!isset($this)) {
        throw new Exception("Instance build called in static context but no alias name/dir provided.");
      }
    }

    $alias_name = $alias_name ? $alias_name : $this->alias_name;
    $alias_dir = $alias_dir ? $alias_dir : $this->alias_dir;

    $success = drush_shell_exec("/usr/bin/drush instance-build @$alias_name "
      . " --alias-path=$alias_dir --templates=all --include="
      . self::$drush_instance_root
      . " $other_arguments");

    return array(
      'success' => $success,
      'output' => drush_shell_exec_output(),
    );
  }
}

/**
 * @class BuildTestWithBuild
 *
 * Extend BuildTest to actually perform a build as part of class setup
 */
abstract class BuildTestWithBuild extends BuildTest {
  /**
   * Implements setUpBeforeClass()
   *
   * We want an existing build as a pre-requisite
   */
  public static function setUpBeforeClass($other_arguments = '', $alias_name = NULL) {
    parent::setUpBeforeClass($alias_name);
    return self::_instance_build(
      self::$default_alias['alias_name'],
      self::$default_alias['alias_dir'],
      $other_arguments
    );
  }
}
