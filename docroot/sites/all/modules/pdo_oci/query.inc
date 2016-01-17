<?php

/**
 * @ingroup database
 * @{
 */

/**
 * @file
 * Query code for Oracle embedded database engine.
 */

class InsertQuery_oci extends InsertQuery {

  protected $schema = array();

  public function __construct($connection, $table, array $options = array()) {
    parent::__construct($connection, $table, $options);

    // Fetch the list of blobs and sequences used on that table.
    $schema = $this->connection->schema()->queryTableInformation($table);
    $this->schema = isset($schema['fields']) ? $schema : array('fields' => array());
  }

  public function execute() {
    if (!$this->preExecute()) {
      return NULL;
    }

    // Oracle requires the table name to be specified explicitly
    // when requesting the last insert ID, so we pass that in via
    // the options array.
    $options = $this->queryOptions;

    $options['sequence_name'] = NULL;
    foreach ($this->schema['fields'] as $field => $value) {
      if ($value['type'] == 'serial') {
        $options['sequence_name'] = $this->connection->makeSequenceName($this->table, $field);
        break;
      }
    }
    // If there are no sequences then we can't get a last insert id.
    if ($options['sequence_name'] == NULL && $options['return'] == Database::RETURN_INSERT_ID) {
      $options['return'] = Database::RETURN_NULL;
    }

    // Each insert happens in its own query in the degenerate case. However,
    // we wrap it in a transaction so that it is atomic where possible. On many
    // databases, such as Oracle, this is also a notable performance boost.
    $transaction = $this->connection->startTransaction();

    $stmt = $this->connection->prepareQuery((string) $this)->getStatement(NULL);

    try {
      $blobs = array();
      $blob_count = 0;
      foreach ($this->insertValues as $insert_values) {

        $max_placeholder = 0;
        foreach ($this->insertFields as $idx => $field) {
          if (isset($this->schema['fields'][$field]) && $this->schema['fields'][$field]['type'] == 'blob') {
            if (!is_null($insert_values[$idx])) {
              // Writing NULL into file handler means write nothing. The file
              // handler will contain an empty string but not NULL. Therefore
              // NULL will be translated as empty string when save into
              // database, and so buggy.
              $blobs[$blob_count] = fopen('php://memory', 'a');
              fwrite($blobs[$blob_count], $insert_values[$idx]);
              rewind($blobs[$blob_count]);

              $stmt->bindParam(':db_insert_placeholder_' . $max_placeholder++, $blobs[$blob_count], PDO::PARAM_LOB);

              // Pre-increment is faster in PHP than increment.
              ++$blob_count;
            }
            else {
              // BLOB NULL is a bit different from normal case: we can't use
              // StreamAPI, but bind NULL to target field direcly.
              $stmt->bindParam(':db_insert_placeholder_' . $max_placeholder++, $insert_values[$idx], PDO::PARAM_LOB);
            }
          }
          elseif ($insert_values[$idx] === '') {
            $stmt->bindValue(':db_insert_placeholder_' . $max_placeholder++, OCI_EMPTY_STRING_PLACEHOLDER);
          }
          else {
            $stmt->bindParam(':db_insert_placeholder_' . $max_placeholder++, $insert_values[$idx]);
          }
        }

        // Check if values for a serial field has been passed.
        foreach ($this->schema['fields'] as $field => $value) {
          if ($value['type'] == 'serial') {
            $key = array_search($field, $this->insertFields);
            if ($key !== FALSE) {
              $last_insert_id = $insert_values[$key] > 1 ? $insert_values[$key] : 1;
              // Set the sequence to the bigger value of either the passed
              // value or the max value of the column. It can happen that another
              // thread calls nextval() which could lead to a serial number being
              // used twice. However, trying to insert a value into a serial
              // column should only be done in very rare cases and is not thread
              // safe by definition.
              $current_value = $this->connection->lastInsertId($options['sequence_name']);
              $this->connection->query('DROP SEQUENCE ' . $options['sequence_name']);
              $this->connection->query('CREATE SEQUENCE ' . $options['sequence_name'] . ' MINVALUE 1 INCREMENT BY 1 START WITH ' . max($last_insert_id, $current_value) . ' NOCACHE NOORDER NOCYCLE');
            }
          }
        }

        // Only use the returned last_insert_id if it is not already set.
        $last_insert_id = $this->connection->query($stmt, array(), $options);
      }

      if (!empty($this->fromQuery)) {
        // bindParam stores only a reference to the variable that is followed when
        // the statement is executed. We pass $arguments[$key] instead of $value
        // because the second argument to bindParam is passed by reference and
        // the foreach statement assigns the element to the existing reference.
        $arguments = $this->fromQuery->getArguments();
        foreach ($arguments as $key => $value) {
          if ($arguments[$key] === '') {
            $stmt->bindValue($key, OCI_EMPTY_STRING_PLACEHOLDER);
          }
          else {
            $stmt->bindParam($key, $arguments[$key]);
          }
        }
      }

      // All default query.
      if (count($this->insertValues) == 0) {
        // Only use the returned last_insert_id if it is not already set.
        $last_insert_id = $this->connection->query($stmt, array(), $options);
      }
    }
    catch (Exception $e) {
      // One of the INSERTs failed, rollback the whole batch.
      $transaction->rollback();
      // Rethrow the exception for the calling code.
      throw $e;
    }

    // Re-initialize the values array so that we can re-use this query.
    $this->insertValues = array();

    // Transaction commits here where $transaction looses scope.

    return $last_insert_id;
  }

