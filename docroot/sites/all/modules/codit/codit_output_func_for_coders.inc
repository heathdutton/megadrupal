<?php
/**
 * @file
 * Supporting output functions for the codit module.
 */

/**
 * Accepts counts info from within a foreach loop and returns a class string.
 *
 * @param int $i_current_count
 *   The count of the current loop index.  First loop should start at 1
 * @param int $i_max_count
 *   The maximum number of loops (the count of the items in the array)
 *
 * @return string
 *   'first' if it is the first loop,
 *   'middle' if it is the middle loops,
 *   'last' if it is the last loop, 'first last' if there is only one item
 */
function codit_first_last_class($i_current_count = 0, $i_max_count = 0) {
  $s_first_last_class = '';
  $s_first_last_class .= ($i_current_count == 1) ? 'first' : '';
  $s_first_last_class .= (($i_current_count > 1) && ($i_current_count < $i_max_count)) ? 'middle' : '';
  $s_first_last_class .= ($i_current_count == $i_max_count) ? ' last' : '';
  return $s_first_last_class;
}


/**
 * Takes an array of items and returns in wrapped columns in the direction.
 *
 * @param array $a_items
 *   An array of items to be placed into columns.
 * @param int $i_total_columns
 *   The number of columns to form from the items.
 * @param string $s_direction
 *   The direction that the items should order in the column.
 *   'down' is the default so that items would read down the columns,
 *   all other entries cause them to read across columns. Optional
 * @param string $s_wrapped
 *   The individual list items should be wrapped in (div [default], list, span)
 *   optional
 *
 * @return string
 *   html of formatted columns properly wrapped with column counts,
 *   first/middle/last columns, element counts, first/middle/last elements
 */
function codit_columnize($a_items, $i_total_columns = 2, $s_direction = 'down', $s_wrapped = 'div') {

  switch ($s_wrapped) {
    case 'list':
      $s_major_wrapper = 'ul';
      $s_minor_wrapper = 'li';
      break;

    case 'span':
      $s_major_wrapper = 'div';
      $s_minor_wrapper = 'span';
      break;

    case 'div':
      $s_major_wrapper = 'div';
      $s_minor_wrapper = 'div';
      break;

    default:
      $s_major_wrapper = 'div';
      $s_minor_wrapper = 'div';

  }

  $i_item_num = 1;
  $i_column_num = 1;
  $i_column_element_num = 1;
  $i_total_items = count($a_items);
  $i_column_max_items = ceil($i_total_items / $i_total_columns);
  $output = '';

  // Check for reading direction.
  if ($s_direction == 'down') {
    // Columns should read down.
    foreach (is_array($a_items) || is_object($a_items) ? $a_items : array() as $item) {
      if ($i_column_element_num == 1) {
        // This is a new column.
        $s_column_first_last_class = codit_first_last_class($i_column_num, $i_total_columns);
        $output .= "\n" . '  <' . $s_major_wrapper . ' class="columnize column-' . $i_column_num . ' col-' . $i_column_num . '-of-' . $i_total_columns . ' ' . $s_column_first_last_class . ' list-down">' . "\n";
      }

      $s_element_first_last_class = ($i_item_num < $i_total_items) ? codit_first_last_class($i_column_element_num, $i_column_max_items) : 'last';

      $output .= '         <' . $s_minor_wrapper . ' class="item-' . $i_item_num . ' column-element-' . $i_column_element_num . ' ' . $s_element_first_last_class . '">' . "\n";
      $output .= '           ' . $item . "\n";
      $output .= "         </$s_minor_wrapper>\n";

      if ($i_column_element_num == $i_column_max_items || $i_item_num == $i_total_items) {
        // This column is full so close it.
        $output .= '  </' . $s_major_wrapper . '> <!-- column-' . $i_column_num . ' -->' . "\n";
        $i_column_element_num = 1;
        $i_column_num++;
      }
      else {
        $i_column_element_num++;
      }
      $i_item_num++;
    }
  }
  else {
    // Columns should read across.
    $a_columns = array();

    // Loop through items and put them into columns in the array.
    foreach (is_array($a_items) || is_object($a_items) ? $a_items : array() as $item) {
      $a_columns[$i_column_num][] = $item;
      // Increment and reset increment if too high.
      $i_column_num = (++$i_column_num > $i_total_columns) ? 1 : $i_column_num;
    }

    // Loop through columns array to build columns.
    foreach (is_array($a_columns) || is_object($a_columns) ? $a_columns : array() as $i_column_key => $a_column) {
      $i_column_num = $i_column_key;
      // This is a new column.
      $s_column_first_last_class = codit_first_last_class($i_column_num, $i_total_columns);
      $output .= "\n" . '  <' . $s_major_wrapper . ' class="columnize column-' . $i_column_num . ' col-' . $i_column_num . '-of-' . $i_total_columns . ' ' . $s_column_first_last_class . ' list-across">' . "\n";

      // Loop through columns array to build column.
      foreach (is_array($a_column) || is_object($a_column) ? $a_column : array() as $i_element_key => $element) {
        $i_column_element_num = $i_element_key + 1;
        $s_element_first_last_class = ($i_item_num < $i_total_items) ? codit_first_last_class($i_column_element_num, $i_column_max_items) : 'last';
        $output .= '         <' . $s_minor_wrapper . ' class="item-' . $i_item_num . ' column-element-' . $i_column_element_num . ' ' . $s_element_first_last_class . '">' . "\n";
        $output .= '           ' . $element . "\n";
        $output .= "         </$s_minor_wrapper>\n";
        $i_item_num++;
      }

      // This column is done. Close it.
      $output .= '  </' . $s_major_wrapper . '> <!-- column-' . $i_column_num . ' -->' . "\n";
    }
  }

  return $output;
}


