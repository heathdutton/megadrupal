<?php
/**
 * Returns HTML for a table.
 *
 * @param $variables
 *   An associative array containing:
 *   - header: An array containing the table headers. Each element of the array
 *     can be either a localized string or an associative array with the
 *     following keys:
 *     - "data": The localized title of the table column.
 *     - "field": The database field represented in the table column (required
 *       if user is to be able to sort on this column).
 *     - "sort": A default sort order for this column ("asc" or "desc").
 *     - Any HTML attributes, such as "colspan", to apply to the column header
 *       cell.
 *   - rows: An array of table rows. Every row is an array of cells, or an
 *     associative array with the following keys:
 *     - "data": an array of cells
 *     - Any HTML attributes, such as "class", to apply to the table row.
 *     - "no_striping": a boolean indicating that the row should receive no
 *       'even / odd' styling. Defaults to FALSE.
 *     Each cell can be either a string or an associative array with the
 *     following keys:
 *     - "data": The string to display in the table cell.
 *     - "header": Indicates this cell is a header.
 *     - Any HTML attributes, such as "colspan", to apply to the table cell.
 *     Here's an example for $rows:
 *     @code
 *     $rows = array(
 *       // Simple row
 *       array(
 *         'Cell 1', 'Cell 2', 'Cell 3'
 *       ),
 *       // Row with attributes on the row and some of its cells.
 *       array(
 *         'data' => array('Cell 1', array('data' => 'Cell 2', 'colspan' => 2)), 'class' => array('funky')
 *       )
 *     );
 *     @endcode
 *   - attributes: An array of HTML attributes to apply to the table tag.
 *   - caption: A localized string to use for the <caption> tag.
 *   - colgroups: An array of column groups. Each element of the array can be
 *     either:
 *     - An array of columns, each of which is an associative array of HTML
 *       attributes applied to the COL element.
 *     - An array of attributes applied to the COLGROUP element, which must
 *       include a "data" attribute. To add attributes to COL elements, set the
 *       "data" attribute with an array of columns, each of which is an
 *       associative array of HTML attributes.
 *     Here's an example for $colgroup:
 *     @code
 *     $colgroup = array(
 *       // COLGROUP with one COL element.
 *       array(
 *         array(
 *           'class' => array('funky'), // Attribute for the COL element.
 *         ),
 *       ),
 *       // Colgroup with attributes and inner COL elements.
 *       array(
 *         'data' => array(
 *           array(
 *             'class' => array('funky'), // Attribute for the COL element.
 *           ),
 *         ),
*         'class' => array('jazzy'), // Attribute for the COLGROUP element.
 *       ),
 *     );
 *     @endcode
 *     These optional tags are used to group and set properties on columns
 *     within a table. For example, one may easily group three columns and
 *     apply same background style to all.
 *   - sticky: Use a "sticky" table header.
 *   - empty: The message to display in an extra row if table does not have any
 *     rows.
 */
