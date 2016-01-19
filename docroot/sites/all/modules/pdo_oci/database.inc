<?php

/**
 * @file
 * Database interface code for Oracle database servers.
 */

require_once dirname(__FILE__) . '/tests/devel.inc';

/**
 * Empty string placeholder for Oracle.
 */
define('OCI_EMPTY_STRING_PLACEHOLDER', 'E#');

/**
 * Long identifier placeholder (prefix) for Oracle.
 */
define('OCI_LONG_IDENTIFIER_PLACEHOLDER', 'L#');

/**
 * Range query row number placeholder for Oracle.
 */
define('OCI_RANGE_ROWNUM_PLACEHOLDER', 'R#');

/**
 * Identifier max length limitation for Oracle.
 */
define('OCI_MAX_IDENTIFIER_LENGTH', 30);

/**
 * NVARCHAR2 max length limitation for Oracle.
 */
define('OCI_MAX_NVARCHAR2_LENGTH', 2000);

/**
 * NCHAR max length limitation for Oracle.
 */
define('OCI_MAX_NCHAR_LENGTH', 1000);

/**
 * NUMBER max length limitation for Oracle.
 */
define('OCI_MAX_NUMBER_LENGTH', 38);

/**
 * @ingroup database
 * @{
 */

include_once DRUPAL_ROOT . '/includes/database/prefetch.inc';

/**
 * Specific Oracle implementation of DatabaseConnection.
 */
class DatabaseConnection_oci extends DatabaseConnection {

  public function __construct(array $connection_options = array()) {
    // This driver defaults to transaction support, except if explicitly passed FALSE.
    $this->transactionSupport = !isset($connection_options['transactions']) || ($connection_options['transactions'] !== FALSE);

    // Transactional DDL is not available in Oracle.
    $this->transactionalDDLSupport = FALSE;

    $this->connectionOptions = $connection_options;

    $dsn = 'oci:dbname=//' . $connection_options['host'] . ':' . (empty($connection_options['port']) ? '1521' : $connection_options['port']) . '/' . $connection_options['database'];

    // Allow PDO options to be overridden.
    $connection_options += array(
      'pdo' => array(),
    );
    $connection_options['pdo'] += array(
      // Prepared statements are most effective for performance when queries
      // are recycled (used several times). However, if they are not re-used,
      // prepared statements become ineffecient. Since most of Drupal's
      // prepared queries are not re-used, it should be faster to emulate
      // the preparation than to actually ready statements for re-use. If in
      // doubt, reset to FALSE and measure performance.
      PDO::ATTR_EMULATE_PREPARES => TRUE,
      // Convert numeric values to strings when fetching.
      PDO::ATTR_STRINGIFY_FETCHES => TRUE,
      // Force column names to lower case.
      PDO::ATTR_CASE => PDO::CASE_LOWER,
      // Enable autocommit on every single statement.
      PDO::ATTR_AUTOCOMMIT => TRUE,
    );
    parent::__construct($dsn, $connection_options['username'], $connection_options['password'], $connection_options['pdo']);

    $connection_options += array(
      'init_commands' => array(),
    );
    $connection_options['init_commands'] += array(
      // Force cursor sharing to prevent unfair modules.
      // See http://www.dba-oracle.com/t_cursor_sharing_force.htm.
      "ALTER SESSION SET CURSOR_SHARING = FORCE",
      // Force decimal characters used in string representation of floats to
      // avoid casting problems.
      "ALTER SESSION SET NLS_NUMERIC_CHARACTERS = '.,'",
      // Force date format used in string representation of dates to avoid
      // casting problems.
      "ALTER SESSION SET NLS_DATE_FORMAT = 'YYYY-MM-DD'",
      // Force timestamp format used in string representation of time to avoid
      // casting problems.
      "ALTER SESSION SET NLS_TIMESTAMP_FORMAT = 'HH24:MI:SS'",
      // Force timestamp format used in string representation of datetime to
      // avoid casting problems.
      "ALTER SESSION SET NLS_TIMESTAMP_TZ_FORMAT = 'YYYY-MM-DD HH24:MI:SS'",
    );

    // Execute Oracle init_commands.
    foreach ($connection_options['init_commands'] as $init_command) {
      $this->exec($init_command);
    }
  }