/**
 * Builds the html for menu links in appropriate columns.
 *
 * @param string $s_menu_to_load
 *   The machine name for a menu to load.
 * @param int $i_total_columns
 *   The total number of columns to create.
 * @param string $s_direction
 *   The direction that the columns should read (down [default], across)
 * @param string $s_wrapped
 *   What the items should be wrapped in (list [default], div)
 *
 * @return string
 *   Html of the entire list appropriately wrapped and classed.
 *   The return value is heavy cached.
 */
function codit_build_menu_list($s_menu_to_load, $i_total_columns, $s_direction = 'down', $s_wrapped = 'list') {
  // Check for cached version.
  $s_cache_name = 'BML-' . $s_menu_to_load . '-' . $i_total_columns . '-' . $s_direction;
  // $s_cache_name is the unique identifier for this cached item.
  if (($cache = cache_get($s_cache_name, 'cache')) && !empty($cache->data)) {
    // There is a cached version saved so use it.
    $s_output_list_html = $cache->data;
    $s_output_list_html = $s_output_list_html . "\n<!-- The preceding list is from extra cache -->\n";
  }
  else {
    // There is no cached version, so build it.
    // Load the menu.
    $a_menu_tree = menu_build_tree($s_menu_to_load);
    $a_tree_items = array();
    foreach ($a_menu_tree as $a_menu_item) {
      if (!empty($a_menu_item['link']['access']) && $a_menu_item['link']['access']) {
        // They have permission to view the menu item.
        if (!empty($a_menu_item['link']['href'])) {
          $full_uri = url($a_menu_item['link']['href']);
        }
        // Build the list item.
        if (!empty($a_menu_item['link']['link_title']) && !empty($full_uri)) {
          $a_tree_items[] = '<a href="' . $full_uri . '">' . $a_menu_item['link']['link_title'] . '</a>' . "\n";
        }
      }
    }
    // Generate the list.
    $s_output_list_html = codit_columnize($a_tree_items, $i_total_columns, $s_direction, $s_wrapped);

    // Save the results of the process to cache.
    cache_set($s_cache_name, $s_output_list_html, 'cache', CACHE_TEMPORARY);
    $s_output_list_html = $s_output_list_html . "\n<!-- The preceding list is NOT from cache this time -->\n";
  }

  return $s_output_list_html;
}


 /**
  * Accepts a menu name or array(s) of items and returns a select list.
  *
  * All items  in param are Optional
  *
  * @param array $a_select_items
  *   Items to be included in the select list and options for additional items
  *   - $a_select_items['sMenuName'] = '';// string The machine name of menu to
  *     load into select list.
  *   - $a_select_items['aItemsToLoad']['top'] = '';// array of key value pairs
  *     ('value' => 'test to display') to load and display above the menu items
  *     and mixed items.
  *   - $a_select_items['aItemsToLoad']['mixed'] = '';// array of key value
  *     pairs ('value' => 'test to display') to load and mix and sort with the
  *     menu loaded.
  *   - $a_select_items['aItemsToLoad']['bottom'] = '';// array of key value
  *     pairs ('value' => 'test to display') to load and display below the menu
  *     items and mixed items.
  *   - $a_select_items['sSelected'] = '';// string The value (not the visible
  *     text) of the item to list as selected [defaults to NULLitem]
  *   - $a_select_items['sNULLitem'] = '';// string The name of the first
  *     unselected item in the list.  It gets a value of ' '.
  *   - $a_select_items['iSize'] = ;// integer The number of items to show in
  *     the select list before adding scroll bars.
  *   - $a_select_items['bRequired'] = ; boolean Whether the select should have
  *     the  'required' attribute or not (TRUE | FALSE  default = FALSE)
  *   - $a_select_items['bMultiple'] = ; boolean Whether the select should allow
  *     multiple selections or not (TRUE | FALSE  default = FALSE)
  *   - $a_select_items['bAutoFocus'] = ; boolean Whether the select get focus
  *     upon pageload (TRUE | FALSE  default = FALSE)
  *   - $a_select_items['bDisabled'] = ; boolean Whether the select should
  *     disabled or not (TRUE | FALSE  default = FALSE)
  *     WARNING if you are going to toggle this menu on or off, do not  cached
  *   - $a_select_items['sSelectName'] = ''; string of the Name attribute for
  *     the select element (default = '')
  *     <select name="THIS-NAME-WOULD-APPEAR-HERE" >
  *   - $a_select_items['sSelectForm'] = ''; string of the Name of the form(s)
  *     this select list should be associated with (default = '')
  *     <select form="FORM-NAME-WOULD-APPEAR-HERE" >
  *
  * @param string $s_unique_cache_name
  *   The name to use to uniquely cache this item.  If empty, this will not be
  *   heavy cached. (default empty)
  *
  * @return string
  *   The html of a fully formed select list with all the associated options as
  *   set within $a_select_items
  */