function jqueryui_vtcore_table($variables) {
  $header = $variables['header'];
  $rows = $variables['rows'];
  $attributes = $variables['attributes'];
  $caption = $variables['caption'];
  $colgroups = $variables['colgroups'];
  $sticky = $variables['sticky'];
  $empty = $variables['empty'];

  // Add jQuery UI classes
  $path = drupal_get_path('theme', BASE_THEME);
  $jqueryUI_table = 'ui-table ui-widget-content';
  $jqueryUI_colgroup = 'ui-table-colgroup';
  $jqueryUI_caption = 'ui-table-caption';
  $jqueryUI_empty = 'ui-table-empty';
  $jqueryUI_header = 'ui-table-header ui-state-default';
  $jqueryUI_body = 'ui-table-body';
  $jqueryUI_row = 'ui-table-row ui-widget-content';
  $jqueryUI_row_odd = 'ui-table-row-odd';
  $jqueryUI_row_even = 'ui-table-row-even';

  // Add extra css & js
  global $theme_key;
  _jqueryui_selected_theme_init('load', $theme_key);
  drupal_add_css($path . '/vtcore/plugins/jqueryui/css/table/table.css');
  drupal_add_js($path . '/vtcore/plugins/jqueryui/js/table/jquery.ui.table.js');

  // Add sticky headers, if applicable.
  if (count($header) && $sticky) {
    drupal_add_js('misc/tableheader.js');
    // Add 'sticky-enabled' class to the table to identify it for JS.
    // This is needed to target tables constructed by this function.
    $attributes['class'][] = 'sticky-enabled';
  }
  $table_attributes = $attributes;
  $table_attributes['class'][] = $jqueryUI_table;
  $output = '<table' . drupal_attributes($table_attributes) . ">\n";

  if (isset($caption)) {
    $caption_attributes = array('class' => $jqueryUI_caption);
    $output .= '<caption' . drupal_attributes($caption_attributes) . '>' . $caption . "</caption>\n";
  }

  // Format the table columns:
  if (count($colgroups)) {
    foreach ($colgroups as $number => $colgroup) {
      $attributes = array();

      // Check if we're dealing with a simple or complex column
      if (isset($colgroup['data'])) {
        foreach ($colgroup as $key => $value) {
          if ($key == 'data') {
            $cols = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $cols = $colgroup;
      }

      // Add jQueryUI colgroup class
      $attributes['class'][] = $jqueryUI_colgroup;

      // Build colgroup
      if (is_array($cols) && count($cols)) {
        $output .= ' <colgroup' . drupal_attributes($attributes) . '>';
        $i = 0;
        foreach ($cols as $col) {
          $output .= ' <col' . drupal_attributes($col) . ' />';
        }
        $output .= " </colgroup>\n";
      }
      else {
        $output .= ' <colgroup' . drupal_attributes($attributes) . " />\n";
      }
    }
  }

  // Add the 'empty' row message if available.
  if (!count($rows) && $empty) {
    $header_count = 0;
    foreach ($header as $header_cell) {
      if (is_array($header_cell)) {
        $header_count += isset($header_cell['colspan']) ? $header_cell['colspan'] : 1;
      }
      else {
        $header_count++;
      }
    }
    $rows[] = array(array('data' => $empty, 'colspan' => $header_count, 'class' => array('empty', 'message', $jqueryUI_empty)));
  }

  // Format the table header:
  if (count($header)) {
    $ts = tablesort_init($header);
    // Adding jQueryUI header class
    $header_attributes = array('class' => $jqueryUI_header);
    // HTML requires that the thead tag has tr tags in it followed by tbody
    // tags. Using ternary operator to check and see if we have any rows.
    $output .= (count($rows) ? ' <thead><tr' . drupal_attributes($header_attributes) . '>' : ' <tr' . drupal_attributes($header_attributes) . '>');
    foreach ($header as $cell) {
      $cell = tablesort_header($cell, $header, $ts);
      $output .= _theme_table_cell($cell, TRUE);
    }
    // Using ternary operator to close the tags based on whether or not there are rows
    $output .= (count($rows) ? " </tr></thead>\n" : "</tr>\n");
  }
  else {
    $ts = array();
  }

  // Format the table rows:
  if (count($rows)) {
    $body_attributes = array('class' => $jqueryUI_body);
    $output .= "<tbody" . drupal_attributes($body_attributes) . ">\n";
    $flip = array('even' => 'odd', 'odd' => 'even');
    $class = 'even';
    foreach ($rows as $number => $row) {
      $attributes = array();
      $attributes['class'][] = $jqueryUI_row;
      // Check if we're dealing with a simple or complex row
      if (isset($row['data'])) {
        foreach ($row as $key => $value) {
          if ($key == 'data') {
            $cells = $value;
          }
          else {
            $attributes[$key] = $value;
          }
        }
      }
      else {
        $cells = $row;
      }
      if (count($cells)) {
        // Add odd/even class
        if (empty($row['no_striping'])) {
          $class = $flip[$class];
          $attributes['class'][] = $class;
          $attributes['class'][] = ${'jqueryUI_row_' . $class};
        }

        // Build row
        $output .= ' <tr' . drupal_attributes($attributes) . '>';
        $i = 0;
        if (is_array($header)) {
          $sorted_header = array_values($header);
        }
        else {
          $sorted_header = $header;
        }
        foreach ($cells as $cell) {
          $cell = tablesort_cell($cell, $sorted_header, $ts, $i++);
          $output .= _theme_table_cell($cell);
        }
        $output .= " </tr>\n";
      }
    }
    $output .= "</tbody>\n";
  }

  $output .= "</table>\n";
  return $output;
}