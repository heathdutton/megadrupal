<?php

/**
 * @file
 * PHPUnit Tests for Situs.
 */

/**
 * Situs testing class.
 */
class DrakeCase extends Drush_CommandTestCase {

  public static function setUpBeforeClass() {
    parent::setUpBeforeClass();
    // Copy in the command file, so the sandbox can find it.
    copy(dirname(dirname(__FILE__)) . '/drake.drush.inc', getenv('HOME') . '/.drush/drake.drush.inc');
    copy(dirname(dirname(__FILE__)) . '/drake.actions.inc', getenv('HOME') . '/.drush/drake.actions.inc');
    copy(dirname(dirname(__FILE__)) . '/drake.context.inc', getenv('HOME') . '/.drush/drake.context.inc');
    copy(dirname(dirname(__FILE__)) . '/drake.files.inc', getenv('HOME') . '/.drush/drake.files.inc');
  }

  public function setUp() {
    // Specify file explicitly.
    $this->options = array('file' => dirname(__FILE__) . '/drakefile.php');

    // if (!empty($this->aliases)) {
    //   return;
    // }
    // // Make sure the parent build dir exists.
    // if (!file_exists($this->webroot())) {
    //   mkdir($this->webroot());
    // }
    // // Create an alias.
    // $this->aliases = array(
    //   'homer' => array(
    //     'root' => $this->webroot() . '/homer',
    //     'make-file' => dirname(__FILE__) . '/simple.make',
    //   ),
    // );

  //   $this->saveAliases();
  }

  // protected function saveAliases() {
  //   file_put_contents(UNISH_SANDBOX . '/etc/drush/aliases.drushrc.php', $this->file_aliases($this->aliases));
  // }

  function setUpDrupal($num_sites = 1, $install = FALSE, $version_string = UNISH_DRUPAL_MAJOR_VERSION, $profile = NULL) {
    // Remove any old sites.
    $root = $this->webroot();
    $sites = "$root/sites/*";
    `rm -rf $sites`;
    return parent::setUpDrupal($num_sites, $install, $version_string, $profile);
  }

  function testRecursiveFail() {
    $this->drush('drake 2>&1', array('recur1'), $this->options, NULL, NULL, self::EXIT_ERROR);
    // Check error message.
    $this->assertRegExp('/Recursive dependency/', $this->getOutput());
  }