  public function __toString() {
    // Create a sanitized comment string to prepend to the query.
    $comments = $this->connection->makeComment($this->comments);

    // Prefix and escape table name manually.
    $table_name = $this->connection->prefixTables('{' . $this->table . '}');
    $table_name = $this->connection->escapeForOracle($table_name);

    // Default fields are always placed first for consistency.
    $insert_fields = array_merge($this->defaultFields, $this->insertFields);
    foreach ($insert_fields as $key => $value) {
      $insert_fields[$key] = $this->connection->escapeForOracle($value);
    }

    // If we're selecting from a SelectQuery, finish building the query and
    // pass it back, as any remaining options are irrelevant.
    if (!empty($this->fromQuery)) {
      $query = $comments . 'INSERT INTO ' . $table_name . ' (' . implode(', ', $insert_fields) . ') ' . $this->fromQuery;
      $query = $this->connection->prefixTables($query);
      $query = $this->connection->escapeForOracle($query);
      return $query;
    }

    $query = $comments . 'INSERT INTO ' . $table_name . ' (' . implode(', ', $insert_fields) . ') VALUES ';

    $blob_fields = array();
    $blob_placeholders = array();
    if (count($this->insertValues)) {
      $placeholders = array();

      // Default fields aren't really placeholders, but this is the most convenient
      // way to handle them.
      $placeholders = array_pad($placeholders, count($this->defaultFields), 'DEFAULT');

      $i = 0;
      foreach ($this->insertFields as $field) {
        if (isset($this->schema['fields'][$field]) && $this->schema['fields'][$field]['type'] == 'blob') {
          // BLOB field insert in Oracle required additional handling with
          // EMPTY_BLOB() pleaseholder + RETURNING INTO (see below).
          $placeholders[] = 'EMPTY_BLOB()';
          $blob_fields[] = $this->connection->escapeForOracle($field);
          $blob_placeholders[] = ':db_insert_placeholder_' . $i;
        }
        else {
          $placeholders[] = ':db_insert_placeholder_' . $i;
        }
        ++$i;
      }
      $query .= '(' . implode(', ', $placeholders) . ')';
    }
    else {
      // If there are no values, then this is a default-only query. We still need to handle that.
      $placeholders = array_fill(0, count($this->defaultFields), 'DEFAULT');
      $query .= '(' . implode(', ', $placeholders) . ')';
    }

    // Oracle BLOB additional handling with RETURNING INTO.
    if (count($blob_fields)) {
      $query .= ' RETURNING ' . implode(', ', $blob_fields) . ' INTO ' . implode (', ', $blob_placeholders);
    }

    return $query;
  }
}