  public function query($query, array $args = array(), $options = array()) {

    $options += $this->defaultOptions();

    try {
      if ($query instanceof DatabaseStatementInterface) {
        $stmt = $query;
        $stmt->execute(NULL, $options);
      }
      else {
        $this->expandArguments($query, $args);
        $stmt = $this->prepareQuery($query);
        $stmt->execute($args, $options);
      }

      switch ($options['return']) {
        case Database::RETURN_STATEMENT:
          return $stmt;
        case Database::RETURN_AFFECTED:
          return $stmt->rowCount();
        case Database::RETURN_INSERT_ID:
          return $this->lastInsertId($options['sequence_name']);
        case Database::RETURN_NULL:
          return;
        default:
          throw new PDOException('Invalid return directive: ' . $options['return']);
      }
    }
    catch (PDOException $e) {
      if ($options['throw_exception']) {
        // Add additional debug information.
        if ($query instanceof DatabaseStatementInterface) {
          $e->query_string = $stmt->getQueryString();
        }
        else {
          $e->query_string = $query;
        }
        if (!(isset($e->errorInfo) && is_array($e->errorInfo) && $e->errorInfo[1] == '1')) {
          $e->errorInfo[0] = '23000';
        }
        $e->args = $args;
        throw $e;
      }
      return NULL;
    }
  }

  public function prepare($query, $options = array()) {
    return new DatabaseStatement_oci($this, $query, $options);
  }

  public function PDOPrepare($query, array $options = array()) {
    return parent::prepare($query, $options);
  }

  public function prepareQuery($query) {
    // Only preform long identifier replacement when throw exception for
    // performance concern.
    $query = $this->prefixTables($query);
    $query = $this->escapeReservedWords($query);
    $query = $this->escapeEmptyString($query);
    return $this->prepare($query);
  }

  public function escapeForOracle($query) {
    $query = $this->escapeReservedWords($query);
    $query = $this->escapeEmptyString($query);
    $query = $this->escapeLongIdentifiers($query);
    return $query;
  }

  public function escapeReservedWords($query) {
    static $reserved_words = NULL;

    if ($reserved_words === NULL) {
      $reserved_words = implode('|', $this->getReservedWords());
    }

    $pattern = array(
      '/^(' . $reserved_words . ')$/',
      '/:(' . $reserved_words . ')([\)\.\s\=\,])/',
      '/([\(\.\s\=\,])(' . $reserved_words . ')([\)\.\s\=\,])/',
    );
    $replacement = array(
      '"$1"',
      ':$1$2',
      '$1"$2"$3',
    );
    return preg_replace($pattern, $replacement, $query);
  }

  public function escapeEmptyString($query) {
    $search = array(
      '',
      "''",
    );
    $replace = array(
      OCI_EMPTY_STRING_PLACEHOLDER,
      "'" . OCI_EMPTY_STRING_PLACEHOLDER . "'",
    );
    return str_replace($search, $replace, $query);
  }

  public function escapeLongIdentifiers($query) {
    $pattern = array(
      '/[a-z0-9_]{' . (OCI_MAX_IDENTIFIER_LENGTH + 1) . ',}/',
      '/^[a-z0-9_]{' . (OCI_MAX_IDENTIFIER_LENGTH + 1) . ',}$/',
    );
    return preg_replace_callback($pattern, array($this, 'escapeLongIdentifier'), $query);
  }

  public function escapeLongIdentifier($matches) {
    global $conf;

    // Do we have the mapping?
    if (!isset($conf['long_identifiers'][$matches[0]])) {
      // Generate the mapping with prefix.
      $shorthand = OCI_LONG_IDENTIFIER_PLACEHOLDER . crc32($matches[0]);
      $conf['long_identifiers'][$matches[0]] = $shorthand;

      // We put temporary mapping for simpletest into separate file.
      if (strpos($matches[0], 'simpletest') === 0) {
        $settings_file = sys_get_temp_dir() . '/' . preg_replace('/^(simpletest[0-9]*).*$/i', '$1', $matches[0]) . '.tmp';
        if (!file_exists($settings_file)) {
          $fh = fopen($settings_file, 'w') or die ("Can't create file.");
          fclose($fh);
        }
      }
      else {
        $settings_file = DRUPAL_ROOT . '/' . conf_path() . '/settings.php';
      }

      // Save the long identifiers mapping to settings.php.
      chmod($settings_file, 0666) or die ("Can't chmod($settings_file, 0666");
      $fh = fopen($settings_file, 'a') or die ("Can't open file.");
      fwrite($fh, "\$conf['long_identifiers']['$matches[0]'] = '$shorthand';\n");
      fclose($fh);
      chmod($settings_file, 0444) or die ("Can't chmod($settings_file, 0444");
    }

    return $conf['long_identifiers'][$matches[0]];
  }

