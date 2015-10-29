<?php

/**
 * @file
 * Implemention of views for ec_receipts module.
 */

/**
 * Implements hook_views_data().
 */
function ec_receipt_views_data() {
  $data = array();

  $data['ec_receipt']['table']['group'] = t('Receipts');

  $data['ec_receipt']['table']['base'] = array(
    'field' => 'erid',
    'title' => t('Receipt'),
    'help' => t('A unique identifier for each receipt'),
  );

  $data['ec_receipt']['table']['join'] = array(
    'ec_receipt_allocation' => array(
      'left_field' => 'erid',
      'field' => 'erid',
      'type' => 'INNER',
    ),
  );

  $data['ec_receipt']['erid'] = array(
    'title' => t('Receipt no.'),
    'help' => t('Provides Unique identifier for each receipt'),
    'field' => array(
      'field' => 'erid',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['ec_receipt']['type'] = array(
    'title' => t('Type'),
    'help' => t('Provides the type of receipt'),
    'field' => array(
      'handler' => 'ec_receipt_views_handler_field_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'ec_receipt_views_handler_filter_type',
    ),
  );
  $data['ec_receipt']['currency'] = array(
    'title' => t('Currency'),
    'help' => t('Provides the currency of the receipt'),
    'field' => array(
      'field' => 'currency',
      'click sortable' => TRUE,
    ),
  );
  $data['ec_receipt']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('Provides the amount of the receipt'),
    'field' => array(
      'handler' => 'ec_common_views_handler_field_format_currency',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_float',
    ),
  );
  $data['ec_receipt']['allocated'] = array(
    'title' => t('Allocated'),
    'help' => t('Provides the amount allocated of the receipt'),
    'field' => array(
      'handler' => 'ec_common_views_handler_field_format_currency',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_float',
    ),
  );
  $data['ec_receipt']['balance'] = array(
    'title' => t('Balance'),
    'help' => t('Provides the balance of the receipt'),
    'field' => array(
      'handler' => 'ec_common_views_handler_field_format_currency',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_float',
    ),
  );
  $data['ec_receipt']['status'] = array(
    'title' => t('Status'),
    'help' => t('Provides the status of the receipt'),
    'field' => array(
      'handler' => 'ec_receipt_views_handler_field_status',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'ec_receipt_views_handler_filter_status',
    ),
  );
  $data['ec_receipt']['created'] = array(
    'title' => t('Created'),
    'help' => t('Provides the created date of the receipt'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),    
  );
  $data['ec_receipt']['changed'] = array(
    'title' => t('Changed'),
    'help' => t('Provides the last change date of the receipt'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  $data['ec_receipt']['operations'] = array(
    'title' => t('Operations'),
    'help' => t('Provides links for doing other functions'),
    'field' => array(
      'handler' => 'ec_receipt_views_handler_field_operations',
      'real field' => 'erid',
    ),
  );
  $data['ec_receipt']['views_bulk_operations'] = array(
    'title' => t('Bulk operations'),
    'help' => t('Provide a checkbox to select the row for bulk operations.'),
    'real field' => 'erid',
    'field' => array(
      'handler' => 'views_bulk_operations_handler_field_operations',
      'click sortable' => FALSE,
    ),
  );
  

  $data['ec_receipt_allocation']['table']['group'] = t('Receipt allocation');

  $data['ec_receipt_allocation']['table']['base'] = array(
    'field' => 'eaid',
    'title' => t('Receipt Allocation'),
    'help' => t('Show allocation of Receipts'),
  );

  $data['ec_receipt_allocation']['table']['join'] = array(
    'ec_receipt' => array(
      'left_field' => 'erid',
      'field' => 'erid',
      'type' => 'LEFT',
    ),
    'ec_transaction' => array(
      'left_field' => 'etid',
      'field' => 'txnid',
      'type' => 'LEFT',
    ),
    'ec_transaction_address' => array(
      'left_field' => 'etid',
      'field' => 'txnid',
      'type' => 'LEFT',
    ),
  );

  $data['ec_receipt_allocation']['eaid'] = array(
    'title' => t('Allocation id'),
    'help' => t('Provides the unique identifier for each allocation'),
    'field' => array(
      'field' => 'eaid',
      'click sortable' => TRUE,
    ),
  );
  $data['ec_receipt_allocation']['erid'] = array(
    'title' => t('Receipt Id'),
    'help' => t('Provides an argument for allocations by receipt id.'),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['ec_receipt_allocation']['type'] = array(
    'title' => t('Type'),
    'help' => t('Provides the type of object that the receipt was allocated too'),
    'field' => array(
      'handler' => 'ec_receipt_views_handler_field_allocation_type',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'ec_receipt_views_handler_filter_allocation_type',
    ),
  );
  $data['ec_receipt_allocation']['etid'] = array(
    'title' => t('External id'),
    'help' => t('Provides th external id of the object type the receipt was allocated to.'),
    'field' => array(
      'field' => 'etid',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['ec_receipt_allocation']['created'] = array(
    'title' => t('Created'),
    'help' => t('Provides the created date of the receipt allocation'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  $data['ec_receipt_allocation']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('Provides the amount of the receipt allocation'),
    'field' => array(
      'handler' => 'ec_common_views_handler_field_format_currency',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );
  $data['ec_receipt_allocation']['reversed'] = array(
    'title' => t('Reversed'),
    'help' => t('Provides reversed flag status'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
  );
  $data['ec_receipt_allocation']['operations'] = array(
    'title' => t('Operations'),
    'help' => t('Provides links for doing other functions'),
    'field' => array(
      'handler' => 'ec_receipt_views_handler_field_allocation_operations',
      'real field' => 'eaid',
    ),
  );
  $data['ec_receipt_allocation']['views_bulk_operations'] = array(
    'title' => t('Bulk operations'),
    'help' => t('Provide a checkbox to select the row for bulk operations.'),
    'real field' => 'eaid',
    'field' => array(
      'handler' => 'views_bulk_operations_handler_field_operations',
      'click sortable' => FALSE,
    ),
  );
  

  if (module_exists('search')) {
    $data['search_index']['table']['join'] = array(
      'ec_receipt' => array(
        'left_field' => 'erid',
        'field' => 'sid',
      ),
    );

    $data['search_total']['table']['join'] = array(
      'ec_receipt' => array(
        'left_table' => 'search_index',
        'left_field' => 'word',
        'field' => 'word',
      ),
    );

    $data['search_dataset']['table']['join'] = array(
      'ec_receipt' => array(
        'left_table' => 'search_index',
        'left_field' => 'sid',
        'field' => 'sid',
        'extra' => 'search_index.type = search_dataset.type',
        'type' => 'INNER',
      ),
    );
  }

  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function ec_receipt_views_plugins() {
  return array(
    'module' => 'ec_receipt',
    'argument default' => array(
      'receipt_id' => array(
        'title' => t('Receipt Id from URL'),
        'handler' => 'ec_receipt_views_plugin_argument_default_receipt',
        'path' => drupal_get_path('module', 'ec_receipt') . '/views',
        'parent' => 'fixed',
      ),
    ),
  );
}

/**
 * Implements hook_views_handler().
 */
function ec_receipt_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'ec_receipt') . '/views',
    ),
    'handlers' => array(
      'ec_receipt_views_handler_field_type' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_receipt_views_handler_filter_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'ec_receipt_views_handler_field_status' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_receipt_views_handler_filter_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'ec_receipt_views_handler_field_operations' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_receipt_views_handler_field_allocation_type' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_receipt_views_handler_field_allocation_operations' => array(
        'parent' => 'views_handler_field',
      ),
      'ec_receipt_views_handler_filter_allocation_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
    ),
  );
}

/**
 * Allow fields in joins to be cast to a specific data type. This recognizes two
 * new elements in the join definition: left_cast and cast. The elements should
 * contain a type definition array in accordance with the Schema API.
 */
class ec_receipt_views_join_cast extends views_join {
  /**
   * Implementation of construct().
   */
  function construct($table = NULL, $left_table = NULL, $left_field = NULL, $field = NULL, $extra = array(), $type = 'LEFT', $left_cast = NULL, $cast = NULL) {
    parent::construct($table, $left_table, $left_field, $field, $extra, $type);
    if (!empty($table)) {
      if (!empty($left_cast)) {
        $this->left_cast = $left_cast;
      }
      if (!empty($cast)) {
        $this->cast = $cast;
      }
    }
    elseif (!empty($this->definition)) {
      if (!empty($this->definition['left_cast'])) {
        $this->left_cast = $this->definition['left_cast'];
      }
      if (!empty($this->definition['cast'])) {
        $this->cast = $this->definition['cast'];
      }
    }
  }

  /**
   * Create the cast clause based on the spec.
   */
  function cast_field_spec($field, $spec) {
    if (!isset($spec['size'])) {
      $spec['size'] = 'normal';
    }

    $type = explode(' ', $spec['type']);
    if (count($type) == 2) {
      $modifier = $type[0];
      $spec['type'] = $type[1];
    }

    $map = db_type_map();
    $cast = $map[$spec['type'] . ':' . $spec['size']];
    if (isset($spec['length'])) {
      $cast .= '(' . $spec['length'] . ')';
    }
    elseif (isset($spec['precision']) && isset($spec['scale'])) {
      $cast .= '(' . $spec['precision'] . ', ' . $spec['scale'] . ')';
    }

    if (isset($modifier)) {
      $cast = $modifier . ' ' . $cast;
    }

    return "CAST($field AS $cast)";
  }

  /**
   * Implementation of join().
   */
  function join($table, &$query) {
    if ($this->left_table) {
      $left = $query->get_table_info($this->left_table);
      $left_field = isset($this->left_cast) ? $this->cast_field_spec("$left[alias].$this->left_field", $this->left_cast) : "$left[alias].$this->left_field";
    }
    else {
      // This can be used if left_field is a formula or something. It should be used only *very* rarely.
      $left_field = isset($this->left_cast) ? $this->cast_field_spec($this->left_field, $this->left_cast) : $this->left_field;
    }
    $field = isset($this->cast) ? $this->cast_field_spec("$table[alias].$this->field", $this->cast) : "$table[alias].$this->field";
    $output = " $this->type JOIN {" . $this->table . "} $table[alias] ON $left_field = $field";
    // Tack on the extra.
    if (isset($this->extra)) {
      if (is_array($this->extra)) {
        $extras = array();
        foreach ($this->extra as $info) {
          $extra = '';
          // Figure out the table name. Remember, only use aliases provided
          // if at all possible.
          $join_table = '';
          if (!array_key_exists('table', $info)) {
            $join_table = $table['alias'] . '.';
          }
          elseif (isset($info['table'])) {
            $join_table = $info['table'] . '.';
          }

          // And now deal with the value and the operator.  Set $q to
          // a single-quote for non-numeric values and the
          // empty-string for numeric values, then wrap all values in $q.
          $raw_value = $this->db_safe($info['value']);
          $q = (empty($info['numeric']) ? "'" : '');

          if (is_array($raw_value)) {
            $operator = !empty($info['operator']) ? $info['operator'] : 'IN';
            // Transform from IN() notation to = notation if just one value.
            if (count($raw_value) == 1) {
              $value = $q . array_shift($raw_value) . $q;
              $operator = $operator == 'NOT IN' ? '!=' : '=';
            }
            else {
              $value = "($q" . implode("$q, $q", $raw_value) . "$q)";
            }
          }
          else {
            $operator = !empty($info['operator']) ? $info['operator'] : '=';
            $value = "$q$raw_value$q";
          }
          $extras[] = "$join_table$info[field] $operator $value";
        }

        if ($extras) {
          if (count($extras) == 1) {
            $output .= ' AND ' . array_shift($extras);
          }
          else {
            $output .= ' AND (' . implode(' ' . $this->extra_type . ' ', $extras) . ')';
          }
        }
      }
      elseif ($this->extra && is_string($this->extra)) {
        $output .= " AND ($this->extra)";
      }
    }
    return $output;
  }
}
