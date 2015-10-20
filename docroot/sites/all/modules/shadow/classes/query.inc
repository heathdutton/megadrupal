<?php

class ShadowQuery extends ShadowStateMachine
{
  protected $base_table;
  protected $base_table_alias;
  protected $joins;
  protected $filter_fields;
  protected $sort_fields;
  protected $fields;
  protected $tables;
  protected $filters;

  public function __construct($sql) {
    $this->base_table = NULL;
    $this->base_table_alias = NULL;
    $this->joins = array();
    $this->filter_fields = array();
    $this->sort_fields = array();
    $this->fields = array();
    $this->tables = array();
    $this->filters = array();

    $transitions = array(
      'start' => array(
        array('SELECT', 'select', 'select', FALSE),
      ),
      'select' => array(
        array('*', 'all_fields', 'all_fields', FALSE),
        array('[a-z][a-z0-9_\\.]*', 'field', 'field', FALSE),
        array('[a-z][a-z0-9_]*', 'field_function', 'function', FALSE),
        array('DISTINCT', 'distinct', 'distinct', FALSE),
        array('^\'$', 'fieldlist_string', NULL, FALSE),
        array('\\-?(\\.[0-9]+|[0-9]+|[0-9]+\\.[0-9]+)', 'fieldlist_numeric', 'numeric', FALSE),
      ),
      'distinct' => array(
        array('[a-z][a-z0-9_\\.]*', 'field', 'field', FALSE),
        array('*', 'all_fields', 'all_fields', FALSE),
      ),
      'field' => array(
        array(', *', 'fieldlist_comma', 'comma', FALSE),
        array('AS', 'field_as', 'field_as', FALSE),
        array('FROM', 'from', 'from', FALSE),
        array(')', 'field_function_paren_close', 'paren_close', FALSE),
      ),
      'field_as' => array(
        array('[a-z][a-z0-9_]*', 'field_alias', 'field_alias', FALSE),
      ),
      'field_alias' => array(
        array('FROM', 'from', 'from', FALSE),
        array(', *', 'fieldlist_comma', 'comma', FALSE),
      ),
      'fieldlist_comma' => array(
        array('[a-z][a-z0-9_\\.]*', 'field', 'field', FALSE),
        array('[a-z][a-z0-9_]*', 'field_function', 'function', FALSE),
        array('^\'$', 'fieldlist_string', NULL, FALSE),
        array('\\-?(\\.[0-9]+|[0-9]+|[0-9]+\\.[0-9]+)', 'fieldlist_numeric', 'numeric', FALSE),
      ),
      'all_fields' => array(
        array('FROM', 'from', 'from', FALSE),
      ),
      'field_function' => array(
        array('\\(', 'field_function_paren_open', 'paren_open', FALSE),
      ),
      'field_function_paren_open' => array(
        array('[a-z][a-z0-9_\\.]*', 'field', 'field', FALSE),
        array('\\(', 'field_function_paren_open', 'paren_open', FALSE),
        array('[a-z][a-z0-9_]*', 'field_function', 'function', FALSE),
      ),
      'field_function_paren_close' => array(
        array('FROM', 'from', 'from', FALSE),
        array(', *', 'fieldlist_comma', 'comma', FALSE),
        array('\\+', 'fieldlist_operator', 'plus', FALSE),
        array('\\-', 'fieldlist_operator', 'minus', FALSE),
        array('AS', 'field_as', 'field_as', FALSE),
      ),
      'fieldlist_operator' => array(
        array('[a-z][a-z0-9_]*', 'field_function', 'function', FALSE),
        array('INTERVAL *[0-9]+ *(SECOND|MINUTE|HOUR|DAY|MONTH|YEAR)', 'fieldlist_interval', 'interval', FALSE),
      ),
      'fieldlist_interval' => array(
        array(')', 'field_function_paren_close', 'paren_close', FALSE),
        array(', *', 'fieldlist_comma', 'comma', FALSE),
      ),
      'fieldlist_string' => array(
        array('\'$', 'fieldlist_string_end', 'string', FALSE),
        array('^[^\\\\\\\\\']+$', 'fieldlist_string', NULL, FALSE),
        array('^\\\\$', 'fieldlist_string_bs', NULL, FALSE),
      ),
      'fieldlist_string_bs' => array(
        array('^.$', 'fieldlist_string', NULL, FALSE),
      ),
      'fieldlist_string_end' => array(
        array('AS', 'field_as', 'field_as', FALSE),
        array('\\) *', 'field_function_paren_close', 'paren_close', FALSE),
      ),
      'fieldlist_numeric' => array(
        array('AS', 'field_as', 'field_as', FALSE),
      ),
      'from' => array(
        array('(\\{[a-z][a-z0-9_]*\\}|[a-z][a-z0-9_]*)', 'basetable', 'basetable', FALSE),
      ),
      'basetable' => array(
        array('AS', 'basetable_as', 'basetable_as', FALSE),
        array('[a-z][a-z0-9_]*', 'basetable_alias', 'basetable_alias', FALSE),
        array('', 'end', NULL, FALSE),
        array('(LEFT |RIGHT |INNER )?JOIN', 'join', 'join', FALSE),
        array('WHERE', 'where', 'where', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
      ),
      'basetable_as' => array(
        array('[a-z][a-z0-9_]*', 'basetable_alias', 'basetable_alias', FALSE),
      ),
      'basetable_alias' => array(
        array('(LEFT |RIGHT |INNER )?JOIN', 'join', 'join', FALSE),
        array('WHERE', 'where', 'where', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
      ),
      'join' => array(
        array('(\\{[a-z][a-z0-9_]*\\}|[a-z][a-z0-9_]*)', 'join_table', 'table', FALSE),
      ),
      'join_table' => array(
        array('AS', 'join_table_as', 'table_as', FALSE),
        array('ON', 'join_table_on', 'join_on', FALSE),
        array('[a-z][a-z0-9_]*', 'join_table_alias', 'table_alias', FALSE),
      ),
      'join_table_alias' => array(
        array('ON', 'join_table_on', 'join_on', FALSE),
      ),
      'join_table_as' => array(
        array('[a-z][a-z0-9]*', 'join_table_on', 'table_alias', FALSE),
      ),
      'join_table_on' => array(
        array('[a-z][a-z0-9\\._]*', 'join_left_field', 'field', FALSE),
        array('(', 'join_paren_open', 'paren_open', FALSE),
      ),
      'join_left_field' => array(
        array('= *', 'join_operator', 'operator', FALSE),
      ),
      'join_operator' => array(
        array('[a-z][a-z0-9\\._]*', 'join_right_field', 'field', FALSE),
        array('^\'$', 'join_string', NULL, FALSE),
        array('\\-?(\\.[0-9]+|[0-9]+|[0-9]+\\.[0-9]+)', 'join_numeric', 'numeric', FALSE),
        array('\\:[a-z0-9_]+', 'join_argument', 'argument', FALSE),
      ),
      'join_string' => array(
        array('\'$', 'join_string_end', 'string', FALSE),
        array('^[^\\\\\\\\\']+$', 'join_string', NULL, FALSE),
        array('^\\\\$', 'join_string_bs', NULL, FALSE),
      ),
      'join_string_bs' => array(
        array('^.$', 'join_string', NULL, FALSE),
      ),
      'join_string_end' => array(
        array('AND', 'join_cond_and', 'and', FALSE),
        array(')', 'join_paren_close', 'paren_close', FALSE),
        array('(LEFT |RIGHT |INNER )?JOIN', 'join', 'join', FALSE),
        array('[\\s]*WHERE', 'where', 'where', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
      ),
      'join_numeric' => array(
        array('AND', 'join_cond_and', 'and', FALSE),
        array(')', 'join_paren_close', 'paren_close', FALSE),
        array('(LEFT |RIGHT |INNER )?JOIN', 'join', 'join', FALSE),
        array('[\\s]*WHERE', 'where', 'where', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
      ),
      'join_argument' => array(
        array('AND', 'join_cond_and', 'and', FALSE),
        array(')', 'join_paren_close', 'paren_close', FALSE),
        array('(LEFT |RIGHT |INNER )?JOIN', 'join', 'join', FALSE),
        array('[\\s]*WHERE', 'where', 'where', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
      ),
      'join_right_field' => array(
        array('(LEFT |RIGHT |INNER )?JOIN', 'join', 'join', FALSE),
        array('WHERE', 'where', 'where', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
        array('AND', 'join_cond_and', 'and', FALSE),
        array(')', 'join_paren_close', 'paren_close', FALSE),
      ),
      'join_cond_and' => array(
        array('[a-z][a-z0-9\\._]*', 'join_left_field', 'field', FALSE),
        array('(', 'join_paren_open', 'paren_open', FALSE),
      ),
      'join_paren_open' => array(
        array('(', 'join_paren_open', 'paren_open', FALSE),
        array('[a-z][a-z0-9\\._]*', 'join_left_field', 'field', FALSE),
      ),
      'join_paren_close' => array(
        array(')', 'join_paren_close', 'paren_close', FALSE),
        array('AND', 'join_cond_and', 'and', FALSE),
        array('(LEFT |RIGHT |INNER )?JOIN', 'join', 'join', FALSE),
        array('WHERE', 'where', 'where', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
      ),
      'join_paren_alias' => array(
        array('(LEFT |RIGHT |INNER )?JOIN', 'join', 'join', FALSE),
        array('WHERE', 'where', 'where', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
      ),
      'where' => array(
        array('(', 'subquery_paren_open', 'subquery_paren_open', FALSE),
        array('(', 'where_paren_open', 'paren_open', FALSE),
        array('[a-z][a-z0-9\\._]*', 'where_field', 'field', FALSE),
        array('[a-z][a-z0-9_]*', 'where_function', 'function', FALSE),
      ),
      'where_paren_open' => array(
        array(' *\\( *', 'where_paren_open', 'paren_open', FALSE),
        array('(', 'subquery_paren_open', 'subquery_paren_open', FALSE),
        array('[a-z][a-z0-9\\._]*', 'where_field', 'field', FALSE),
        array('^\'$', 'where_string', NULL, FALSE),
        array('[a-z][a-z0-9_]*', 'where_function', 'function', FALSE),
        array(')', 'where_paren_close', 'paren_close', FALSE),
        array('\\-?(\\.[0-9]+|[0-9]+|[0-9]+\\.[0-9]+)', 'where_numeric', 'numeric', FALSE),
      ),
      'where_operator' => array(
        array('[a-z][a-z0-9\\._]*', 'where_field', 'field', FALSE),
        array('^\'$', 'where_string', NULL, FALSE),
        array('[a-z][a-z0-9_]*', 'where_function', 'function', FALSE),
        array('[a-z][a-z0-9\\._]*', 'where_field', 'field', FALSE),
        array('\\-?(\\.[0-9]+|[0-9]+|[0-9]+\\.[0-9]+)', 'where_numeric', 'numeric', FALSE),
        array('\\:[a-z0-9_]+', 'where_argument', 'argument', FALSE),
        array('\\*\\*\\*[A-Z0-9_]+\\*\\*\\*', 'where_substitution', 'substitution', FALSE),
      ),
      'where_field' => array(
        array('= *', 'where_operator', 'where_e', FALSE),
        array('(\\!=|<>)', 'where_operator', 'where_ne', FALSE),
        array('< *', 'where_operator', 'where_lt', FALSE),
        array('> *', 'where_operator', 'where_gt', FALSE),
        array('<=', 'where_operator', 'where_lte', FALSE),
        array('>=', 'where_operator', 'where_gte', FALSE),
        array('\\+', 'where_operator', 'where_plus', FALSE),
        array('\\-', 'where_operator', 'where_minus', FALSE),
        array('LIKE', 'where_operator', 'where_like', FALSE),
        array('REGEXP', 'where_operator', 'where_regexp', FALSE),
        array('NOT LIKE', 'where_operator', 'where_not_like', FALSE),
        array('NOT REGEXP', 'where_operator', 'where_not_regexp', FALSE),
        array('in', 'where_in', 'where_in', FALSE),
        array('AND', 'where_and', 'and', FALSE),
        array('OR', 'where_or', 'or', FALSE),
        array(')', 'where_paren_close', 'paren_close', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array(')', 'where_paren_close', 'paren_close', FALSE),
        array(', *', 'where_comma', 'comma', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
      ),
      'where_string' => array(
        array('\'$', 'where_string_end', 'string', FALSE),
        array('^[^\\\\\\\\\']+$', 'where_string', NULL, FALSE),
        array('^\\\\$', 'where_string_bs', NULL, FALSE),
      ),
      'where_string_bs' => array(
        array('^.$', 'where_string', NULL, FALSE),
      ),
      'where_string_end' => array(
        array('AND', 'where_and', 'and', FALSE),
        array('OR', 'where_or', 'or', FALSE),
        array(')', 'where_paren_close', 'paren_close', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array(')', 'where_paren_close', 'paren_close', FALSE),
        array(', *', 'where_comma', 'comma', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
        array('ESCAPE', 'where_escape', 'escape', FALSE),
      ),
      'where_and' => array(
        array('[a-z][a-z0-9\\._]*', 'where_field', 'field', FALSE),
        array('(', 'where_paren_open', 'paren_open', FALSE),
        array('(', 'subquery_paren_open', 'subquery_paren_open', FALSE),
        array('\\-?(\\.[0-9]+|[0-9]+|[0-9]+\\.[0-9]+)', 'where_numeric', 'numeric', FALSE),
      ),
      'where_or' => array(
        array('[a-z][a-z0-9\\._]*', 'where_field', 'field', FALSE),
        array('(', 'where_paren_open', 'paren_open', FALSE),
        array('(', 'subquery_paren_open', 'subquery_paren_open', FALSE),
        array('\\-?(\\.[0-9]+|[0-9]+|[0-9]+\\.[0-9]+)', 'where_numeric', 'numeric', FALSE),
      ),
      'where_function' => array(
        array('(', 'where_paren_open', 'paren_open', FALSE),
      ),
      'where_paren_close' => array(
        array(', *', 'where_comma', 'comma', FALSE),
        array('AND', 'where_and', 'and', FALSE),
        array('OR', 'where_or', 'or', FALSE),
        array(' *\\) *', 'where_paren_close', 'paren_close', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
      ),
      'where_comma' => array(
        array('\\-?(\\.[0-9]+|[0-9]+|[0-9]+\\.[0-9]+)', 'where_numeric', 'numeric', FALSE),
        array('^\'$', 'where_string', NULL, FALSE),
        array('[a-z][a-z0-9\\._]*', 'where_field', 'field', FALSE),
        array('[a-z][a-z0-9_]*', 'where_function', 'function', FALSE),
      ),
      'where_open' => array(
        array('[a-z][a -z0-9\\.]*', 'where_field', 'field', FALSE),
      ),
      'where_numeric' => array(
        array('AND', 'where_and', 'and', FALSE),
        array('OR', 'where_or', 'or', FALSE),
        array(')', 'where_paren_close', 'paren_close', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array(')', 'where_paren_close', 'paren_close', FALSE),
        array('= *', 'where_operator', 'where_e', FALSE),
        array('(\\!=|<>)', 'where_operator', 'where_ne', FALSE),
        array('< *', 'where_operator', 'where_lt', FALSE),
        array('> *', 'where_operator', 'where_gt', FALSE),
        array('<=', 'where_operator', 'where_lte', FALSE),
        array('>=', 'where_operator', 'where_gte', FALSE),
        array('\\+', 'where_operator', 'where_plus', FALSE),
        array('\\-', 'where_operator', 'where_minus', FALSE),
        array('', 'end', NULL, FALSE),
      ),
      'where_argument' => array(
        array('ESCAPE', 'where_escape', 'escape', FALSE),
        array('AND', 'where_and', 'and', FALSE),
        array('OR', 'where_or', 'or', FALSE),
        array('\\) *', 'where_paren_close', 'paren_close', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array('', 'end', NULL, FALSE),
      ),
      'where_substitution' => array(
        array('AND', 'where_and', 'and', FALSE),
        array('OR', 'where_or', 'or', FALSE),
        array('\\) *', 'where_paren_close', 'paren_close', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array('', 'end', NULL, FALSE),
      ),
      'where_escape' => array(
        array('^\'$', 'where_escape_string', NULL, FALSE),
      ),
      'where_escape_string' => array(
        array('\'$', 'where_escape_string_end', 'string', FALSE),
        array('^[^\\\\\\\\\']+$', 'where_escape_string', NULL, FALSE),
        array('^\\\\$', 'where_escape_string_bs', NULL, FALSE),
      ),
      'where_escape_string_bs' => array(
        array('(.)', 'where_escape_string', NULL, FALSE),
      ),
      'where_escape_string_end' => array(
        array('AND', 'where_and', 'and', FALSE),
        array('OR', 'where_or', 'or', FALSE),
        array(' *\\) *', 'where_paren_close', 'paren_close', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
      ),
      'where_in' => array(
        array('(', 'where_set_paren_open', 'set_paren_open', FALSE),
      ),
      'where_set_paren_open' => array(
        array('\\:[a-z0-9_]+', 'where_set_argument', 'argument', FALSE),
        array('^\'$', 'where_set_string', NULL, FALSE),
        array('\\-?(\\.[0-9]+|[0-9]+|[0-9]+\\.[0-9]+)', 'where_set_numeric', 'numeric', FALSE),
      ),
      'where_set_comma' => array(
        array('^\'$', 'where_set_string', NULL, FALSE),
        array('\\-?(\\.[0-9]+|[0-9]+|[0-9]+\\.[0-9]+)', 'where_set_numeric', 'numeric', FALSE),
        array('\\:[a-z0-9_]+', 'where_set_argument', 'argument', FALSE),
      ),
      'where_set_string' => array(
        array('\'$', 'where_set_string_end', 'string', FALSE),
        array('^[^\\\\\\\\\']+$', 'where_set_string', NULL, FALSE),
        array('^\\\\$', 'where_set_string_bs', NULL, FALSE),
      ),
      'where_set_string_bs' => array(
        array('^.$', 'where_set_string', NULL, FALSE),
      ),
      'where_set_string_end' => array(
        array(', *', 'where_set_comma', 'comma', FALSE),
        array(')', 'where_set_paren_close', 'set_paren_close', FALSE),
      ),
      'where_set_numeric' => array(
        array(', *', 'where_set_comma', 'comma', FALSE),
        array(')', 'where_set_paren_close', 'set_paren_close', FALSE),
      ),
      'where_set_argument' => array(
        array(', *', 'where_set_comma', 'comma', FALSE),
        array(')', 'where_set_paren_close', 'set_paren_close', FALSE),
      ),
      'where_set_paren_close' => array(
        array('AND', 'where_and', 'and', FALSE),
        array('OR', 'where_or', 'or', FALSE),
        array(' *\\) *', 'where_paren_close', 'paren_close', FALSE),
        array('GROUP BY', 'group_by', NULL, FALSE),
        array('ORDER BY', 'order_by', NULL, FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
      ),
      'group_by' => array(
        array('[a-z][a-z0-9\\._]*', 'group_by_field', 'field', FALSE),
      ),
      'group_by_field' => array(
        array(', *', 'group_by_comma', 'comma', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
      ),
      'group_by_comma' => array(
        array('[a-z][a-z0-9\\._]*', 'group_by_field', 'field', FALSE),
      ),
      'order_by' => array(
        array('[a-z][a-z0-9\\._]*', 'order_by_field', 'field', FALSE),
      ),
      'order_by_field' => array(
        array('asc', 'order_by_asc', 'order_by_asc', FALSE),
        array('desc', 'order_by_desc', 'order_by_desc', FALSE),
        array(', *', 'order_by_comma', 'comma', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
      ),
      'order_by_asc' => array(
        array(', *', 'order_by_comma', 'comma', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
      ),
      'order_by_desc' => array(
        array(', *', 'order_by_comma', 'comma', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
      ),
      'order_by_comma' => array(
        array('[a-z][a-z0-9\\._]*', 'order_by_field', 'field', FALSE),
      ),
      'limit' => array(
        array('[0-9]+', 'limit_number', 'numeric', FALSE),
        array('\\:[a-z0-9_]+', 'limit_argument', 'argument', FALSE),
      ),
      'limit_number' => array(
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
        array('(\\,|offset)', 'limit_offset', 'limit_offset', FALSE),
      ),
      'limit_argument' => array(
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
        array('(\\,|offset)', 'limit_offset', 'limit_offset', FALSE),
      ),
      'limit_offset' => array(
        array('[0-9]+', 'limit_offset_number', 'numeric', FALSE),
      ),
      'limit_offset_number' => array(
        array('\\:[a-z0-9_]+', 'limit_offset_argument', 'argument', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
      ),
      'limit_offset_argument' => array(
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
      ),
      'subquery_paren_open' => array(
        array('SELECT', 'select', 'select', FALSE),
      ),
      'subquery_paren_close' => array(
        array('AND', 'where_and', 'and', FALSE),
        array('OR', 'where_or', 'or', FALSE),
        array(' *\\) *', 'where_paren_close', 'paren_close', FALSE),
        array('GROUP BY', 'group_by', 'group_by', FALSE),
        array('ORDER BY', 'order_by', 'order_by', FALSE),
        array('LIMIT', 'limit', 'limit', FALSE),
        array('', 'end', NULL, FALSE),
        array(' *\\) *', 'subquery_paren_close', 'subquery_paren_close', FALSE),
        array('in', 'where_in', 'where_in', FALSE),
        array('= *', 'where_operator', 'where_e', FALSE),
        array('(\\!=|<>)', 'where_operator', 'where_ne', FALSE),
        array('< *', 'where_operator', 'where_lt', FALSE),
        array('> *', 'where_operator', 'where_gt', FALSE),
        array('<=', 'where_operator', 'where_lte', FALSE),
        array('>=', 'where_operator', 'where_gte', FALSE),
        array('\\+', 'where_operator', 'where_plus', FALSE),
        array('\\-', 'where_operator', 'where_minus', FALSE),
        array('LIKE', 'where_operator', 'where_like', FALSE),
        array('REGEXP', 'where_operator', 'where_regexp', FALSE),
        array('NOT LIKE', 'where_operator', 'where_not_like', FALSE),
        array('NOT REGEXP', 'where_operator', 'where_not_regexp', FALSE),
      ),
    );
    
    parent::__construct($transitions);

    $this->regex_allow_whitespace = TRUE;
    $this->regex_case_sensitive = FALSE;

    if (!empty($sql)) {
      $this->parse($sql);
    }
  }

  public function parse($sql) {
    parent::parse($sql);
    
    $token_count = count($this->state_holder->token_values);
    
    // Pass the query parts to the parse_* functions.
    $section_start = 0;
    $section = NULL;
    $nesting = 0;
    for ($i = 0; $i < $token_count; ++$i) {
      switch ($this->state_holder->token_types[$i]) {
        case 'select':
        case 'from':
        case 'join':
        case 'where':
        case 'group_by':
        case 'order_by':
        case 'limit':
          // Only process if not in a subquery.
          if ($nesting == 0) {
            if ($section) {
              $function = "parse_$section";
              $this->$function($section_start, $i);
            }
            $section = $this->state_holder->token_types[$i];
            $section_start = $i + 1;
          }
          break;
        case 'subquery_paren_open':
          ++$nesting;
          break;
        case 'subquery_paren_close':
          --$nesting;
          break;
      }
    }
    if ($section) {
      $function = "parse_$section";
      $this->$function($section_start, $i);
    }
  }
  
  /**
   * Parse tokens in SELECT section of query.
   * 
   * This function fills $this->fields:
   * array(
   *   'node_nid' => 'n.nid',
   *   'node_status' => 'n.status',
   * )
   */
  protected function parse_select($start, $end) {
    for ($i = $start; $i < $end; ++$i) {
      if (!isset($this->state_holder->token_types[$i + 2])) {
        continue;
      }
      if ($this->state_holder->token_types[$i] == 'field'
       && $this->state_holder->token_types[$i + 1] == 'field_as'
       && $this->state_holder->token_types[$i + 2] == 'field_alias') {
        $field = trim($this->state_holder->token_values[$i]);
        $alias = trim($this->state_holder->token_values[$i + 2]);
        $this->fields[$alias] = $field;
      }
    }
  }
  
  /**
   * Parse tokens in FROM section of query.
   * 
   * This function fills $this->base_table, $this->base_table_alias and adds the
   * base table to $this->tables:
   * 
   * array(
   *   'n' => 'node',
   * )
   */
  protected function parse_from($start, $end) {
    $this->base_table_alias = NULL;
    for ($i = $start; $i < $end; ++$i) {
      $type = $this->state_holder->token_types[$i];
      $value = $this->rawName($this->state_holder->token_values[$i]);
      switch ($type) {
        case 'basetable':
          $this->base_table = $value;
          break;
        case 'basetable_alias':
          $this->base_table_alias = $value;
          break;
      }
    }
    if (empty($this->base_table_alias)) {
      $this->base_table_alias = $this->base_table;
    }
    $this->tables[$this->base_table_alias] = $this->base_table;
  }
  
  /**
   * Parse tokens from a single JOIN element.
   * 
   * Add the table to $this->tables and the join to $this->joins.
   * 
   * Example:
   * 
   * FROM node n
   * JOIN node_access na ON n.nid = na.nid AND na.realm = 'domain_id'
   * 
   * Is converted to:
   * 
   *   'na' => 'node.nid=node_access.nid[realm:\'domain_id\']',
   * 
   * And the row added in $this->tables is:
   * 
   *   'na' => 'node_access'
   */
  protected function parse_join($start, $end) {
    $table = NULL;
    $alias = NULL;
    $conditions = array();
    $type_buffer = array();
    $value_buffer = array();
    for ($i = $start; $i < $end; ++$i) {
      $type = $this->state_holder->token_types[$i];
      $value = trim($this->state_holder->token_values[$i]);
      switch ($type) {
        case 'join_on':
          break;
        case 'table':
          $table = $this->rawName($value);
          $alias = $this->rawName($value);
          break;
        case 'table_alias':
          $alias = $this->rawName($value);
          break;
        case 'paren_open':
        case 'paren_close':
          break;
        case 'and':
          $conditions[] = array(
            'type' => $type_buffer,
            'value' => $value_buffer,
          );
          $type_buffer = array();
          $value_buffer = array();
          break;
        default:
          $type_buffer[] = $type;
          $value_buffer[] = $value;
          break;
      }
    }
    if ($type_buffer) {
      $conditions[] = array(
        'type' => $type_buffer,
        'value' => $value_buffer,
      );
    }
    if (empty($alias)) {
      $alias = $table;
    }
    
    $this->tables[$alias] = $table;
    
    $filters = array();
    $definition = '';
    foreach ($conditions as $condition) {
      if (count($condition['type']) == 3) {
        // Parse "field operator field" conditions.
        if ($condition['type'][0] == 'field'
         && $condition['type'][1] == 'operator'
         && $condition['type'][2] == 'field') {
          if (trim($condition['value'][1]) != '=') {
            // Condition is not "=".
            continue;
          }
          $left = explode('.', trim($condition['value'][0]));
          $right = explode('.', trim($condition['value'][2]));
          if (count($left) != 2 || count($right) != 2) {
            // Not all fields have the supported format "table_alias.field".
            continue;
          }
          if ($left[0] == $alias) {
            // Left and right fields are swapped.
            $tmp = $left;
            $right = $left;
            $left = $tmp;
          }
          if ($right[0] != $alias) {
            // The right table is not the joined table, which is weird.
            continue;
          }
          if ($left[0] == $this->base_table_alias) {
            $definition = "{$this->base_table}.{$left[1]}={$table}.{$right[1]}";
          }
          elseif (isset($this->joins[$left[0]])) {
            $left_table = $this->tables[$left[0]];
            $definition = $this->joins[$left[0]] . ",{$left_table}.{$left[1]}={$table}.{$right[1]}";
          }
        }
        // Parse "field operator value" conditions.
        if ($condition['type'][0] == 'field'
         && $condition['type'][1] == 'operator'
         && in_array($condition['type'][2], array('string', 'numeric'))) {
          if (trim($condition['value'][1]) != '=') {
            // Condition is not "=".
            continue;
          }
          $field = explode('.', trim($condition['value'][0]));
          if ($field[0] != $alias) {
            // Condition is about a different table.
            continue;
          }
          $filters[] = $field[1] . ':' . $condition['value'][2];
        }
      }
    }
    
    if ($definition) {
      if ($filters) {
        $definition .= '[' . implode(',', $filters) . ']';
      }
      $this->joins[$alias] = $definition;
    }
  }
  
  /**
   * Parse tokens in WHERE section of query.
   * 
   * Fill $this->filter_fields and $this->filters.
   * 
   * Example:
   * 
   * SELECT *
   * FROM node n
   * JOIN users u ON n.uid = u.uid
   * WHERE u.name = 'John Doe'
   * AND u.created > 1234567890
   * AND n.status = 1
   * 
   * $this->filter_fields = array(
   *   'node.uid=users.uid,users.name',
   *   'node.uid=users.uid,users.created',
   *   'node.status',
   * );
   * 
   * $this->filters = array(
   *   'node.uid=users.uid,users.name:\'John Doe\'',
   *   'node.status=1',
   * );
   */
  protected function parse_where($start, $end) {
    for ($i = $start; $i < $end; ++$i) {
      $type = $this->state_holder->token_types[$i];
      $value = $this->state_holder->token_values[$i];
      
      if ($type != 'field') {
        continue;
      }
      
      $field = explode('.', trim($value));
      if (!isset($field[1])) {
        continue;
      }
      if ($field[0] == $this->base_table_alias) {
        $definition = "{$this->base_table}.{$field[1]}";
      }
      elseif (isset($this->joins[$field[0]])) {
        $definition = $this->joins[$field[0]]
                . ','
                . $this->tables[$field[0]]
                . '.'
                . $field[1];
      }
      else {
        continue;
      }
      
      // Add filter field.
      if (!in_array($definition, $this->filter_fields)) {
        $this->filter_fields[] = $definition;
      }
      
      // Look forward for see if this field can be added as a filter.
      $filter_definition = NULL;
      if (!isset($this->state_holder->token_types[$i + 1])) {
        continue;
      }
      $type_next = $this->state_holder->token_types[$i + 1];
      if ($type_next == 'where_e') {
        switch ($this->state_holder->token_types[$i + 2]) {
          case 'string':
          case 'numeric':
            $filter_definition = $definition . ':' . $this->state_holder->token_values[$i + 2];
            break;
        }
      }
      if ($type_next == 'where_in') {
        $values = array();
        for ($j = $i + 2; $j < $end; ++$j) {
          switch ($this->state_holder->token_types[$j]) {
            case 'set_paren_close':
              break(2);
            case 'string':
            case 'numeric':
              $values[] = trim($this->state_holder->token_values[$j]);
              break;
          }
        }
        if ($values) {
          $filter_definition = $definition . ':' . implode(',', $values);
        }
      }
      if (!empty($filter_definition) && !in_array($filter_definition, $this->filters)) {
        $this->filters[] = $filter_definition;
      }
    }
  }
  
  /**
   * Parse tokens in GROUP BY part of query.
   * 
   * Add fields used in group by to the filter fields.
   */
  protected function parse_group_by($start, $end) {
    for ($i = $start; $i < $end; ++$i) {
      $type = $this->state_holder->token_types[$i];
      $value = $this->state_holder->token_values[$i];
      
      if ($type != 'field') {
        continue;
      }
      
      $field = explode('.', trim($value));
      if (!isset($field[1])) {
        continue;
      }
      if ($field[0] == $this->base_table_alias) {
        $definition = "{$this->base_table}.{$field[1]}";
      }
      elseif (isset($this->joins[$field[0]])) {
        $definition = $this->joins[$field[0]]
                . ','
                . $this->tables[$field[0]]
                . '.'
                . $field[1];
      }
      else {
        continue;
      }
      
      // Add as filter field.
      if (!in_array($definition, $this->filter_fields)) {
        $this->filter_fields[] = $definition;
      }
    }
  }
  
  /**
   * Parse tokens in ORDER BY part of query.
   * 
   * Fill $this->sortfields:
   * array(
   *   'node.sticky*',
   *   'node.created*',
   * )
   */
  protected function parse_order_by($start, $end) {
    for ($i = $start; $i < $end; ++$i) {
      $type = $this->state_holder->token_types[$i];
      $value = $this->state_holder->token_values[$i];
      
      if ($type != 'field') {
        continue;
      }
      
      $field = explode('.', trim($value));
      if (!isset($field[1])) {
        continue;
      }
      if ($field[0] == $this->base_table_alias) {
        $definition = "{$this->base_table}.{$field[1]}";
      }
      elseif (isset($this->joins[$field[0]])) {
        $definition = $this->joins[$field[0]]
                . ','
                . $this->tables[$field[0]]
                . '.'
                . $field[1];
      }
      else {
        continue;
      }
      
      if (isset($this->state_holder->token_types[$i + 1])) {
        if ($this->state_holder->token_types[$i + 1] == 'order_by_desc') {
          $definition .= '*';
        }
      }
      
      $this->sort_fields[] = $definition;
    }
  }
  
  /**
   * Parse tokens in LIMIT part of query.
   */
  protected function parse_limit($start, $end) {
    for ($i = $start; $i < $end; ++$i) {
      $type = $this->state_holder->token_types[$i];
      $value = $this->state_holder->token_values[$i];
      
    }
  }
  
  /**
   * Replace basetable by shadow table.
   * 
   * @param string $table
   * @param array $relation
   *   array(
   *     'node_nid' => 'nid',
   *   )
   * @param array $fields
   *   array(
   *     'node.status' => 'node_status',
   *     'node.created*' => 'node_created',
   *     'node.sticky' => 'node_sticky',
   *   )
   * @todo Break this function into multiple functions
   */
  public function shadow($table, $relation, $fields, $runtime_filters = array()) {
    // Generate an unique alias for the shadow table.
    $num = 1;
    $table_alias = 's';
    while (isset($this->tables[$table_alias])) {
      ++$num;
      $table_alias = "s$num";
    }
    $this->tables[$table_alias] = $table;
    
    $new_token_types = array();
    $new_token_values = array();
    
    $token_count = count($this->state_holder->token_values);
    
    // Pass the query parts to the parse_* functions.
    $section_start = 0;
    $section = NULL;
    for ($i = 0; $i < $token_count; ++$i) {
      $type = $this->state_holder->token_types[$i];
      $value = $this->state_holder->token_values[$i];
      
      if (in_array($type, array('select', 'from', 'join', 'where', 'group_by', 'order_by', 'limit'))) {
        $section = $type;
      }
      
      if ($type == 'from') {
        $add_types = array(
          'from',
          'basetable',
          'basetable_alias',
          'join',
          'table',
          'table_alias',
          'join_on',
        );
        $add_values = array(
          $this->preserveWhitespace('FROM', $value),
          "{{$table}} ",
          "$table_alias ",
          "\nJOIN ",
          "{{$this->base_table}} ",
          "{$this->base_table_alias} ",
          "ON ",
        );
        $first = TRUE;
        foreach ($relation as $shadow_field => $basetable_field) {
          if (!$first) {
            $add_types[] = 'and';
            $add_values[] = 'AND ';
          }
          $shadow_field = $table_alias . '.' . $shadow_field;
          $basetable_field = $this->base_table_alias . '.' . $basetable_field;
          $add_types = array_merge($add_types, array('field', 'operator', 'field'));
          $add_values = array_merge($add_values, array("$shadow_field ", '= ', "$basetable_field "));
          $first = FALSE;
        }
        $new_token_types = array_merge($new_token_types, $add_types);
        $new_token_values = array_merge($new_token_values, $add_values);
      }
      
      if ($section == 'from') {
        continue;
      }
      
      if ($type == 'field') {
        // Build field definition for current token.
        $definition = '';
        $field = explode('.', trim($value));
        if (isset($field[1]) && isset($this->tables[$field[0]])) {
          if (isset($this->joins[$field[0]])) {
            $definition .= $this->joins[$field[0]] . ',';
          }
          $definition .= $this->tables[$field[0]] . '.' . $field[1];
        }
        
        // Generate a definition without join conditions.
        // We may fall back to this field if the same join is not found.
        $definition_simple = preg_replace('/\\[.*?\\]/', '', $definition);
        
        // Replace filter fields.
        if (in_array($section, array('where', 'group_by')) && isset($fields[$definition])) {
          $new_field = $table_alias . '.' . $fields[$definition];
          $value = $this->preserveWhitespace($new_field, $value);
        }
        elseif (in_array($section, array('where', 'group_by')) && isset($fields[$definition_simple])) {
          // Use the field without join conditions.
          
          // TODO: Make this work with arguments and sets.
          
          $t1 = isset($this->state_holder->token_types[$i + 1]) ? $this->state_holder->token_types[$i + 1] : '';
          $t2 = isset($this->state_holder->token_types[$i + 2]) ? $this->state_holder->token_types[$i + 2] : '';
          $t3 = isset($this->state_holder->token_types[$i + 3]) ? $this->state_holder->token_types[$i + 3] : '';
          switch ($t1) {
            case 'where_e':
            case 'where_ne':
            case 'where_gt':
            case 'where_lt':
            case 'where_gte':
            case 'where_lte':
              if (($t2 == 'numeric' || $t2 == 'string') && ($t3 != 'where_plus' && $t3 != 'where_minus')) {
                $new_token_types[] = 'where_paren_open';
                $new_token_values[] = $this->preserveWhitespace('(', $value);
                $new_token_types[] = 'field';
                $new_token_values[] = $table_alias . '.' . $fields[$definition_simple];
                $new_token_types[] = $this->state_holder->token_types[$i + 1];
                $new_token_values[] = $this->state_holder->token_values[$i + 1];
                $new_token_types[] = $this->state_holder->token_types[$i + 2];
                $new_token_values[] = $this->state_holder->token_values[$i + 2];
                $new_token_types[] = 'where_and';
                $new_token_values[] = ' AND ';
                $new_token_types[] = 'field';
                $new_token_values[] = $value;
                $new_token_types[] = $this->state_holder->token_types[$i + 1];
                $new_token_values[] = $this->state_holder->token_values[$i + 1];
                $new_token_types[] = $this->state_holder->token_types[$i + 2];
                $new_token_values[] = $this->state_holder->token_values[$i + 2];
                $new_token_types[] = 'where_paren_close';
                $new_token_values[] = ')';
                $i += 2;
                continue(2);
              }
              break;
            case 'where_like':
              if ($t3 == 'escape') {
                // convert "field LIKE string ESCAPE string" to
                // "(shadowfield LIKE string ESCAPE string AND origfield LIKE string ESCAPE string)"
                $new_token_types[] = 'where_paren_open';
                $new_token_values[] = $this->preserveWhitespace('(', $value);
                $new_token_types[] = 'field';
                $new_token_values[] = $this->preserveWhitespace($table_alias . '.' . $fields[$definition_simple], $value);
                $new_token_types[] = $this->state_holder->token_types[$i + 1];
                $new_token_values[] = $this->state_holder->token_values[$i + 1];
                $new_token_types[] = $this->state_holder->token_types[$i + 2];
                $new_token_values[] = $this->state_holder->token_values[$i + 2];
                $new_token_types[] = $this->state_holder->token_types[$i + 3];
                $new_token_values[] = $this->state_holder->token_values[$i + 3];
                $new_token_types[] = $this->state_holder->token_types[$i + 4];
                $new_token_values[] = $this->state_holder->token_values[$i + 4];
                $new_token_types[] = 'where_and';
                $new_token_values[] = ' AND ';
                $new_token_types[] = 'field';
                $new_token_values[] = $value;
                $new_token_types[] = $this->state_holder->token_types[$i + 1];
                $new_token_values[] = $this->state_holder->token_values[$i + 1];
                $new_token_types[] = $this->state_holder->token_types[$i + 2];
                $new_token_values[] = $this->state_holder->token_values[$i + 2];
                $new_token_types[] = $this->state_holder->token_types[$i + 3];
                $new_token_values[] = $this->state_holder->token_values[$i + 3];
                $new_token_types[] = $this->state_holder->token_types[$i + 4];
                $new_token_values[] = $this->state_holder->token_values[$i + 4];
                $new_token_types[] = 'where_paren_close';
                $new_token_values[] = ')';
                $i += 4;
                continue(2);
              }
              else {
                // convert "field LIKE string" to "(shadowfield LIKE string AND origfield LIKE string)"
                $new_token_types[] = 'where_paren_open';
                $new_token_values[] = $this->preserveWhitespace('(', $value);
                $new_token_types[] = 'field';
                $new_token_values[] = $this->preserveWhitespace($table_alias . '.' . $fields[$definition_simple], $value);
                $new_token_types[] = $this->state_holder->token_types[$i + 1];
                $new_token_values[] = $this->state_holder->token_values[$i + 1];
                $new_token_types[] = $this->state_holder->token_types[$i + 2];
                $new_token_values[] = $this->state_holder->token_values[$i + 2];
                $new_token_types[] = 'where_and';
                $new_token_values[] = ' AND ';
                $new_token_types[] = 'field';
                $new_token_values[] = $value;
                $new_token_types[] = $this->preserveWhitespace($this->state_holder->token_types[$i + 1], $value);
                $new_token_values[] = $this->state_holder->token_values[$i + 1];
                $new_token_types[] = $this->state_holder->token_types[$i + 2];
                $new_token_values[] = $this->state_holder->token_values[$i + 2];
                $new_token_types[] = 'where_paren_close';
                $new_token_values[] = ')';
                $i += 2;
                continue(2);
              }
              break;
          }
        }
        
        // Replace sort fields.
        if ($section == 'order_by') {
          if (isset($this->state_holder->token_types[$i + 1])
           && $this->state_holder->token_types[$i + 1] == 'order_by_desc') {
            // Descending, check for an inverted column first.
            if (isset($fields["$definition*"])) {
              $this->state_holder->token_types[$i + 1] = 'order_by_asc';
              $this->state_holder->token_values[$i + 1] = $this->preserveWhitespace('ASC', $this->state_holder->token_values[$i + 1]);
              $new_field = $table_alias . '.' . $fields["$definition*"];
              $value = $this->preserveWhitespace($new_field, $value);
            }
            elseif (isset($fields[$definition])) {
              $new_field = $table_alias . '.' . $fields[$definition];
              $value = $this->preserveWhitespace($new_field, $value);
            }
          }
          else {
            // Ascending, check for the normal column first.
            if (isset($fields[$definition])) {
              $new_field = $table_alias . '.' . $fields[$definition];
              $value = $this->preserveWhitespace($new_field, $value);
            }
            elseif (isset($fields["$definition*"])) {
              $this->state_holder->token_types[$i + 1] = 'order_by_desc';
              $this->state_holder->token_values[$i + 1] = $this->preserveWhitespace('DESC', $this->state_holder->token_values[$i + 1]);
              $new_field = $table_alias . '.' . $fields["$definition*"];
              $value = $this->preserveWhitespace($new_field, $value);
            }
          }
        }
      }
      
      $new_token_types[] = $type;
      $new_token_values[] = $value;
    }
    
    $this->state_holder->token_types = $new_token_types;
    $this->state_holder->token_values = $new_token_values;
  }
  
  /**
   * Get SQL code.
   */
  public function __toString() {
    return implode($this->state_holder->token_values);
  }
  
  /**
   * Get base table.
   */
  public function getBaseTable() {
    return $this->base_table;
  }
  
  /**
   * Get base table.
   */
  public function getFilterFields() {
    return $this->filter_fields;
  }
  
  /**
   * Get base table.
   */
  public function getSortFields() {
    return $this->sort_fields;
  }
  
  /**
   * Get base table.
   */
  public function getFilters() {
    return $this->filters;
  }
  
  /**
   * Get name without whitespace, quotes and backticks.
   * 
   * @param string $str
   * @return string
   */
  private function rawName($str) {
    return preg_replace('/[^a-z0-9_\\.]/si', '', $str);
  }
  
  /**
   * Add surrounding whitespace from old token to new token.
   * 
   * @param string $new New token w/o whitespace
   * @param string $old Old token w/ whitespace
   * @return string New token w/ whitespace
   */
  private function preserveWhitespace($new, $old) {
    if (preg_match('/^([\\s]*).+?([\\s]*)$/', $old, $match)) {
      return $match[1] . $new . $match[2];
    }
    else {
      return $new;
    }
  }
}
