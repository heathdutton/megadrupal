<?php

/**
 * @file
 * Data operations file.
 *
 * Contains functions that interact with database.
 */

define('EC_FORM_NODE', 'node_admin_content');
define('EC_FORM_COMMENT', 'comment_admin_overview');
define('EC_FORM_USER', 'user_admin_account');

define('EC_SETTING_HEADER_NO_WRAP', 'extra_columns_header_no_wrap');
define('EC_SETTING_CELL_NO_WRAP', 'extra_columns_cell_no_wrap');
define('EC_SETTING_DATE_FORMAT', 'extra_columns_date_format');

define('EC_SETTING_FORM_NODE', 'extra_columns_form_node');
define('EC_SETTING_FORM_COMMENT', 'extra_columns_form_comment');
define('EC_SETTING_FORM_USER', 'extra_columns_form_user');

define('EC_FIELD_COMMENT_COUNT', 'extra_columns_field_node_comment_count');
define('EC_FIELD_COMMENT_HOST', 'extra_columns_field_comment_host');
define('EC_FIELD_COMMENT_MAIL', 'extra_columns_field_comment_mail');

define('EC_FIELD_NODE_COUNT', 'extra_columns_field_node_count');

/**
 * Saves columns data into database according to $form_id.
 *
 * @param string $form_id
 *   the form id
 * @param array $data
 *   column data
 */
function extra_columns_set_form_columns($form_id, $data) {
  variable_set('extra_columns_fields_' . $form_id, $data);
}

/**
 * Get column data according to $form_id.
 *
 * @param string $form_id
 *   the form id
 *
 * @return array
 *   the columns
 */
function extra_columns_get_form_columns($form_id) {
  return variable_get('extra_columns_fields_' . $form_id, array());
}

/**
 * Retrieves columns that will be added to a table.
 *
 * @param string $form_id
 *   the form id
 *
 * @return array
 *   columns to be displayed
 */
function extra_columns_get_extra_columns($form_id) {
  $columns = array();

  $extra_columns = extra_columns_get_form_columns($form_id);

  if (!empty($extra_columns)) {
    foreach ($extra_columns as $column) {
      $columns[$column] = array(
        'data'  => extra_columns_get_field_label($column),
        'class' => 'extra_columns_column',
      );
    }
  }

  return $columns;
}

/**
 * Gets the value of a column.
 *
 * @param string $field
 *   name of the field
 * @param string $form_id
 *   the form id
 * @param int $id
 *   the id of the entity/row(node, comment, or user)
 *
 * @return string
 *   value of field
 */
