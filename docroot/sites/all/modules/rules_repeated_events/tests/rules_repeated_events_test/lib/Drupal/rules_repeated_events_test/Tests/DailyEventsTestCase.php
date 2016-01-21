<?php

//namespace Drupal\rules_repeated_events\Plugin\Rules\Event {
//  // Override built-in functions in current namespace.
//  // @see - http://www.schmengler-se.de/en/2011/03/php-mocking-built-in-functions-like-time-in-unit-tests/
//  // @todo - What if time() appear in other places.
//  function time() {
//    return \Clock::$now ?: \time();
//  }
//}

namespace Drupal\rules_repeated_events_test\Tests {

  /**
   * Tests daily events rule.
   */
  class DailyEventsTestCase extends RepeatedEventsTestBase {

    public static function getInfo() {
      return static::getInfoDefaults() + array(
        'name' => 'Daily Events Tests',
        'description' => 'Tests rules on daily events.',
      );
    }

    /**
     * Tests that event is set too late.
     */
    function testLateEvent() {
      $this->createRule('-1 hour');

      drupal_cron_run();

      $count = count($this->drupalGetMails());
      $this->assertEqual(0, $count, 'No emails have been sent.');
    }

    /**
     * Tests that event is set early.
     */
    function testEarlyEvent() {
      $this->createRule('+1 hour');

      drupal_cron_run();

      $count = count($this->drupalGetMails());
      $this->assertEqual(0, $count, 'No emails have been sent.');

      // We passed the scheduled time.
      //\Clock::$now = strtotime('+1 hours 1 second');
      rules_repeated_events_test_fake_time('+1 hours 1 second');

      drupal_cron_run();

      $count = count($this->drupalGetMails());
      $this->assertEqual(1, $count, '1 email has been sent.');
    }

    /**
     * Tests that rule is re-scheduled to tomorrow.
     */
    function testRuleReScheduled() {
      $this->createRule('-1 hour');

      // We passed the scheduled time.
      //\Clock::$now = strtotime('+1 day');
      rules_repeated_events_test_fake_time('+1 day');

      drupal_cron_run();

      $count = count($this->drupalGetMails());
      $this->assertEqual(1, $count, '1 email has been sent.');
    }

    /**
     * Tests that only one event run at the same time.
     */
    function testOnlyOneEventRunAtTheSameTime() {
      $this->createRule();

      // We passed the scheduled time for a month.
      rules_repeated_events_test_fake_time('+1 month');

      drupal_cron_run();
      // Run cron again, just for sure that event doesn't run on next cron.
      drupal_cron_run();

      // Only 1 email has been sent.
      $count = count($this->drupalGetMails());
      $this->assertEqual(1, $count, 'Only 1 email has been sent.');
    }

    private function createRule($created_at = '') {
      if (empty($created_at) || !is_string($created_at)) {
        $timestamp = time();
      }
      else {
        $timestamp = strtotime($created_at);
      }

      // Create daily events rule.
      $rule = rules_reaction_rule();
      $rule->event('daily_events', array('time' => date('g', $timestamp) . ':' . date('i', $timestamp) . date('a', $timestamp)))
        ->action('repeated_events_mail_action');
      $rule->integrityCheck()->save('daily_events_rule');

      // Start the event watcher by clear the event cache.
      \RulesEventSet::rebuildEventCache();
    }
  }
}
