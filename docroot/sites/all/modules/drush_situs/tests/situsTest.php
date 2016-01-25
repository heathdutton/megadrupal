<?php

/**
 * @file
 * PHPUnit Tests for Situs.
 */

/**
 * Situs testing class.
 */
class situsCase extends Drush_CommandTestCase {

  public static function setUpBeforeClass() {
    parent::setUpBeforeClass();
    // Copy in the command file, so the sandbox can find it.
    copy(dirname(dirname(__FILE__)) . '/situs.drush.inc', getenv('HOME') . '/.drush/situs.drush.inc');
  }

  public function setUp() {
    if (!empty($this->aliases)) {
      return;
    }
    // Make sure the parent build dir exists.
    if (!file_exists($this->webroot())) {
      mkdir($this->webroot());
    }
    // Create an alias.
    $this->aliases = array(
      'homer' => array(
        'root' => $this->webroot() . '/homer',
        'make-file' => dirname(__FILE__) . '/simple.make',
      ),
      'marge' => array(
        'root' => $this->webroot() . '/marge',
        'make-file' => dirname(__FILE__) . '/simple.make',
      ),
      'bart' => array(
        'root' => $this->webroot() . '/bart',
        'make-file' => dirname(__FILE__) . '/with_git.make',
      ),
      'lisa' => array(
        'root' => $this->webroot() . '/lisa',
        'make-file' => dirname(__FILE__) . '/failing.make',
      ),
      'maggie' => array(
        'root' => $this->webroot() . '/maggie',
        'make-file' => dirname(__FILE__) . '/simple.make',
      ),
      'apu' => array(
        'root' => $this->webroot() . '/apu',
        'make-file' => dirname(__FILE__) . '/simple.make',
      ),
    );

    $this->saveAliases();
  }

  protected function saveAliases() {
    file_put_contents(UNISH_SANDBOX . '/etc/drush/aliases.drushrc.php', unish_file_aliases($this->aliases));
  }

  function testBuild() {
    $root = $this->aliases['homer']['root'];
    $this->drush('situs-build', array('@homer'));

    // Test for some random stuff, just to make sure the make worked.
    $this->assertFileExists($root . '/index.php', 'Index is there.');
    $this->assertFileExists($root . '/modules/system/system.module', 'System module is there.');
    $this->assertFileExists($root . '/sites/all/modules/contrib/devel/devel.module', 'Devel is there.');
  }

  function testBuildAgain() {
    $root = $this->aliases['marge']['root'];
    $this->drush('situs-build', array('@marge'));

    // Test for some random stuff, just to make sure the make worked.
    $this->assertFileExists($root . '/index.php', 'Index is there.');
    $this->assertFileExists($root . '/modules/system/system.module', 'System module is there.');
    $this->assertFileExists($root . '/sites/all/modules/contrib/devel/devel.module', 'Devel is there.');

    // Create some fake sites.
    mkdir($root . '/sites/marge1');
    file_put_contents($root . '/sites/marge1/settings.php', '<?php');
    mkdir($root . '/sites/marge2');
    file_put_contents($root . '/sites/marge2/settings.php', '<?php');

    // Change make file so we have a change to check for.
    $this->aliases['marge']['make-file'] = dirname(__FILE__) . '/simple2.make';
    $this->saveAliases();
    $this->drush('situs-build', array('@marge'));

    $this->assertFileExists($root . '/sites/all/modules/devel/devel.module', 'Devel is in new location.');

    $this->assertFileExists($root . '/sites/marge1/settings.php', 'Site has been moved.');
    $this->assertFileExists($root . '/sites/marge2/settings.php', 'Site has been moved.');
  }