  function testDrakefileDiscovery() {
    // No file should give an error.
    $this->drush('drake 2>&1', array(), array(), NULL, NULL, self::EXIT_ERROR);
    // Check error message.
    $this->assertRegExp('/No drakefile.php files found or specified/', $this->getOutput());

    // A makefile in the current directory should be used.
    copy(dirname(__FILE__) . '/simple.drakefile.php', './drakefile.php');
    $this->drush('drake');
    // Check output.
    $this->assertRegExp('/Simple drakefile\./', $this->getOutput());
    unlink('./drakefile.php');

    // A drakefile.php in sites/all/drush should be used.
    $this->setUpDrupal(1);

    copy(dirname(__FILE__) . '/site.drakefile.php',
      $this->webroot() . '/sites/all/drush/drakefile.php');

    // A "site command". Something like a drush command installed in
    // sites/all/drush.
    mkdir($this->webroot() . '/sites/all/drush/command');
    copy(dirname(__FILE__) . '/site-command.drakefile.php',
      $this->webroot() . '/sites/all/drush/command/command.drakefile.drushrc.php');

    $x = $this->webroot();
    $this->log(`ls $x`);
    // Create a profile drakefile. This should not interfere with any of the
    // discoveries below. We'll test that it works later.
    copy(dirname(__FILE__) . '/profile.drakefile.php', $this->webroot() . '/profiles/standard/drakefile.php');

    $this->drush('@dev drake');
    // Check output.
    $this->assertRegExp('/Site drakefile\./', $this->getOutput());

    // Check that drakefile in the current directory is ignored when alias is
    // specified on the commandline.
    copy(dirname(__FILE__) . '/simple.drakefile.php', './drakefile.php');
    $this->drush('@dev drake');
    $this->assertRegExp('/Site drakefile\./', $this->getOutput());
    $this->drush('@dev drake', array('site'));
    // Check output.
    $this->assertRegExp('/Site drakefile\./', $this->getOutput());
    unlink('./drakefile.php');

    // Test that the site drakefile is found when we're in the site dir.
    chdir($this->webroot());
    $this->drush('drake');
    // Check output.
    $this->assertRegExp('/Site drakefile\./', $this->getOutput());

    // Check that drake files from commands in sites/all/drush is properly
    // incleded.
    $this->drush('drake', array('site-command'));
    // Check output.
    $this->assertRegExp('/Site command drakefile\./', $this->getOutput());

    // Check that a drakefile in the current directory takes precedence over
    // sitewide.
    copy(dirname(__FILE__) . '/simple.drakefile.php', './drakefile.php');
    $this->drush('drake');
    $this->assertRegExp('/Simple drakefile\./', $this->getOutput());
    $this->drush('drake', array('site'));
    // Check output.
    $this->assertRegExp('/Site drakefile\./', $this->getOutput());

    // Test that a drakefile in users home dir is loaded.
    copy(dirname(__FILE__) . '/user.drakefile.php', getenv('HOME') . '/.drush/user.drakefile.drushrc.php');
    $this->drush('drake', array('user'));
    // Check output.
    $this->assertRegExp('/User drakefile\./', $this->getOutput());
    unlink(getenv('HOME') . '/.drush/user.drakefile.drushrc.php');

    // Test that user drakefiles also work in subdirs.
    mkdir(getenv('HOME') . '/.drush/test');
    copy(dirname(__FILE__) . '/user.drakefile.php', getenv('HOME') . '/.drush/test/user.drakefile.drushrc.php');
    $this->drush('drake', array('user'));
    // Check output.
    $this->assertRegExp('/User drakefile\./', $this->getOutput());

    // Check that the files are included in the right order.
    $this->drush('drake', array(), array('show-files' => TRUE));
    $expected_output = "{Loaded drakefiles:
" . getenv('HOME'). "/.drush/test/user.drakefile.drushrc.php
" . $this->webroot() . "/sites/all/drush/drakefile.php
" . $this->webroot() . "/sites/all/drush/command/command.drakefile.drushrc.php
drakefile.php}";
    // Check output.
    $this->assertRegExp($expected_output, $this->getOutput());

    // Take the site drakefile out of the equation.
    unlink($this->webroot() . '/sites/all/drush/drakefile.php');
    // No target should hit the drakefile in the current dir.
    $this->drush('drake');
    $this->assertRegExp('/Simple drakefile\./', $this->getOutput());

    // Remove the drakefile in the current dir. Now only the profile and user
    // drakefile is left.
    unlink('./drakefile.php');

    // We should hit the profile drakefile now.
    $this->drush('drake');
    $this->assertRegExp('/Profile drakefile\./', $this->getOutput());

    // Copy in another profile drakefile.
    copy(dirname(__FILE__) . '/profile.drakefile.php', $this->webroot() . '/profiles/minimal/drakefile.php');

    $this->drush('drake 2>&1', array(), array(), NULL, NULL, self::EXIT_ERROR);
    // Check error message.
    $this->assertRegExp('/Multiple profile drakefile.phps found/', $this->getOutput());

    // Remove the profile drakefiles. Now only the user drakefile is left.
    unlink($this->webroot() . '/profiles/minimal/drakefile.php');
    unlink($this->webroot() . '/profiles/standard/drakefile.php');

    // No target  should give an error, even when there's user drakefiles.
    $this->drush('drake 2>&1', array(), array(), NULL, NULL, self::EXIT_ERROR);
    // Check error message.
    $this->assertRegExp('/No drakefile.php files found or specified/', $this->getOutput());

    // But specific targets should work.
    $this->drush('drake', array('user'));
    // Check output.
    $this->assertRegExp('/User drakefile\./', $this->getOutput());

