<?php
/**
 * @file
 * The actual tests for Cache Consistent.
 */

trait CacheConsistentTestHelper {

  /**
   * Simple cache tests.
   */
  public function cacheTest($cache_object) {
    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = NULL;

    $this->assertEqual($expect, $value, format_string('Expect "@expect" in buffer, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);

    $cache_object->set('test1', 'value0');
    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = 'value0';

    $this->assertEqual($expect, $value, format_string('Expect "@expect" in buffer, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
  }

  /**
   * Simple buffer tests.
   */
  public function bufferTest($cache_object, $expect) {
    $cache_object->set('test1', 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value1');

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;

    $this->assertEqual('value1', $value, format_string('Expect "@expect" in buffer, found "@data"', array(
      '@expect' => 'value1',
      '@data' => $value,
    )), $cache_object->testGroup);

    $tx->rollback();
    unset($tx);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;

    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
  }

  /**
   * Simple transaction test.
   */
  public function simpleTransactionTest($cache_object, $expect) {
    $cache_object->set('test1', 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value1');
    $tx->rollback();
    unset($tx);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;

    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
  }

  /**
   * Nested transaction tests.
   */
  public function nestedTransactionTest($cache_object, $expect) {
    $cache_object->set('test1', 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value1');
    $tx1 = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value2');
    $tx1->rollback();
    unset($tx1);
    unset($tx);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;

    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);

    $cache_object->set('test1', 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value1');
    $tx1 = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value3');
    $tx2 = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value2');
    unset($tx2);
    $tx1->rollback();
    unset($tx1);
    unset($tx);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;

    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
  }

  /**
   * Double transaction tests.
   */
  public function doubleTransactionTest($cache_object) {
    $tx = $this->db_transaction($cache_object);
    $cache_object->clear('test1');
    $cache_object->clear('test2');
    $cache_object->clear('test3');
    unset($tx);

    $tx = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value0');
    $cache = $cache_object->get('test2');
    unset($tx);

    $value = $cache ? $cache->data : NULL;
    $expect = NULL;
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
  }

  /**
   * Parallel transaction tests.
   */
  public function parallelTransactionTest($cache_object, $expect) {
    $cache_object->set('test1', 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value1');
    $tx1 = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value2');
    unset($tx1);
    $tx2 = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value3');
    $tx2->rollback();
    unset($tx2);
    unset($tx);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;

    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
  }

  /**
   * Out of order transaction tests.
   */
  public function outOfOrderTransactionTest($cache_object, $expect) {
    $cache_object->set('test1', 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value1');
    $tx1 = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value2');
    $tx2 = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value3');
    unset($tx);
    unset($tx1);
    unset($tx2);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;

    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
  }

  /**
   * Wildcard tests.
   */
  public function wildcardTest($cache_object) {
    $cache_object->set('test1', 'value0');
    $tx = $this->db_transaction($cache_object);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = 'value0';
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);

    $cache_object->clear('test', TRUE);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $this->assertTrue(empty($value), format_string('Cache was cleared for test*'), $cache_object->testGroup);

    $cache_object->set('test1', 'value1');

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = 'value1';
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
    unset($tx);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = 'value1';
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);

    $cache_object->set('test1', 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value1');
    $cache_object->clear('test', TRUE);
    $cache_object->clear('0', TRUE);
    $cache_object->clear('justsomeprefix', TRUE);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = NULL;
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
    unset($tx);

  }

  /**
   * Multiple clear tests.
   */
  public function multiClearTest($cache_object) {
    $cache_object->set('test1', 'value0');
    $cache_object->set('test2', 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->clear(array('test1', 'test2'));
    $cache_object->set('test1', 'value1');
    $cache_object->set('test2', 'value1');
    unset($tx);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = 'value1';
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);

    $cache = $cache_object->get('test2');
    $value = $cache ? $cache->data : NULL;
    $expect = 'value1';
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);

    $cache_object->set('test1', 'value0');
    $cache_object->set('test2', 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->clear(array('test1', 'test2'));
    $cache_object->set('test1', 'value1');
    $cache_object->set('test2', 'value1');
    $cache_object->clear(array('test1', 'test2'));
    unset($tx);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = NULL;
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);

    $cache = $cache_object->get('test2');
    $value = $cache ? $cache->data : NULL;
    $expect = NULL;
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
  }

  /**
   * Pruning tests.
   */
  public function pruneTest($cache_object) {
    $cache_object->set('test1', 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value1');
    $cache_object->set('test1', 'value2');
    $cache_object->clear('test1');
    $cache_object->set('test1', 'value3');
    $cache_object->clear('test1');
    $cache_object->set('test1', 'value4');
    $cache_object->clear('test', TRUE);
    $cache_object->set('test1', 'value5');
    $cache_object->getBuffer()->prune();
    $buffer = $this->cleanseBuffer($cache_object->getBuffer()->getBuffer());
    unset($tx);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = 'value5';

    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);

    $expect = array(
      array(1, 'clear', 'test', TRUE),
      array(1, 'set', 'test1', 'value5'),
    );
    $this->assertEqual($expect, $buffer, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => json_encode($expect),
      '@data' => json_encode($buffer),
    )), $cache_object->testGroup);

    $tx = $this->db_transaction($cache_object);
    $cache_object->set('test11', 'value1');
    $cache_object->clear('test11');
    $cache_object->set('test11', 'value3');
    $cache_object->set('test12', 'value1');
    $cache_object->set('test12', 'value2');
    $cache_object->clear('test12');
    $cache_object->set('test21', 'value1');
    $cache_object->set('test21', 'value2');
    $cache_object->set('test21', 'value3');
    $cache_object->clear('test1', TRUE);
    $cache_object->getBuffer()->prune();
    $buffer = $this->cleanseBuffer($cache_object->getBuffer()->getBuffer());
    $tx->rollback();
    unset($tx);

    $expect = array(
      array(1, 'set', 'test21', 'value3'),
      array(1, 'clear', 'test1', TRUE),
    );
    $this->assertEqual($expect, $buffer, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => json_encode($expect),
      '@data' => json_encode($buffer),
    )), $cache_object->testGroup);

    $cache_object->set('test1', 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->set('test1', 'value1');
    $cache_object->set('test1', 'value2');
    $cache_object->clear('test1');
    $cache_object->set('test1', 'value3');
    $cache_object->clear('test1');
    $cache_object->set('test1', 'value4');
    $cache_object->clear('test', TRUE);
    $cache_object->set('test1', 'value5');
    $buffer = $this->cleanseBuffer($cache_object->getBuffer()->getBuffer());
    unset($tx);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = 'value5';

    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);

    $expect = array(
      array(1, 'set', 'test1', 'value1'),
      array(1, 'set', 'test1', 'value2'),
      array(1, 'clear', 'test1', FALSE),
      array(1, 'set', 'test1', 'value3'),
      array(1, 'clear', 'test1', FALSE),
      array(1, 'set', 'test1', 'value4'),
      array(1, 'clear', 'test', TRUE),
      array(1, 'set', 'test1', 'value5'),
    );
    $this->assertEqual($expect, $buffer, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => json_encode($expect),
      '@data' => json_encode($buffer),
    )), $cache_object->testGroup);

    $tx = $this->db_transaction($cache_object);
    $cache_object->set('test11', 'value1');
    $cache_object->clear('test11');
    $cache_object->set('test11', 'value3');
    $cache_object->set('test12', 'value1');
    $cache_object->set('test12', 'value2');
    $cache_object->clear('test12');
    $cache_object->set('test21', 'value1');
    $cache_object->set('test21', 'value2');
    $cache_object->set('test21', 'value3');
    $cache_object->clear('test1', TRUE);
    $buffer = $this->cleanseBuffer($cache_object->getBuffer()->getBuffer());
    $tx->rollback();
    unset($tx);

    $expect = array(
      array(1, 'set', 'test11', 'value1'),
      array(1, 'clear', 'test11', FALSE),
      array(1, 'set', 'test11', 'value3'),
      array(1, 'set', 'test12', 'value1'),
      array(1, 'set', 'test12', 'value2'),
      array(1, 'clear', 'test12', FALSE),
      array(1, 'set', 'test21', 'value1'),
      array(1, 'set', 'test21', 'value2'),
      array(1, 'set', 'test21', 'value3'),
      array(1, 'clear', 'test1', TRUE),
    );
    $this->assertEqual($expect, $buffer, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => json_encode($expect),
      '@data' => json_encode($buffer),
    )), $cache_object->testGroup);

  }

  /**
   * Integer as cache id tests.
   */
  public function integerCacheIdTest($cache_object) {
    $cache_object->set(1234, 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->set(1234, 'value1');
    $cache_object->set(1234, 'value2');

    $cache = $cache_object->get(1234);
    $value = $cache ? $cache->data : NULL;
    $expect = 'value2';
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
    unset($tx);

    $cache_object->set(1234, 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->set(1234, 'value1');
    $cache_object->clear(5678, TRUE);

    $cache = $cache_object->get(1234);
    $value = $cache ? $cache->data : NULL;
    $expect = 'value1';
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
    unset($tx);

    $cache_object->set(1234, 'value0');
    $tx = $this->db_transaction($cache_object);
    $cache_object->set(1234, 'value1');
    // 49 is character '1'.
    $cache_object->clear(49, TRUE);

    $cache = $cache_object->get(1234);
    $value = $cache ? $cache->data : NULL;
    $expect = 'value1';
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
    unset($tx);
  }

  /**
   * Clear/flush tests.
   */
  public function clearAllTest($cache_object) {
    // Test flushing of temporary cache objects without a minimum cache
    // lifetime.
    $this->setVariable('cache_lifetime', 0);
    $tx = $this->db_transaction($cache_object);
    $cache_object->clear('test1');
    $cache_object->set('test1', 'value1', CACHE_TEMPORARY);
    $cache_object->clear(NULL);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = NULL;
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
    unset($tx);

    // Test flushing of permanent cache objects without a minimum cache
    // lifetime.
    $this->setVariable('cache_lifetime', 0);
    $tx = $this->db_transaction($cache_object);
    $cache_object->clear('test1');
    $cache_object->set('test1', 'value1', CACHE_PERMANENT);
    $cache_object->clear(NULL);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = 'value1';
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
    unset($tx);

    // Test cache bin truncation.
    $tx = $this->db_transaction($cache_object);
    $cache_object->clear('test1');
    $cache_object->set('test1', 'value1');
    $cache_object->clear('*', TRUE);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = NULL;
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
    unset($tx);

    // Test flushing of non-expired cache objects with a minimum cache lifetime.
    $this->setVariable('cache_lifetime', 60);
    $this->setVariable('cache_flush_' . $this->bin, REQUEST_TIME - 30);
    $tx = $this->db_transaction($cache_object);
    $cache_object->clear('test1');
    $cache_object->set('test1', 'value1', REQUEST_TIME - 30);
    $cache_object->clear(NULL);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = 'value1';
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
    unset($tx);

    // Test flushing of expired cache objects with a minimum cache lifetime.
    $this->setVariable('cache_lifetime', 60);
    $this->setVariable('cache_flush_' . $this->bin, REQUEST_TIME - 90);
    $tx = $this->db_transaction($cache_object);
    $cache_object->clear('test1');
    $cache_object->set('test1', 'value1', REQUEST_TIME - 30);
    $cache_object->clear(NULL);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = NULL;
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
    unset($tx);

    // Test flushing of non-expired cache objects with a minimum cache lifetime.
    $this->setVariable('cache_lifetime', 60);
    $this->setVariable('cache_flush_' . $this->bin, REQUEST_TIME - 30);
    $cache_object->clear('test1');
    $cache_object->set('test1', 'value1', REQUEST_TIME + 30);
    $tx = $this->db_transaction($cache_object);
    $cache_object->clear(NULL);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = 'value1';
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
    unset($tx);

    // Test flushing of expired cache objects with a minimum cache lifetime.
    $this->setVariable('cache_lifetime', 60);
    $this->setVariable('cache_flush_' . $this->bin, REQUEST_TIME - 90);
    $cache_object->clear('test1');
    $cache_object->set('test1', 'value1', REQUEST_TIME - 30);
    $tx = $this->db_transaction($cache_object);
    $cache_object->clear(NULL);

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = NULL;
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
    unset($tx);

    // Test flushing of expired cache objects with a minimum cache lifetime
    // and with buffer set.
    $this->setVariable('cache_lifetime', 60);
    $this->setVariable('cache_flush_' . $this->bin, REQUEST_TIME - 90);
    $cache_object->clear('test1');
    $cache_object->set('test1', 'value1', REQUEST_TIME - 30);
    $tx = $this->db_transaction($cache_object);
    $cache_object->clear(NULL);
    $cache_object->set('test1', 'value1');

    $cache = $cache_object->get('test1');
    $value = $cache ? $cache->data : NULL;
    $expect = 'value1';
    $this->assertEqual($expect, $value, format_string('Expect "@expect" in cache, found "@data"', array(
      '@expect' => $expect,
      '@data' => $value,
    )), $cache_object->testGroup);
    unset($tx);
  }

  /**
   * Clean up operations buffer for easy array comparison.
   */
  public function cleanseBuffer($buffer) {
    $cleansed = array();
    foreach ($buffer as $operation) {
      $cleansed[] = array_slice($operation, 0, 4);
    }
    return $cleansed;
  }

}