  function testGitCheck() {
    $root = $this->aliases['bart']['root'];
    $this->drush('situs-build', array('@bart'));

    // Test for some random stuff, just to make sure the make worked.
    $this->assertFileExists($root . '/sites/all/modules/devel/devel.module', 'Devel is there.');
    $this->assertFileExists($root . '/sites/all/modules/devel/.git', 'Devel is a checkout.');

    $file = $root . '/sites/all/modules/devel/devel.info';
    $new_content = '; New content.';
    file_put_contents($file, $new_content);
    $this->assertStringEqualsFile($file, $new_content, 'File has new content.');

    $this->drush('situs-build', array('@bart'), array('git-check' => TRUE), NULL, NULL, self::EXIT_ERROR);
    $this->assertStringEqualsFile($file, $new_content, 'File still has new content.');

    // Check that it works when set in alias.
    $this->aliases['bart']['git-check'] = TRUE;
    $this->saveAliases();
    $this->drush('situs-build', array('@bart'), array(), NULL, NULL, self::EXIT_ERROR);
    $this->assertStringEqualsFile($file, $new_content, 'File still has new content.');

    // Check --git-check-ignore
    $this->drush('situs-build', array('@bart'), array('git-check' => TRUE, 'git-check-ignore' => 'sites/all/modules/devel'), NULL, NULL);
    $this->assertStringNotEqualsFile($file, $new_content, 'File dont have new content.');

    $file = $root . '/sites/all/modules/devel/devel.info';
    $new_content = '; New content.';
    file_put_contents($file, $new_content);
    $this->assertStringEqualsFile($file, $new_content, 'File has new content.');

    // Check --git-check-ignore-regex
    $this->drush('situs-build', array('@bart'), array('git-check' => TRUE, 'git-check-ignore-regex' => 'devel'), NULL, NULL);
    $this->assertStringNotEqualsFile($file, $new_content, 'File dont have new content.');

    $file = $root . '/sites/all/modules/devel/devel.info';
    $new_content = '; New content.';
    file_put_contents($file, $new_content);
    $this->assertStringEqualsFile($file, $new_content, 'File has new content.');

    // Run without git check.
    $this->aliases['bart']['git-check'] = FALSE;
    $this->saveAliases();
    $this->drush('situs-build', array('@bart'));
    $this->assertStringNotEqualsFile($file, $new_content, 'File dont have new content.');
  }

  function testFail() {
    $root = $this->aliases['lisa']['root'];
    $this->drush('situs-build', array('@lisa'), array(), NULL, NULL, self::EXIT_ERROR);
    $this->assertFileNotExists($root, 'Failing build creates no directory.');

    // Change to a make file that works.
    $this->aliases['lisa']['make-file'] = dirname(__FILE__) . '/with_git.make';
    $this->saveAliases();
    $this->drush('situs-build', array('@lisa'));

    $this->assertFileExists($root . '/index.php', 'Index is there.');

    // Change it back and check that the build command returns an error.
    $this->aliases['lisa']['make-file'] = dirname(__FILE__) . '/failing.make';
    $this->saveAliases();

    $this->drush('situs-build', array('@lisa'), array(), NULL, NULL, self::EXIT_ERROR);

    // Coder was only specified in the failing make file.
    $this->assertFileNotExists($root . '/sites/all/modules/coder/coder.module', 'Coder is there.');
  }

  function testPermissions() {
    $root = $this->aliases['maggie']['root'];
    $this->drush('situs-build', array('@maggie'));

    mkdir($root . '/sites/maggie1');
    file_put_contents($root . '/sites/maggie1/settings.php', '<?php');
    mkdir($root . '/sites/maggie2');
    file_put_contents($root . '/sites/maggie2/settings.php', '<?php');
    mkdir($root . '/sites/maggie3');
    file_put_contents($root . '/sites/maggie3/settings.php', '<?php');


    // Make dir read only.
    // chmod(dirname($root), 0555);
    chmod($root, 0000);
    $this->drush('situs-build', array('@maggie'), array(), NULL, NULL, self::EXIT_ERROR);
    chmod($root, 0755);
    $this->assertFileExists($root . '/index.php', 'Index is still there.');
    $this->assertFileExists($root . '/sites/maggie1/settings.php', 'Site 1 is still there.');
    $this->assertFileExists($root . '/sites/maggie2/settings.php', 'Site 2 is still there.');
    $this->assertFileExists($root . '/sites/maggie3/settings.php', 'Site 3 is still there.');
    // chmod(dirname($root), 0755);

    // Make a site dir unreadable.
    chmod($root . '/sites/maggie2', 0000);
    $this->drush('situs-build', array('@maggie'), array(), NULL, NULL, self::EXIT_ERROR);
    chmod($root . '/sites/maggie2', 0755);
    $this->assertFileExists($root . '/index.php', 'Index is still there.');
    $this->assertFileExists($root . '/sites/maggie1/settings.php', 'Site 1 is still there.');
    $this->assertFileExists($root . '/sites/maggie2/settings.php', 'Site 2 is still there.');
    $this->assertFileExists($root . '/sites/maggie3/settings.php', 'Site 3 is still there.');
  }