function extra_columns_get_column_value($field, $form_id, $id) {
  $value = '';
  $field_info = field_info_field($field);

  // Get the entity.
  $entity = NULL;

  switch ($form_id) {
    case EC_FORM_NODE:
      $entity = node_load($id);
      break;

    case EC_FORM_COMMENT:
      $entity = comment_load($id);
      break;

    case EC_FORM_USER:
      $entity = user_load($id);
      break;

  }

  // Handing field values.
  if (!empty($field_info)) {
    if (!empty($entity) && !empty($entity->{$field})) {
      $temp = array();

      switch ($field_info['type']) {
        case 'image':
          foreach ($entity->{$field}[LANGUAGE_NONE] as $file) {
            $value .= theme('image', array(
              'path'   => file_create_url($file['uri']),
              'width'  => 50,
              'height' => 50,
            ));
          }
          break;

        case 'file':
          foreach ($entity->{$field}[LANGUAGE_NONE] as $f) {
            $temp[] = l($f['filename'], file_create_url($f['uri']), array(
              'attributes' => array('target' => 'blank'),
            ));
          }
          $value = implode(', ', $temp);
          break;

        case 'list_boolean':
          foreach ($entity->{$field}[LANGUAGE_NONE] as $bool) {
            $temp[] = $bool ? t('Yes') : t('No');
          }
          $value = implode(', ', $temp);
          break;

        case 'text':
        case 'text_long':
        case 'text_with_summary':
          $value = check_plain(truncate_utf8($entity->{$field}[LANGUAGE_NONE][0]['value'], 50, 1, 1));
          break;

        case 'number_integer':
        case 'number_float':
        case 'list_text':
        case 'number_decimal':
          foreach ($entity->{$field}[LANGUAGE_NONE] as $dec) {
            $temp[] = $dec['value'];
          }
          $value = implode(', ', $temp);
          break;

        case 'taxonomy_term_reference':
          foreach ($entity->{$field}[LANGUAGE_NONE] as $t) {
            $term = taxonomy_term_load($t['tid']);
            if ($term) {
              $temp[] = l($term->name, 'taxonomy/term/' . $term->tid);
            }
          }
          $value = implode(', ', $temp);
          break;

        case 'datestamp':
        case 'datetime':
        case 'date':
          foreach ($entity->{$field}[LANGUAGE_NONE] as $i => $t) {
            $start_time = !empty($t['value']) ? $t['value'] : 0;
            $end_time = !empty($t['value2']) ? $t['value2'] : 0;

            $start_time = $start_time && !is_numeric($t['value']) ? strtotime($t['value']) : 0;
            $end_time = $end_time && !is_numeric($t['value2']) ? strtotime($t['value2']) : 0;

            $temp[$i] = format_date($start_time, variable_get(EC_SETTING_DATE_FORMAT, 'medium'));
            $temp[$i] .= $end_time ? ' to ' . format_date($end_time, variable_get(EC_SETTING_DATE_FORMAT, 'medium')) : '';
          }
          $value = implode(',<br/>', $temp);
          break;

        default:
          $value = '-';
          break;
      }
    }
  }
  else {
    // Handling non-field values.
    switch ($field) {
      case EC_FIELD_COMMENT_COUNT:
        $query = db_select('comment', 'c');
        $query->groupBy('c.status');

        if ($form_id == EC_FORM_NODE) {
          $query->condition('c.nid', $entity->nid);
        }
        elseif ($form_id == EC_FORM_USER) {
          $query->condition('c.uid', $entity->uid);
        }

        $query->addExpression('COUNT(c.cid)', 'count');
        $query->fields('c', array('status'));
        $result = $query->execute()->fetchAll();

        $counts = array();
        $total = 0;
        foreach ($result as $r) {
          $counts[$r->status] = $r->count;
          $total += $r->count;
        }
        $value .= t('Published: @count', array('@count' => (isset($counts[COMMENT_PUBLISHED]) ? $counts[COMMENT_PUBLISHED] : '0'))) . '<br/>';
        $value .= t('Unpublished: @count', array('@count' => (isset($counts[COMMENT_NOT_PUBLISHED]) ? $counts[COMMENT_NOT_PUBLISHED] : '0'))) . '<br/>';
        $value .= t('Total: @count', array('@count' => $total));

        break;

      case EC_FIELD_NODE_COUNT:
        $query = db_query('SELECT status, COUNT(nid) AS count FROM {node} WHERE uid = :uid GROUP BY status',
          array(':uid' => $entity->uid)
        );

        $counts = array();
        $total = 0;
        while ($r = $query->fetch()) {
          $counts[$r->status] = $r->count;
          $total += $r->count;
        }
        $value .= t('Published: @count', array('@count' => (isset($counts[NODE_PUBLISHED]) ? $counts[NODE_PUBLISHED] : '0'))) . '<br/>';
        $value .= t('Unpublished: @count', array('@count' => (isset($counts[NODE_NOT_PUBLISHED]) ? $counts[NODE_NOT_PUBLISHED] : '0'))) . '<br/>';
        $value .= t('Total: @count', array('@count' => $total));

        break;

      case EC_FIELD_COMMENT_HOST:
        $value .= check_plain($entity->hostname);
        break;

      case EC_FIELD_COMMENT_MAIL:
        $value .= check_plain($entity->mail);
        break;

      default:
        $value = '';
        break;

    }
  }

  return $value;
}

/**
 * Gets the label for a field.
 *
 * @param string $field
 *   name of the field
 *
 * @return string
 *   label of the field
 */
function extra_columns_get_field_label($field, $glue = ',<br/>') {
  $labels = array();

  switch ($field) {
    case EC_FIELD_COMMENT_COUNT:
      $labels[] = t('Comment Count');
      break;

    case EC_FIELD_COMMENT_HOST:
      $labels[] = t('Author IP');
      break;

    case EC_FIELD_COMMENT_MAIL:
      $labels[] = t('Author E-mail');
      break;

    case EC_FIELD_NODE_COUNT:
      $labels[] = t('Node Count');
      break;

    default:
      $query = db_query("SELECT field_name, data, bundle FROM {field_config_instance} WHERE field_name = :field",
        array(':field' => $field)
      );

      while ($i = $query->fetch()) {
        $data = unserialize($i->data);

        if (!empty($data) && is_array($data)) {
          $labels[] = $data['label'] . ' (' . $i->bundle . ')';
        }
      }
      break;
  }

  foreach ($labels as &$label) {
    $label = check_plain($label);
  }

  return implode($glue, $labels);
}

/**
 * Inserts an element in front of a specific element by key in an array.
 *
 * @param array $array
 *   the original array
 * @param string $key
 *   which key to insert in front
 * @param array $new
 *   the array to insert
 *
 * @return array
 *   the updated array
 */
function extra_columns_array_before($array, $key, $new) {
  $keys = array_keys($array);
  $pos = (int) array_search($key, $keys);

  return array_merge(
    array_slice($array, 0, $pos),
    $new,
    array_slice($array, $pos)
  );
}

/**
 * Get form table header and rows.
 *
 * @param string $form_id
 *   form id
 *
 * @param array $form
 *   form array
 *
 * @return array
 *   form table data
 */
