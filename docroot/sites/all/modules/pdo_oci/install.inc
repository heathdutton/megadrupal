<?php

/**
 * @file
 * Installation code for Oracle embedded database engine.
 */

/**
 * Specifies installation tasks for Oracle and equivalent databases.
 */
class DatabaseTasks_oci extends DatabaseTasks {
  /**
   * The PDO driver name for Oracle and equivalent databases.
   *
   * @var string
   */
  protected $pdoDriver = 'oci';

  public function __construct() {
    $this->tasks[] = array(
      'function' => 'initializeDatabase',
      'arguments' => array(),
    );
  }

  /**
   * Returns a human-readable name string for Oracle and equivalent databases.
   */
  public function name() {
    return st('Oracle');
  }

  /**
   * Returns the minimum version for Oracle.
   */
  public function minimumVersion() {
    return '11.2.0.1.0';
  }

  /**
   * Make Oracle Drupal friendly.
   */
  public function initializeDatabase() {
    try {
      db_query('CREATE OR REPLACE
        FUNCTION greatest
          (
            p1 NUMBER,
            p2 NUMBER,
            p3 NUMBER DEFAULT NULL)
          RETURN NUMBER
        AS
        BEGIN
          IF p3 IS NULL THEN
            IF p1 > p2 OR p2 IS NULL THEN
              RETURN p1;
            ELSE
              RETURN p2;
            END IF;
          ELSE
            RETURN greatest(p1, greatest(p2, p3));
          END IF;
        END;');
      db_query('CREATE OR REPLACE
        FUNCTION substring
          (
            p_s NVARCHAR2,
            p_start NUMBER,
            p_length NUMBER DEFAULT NULL)
          RETURN NVARCHAR2
        AS
        BEGIN
          IF p_length IS NULL THEN
            RETURN SUBSTR(p_s, p_start);
          ELSE
            RETURN SUBSTR(p_s, p_start, p_length);
          END IF;
        END;');
      db_query('CREATE OR REPLACE
        FUNCTION substring_index
          (
            p_buff NVARCHAR2,
            p_delimiter NVARCHAR2,
            p_count NUMBER)
          RETURN NVARCHAR2
        AS
          v_end NUMBER:= 1;
        BEGIN
          IF p_count > 0 THEN
            FOR i IN 1..p_count
            LOOP
              v_end := instr(p_buff, p_delimiter, v_end + 1);
              IF v_end = 0 THEN
                v_end:= LENGTH(p_buff);
              END IF;
            END LOOP;
            RETURN SUBSTR(p_buff, 1, v_end - 1);
          ELSE
            v_end := LENGTH(p_buff);
            FOR i IN 1..(p_count* - 1)
            LOOP
              v_end := instr(p_buff, p_delimiter, (LENGTH(p_buff) - (v_end - 2))* - 1);
              IF v_end = 0 THEN
                v_end:= LENGTH(p_buff);
              END IF;
            END LOOP;
            RETURN SUBSTR(p_buff, v_end + 1);
          END IF;
        END;');
      db_query('CREATE OR REPLACE
        FUNCTION rand
          RETURN NUMBER
        AS
        BEGIN
          RETURN ABS(MOD(DBMS_RANDOM.RANDOM,10000001)/10000000);
        END;');
    }
    catch (Exception $e) {
      $this->fail(st('Drupal could not be correctly setup with the existing database. Revise any errors.'));
    }
  }
}