function codit_menu_or_array_as_select_list($a_select_items = array(), $s_unique_cache_name = '') {
  // Initialize vars.
  $s_select_list_html = '';
  $a_menu_items = array();

  // Check for cached version.
  // $s_unique_cache_name is the unique identifier for this cached item.
  if (!empty($s_unique_cache_name) && ($cache = cache_get($s_unique_cache_name, 'cache')) && !empty($cache->data)) {
    // There is a cached version saved so use it.
    $s_select_list_html = $cache->data;
    $s_select_list_html .= " <!-- Came from Cache -->\n";
  }
  else {
    // There is no cached version or it was not to be cached, so build it.
    // Grab menu items if $a_select_items['sMenuName'].
    if (!empty($a_select_items['sMenuName'])) {
      // Load the menu.
      $a_menu_tree = menu_build_tree($a_select_items['sMenuName']);
      foreach ($a_menu_tree as $a_menu_item) {
        if (!empty($a_menu_item['link']['access']) && $a_menu_item['link']['access']) {
          // Means they have permission to view the menu item.
          if (!empty($a_menu_item['link']['href'])) {
            $full_uri = url($a_menu_item['link']['href']);
          }
          // Build list item.
          if (!empty($a_menu_item['link']['link_title']) && !empty($full_uri)) {
            $a_menu_items[$a_menu_item['link']['link_title']] = $full_uri;
          }
        }
      }
    }
    // Merge $a_select_items['aItemsToLoad']['mixed'] with  the menu items.
    if (!empty($a_select_items['aItemsToLoad']['mixed']) && is_array($a_select_items['aItemsToLoad']['mixed'])) {
      // There is something to merge.
      $a_menu_items = array_merge($a_menu_items, $a_select_items['aItemsToLoad']['mixed']);
    }
    // Sort the combined array.
    uksort($a_menu_items, "strnatcasecmp");
    // Add $a_select_items['aItemsToLoad']['top'] to the beginning of the array.
    if (!empty($a_select_items['aItemsToLoad']['top']) && is_array($a_select_items['aItemsToLoad']['top'])) {
      $a_menu_items = array_merge($a_select_items['aItemsToLoad']['top'], $a_menu_items);
    }
    // Add null item $a_select_items['sNULLitem'] to the beginning of the array.
    if (!empty($a_select_items['sNULLitem'])) {
      $a_default = array($a_select_items['sNULLitem'] => ' ');
      $a_menu_items = array_merge($a_default, $a_menu_items);
    }
    // Add $a_select_items['aItemsToLoad']['bottom'] to the end of the array.
    if (!empty($a_select_items['aItemsToLoad']['bottom']) && is_array($a_select_items['aItemsToLoad']['bottom'])) {
      $a_menu_items = array_merge($a_menu_items, $a_select_items['aItemsToLoad']['bottom']);
    }

    $a_select_items['sSelected'] = (!empty($a_select_items['sSelected'])) ? $a_select_items['sSelected'] : ' ';

    // Build the select list while looking for $a_select_items['sSelected'].
    foreach ($a_menu_items as $s_select_text => $s_select_value) {
      if (!empty($a_select_items['sSelected']) && ($a_select_items['sSelected'] == $s_select_value)) {
        // This is the selected item.
        $s_select_list_html .= '        <option value="' . $s_select_value . '" selected>' . $s_select_text . '</option>' . "\n";
      }
      else {
        // This is not the selected item.
        $s_select_list_html .= '        <option value="' . $s_select_value . '">' . $s_select_text . '</option>' . "\n";
      }
    }

    // Build select tag.
    if (!empty($s_select_list_html)) {
      // I have items in the select list so build the <select> tag.
      $s_select = '    <select';
      $s_select .= (!empty($a_select_items['sSelectName'])) ? ' name="' . $a_select_items['sSelectName'] . '"' : '';
      $s_select .= (!empty($a_select_items['bRequired']) && ($a_select_items['bRequired'] === TRUE)) ? ' required' : '';
      $s_select .= (!empty($a_select_items['iSize']) && is_int($a_select_items['iSize'])) ? ' size="' . $a_select_items['iSize'] . '"' : '';
      $s_select .= (!empty($a_select_items['bMultiple'])  && ($a_select_items['bMultiple'] === TRUE)) ? ' multiple' : '';
      $s_select .= (!empty($a_select_items['sSelectForm'])) ? ' form="' . $a_select_items['sSelectForm'] . '"' : '';
      $s_select .= (!empty($a_select_items['bDisabled'])  && ($a_select_items['bDisabled'] === TRUE)) ? ' disabled' : '';
      $s_select .= (!empty($a_select_items['bAutoFocus'])  && ($a_select_items['bAutoFocus'] === TRUE)) ? ' autofocus' : '';
      $s_select .= ">\n";
      $s_select_list_html = $s_select . $s_select_list_html . "    </select>\n";
    }
    if (!empty($s_unique_cache_name)) {
      // Save the results of the heavy process,to cache.
      cache_set($s_unique_cache_name, $s_select_list_html, 'cache', CACHE_TEMPORARY);
    }
    $s_select_list_html .= " <!-- Not from Cache -->\n";
  }

  return $s_select_list_html;
}