    unlink(getenv('HOME') . '/.drush/test/user.drakefile.drushrc.php');
    rmdir(getenv('HOME') . '/.drush/test');
  }

  function testMissingTask() {
    copy(dirname(__FILE__) . '/simple.drakefile.php', './drakefile.php');
    // Check that an unknown task fails.
    $this->drush('drake 2>&1', array('unknown-task'), array(), NULL, NULL, self::EXIT_ERROR);
    // Check output.
    $this->assertRegExp('/Unknown task unknown-task/', $this->getOutput());

    // But with the proper argument, it's just quiet.
    $this->drush('drake', array('unknown-task'), array('no-task-ok' => TRUE));
    unlink('./drakefile.php');
  }

  function testContexts() {
    copy(dirname(__FILE__) . '/context.drakefile.php', './drakefile.php');
    $this->drush('drake', array('simple'));
    // Check output.
    $this->assertRegExp('/simple: value1/', $this->getOutput());

    $this->drush('drake', array('replaced'));
    // Check output.
    $this->assertRegExp('/replaced: value1/', $this->getOutput());

    $this->drush('drake', array('replaced-with-extra'));
    // Check output.
    $this->assertRegExp('/replaced-with-extra: before value1 after/', $this->getOutput());

    $this->drush('drake', array('string-manipulation'));
    // Check output.
    $this->assertRegExp('/string-manipulation: before VALUE1 after/', $this->getOutput());

    $this->drush('drake 2>&1', array('unknown-manipulation'), array(), NULL, NULL, self::EXIT_ERROR);
    // Check output.
    $this->assertRegExp('/Unknown or invalid operation/', $this->getOutput());

    $this->drush('drake', array('optional-context-set'));
    // Check output.
    $this->assertRegExp('/optional-context: Context is set with value: ocs-value/', $this->getOutput());

    $this->drush('drake', array('optional-context-unset'));
    // Check output.
    $this->assertRegExp('/optional-context: Context is not set./', $this->getOutput());

    $this->drush('drake', array('optional-context-set-default'));
    // Check output.
    $this->assertRegExp('/optional-context-default: Context is set with value: ocsd-value/', $this->getOutput());

    $this->drush('drake', array('optional-context-unset-default'));
    // Check output.
    $this->assertRegExp('/optional-context-default: Context is set with value: ocd-default-value/', $this->getOutput());

    // Test that command line definition overrides.
    // First check the set value.
    $this->drush('drake', array('override-context'));
    // Check output.
    $this->assertRegExp('/override-context: Context is set with value: value1/', $this->getOutput());

    // Then try to override.
    $this->drush('drake', array('override-context', 'context1=new-value'));
    // Check output.
    $this->assertRegExp('/override-context: Context is set with value: new-value/', $this->getOutput());

    // Ensure that the default tasks is run, even when we specify context.
    $this->drush('drake', array('context1=new-value'));
    // Check output.
    $this->assertRegExp('/simple: new-value/', $this->getOutput());

    // Tests that context works in dependency, when given on the same task.
    $this->drush('drake', array('context-dependency'));
    // Check output.
    $this->assertRegExp('/sub1 run/', $this->getOutput());

    // Ensure that a missing context in dependencies gives a proper error.
    $this->drush('drake 2>&1', array('context-dependency2'), array(), NULL, NULL, self::EXIT_ERROR);
    // Check output.
    $this->assertRegExp('/In context-dependency2 dependencies: No such context "sub"/', $this->getOutput());

    // Tests that context works in dependency
    $this->drush('drake', array('context-dependency2', 'sub=sub1'));
    // Check output.
    $this->assertRegExp('/sub1 run/', $this->getOutput());

    // Tests that context works in dependency, again
    $this->drush('drake', array('context-dependency2', 'sub=sub2'));
    // Check output.
    $this->assertRegExp('/sub2 run/', $this->getOutput());

    // Test that unknown tasks after context resolving errors properly.
    $this->drush('drake 2>&1', array('context-dependency2', 'sub=sub3'), array(), NULL, NULL, self::EXIT_ERROR);
    // Check output.
    $this->assertRegExp('/Unknown task context-dependency-sub3/', $this->getOutput());

    unlink('./drakefile.php');
  }

  function testArguments() {
    copy(dirname(__FILE__) . '/arguments.drakefile.php', './drakefile.php');

    // Simple argument.
    $this->drush('drake', array('simple', 'testvalue'));
    // Check output.
    $this->assertRegExp('/simple: Context is set with value: testvalue/', $this->getOutput());

    // Simple argument should be required.
    $this->drush('drake 2>&1', array('simple'), array(), NULL, NULL, self::EXIT_ERROR);
    // Check output.
    $this->assertRegExp('/Missing argument: string to print/', $this->getOutput());
    // Optional argument set.
    $this->drush('drake', array('simple-optional', 'testvalue'));
    // Check output.
    $this->assertRegExp('/simple-optional: Context is set with value: testvalue/', $this->getOutput());

    // Optional argument not set.
    $this->drush('drake', array('simple-optional'));
    // Check output.
    $this->assertRegExp('/simple-optional: Context is not set./', $this->getOutput());

    // Optional argument with default  set.
    $this->drush('drake', array('simple-optional-default', 'testvalue'));
    // Check output.
    $this->assertRegExp('/simple-optional-default: Context is set with value: testvalue/', $this->getOutput());

    // Optional argument with default not set.
    $this->drush('drake', array('simple-optional-default'));
    // Check output.
    $this->assertRegExp('/simple-optional-default: Context is set with value: default-value/', $this->getOutput());

    unlink('./drakefile.php');
  }

  function testStringDependency() {
    $this->drush('drake', array('string-dependency'), $this->options);
    // Check for shell command output.
    $this->assertRegExp('/Slartibartfast/', $this->getOutput());
  }

  function testBadActions() {
    // No action or depends are useless, but not considered an error.
    $this->drush('drake', array('actionless'), $this->options);

    $this->drush('drake 2>&1', array('unknown-action'), $this->options, NULL, NULL, self::EXIT_ERROR);
    $this->assertRegExp('/Unknown action "unknown"./', $this->getOutput());

    $this->drush('drake 2>&1', array('unknown-action-callback'), $this->options, NULL, NULL, self::EXIT_ERROR);
    $this->assertRegExp('/Callback for action "bad-callback" does not exist./', $this->getOutput());
  }

  function testBasicActions() {
    $this->drush('drake', array('task-with-working-action'), $this->options);

    $this->drush('drake 2>&1', array('task-with-failing-action'), $this->options, NULL, NULL, self::EXIT_ERROR);
  }

  function testShellAction() {
    $this->drush('drake', array('shell-action'), $this->options);
    // Check for shell command output.
    $this->assertRegExp('/Slartibartfast/', $this->getOutput());

    // Missing params should fail the command.
    $this->drush('drake 2>&1', array('bad-arg-shell-action'), $this->options, NULL, NULL, self::EXIT_ERROR);
    $this->assertRegExp('/Action "shell" failed: Required param\(s\) not supplied: "command"./', $this->getOutput());

    // Check that an simple array of commands work.
    $this->drush('drake', array('multiple-shell-action-simple'), $this->options);
    // Check for shell command output.
    $this->assertRegExp('/Slartibartfast/', $this->getOutput());
    $this->assertRegExp('/Deriparamaxx/', $this->getOutput());

    // Test that an array of commands with additional params work.
    $this->drush('drake', array('multiple-shell-action-params-keyed'), $this->options);
    // Check for shell command output.
    $this->assertRegExp('/Slartibartfast/', $this->getOutput());
    $this->assertRegExp('/Deriparamaxx/', $this->getOutput());

    // Test that an array of commands with additional params work.
    $this->drush('drake', array('multiple-shell-action-params-flat'), $this->options);
    // Check for shell command output.
    $this->assertRegExp('/Slartibartfast/', $this->getOutput());
    $this->assertRegExp('/Deriparamaxx/', $this->getOutput());

    // Failing commands should fail the whole process.
    $this->drush('drake 2>&1', array('failing-shell-action'), $this->options, NULL, NULL, self::EXIT_ERROR);
    $this->assertRegExp('/Action "shell" failed: command failed./', $this->getOutput());
  }

  function testContextActions() {
    $this->drush('drake', array('context-shell-action'), $this->options);
    // Check for shell command output.
    $this->assertRegExp('/Arthur Dent/', $this->getOutput());

    $this->drush('drake', array('multiple-contexts'), $this->options);
    // Check for shell command output.
    $this->assertRegExp('/Arthur Dent/', $this->getOutput());
    $this->assertRegExp('/Ford Prefect/', $this->getOutput());

    $this->drush('drake', array('global-context-shell-action'), $this->options);
    // Check for shell command output.
    $this->assertRegExp('/Marvin/', $this->getOutput());

  }

  function testFilesets() {
    // Use a Drupal install to test filesets.
    $this->setUpDrupal(1);
    chdir($this->webroot());

    copy(dirname(__FILE__) . '/filesets.drakefile.php', './drakefile.php');

    $this->drush('drake', array('field-modules', 'root=' . $this->webroot()));
    // Check output.
    $expected = "field-modules: modules/field/tests/field_test.module
field-modules: modules/field/modules/number/number.module
field-modules: modules/field/modules/field_sql_storage/field_sql_storage.module
field-modules: modules/field/modules/text/text.module
field-modules: modules/field/modules/options/options.module
field-modules: modules/field/modules/list/tests/list_test.module
field-modules: modules/field/modules/list/list.module";
    $this->assertEquals($this->sortLines($expected), $this->sortLines($this->getOutput()));

    $this->drush('drake', array('field-modules-less', 'root=' . $this->webroot()));
    // Check output.
    $expected = "field-modules-less: modules/field/tests/field_test.module
field-modules-less: modules/field/modules/number/number.module
field-modules-less: modules/field/modules/field_sql_storage/field_sql_storage.module
field-modules-less: modules/field/modules/text/text.module";
    $this->assertEquals($this->sortLines($expected), $this->sortLines($this->getOutput()));

    $this->drush('drake', array('system-module', 'root=' . $this->webroot()));
    // Check output.
    $expected = "system-module: modules/system/system.module
system-module: " . $this->webroot() . "/modules/system/system.module
system-module: " . $this->webroot() . "/modules/system/system.module";
    $this->assertEquals($expected, trim($this->getOutput()));

    // Check that prefixing / anchors to the root.
    copy($this->webroot() . '/includes/form.inc', $this->webroot() . '/form.inc');
    $this->drush('drake', array('anchoring', 'root=' . $this->webroot()));
    // Check output.
    $expected = "anchoring: form.inc";
    $this->assertEquals($this->sortLines($expected), $this->sortLines($this->getOutput()));

    // Check that patterns are anchored to the end of the file.
    $this->drush('drake', array('scripts', 'root=' . $this->webroot()));
    // Check output.
    $expected = "scripts: scripts/test.script
scripts: modules/simpletest/files/javascript-2.script";
    $this->assertEquals($this->sortLines($expected), $this->sortLines($this->getOutput()));

    // Check slash matching.
    $this->drush('drake', array('php-module', 'root=' . $this->webroot()));
    // Check output.
    $expected = "php-module: modules/php/php.test
php-module: modules/php/php.module
php-module: modules/php/php.info
php-module: modules/php/php.install";
    $this->assertEquals($this->sortLines($expected), $this->sortLines($this->getOutput()));

  }


  function testTaskSkip() {
    copy(dirname(__FILE__) . '/skip.drakefile.php', './drakefile.php');

    $this->drush('drake');
    // Check that all tasks was run.
    $this->assertRegExp('/Task 1/', $this->getOutput());
    $this->assertRegExp('/Task 2/', $this->getOutput());
    $this->assertRegExp('/Subtask 1/', $this->getOutput());
    $this->assertRegExp('/Subtask 2/', $this->getOutput());
    $this->assertRegExp('/Task 4/', $this->getOutput());

    $this->drush('drake', array(), array('skip-tasks' => 'task1,task4'));
    // Check that all but task 1 and 4 was run.
    $this->assertNotRegExp('/Task 1/', $this->getOutput());
    $this->assertRegExp('/Task 2/', $this->getOutput());
    $this->assertRegExp('/Subtask 1/', $this->getOutput());
    $this->assertRegExp('/Subtask 2/', $this->getOutput());
    $this->assertNotRegExp('/Task 4/', $this->getOutput());

    $this->drush('drake', array(), array('skip-tasks' => 'task1,subtask1'));
    // Check that all but task 1 and subtask 1  was run.
    $this->assertNotRegExp('/Task 1/', $this->getOutput());
    $this->assertRegExp('/Task 2/', $this->getOutput());
    $this->assertNotRegExp('/Subtask 1/', $this->getOutput());
    $this->assertRegExp('/Subtask 2/', $this->getOutput());
    $this->assertRegExp('/Task 4/', $this->getOutput());

    $this->drush('drake', array(), array('skip-tasks' => 'task4,task3'));
    // Check that all but task 1 and subtasks of task 3 was run.
    $this->assertRegExp('/Task 1/', $this->getOutput());
    $this->assertRegExp('/Task 2/', $this->getOutput());
    $this->assertNotRegExp('/Subtask 1/', $this->getOutput());
    $this->assertNotRegExp('/Subtask 2/', $this->getOutput());
    $this->assertNotRegExp('/Task 4/', $this->getOutput());

    unlink('./drakefile.php');
  }

  function sortLines($string) {
    $tmp = explode("\n", trim($string));
    sort($tmp);
    return implode("\n", $tmp);
  }

  function testRegressionRecursiveContext() {
    copy(dirname(__FILE__) . '/regression_context.drakefile.php', './drakefile.php');

    // Should fail with a 'context not set'.
    $this->drush('drake 2>&1', array('recursive-undef'), array(), NULL, NULL, self::EXIT_ERROR);
    // Check output.
    $this->assertRegExp('/check-value: No such context "value"/', $this->getOutput());

    // Should pick up the parent context value..
    $this->drush('drake', array('recursive-def'));
    // Check output.
    $this->assertRegExp('/check-value: Context is set with value: recursive-value/', $this->getOutput());
  }

  function testRegressionMultipleArguments() {
    copy(dirname(__FILE__) . '/regression_arguments.drakefile.php', './drakefile.php');

    // Should print both arguments..
    $this->drush('drake', array('multiple', 'testvalue1', 'testvalue2'));
    // Check output.
    $this->assertRegExp('/multiple: Context is set with value: testvalue1/', $this->getOutput());
    $this->assertRegExp('/multiple: Context is set with value: testvalue2/', $this->getOutput());
  }

  function testRegressionActionDefaultArguments() {
    copy(dirname(__FILE__) . '/regression_action_arguments.drakefile.php', './drakefile.php');

    // Should print the set parametera.
    $this->drush('drake', array('optional-set'));
    // Check output.
    $this->assertRegExp('/optional-set: Optional has the value set_value./', $this->getOutput());

    // Should print default value.
    $this->drush('drake', array('optional-unset'));
    // Check output.
    $this->assertRegExp('/optional-unset: Optional has the value default_value./', $this->getOutput());

    // Without a context value, is should print default value.
    $this->drush('drake', array('optional-context'));
    // Check output.
    $this->assertRegExp('/optional-context: Optional has the value default_value./', $this->getOutput());

    // With a context value, it should print the context value.
    $this->drush('drake', array('optional-context', 'optional_context=test'));
    // Check output.
    $this->assertRegExp('/optional-context: Optional has the value test./', $this->getOutput());
}
}