  public function restoreForOracle($data) {
    global $conf;

    // Always update $shorthand from $conf for latest version.
    $shorthand = array_flip(empty($conf['long_identifiers']) ? array() : $conf['long_identifiers']);

    // Preserve with array handling.
    if (is_array($data)) {
      foreach ($data as $key => $value) {
        // Remove the range query row number placeholder.
        if (strtoupper($key) === OCI_RANGE_ROWNUM_PLACEHOLDER) {
          unset($data[$key]);
        }
        // Column name is present as shorthand.
        elseif (strpos($key, OCI_LONG_IDENTIFIER_PLACEHOLDER) === 0 && isset($shorthand[$key])) {
          $data[$shorthand[$key]] = $this->restoreForOracle($value);
          unset($data[$key]);
        }
        else {
          $data[$key] = $this->restoreForOracle($value);
        }
      }
    }
    // Preserve with object handling.
    elseif (is_object($data)) {
      foreach ($data as $key => $value) {
        // Remove the range query row number placeholder.
        if (strtoupper($key) === OCI_RANGE_ROWNUM_PLACEHOLDER) {
          unset($data->{$key});
        }
        // Column name is present as shorthand.
        elseif (strpos($key, OCI_LONG_IDENTIFIER_PLACEHOLDER) === 0 && isset($shorthand[$key])) {
          $data->{$shorthand[$key]} = $this->restoreForOracle($value);
          unset($data->{$key});
        }
        else {
          $data->{$key} = $this->restoreForOracle($value);
        }
      }
    }
    // Restore string value.
    elseif (is_string($data)) {
      if ($data === OCI_EMPTY_STRING_PLACEHOLDER) {
        return '';
      }
      elseif (strpos($data, OCI_LONG_IDENTIFIER_PLACEHOLDER) === 0 && isset($shorthand[$data])) {
        return $shorthand[$data];
      }
    }
    // LOBs greater than 4000 bytes are always stored externally.
    // pdo_oci return bytea as stream, so we just need to use Streams API.
    // Check http://bugs.php.net/bug.php?id=37124 for more information.
    elseif (is_resource($data)) {
      if ($content = stream_get_contents($data) !== FALSE) {
        return $content;
      }
    }

    return $data;
  }

  public function queryRange($query, $from, $count, array $args = array(), array $options = array()) {
    return $this->query('SELECT * FROM (SELECT SUB1.*, ROWNUM AS ' . OCI_RANGE_ROWNUM_PLACEHOLDER . ' FROM (' . $query . ') SUB1) WHERE ' . OCI_RANGE_ROWNUM_PLACEHOLDER . ' BETWEEN ' . ($from + 1) . ' AND ' . ($from + $count), $args, $options);
  }

  public function queryTemporary($query, array $args = array(), array $options = array()) {
    $tablename = $this->generateTemporaryTableName();

    try {
      $this->query('DROP TABLE {' . $tablename . '}');
    }
    catch (Exception $e) {
      // No action required.
    }

    $this->query(preg_replace('/^SELECT/i', 'CREATE GLOBAL TEMPORARY TABLE {' . $tablename . '} ON COMMIT PRESERVE ROWS AS SELECT', $query), $args, $options);
    return $tablename;
  }

  public function driver() {
    return 'oci';
  }

  public function databaseType() {
    return 'oci';
  }