class TruncateQuery_oci extends TruncateQuery {

  public function __toString() {
    // Create a sanitized comment string to prepend to the query.
    $comments = $this->connection->makeComment($this->comments);

    return $comments . 'TRUNCATE TABLE {' . $this->table . '} ';
  }
}

class UpdateQuery_oci extends UpdateQuery {

  protected $schema = array();

  public function __construct(DatabaseConnection $connection, $table, array $options = array()) {
    parent::__construct($connection, $table, $options);

    // Fetch the list of blobs and sequences used on that table.
    $schema = $this->connection->schema()->queryTableInformation($table);
    $this->schema = isset($schema['fields']) ? $schema : array('fields' => array());
  }

  protected function removeFieldsInCondition(&$fields, QueryConditionInterface $condition) {
    foreach ($condition->conditions() as $child_condition) {
      if (isset($child_condition['field']) && $child_condition['field'] instanceof QueryConditionInterface) {
        $this->removeFieldsInCondition($fields, $child_condition['field']);
      }
      elseif (isset($child_condition['field'])) {
        unset($fields[$child_condition['field']]);
      }
    }
  }

  public function execute() {
    $max_placeholder = 0;
    $blobs = array();
    $blob_count = 0;

    // Oracle requires the table name to be specified explicitly
    // when requesting the last insert ID, so we pass that in via
    // the options array.
    $options = $this->queryOptions;
    $options['already_prepared'] = TRUE;

    // Oracle counts all the rows that match the conditions as modified, even if they
    // will not be affected by the query. We workaround this by ensuring that
    // we don't select those rows.
    if (empty($this->queryOptions['oci_return_matched_rows'])) {
      // Get the fields used in the update query, and remove those that are already
      // in the condition.
      $fields = $this->expressionFields + $this->fields;
      $this->removeFieldsInCondition($fields, $this->condition);

      // Add the inverse of the fields to the condition.
      $condition = new DatabaseCondition('OR');
      foreach ($fields as $field => $data) {
        if (!(isset($this->schema['fields'][$field]) && $this->schema['fields'][$field]['type'] == 'blob')) {
          if (is_array($data)) {
            // The field is an expression.
            $condition->where($field . ' <> ' . $data['expression']);
            $condition->isNull($field);
          }
          elseif (!isset($data)) {
            // The field will be set to NULL.
            $condition->isNotNull($field);
          }
          else {
            $condition->condition($field, $data, '<>');
            $condition->isNull($field);
          }
        }
      }
      if (count($condition)) {
        $condition->compile($this->connection, $this);
        $this->condition->where((string) $condition, $condition->arguments());
      }
    }

    // Each insert happens in its own query in the degenerate case. However,
    // we wrap it in a transaction so that it is atomic where possible. On many
    // databases, such as Oracle, this is also a notable performance boost.
    $transaction = $this->connection->startTransaction();

    // Because we filter $fields the same way here and in __toString(), the
    // placeholders will all match up properly.
    $stmt = $this->connection->prepareQuery((string) $this)->getStatement(NULL);

    // Expressions take priority over literal fields, so we process those first
    // and remove any literal fields that conflict.
    $fields = $this->fields;
    foreach ($this->expressionFields as $field => $data) {
      if (!empty($data['arguments'])) {
        foreach ($data['arguments'] as $placeholder => $argument) {
          // We assume that an expression will never happen on a BLOB field,
          // which is a fairly safe assumption to make since in most cases
          // it would be an invalid query anyway.
          if ($data['arguments'][$placeholder] === '') {
            $stmt->bindValue($placeholder, OCI_EMPTY_STRING_PLACEHOLDER);
          }
          else {
            $stmt->bindParam($placeholder, $data['arguments'][$placeholder]);
          }
        }
      }
      unset($fields[$field]);
    }

    foreach ($fields as $field => $value) {
      if (isset($this->schema['fields'][$field]) && $this->schema['fields'][$field]['type'] == 'blob') {
        if (!is_null($fields[$field])) {
          $blobs[$blob_count] = fopen('php://memory', 'a');
          fwrite($blobs[$blob_count], $value);
          rewind($blobs[$blob_count]);

          $stmt->bindParam(':db_update_placeholder_' . $max_placeholder++, $blobs[$blob_count], PDO::PARAM_LOB);

          // Pre-increment is faster in PHP than increment.
          ++$blob_count;
        }
        else {
          // BLOB NULL is a bit different from normal case: we can't use
          // StreamAPI, but bind NULL to target field direcly.
          $stmt->bindParam(':db_update_placeholder_' . $max_placeholder++, $fields[$field], PDO::PARAM_LOB);
        }
      }
      elseif ($fields[$field] === '') {
        $stmt->bindValue(':db_update_placeholder_' . $max_placeholder++, OCI_EMPTY_STRING_PLACEHOLDER);
      }
      else {
        $stmt->bindParam(':db_update_placeholder_' . $max_placeholder++, $fields[$field]);
      }
    }

    if (count($this->condition)) {
      $this->condition->compile($this->connection, $this);

      $arguments = $this->condition->arguments();
      foreach ($arguments as $placeholder => $value) {
        if ($arguments[$placeholder] === '') {
          $stmt->bindValue($placeholder, OCI_EMPTY_STRING_PLACEHOLDER);
        }
        else {
          $stmt->bindParam($placeholder, $arguments[$placeholder]);
        }
      }
    }

    $this->connection->query($stmt, $options);

    // Transaction commits here where $transaction looses scope.

    return $stmt->rowCount();
  }

