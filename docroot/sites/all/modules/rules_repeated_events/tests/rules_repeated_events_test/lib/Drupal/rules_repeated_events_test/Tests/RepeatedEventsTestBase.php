<?php

namespace Drupal\rules_repeated_events_test\Tests;

/**
 * Tests per-user rules.
 */
class RepeatedEventsTestBase extends \DrupalWebTestCase {

  public static function getInfoDefaults() {
    return array(
      'name' => 'Rules Repeated Events tests',
      'description' => 'Tests rules on repeated events such as daily, weekly, monthly.',
      'group' => 'Rules',
    );
  }

  function setUp() {
    // Run tests with the install profile of the host install.
    $this->profile = variable_get('install_profile');
    parent::setUp('rules_repeated_events_test');
    $this->uid = $GLOBALS['user']->uid;
    \RulesLog::logger()->clear();
    variable_set('rules_debug_log', 1);
    //\Clock::$now = \time();
  }
}