function extra_columns_get_form_data($form_id, $form) {
  $data = array(
    'header' => NULL,
    'rows'   => NULL,
    'do_ec'  => FALSE,
  );

  switch ($form_id) {
    case EC_FORM_NODE:
      if (variable_get(EC_SETTING_FORM_NODE, FALSE)) {
        $data = array(
          'header' => $form['admin']['nodes']['#header'],
          'rows'   => $form['admin']['nodes']['#options'],
          'do_ec'  => TRUE,
        );
      }
      break;

    case EC_FORM_COMMENT:
      if (variable_get(EC_SETTING_FORM_COMMENT, FALSE)) {
        $data = array(
          'header' => $form['comments']['#header'],
          'rows'   => $form['comments']['#options'],
          'do_ec'  => TRUE,
        );
      }
      break;

    case EC_FORM_USER:
      if (variable_get(EC_SETTING_FORM_USER, FALSE)) {
        $data = array(
          'header' => $form['accounts']['#header'],
          'rows'   => $form['accounts']['#options'],
          'do_ec'  => TRUE,
        );
      }
      break;
  }

  return $data;
}

/**
 * Set header, rows, and style of form.
 *
 * @param string $form_id
 *   form id
 *
 * @param array $form
 *   the form to modify
 *
 * @param array $header
 *   table header
 *
 * @param array $rows
 *   table rows
 */
function extra_columns_set_form_data($form_id, $form, $header, $rows) {
  switch ($form_id) {
    case EC_FORM_NODE:
      // Set header and rows.
      $form['admin']['nodes']['#header'] = $header;
      $form['admin']['nodes']['#options'] = $rows;

      // Set the add column link.
      if (empty($form['admin']['nodes']['#prefix'])) {
        $form['admin']['nodes']['#prefix'] = '';
      }
      $form['admin']['nodes']['#prefix'] .= extra_columns_add_column_link($form_id);

      if (variable_get(EC_SETTING_HEADER_NO_WRAP, FALSE)) {
        $form['admin']['nodes']['#attributes']['class'][] = 'extra_column_header_no_wrap';
      }

      if (variable_get(EC_SETTING_CELL_NO_WRAP, FALSE)) {
        $form['admin']['nodes']['#attributes']['class'][] = 'extra_column_cell_no_wrap';
      }
      break;

    case EC_FORM_COMMENT:
      $form['comments']['#header'] = $header;
      $form['comments']['#options'] = $rows;

      // Set the add column link.
      if (empty($form['comments']['#prefix'])) {
        $form['comments']['#prefix'] = '';
      }
      $form['comments']['#prefix'] .= extra_columns_add_column_link($form_id);

      if (variable_get(EC_SETTING_HEADER_NO_WRAP, FALSE)) {
        $form['comments']['#attributes']['class'][] = 'extra_column_header_no_wrap';
      }

      if (variable_get(EC_SETTING_CELL_NO_WRAP, FALSE)) {
        $form['comments']['#attributes']['class'][] = 'extra_column_cell_no_wrap';
      }
      break;

    case EC_FORM_USER:
      $form['accounts']['#header'] = $header;
      $form['accounts']['#options'] = $rows;

      // Set the add column link.
      if (empty($form['accounts']['#prefix'])) {
        $form['accounts']['#prefix'] = '';
      }
      $form['accounts']['#prefix'] .= extra_columns_add_column_link($form_id);

      if (variable_get(EC_SETTING_HEADER_NO_WRAP, FALSE)) {
        $form['accounts']['#attributes']['class'][] = 'extra_column_header_no_wrap';
      }

      if (variable_get(EC_SETTING_CELL_NO_WRAP, FALSE)) {
        $form['accounts']['#attributes']['class'][] = 'extra_column_cell_no_wrap';
      }
      break;
  }

  return $form;
}

/**
 * Returns add extra column link html.
 *
 * @param string $form_id
 *   form id
 *
 * @return string
 *   fieldset html
 */
function extra_columns_add_column_link($form_id) {
  return theme('fieldset', array(
    'element' => array(
      '#title'    => t('Extra Columns'),
      '#children' => l(t('Add Extra Columns'), 'admin/extra_columns/fields/' . $form_id, array(
        'attributes' => array(
          'id'    => 'extra_columns_add',
          'class' => 'button',
        ),
      )),
    ),
  ));
}

/**
 * Returns forms that are enabled.
 *
 * @return array
 *   any form enabled
 */
function extra_columns_forms_enabled() {
  $form = array();
  if (variable_get(EC_SETTING_FORM_NODE, FALSE)) {
    $form[] = EC_FORM_NODE;
  }

  if (variable_get(EC_SETTING_FORM_COMMENT, FALSE)) {
    $form[] = EC_FORM_COMMENT;
  }

  if (variable_get(EC_SETTING_FORM_USER, FALSE)) {
    $form[] = EC_FORM_USER;
  }

  return $form;
}

/**
 * Check if current page is one of the forms.
 *
 * @return bool
 *   within a form page
 */
function extra_columns_form_within() {
  $current_path = current_path();

  if ($current_path == 'admin/content') {
    return EC_FORM_NODE;
  }
  elseif ($current_path == 'admin/content/comment') {
    return EC_FORM_COMMENT;
  }
  elseif ($current_path == 'admin/people') {
    return EC_FORM_USER;
  }

  return FALSE;
}