  public function __toString() {
    // Create a sanitized comment string to prepend to the query.
    $comments = $this->connection->makeComment($this->comments);

    // Prefix and escape table name manually.
    $table_name = $this->connection->prefixTables('{' . $this->table . '}');
    $table_name = $this->connection->escapeForOracle($table_name);

    // Expressions take priority over literal fields, so we process those first
    // and remove any literal fields that conflict.
    $fields = $this->fields;
    $update_fields = array();
    foreach ($this->expressionFields as $field => $data) {
      $update_fields[] = $this->connection->escapeForOracle($field) . ' = ' . $data['expression'];
      unset($fields[$field]);
    }

    $max_placeholder = 0;
    $blob_fields = array();
    $blob_placeholders = array();
    foreach ($fields as $field => $value) {
      if (isset($this->schema['fields'][$field]) && $this->schema['fields'][$field]['type'] == 'blob') {
        // BLOB field update in Oracle required additional handling with
        // EMPTY_BLOB() pleaseholder + RETURNING INTO (see below).
        $update_fields[] = $this->connection->escapeForOracle($field) . ' = EMPTY_BLOB()';
        $blob_fields[] = $this->connection->escapeForOracle($field);
        $blob_placeholders[] = ':db_update_placeholder_' . $max_placeholder++;
      }
      else {
        $update_fields[] = $this->connection->escapeForOracle($field) . ' = :db_update_placeholder_' . $max_placeholder++;
      }
    }

    $query = $comments . 'UPDATE ' . $table_name . ' SET ' . implode(', ', $update_fields);

    if (count($this->condition)) {
      $this->condition->compile($this->connection, $this);
      // There is an implicit string cast on $this->condition.
      $query .= " WHERE " . $this->condition;
    }

    // Oracle BLOB additional handling with RETURNING INTO.
    if (count($blob_fields)) {
      $query .= ' RETURNING ' . implode(', ', $blob_fields) . ' INTO ' . implode (', ', $blob_placeholders);
    }

    return $query;
  }
}
