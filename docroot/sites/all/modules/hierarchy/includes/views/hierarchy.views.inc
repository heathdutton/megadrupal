<?php

/**
 * @file
 * Provides support for the Views module.
 */

/**
 * Implements hook_field_views_data().
 */
function hierarchy_field_views_data($field) {
  $data = field_views_field_default_views_data($field);

  if ($field['type'] !== 'hierarchy_position') {
    return $data;
  }

  $group = t('Hierarchy');

  $field_name = $field['field_name'];
  list($field_label) = field_views_field_label($field['field_name']);
  $table_name = 'field_data_' . $field_name;

  if (isset($data[$table_name])) {
    if (!empty($data[$table_name]['table']['join'])) {
      foreach (array_keys($data[$table_name]['table']['join']) as $base_table) {
        $data[$table_name]['table']['join'][$base_table]['handler'] = 'hierarchy_position_views_join';
      }
    }
    $data[$table_name][$field_name . '_relation_id']['relationship'] = array(
      'title' => t('Hierarchy relation from !field_name', array('!field_name' => $field['field_name'])),
      'label' => t('Hierarchy relation from !field_name', array('!field_name' => $field['field_name'])),
      'group' => $group,
      'handler' => 'hierarchy_views_relationship_handler',
      'base' => 'hierarchy_relation',
      'base field' => 'rid',
      'field_name' => $field['field_name'],
    );

    $data[$table_name][$field_name . '_hid']['relationship'] = array(
      'title' => t('Hierarchy from !field_name', array('!field_name' => $field['field_name'])),
      'label' => t('Hierarchy from !field_name', array('!field_name' => $field['field_name'])),
      'group' => $group,
      'handler' => 'hierarchy_views_relationship_handler',
      'base' => 'hierarchy',
      'base field' => 'hid',
      'field_name' => $field['field_name'],
    );

    foreach (field_info_field_map() as $other_field_name => $other_field_info) {
      if ($other_field_info['type'] !== 'hierarchy_position') {
        continue;
      }
      foreach (array_keys($other_field_info['bundles']) as $entity_type) {
        $entity_info = entity_get_info($entity_type);
        $entity = $entity_info['label'];
        if ($entity == t('Node')) {
          $entity = t('Content');
        }
        $title = t('@entity hierarchy parent with @field (@field_name)', array(
          '@entity' => $entity,
          '@field' => $field_label,
          '@field_name' => $field['field_name'],
        ));
        $data[$table_name][$field_name . '_parent_id_' . $entity_type]['relationship'] = array(
          'title' => $title,
          'label' => $title,
          'help' => $title,
          'group' => $group,
          'handler' => 'hierarchy_views_relationship_handler',
          'relationship field' => $field['field_name'] . '_parent_id',
          'relationship table' => _field_sql_storage_tablename($field),
          'base' => $entity_info['base table'],
          'base field' => $entity_info['entity keys']['id'],
          'field_name' => $field['field_name'],
          'parent_type' => $entity_type,
        );
      }
    }
  }

  foreach ($field['bundles'] as $entity_type => $bundles) {
    $entity_info = entity_get_info($entity_type);
    $pseudo_field_name = $field['field_name'] . '_' . $entity_type;

    $entity = $entity_info['label'];
    if ($entity == t('Node')) {
      $entity = t('Content');
    }

    $data['hierarchy_relation'][$pseudo_field_name]['relationship'] = array(
      'title' => t('@entity with @field (@field_name)', array(
        '@entity' => $entity,
        '@field' => $field_label,
        '@field_name' => $field['field_name'],
      )),
      'help' => t('Relate each @entity using @field.', array('@entity' => $entity, '@field' => $field_label)),
      'label' => t('!field_name', array('!field_name' => $field['field_name'])),
      'group' => $group,
      'handler' => 'views_handler_relationship_entity_reverse',
      'field_name' => $field['field_name'],
      'field table' => _field_sql_storage_tablename($field),
      'field field' => $field['field_name'] . '_relation_id',
      'base' => $entity_info['base table'],
      'base field' => $entity_info['entity keys']['id'],
      'parent_type' => $entity_type,
    );
  }

  return $data;
}

class HierarchyViewsController extends EntityDefaultViewsController {

  public function views_data() {
    $data = parent::views_data();

    return $data;
  }
}

class HierarchyRelationViewsController extends EntityDefaultViewsController {

  public function views_data() {
    $data = parent::views_data();

    return $data;
  }
}

class hierarchy_position_views_join extends views_join {

  function build_join($select_query, $table, $view_query) {
    if (empty($this->definition['table formula'])) {
      $right_table = $this->table;
    }
    else {
      $right_table = $this->definition['table formula'];
    }

    if ($this->left_table) {
      $left = $view_query->get_table_info($this->left_table);
      $left_field = "$left[alias].$this->left_field";
    }
    else {
      // This can be used if left_field is a formula or something.
      // It should be used only *very* rarely.
      $left_field = $this->left_field;
    }

    $condition = "$left_field = $table[alias].$this->field";
    $arguments = array();

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
            // If we're aware of a table alias for this table, use the table
            // alias instead of the table name.
            if (isset($left) && $left['table'] == $info['table']) {
              $join_table = $left['alias'] . '.';
            }
            else {
              $join_table = $info['table'] . '.';
            }
          }

          // Convert a single-valued array of values to the single-value case,
          // and transform from IN() notation to = notation.
          if (is_array($info['value']) && count($info['value']) == 1) {
            if (empty($info['operator'])) {
              $operator = '=';
            }
            else {
              $operator = $info['operator'] == 'NOT IN' ? '!=' : '=';
            }
            $info['value'] = array_shift($info['value']);
          }

          if (is_array($info['value'])) {
            // With an array of values, we need multiple placeholders and the
            // 'IN' operator is implicit.
            foreach ($info['value'] as $value) {
              $placeholder_i = $view_query->placeholder('views_join_condition_');
              $arguments[$placeholder_i] = $value;
            }

            $operator = !empty($info['operator']) ? $info['operator'] : 'IN';
            $placeholder = '( ' . implode(', ', array_keys($arguments)) . ' )';
          }
          else {
            // With a single value, the '=' operator is implicit.
            $operator = !empty($info['operator']) ? $info['operator'] : '=';
            if (!is_null($info['value'])) {
              $placeholder = $view_query->placeholder('views_join_condition_');
              $arguments[$placeholder] = $info['value'];
            }
            else {
              $placeholder = '';
            }
          }
          $extras[] = "$join_table$info[field] $operator $placeholder";
        }

        if ($extras) {
          if (count($extras) == 1) {
            $condition .= ' AND ' . array_shift($extras);
          }
          else {
            $condition .= ' AND (' . implode(' ' . $this->extra_type . ' ', $extras) . ')';
          }
        }
      }
      elseif ($this->extra && is_string($this->extra)) {
        $condition .= " AND ($this->extra)";
      }
    }

    $select_query->addJoin($this->type, $right_table, $table['alias'], $condition, $arguments);
  }

}
