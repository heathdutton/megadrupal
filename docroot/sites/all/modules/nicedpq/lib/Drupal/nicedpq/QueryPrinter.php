<?php

namespace Drupal\nicedpq;

class QueryPrinter {

  /**
   * Print a select query, with more linebreaks and indentation than usual.
   *
   * @param Drupal\nicedpq\IndentedText $out
   *   An object that we can write to.
   * @param SelectQuery $q
   *   The query object we want to print.
   */
  function printSelectQuery($out, $q) {

    // Instead of the original query, we operate with a stdClass clone,
    // where we can see all the attributes that were originally protected.
    $q = SelectQueryInspector::extractAttributes($q);

    // SELECT
    $out->println('SELECT');
    if ($q->distinct) {
      $out->println(' DISTINCT');
    }

    // FIELDS and EXPRESSIONS
    $this->printFields($out->indent(), $q);


    // FROM - We presume all queries have a FROM, as any query that doesn't won't need the query builder anyway.
    $out->println('FROM');
    $this->printFrom($out->indent(), $q);

    // WHERE
    if (count($q->where)) {
      $out->println('WHERE');
      $this->printCondition($out->indent(), $q->where);
    }

    // GROUP BY
    if ($q->group) {
      $out->println('GROUP BY');
      $out->indent()->printList($q->group);
    }

    // HAVING
    if (count($q->having)) {
      // There is an implicit string cast on $q->having.
      $out->println('HAVING ' . $q->having);
    }

    // ORDER BY
    if ($q->order) {
      $out->println('ORDER BY');
      $fields = array();
      foreach ($q->order as $field => $direction) {
        $fields[] = $field . ' ' . $direction;
      }
      $out->indent()->printList($fields);
    }

    // RANGE
    // There is no universal SQL standard for handling range or limit clauses.
    // Fortunately, all core-supported databases use the same range syntax.
    // Databases that need a different syntax can override this method and
    // do whatever alternate logic they need to.
    if (!empty($q->range)) {
      $out->println('LIMIT ' . (int) $q->range['length'] . ' OFFSET ' . (int) $q->range['start']);
    }

    // UNION is a little odd, as the select queries to combine are passed into
    // this query, but syntactically they all end up on the same level.
    if ($q->union) {
      foreach ($q->union as $union) {
        $out->println($union['type'] . ' ' . (string) $union['query']);
      }
    }

    if ($q->forUpdate) {
      $out->println('FOR UPDATE');
    }
  }

  /**
   * Print the fields information of a query.
   *
   * @param Drupal\nicedpq\IndentedText $out
   *   An object that we can write to.
   * @param SelectQuery $q
   *   The query object we want to print.
   */
  protected function printFields($out, $q) {
    $fields = array();
    foreach ($q->tables as $alias => $table) {
      if (!empty($table['all_fields'])) {
        $fields[] = $q->connection->escapeTable($alias) . '.*';
      }
    }
    foreach ($q->fields as $alias => $field) {
      $str = $q->connection->escapeField($field['field']);
      if (isset($field['table'])) {
        $str = $q->connection->escapeTable($field['table']) . '.' . $str;
      }
      // Always use the AS keyword for field aliases, as some
      // databases require it (e.g., PostgreSQL).
      $fields[] = $str . ' AS ' . $q->connection->escapeAlias($field['alias']);
    }
    foreach ($q->expressions as $alias => $expression) {
      $fields[] = $expression['expression'] . ' AS ' . $q->connection->escapeAlias($expression['alias']);
    }
    $out->printList($fields);
  }

  /**
   * Print the FROM information of a query.
   *
   * @param Drupal\nicedpq\IndentedText $out
   *   An object that we can write to.
   * @param SelectQuery $q
   *   The query object we want to print.
   */
  protected function printFrom($out, $q) {
    foreach ($q->tables as $alias => $table) {
      $out->println();
      if (isset($table['join type'])) {
        $out->pr($table['join type'] . ' JOIN ');
      }

      // If the table is a subquery, compile it and integrate it into this query.
      if ($table['table'] instanceof \SelectQueryInterface) {
        // Run preparation steps on this sub-query before converting to string.
        $subquery = $table['table'];
        $subquery->preExecute();
        $subquery->__toString();
        $out->pr('(');
        $this->printSelectQuery($out->indent(), $subquery);
        $out->println(')');
      }
      else {
        $out->pr($q->connection->escapeTable($table['table']));
      }

      // Don't use the AS keyword for table aliases, as some
      // databases don't support it (e.g., Oracle).
      $out->pr(' ' . $q->connection->escapeTable($table['alias']));

      if (!empty($table['condition'])) {
        $out->pr(' ON ');
        $out->indent()->printList($table['condition'], ' AND');
      }
    }
  }

  /**
   * Print the WHERE information of a query.
   *
   * @param Drupal\nicedpq\IndentedText $out
   *   An object that we can write to.
   * @param DatabaseCondition $cond
   *   A database condition object we want to print.
   */
  protected function printCondition($out, $cond) {
    // Access the protected attributes.
    $extracted = QueryConditionInspector::extractAttributes($cond);
    $printed = trim($cond);
    if (!empty($extracted->arguments)) {
      $printed = strtr($printed, $extracted->arguments);
    }
    $out->printList($printed, ' AND');
  }
}