  function testNoAlias() {
    $root = $this->webroot() . '/moe';
    $make_file = dirname(__FILE__) . '/simple.make';

    $this->drush('situs-build', array(), array('root' => $root, 'make-file' => $make_file));

    // Test for some random stuff, just to make sure the make worked.
    $this->assertFileExists($root . '/index.php', 'Index is there.');
    $this->assertFileExists($root . '/modules/system/system.module', 'System module is there.');
    $this->assertFileExists($root . '/sites/all/modules/contrib/devel/devel.module', 'Devel is there.');
  }

  function testNoAliasRelative() {
    $root = $this->webroot() . '/ned';
    copy(dirname(__FILE__) . '/simple.make', UNISH_SANDBOX . '/share/ned.make');
    $make_file = './share/ned.make';

    $this->drush('situs-build', array(), array('root' => $root, 'make-file' => $make_file));

    // Test for some random stuff, just to make sure the make worked.
    $this->assertFileExists($root . '/index.php', 'Index is there.');
    $this->assertFileExists($root . '/modules/system/system.module', 'System module is there.');
    $this->assertFileExists($root . '/sites/all/modules/contrib/devel/devel.module', 'Devel is there.');
  }

  // function testDrushCommands() {
  //   $root = $this->webroot() . '/carl';
  //   $make_file = dirname(__FILE__) . '/simple.make';

  //   $this->drush('situs-build', array(), array('root' => $root, 'make-file' => $make_file, 'drush-pre-bulid' => array()));

  // }

  function testSaveFiles() {
    $root = $this->aliases['apu']['root'];
    $this->drush('situs-build', array('@apu'));

    // Test for some random stuff, just to make sure the make worked.
    $this->assertFileExists($root . '/index.php', 'Index is there.');
    $this->assertFileExists($root . '/modules/system/system.module', 'System module is there.');
    $this->assertFileExists($root . '/sites/all/modules/contrib/devel/devel.module', 'Devel is there.');

    // Create some fake sites.
    mkdir($root . '/sites/apu1');
    file_put_contents($root . '/sites/apu1/settings.php', '<?php');
    mkdir($root . '/sites/apu2');
    file_put_contents($root . '/sites/apu2/settings.php', '<?php');

    // Add some random files we want to save.
    file_put_contents($root . '/random1', 'random');
    file_put_contents($root . '/.random1', 'random');
    file_put_contents($root . '/profiles/random1', 'random');
    mkdir($root . '/testdir');
    mkdir($root . '/.testdir');

    // Also some files that should disappear..
    file_put_contents($root . '/random2', 'random');
    file_put_contents($root . '/profiles/random2', 'random');

    // Change make file so we have a change to check for.
    $this->aliases['apu']['make-file'] = dirname(__FILE__) . '/simple2.make';
    $this->saveAliases();
    $this->drush('situs-build', array('@apu'), array('save-files' => 'random1,.random1,profiles/random1,testdir,.testdir'));

    $this->assertFileExists($root . '/sites/all/modules/devel/devel.module', 'Devel is in new location.');

    $this->assertFileExists($root . '/sites/apu1/settings.php', 'Site has been moved.');
    $this->assertFileExists($root . '/sites/apu2/settings.php', 'Site has been moved.');

    // Check that the saved files were.
    $this->assertFileExists($root . '/random1', 'Saved file exists.');
    $this->assertFileExists($root . '/.random1', 'Saved dot file exists.');
    $this->assertFileExists($root . '/profiles/random1', 'Saved file in directory exists.');
    $this->assertFileExists($root . '/testdir', 'Saved directory exists.');
    $this->assertFileExists($root . '/.testdir', 'Saved dot directory exists.');

    // And the others wasn't.
    $this->assertFileNotExists($root . '/random2', 'Saved file exists.');
    $this->assertFileNotExists($root . '/profiles/random2', 'Saved file exists.');
  }
}
