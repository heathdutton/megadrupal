<?php
require_once(__DIR__ . '/../../lib/Drupal/smartling/Wrappers/SmartlingUtils.php');

use Drupal\smartling\Wrappers;

/**
 * @file
 * Tests for smartling.
 */

/**
 * SmartlingFileCleanTest.
 */
class SmartlingUtilsTest extends \PHPUnit_Framework_TestCase {

  /**
   * Test info.
   *
   * @return array
   *   Return test info.
   */
  public static function getInfo() {
    return array(
      'name' => 'Smartling clean file',
      'description' => 'Test Smartling file cleaning to avoid path traversal',
      'group' => 'Smartling UnitTests',
    );
  }

  /**
   * Test clean file.
   */
  public function testCleanFileName() {
    $utils = new Wrappers\SmartlingUtils(null, null);

    $filename = $utils->cleanFileName("");
    $this->assertEquals($filename, "", 'Path traversal test for empty filenames.');

    $filename = $utils->cleanFileName("./../asd.abc");
    $this->assertEquals($filename, "asd.abc", 'Path traversal test for: "./../asd.abc"');

    $filename = $utils->cleanFileName("./../asd.abc", TRUE);
    $this->assertEquals($filename, "_/__/asd.abc", 'Path traversal test for: "./../asd.abc" (path enabled)');

    $filename = $utils->cleanFileName("qwe.ert\n\n.pdf");
    $this->assertEquals($filename, "qwe_ert__.pdf", 'Path traversal test for: "qwe.ert\n\n.pdf"');

    $filename = $utils->cleanFileName("%u002e%u002e%u2215qwrtyu.htm");
    $this->assertEquals($filename, "_u002e_u002e_u2215qwrtyu.htm", 'Path traversal test for: "%u002e%u002e%u2215qwrtyu.htm"');

    $filename = $utils->cleanFileName("liuerg");
    $this->assertEquals($filename, "liuerg", 'Path traversal test for: "liuerg"');

    $filename = $utils->cleanFileName("\n\n");
    $this->assertEquals($filename, "", 'Path traversal test for: "\n\n"');

    $filename = $utils->cleanFileName("dir1/dir2/dir3/dir4", TRUE);
    $this->assertEquals($filename, "dir1/dir2/dir3/dir4", 'Path traversal test for: "dir1/dir2/dir3/dir4"');
  }

}
