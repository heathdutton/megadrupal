<?php

namespace Drupal\zeitgeist\Tests;

use Drupal\zeitgeist\Block;

class BlockTest extends \DrupalUnitTestCase {

  public static function getInfo() {
    return array(
      'name' => 'Zeitgeist Block unit test',
      'description' => 'We want to assert that this PSR-0 test case is being discovered.',
      'group' => 'Zeitgeist',
    );
  }

  public function testInstance() {
    $deltas = array_keys(zeitgeist_block_info());
    $blocks = array();
    foreach ($deltas as $delta) {
      $blocks[$delta] = Block::instance($delta);
    }
    $first = reset($deltas);
    $repeated = Block::instance($first);
    $this->assertIdentical($blocks[$first], $repeated, "instance() returns the same block instance for the same delta.");

    $current = NULL;
    foreach ($deltas as $delta) {
      if (isset($current)) {
        $this->assertNotEqual($blocks[$delta], $current, "instance() returns different block instances for different deltas.");
      }
      $current = $blocks[$delta];
    }
  }
}