  public function mapConditionOperator($operator) {
    static $specials;

    // Function calls not allowed in static declarations, thus this method.
    if (!isset($specials)) {
      $specials = array(
        'IFNULL' => array('operator' => 'NVL'),
        'LIKE' => array('postfix' => " ESCAPE '\\'"),
        'NOT LIKE' => array('postfix' => " ESCAPE '\\'"),
      );
    }

    return isset($specials[$operator]) ? $specials[$operator] : NULL;
  }

  /**
   * Retrive a next id in a sequence.
   *
   * Oracle has built in sequence. We'll use these instead of inserting
   * and updating a sequences table.
   */
  public function nextId($existing_id = 0) {
    // Retrive the name of the sequence. This information cannot be cached
    // because the prefix may change, for example, like it does in simpletests.
    $sequence_name = $this->makeSequenceName('sequences', 'value');
    $sequence_name = $this->escapeForOracle($sequence_name);

    // Don't use {} around user_sequences table.
    if ($this->query("SELECT 1 FROM USER_SEQUENCES WHERE LOWER(SEQUENCE_NAME) = :sequence_name", array(':sequence_name' => strtolower($sequence_name)))->fetchField()) {
      // When Oracle gets a value too small then it will lock the table,
      // retry the INSERT and if is's still too small than alter the sequence.
      $id = $this->query('SELECT ' . $sequence_name . '.NEXTVAL FROM DUAL')->fetchField();
      if ($id > $existing_id) {
        return $id;
      }

      // Reset the sequence to a higher value than the existing id.
      $this->query('DROP SEQUENCE ' . $sequence_name);
    }

    $this->query('CREATE SEQUENCE ' . $sequence_name. ' MINVALUE 1 INCREMENT BY 1 START WITH ' . ($existing_id + 1) . ' NOCACHE NOORDER NOCYCLE');

    // Retrive the next id. We know this will be as high as we want it.
    $id = $this->query('SELECT '. $sequence_name . '.NEXTVAL FROM DUAL')->fetchField();

    return $id;
  }

  public function lastInsertId($name = NULL) {
    if (!$name) {
      throw new Exception('The name of the sequence is mandatory for Oracle.');
    }

    try {
      return $this->query('SELECT ' . $name . '.CURRVAL FROM DUAL')->fetchField();
    }
    catch (PDOException $e) {
      // Ignore ORA-08002: sequence $name.CURRVAL is not yet defined in this session.
      if (!(isset($e->errorInfo) && is_array($e->errorInfo) && $e->errorInfo[1] == '8002')) {
        throw $e;
      }
    }
  }

  public function rollback($savepoint_name = 'drupal_transaction') {
    if (!$this->supportsTransactions()) {
      return;
    }
    if (!$this->inTransaction()) {
      throw new DatabaseTransactionNoActiveException();
    }
    // A previous rollback to an earlier savepoint may mean that the savepoint
    // in question has already been accidentally committed.
    if (!isset($this->transactionLayers[$savepoint_name])) {
      throw new DatabaseTransactionNoActiveException();
    }

    // We need to find the point we're rolling back to, all other savepoints
    // before are no longer needed. If we rolled back other active savepoints,
    // we need to throw an exception.
    $rolled_back_other_active_savepoints = FALSE;
    while ($savepoint = array_pop($this->transactionLayers)) {
      if ($savepoint == $savepoint_name) {
        // If it is the last the transaction in the stack, then it is not a
        // savepoint, it is the transaction itself so we will need to roll back
        // the transaction rather than a savepoint.
        if (empty($this->transactionLayers)) {
          break;
        }
        $this->query('ROLLBACK TO SAVEPOINT ' . $savepoint);
        $this->popCommittableTransactions();
        if ($rolled_back_other_active_savepoints) {
          throw new DatabaseTransactionOutOfOrderException();
        }
        return;
      }
      else {
        $rolled_back_other_active_savepoints = TRUE;
      }
    }

    // Just use raw SQL instead of PDO::rollBack() to prevent PHP bugs.
    $this->query('ROLLBACK');
    // Ensure we are back to autocommit mode after transaction.
    $this->setAttribute(PDO::ATTR_AUTOCOMMIT, TRUE);

    if ($rolled_back_other_active_savepoints) {
      throw new DatabaseTransactionOutOfOrderException();
    }
  }

  public function pushTransaction($name) {
    if (!$this->supportsTransactions()) {
      return;
    }
    if (isset($this->transactionLayers[$name])) {
      throw new DatabaseTransactionNameNonUniqueException($name . " is already in use.");
    }
    // If we're already in a transaction then we want to create a savepoint
    // rather than try to create another transaction.
    if ($this->inTransaction()) {
      $this->query('SAVEPOINT ' . $name);
    }
    else {
      // Ensure we are NOT in autocommit mode before transaction start.
      $this->setAttribute(PDO::ATTR_AUTOCOMMIT, FALSE);
      // Just use raw SQL instead of PDO::beginTransaction() to prevent PHP bugs.
      $this->query('COMMIT');
    }
    $this->transactionLayers[$name] = $name;
  }

  public function popTransaction($name) {
    if (!$this->supportsTransactions()) {
      return;
    }
    // The transaction has already been committed earlier. There is nothing we
    // need to do. If this transaction was part of an earlier out-of-order
    // rollback, an exception would already have been thrown by
    // Database::rollback().
    if (!isset($this->transactionLayers[$name])) {
      return;
    }

    // Mark this layer as committable.
    $this->transactionLayers[$name] = FALSE;
    $this->popCommittableTransactions();
  }

  protected function popCommittableTransactions() {
    // Commit all the committable layers.
    foreach (array_reverse($this->transactionLayers) as $name => $active) {
      // Stop once we found an active transaction.
      if ($active) {
        break;
      }

      // If there are no more layers left then we should commit.
      unset($this->transactionLayers[$name]);
      if (empty($this->transactionLayers)) {
        try {
          // Use raw SQL instead of PDO::commit() to prevent PHP bugs.
          if (!$this->query('COMMIT')) {
            throw new DatabaseTransactionCommitFailedException();
          }
          // Ensure we are back to autocommit mode after transaction.
          $this->setAttribute(PDO::ATTR_AUTOCOMMIT, TRUE);
        }
        catch (Exception $e) {
          // Ignore ORA-22297: warning: Open LOBs exist at transaction commit time.
          if (!(isset($e->errorInfo) && is_array($e->errorInfo) && $e->errorInfo[1] == '22297')) {
            throw $e;
          }
        }
      }
    }
  }

  /**
   * Returns an array of reserved words (lowercase) for this DB.
   *
   * This file contains the reserved words for Oracle databases.
   * See http://docs.oracle.com/cd/E11882_01/server.112/e26088/ap_keywd001.htm
   */
  public function getReservedWords() {
    $reserved_words = array (
      'access', 'add', 'all', 'alter', 'and', 'any', 'as', 'asc', 'audit',
      'between', 'by', 'char', 'check', 'cluster', 'column', 'comment',
      'compress', 'connect', 'create', 'current', 'date', 'decimal',
      'default', 'delete', 'desc', 'distinct', 'drop', 'else', 'exclusive',
      'exists', 'file', 'float', 'for', 'from', 'grant', 'group', 'having',
      'identified', 'immediate', 'in', 'increment', 'index', 'initial',
      'insert', 'integer', 'intersect', 'into', 'is', 'level', 'like', 'lock',
      'long', 'maxextents', 'minus', 'mlslabel', 'mode', 'modify', 'nchar',
      'nclob', 'noaudit', 'nocompress', 'not', 'nowait', 'null', 'number',
      'nvarchar2', 'of', 'offline', 'on', 'online', 'option', 'or', 'order',
      'pctfree', 'prior', 'privileges', 'public', 'raw', 'rename', 'resource',
      'revoke', 'row', 'rowid', 'rownum', 'rows', 'select', 'session', 'set',
      'share', 'size', 'smallint', 'start', 'successful', 'synonym',
      'sysdate', 'table', 'then', 'to', 'trigger', 'uid', 'union', 'unique',
      'update', 'user', 'validate', 'values', 'varchar', 'varchar2', 'view',
      'whenever', 'where', 'with'
    );
    return $reserved_words;
  }
}

class DatabaseStatement_oci extends DatabaseStatementPrefetch implements Iterator, DatabaseStatementInterface {

  public $queryString;

  public function __construct(DatabaseConnection $connection, $query, array $driver_options = array()) {
    $this->dbh = $connection;
    $this->queryString = $query;
    $this->driverOptions = $driver_options;
  }

  public function execute($args = array(), $options = array()) {
    if (isset($options['fetch'])) {
      if (is_string($options['fetch'])) {
        // Default to an object. Note: db fields will be added to the object
        // before the constructor is run. If you need to assign fields after
        // the constructor is run, see http://drupal.org/node/315092.
        $this->setFetchMode(PDO::FETCH_CLASS, $options['fetch']);
      }
      else {
        $this->setFetchMode($options['fetch']);
      }
    }

    // Prepare the query.
    $statement = $this->getStatement($this->queryString, $args);
    if (!$statement) {
      $this->throwPDOException();
    }

    // Escape all empty string input.
    if (is_array($args)) {
      foreach ($args as $key => $value) {
        $args[$key] = $value === '' ? OCI_EMPTY_STRING_PLACEHOLDER : $value;
      }
    }

    $return = FALSE;
    try {
      // $statement->execute() will handle query logging so we don't
      // duplicate log within this function.
      $return = $statement->execute($args);
      if (!$return) {
        $this->throwPDOException();
      }
    }
    catch (PDOException $e) {
      // Long identifier replacement is expensive especially if execute
      // for all queries. Therefore just preform during exception be thrown.
      if (isset($e->errorInfo) && is_array($e->errorInfo) && $e->errorInfo[1] == '00972' && !isset($options['ORA-00972'])) {
        $options['ORA-00972'] = TRUE;
        $this->queryString = $this->dbh->escapeLongIdentifiers($this->queryString);
        return $this->execute($args, $options);
      }
    }

    try {
      // Fetch all the data from the reply, in order to release any lock
      // as soon as possible.
      $this->rowCount = $statement->rowCount();
      $this->data = $statement->fetchAll(PDO::FETCH_ASSOC);
      // Destroy the statement as soon as possible. See
      // DatabaseConnection_sqlite::PDOPrepare() for explanation.
      unset($statement);
      $this->resultRowCount = count($this->data);
    }
    catch (PDOException $e) {
      // Ignore ORA-24374: define not done before fetch or execute and fetch.
      if (!(isset($e->errorInfo) && is_array($e->errorInfo) && $e->errorInfo[1] == '24374')) {
        throw $e;
      }
    }

    if ($this->resultRowCount) {
      $this->columnNames = array_keys($this->data[0]);
    }
    else {
      $this->columnNames = array();
    }

    // Initialize the first row in $this->currentRow.
    $this->next();

    return $return;
  }

  public function getStatement($query, &$args = array()) {
    return $this->dbh->PDOPrepare($this->queryString);
  }

  public function current() {
    return $this->dbh->restoreForOracle(parent::current());
  }

  public function next() {
    return $this->dbh->restoreForOracle(parent::next());
  }

  public function fetch($fetch_style = NULL, $cursor_orientation = PDO::FETCH_ORI_NEXT, $cursor_offset = 0) {
    return $this->dbh->restoreForOracle(parent::fetch($fetch_style, $cursor_orientation, $cursor_offset));
  }

  public function fetchField($index = 0) {
    return $this->dbh->restoreForOracle(parent::fetchField($index));
  }

  public function fetchObject($class_name = NULL, $ctor_args = array()) {
    return $this->dbh->restoreForOracle(parent::fetchObject($class_name, $ctor_args));
  }

  public function fetchAssoc() {
    return $this->dbh->restoreForOracle(parent::fetchAssoc());
  }

  public function fetchAll($fetch_style = NULL, $fetch_argument = NULL, $ctor_args = array()) {
    return $this->dbh->restoreForOracle(parent::fetchAll($fetch_style, $fetch_argument, $ctor_args));
  }

  public function fetchCol($index = 0) {
    return $this->dbh->restoreForOracle(parent::fetchCol($index));
  }

  public function fetchAllKeyed($key_index = 0, $value_index = 1) {
    return $this->dbh->restoreForOracle(parent::fetchAllKeyed($key_index, $value_index));
  }

  public function fetchAllAssoc($key, $fetch = NULL) {
    return $this->dbh->restoreForOracle(parent::fetchAllAssoc($key, $fetch));
  }
}

/**
 * @} End of "ingroup database".
 */
