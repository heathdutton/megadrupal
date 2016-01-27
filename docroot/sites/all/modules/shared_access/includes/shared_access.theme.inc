<?php

/**
 * @file
 * Shared Access module theme functions.
 */

function theme_shared_access_summary($variables) {
  //dsm($variables, 'theme_shared_access_summary'); // TODO: REMOVE
  return t('<span>@realm: @gid (@op)</span>', array('@realm' => $variables['realm'], '@gid' => $variables['gid'], '@op' => $variables['op']));
}

/**
 * Callback for themeing the autocomplete matches returned for users.
 */
function theme_sa_usertag($variables) {
  $pseudo_account = $variables['pseudo_account'];
  return '<div class="shared-access-usb-user">' . '&ldquo;' . check_plain($pseudo_account->name) .'&rdquo; &lt;' . $pseudo_account->mail . '&gt;' . '</div>';
}

/**
 * Callback for themeing the autocomplete matches returned for groups.
 */
function theme_sa_grouptag($variables) {
  return '<div class="shared-access-usb-group">' . '&ldquo;' . t('group') . ': ' . check_plain($variables['group_label']) .'&rdquo;' . '</div>';
}

/**
 * Callback for themeing the autocomplete matches returned for a domain.
 */
function theme_sa_domaintag($variables) {
  return '<div class="shared-access-usb-domain">' . '&ldquo;' . t('domain') . ': ' . check_plain($variables['domain']) .'&rdquo;' . '</div>';
}

/**
 * Default Callback for themeing share table item titles.
 */
function theme_sa_item_title($variables) {
  $item = $variables['item'];
  return '<span class="sa-item-realm">' . $item['realm'] . '&nbsp;(' . $item['gid'] . "):</span>&nbsp;\n" .
         '<span class="sa-item-ids">' . $item['gstr'] .  "</span>\n";
}

/**
 * Callback for themeing owner realm share table item titles.
 */
function theme_sa_item_title_owner($variables) {
  $owner = entity_metadata_wrapper('user', $variables['item']['gid']);
  return '<span class="sa-item-owner-name">"' . $owner->name->value() . "\"</span>&nbsp;\n" .
         '<span class="sa-item-owner-mail">&lt;' . $owner->mail->value() . "&gt;</span>\n";
}

/**
 * Callback for themeing owner realm share table item titles.
 */
function theme_sa_item_title_user($variables) {
  $account = entity_metadata_wrapper('user', $variables['item']['gid']);
  return '<span class="sa-item-user-name">"' . $account->name->value() . "\"</span>&nbsp;\n" .
         '<span class="sa-item-user-mail">&lt;' . $account->mail->value() . "&gt;</span>\n";
}

/**
 * Callback for themeing group realm share table item titles.
 */
function theme_sa_item_title_group($variables) {
  $entity = entity_metadata_wrapper($variables['item']['gstr'], $variables['item']['gid']);
  return '<span class="sa-item-group-name">' . t('group') . "</span>&nbsp;\n" .
         '<span class="sa-item-user-mail">"' . $entity->label() . "\"</span>\n";
}

/**
 * Callback for themeing domain realm share table item titles.
 */
function theme_sa_item_title_domain($variables) {
  return '<span class="sa-item-domain-name">' . t('domain') . "</span>&nbsp;\n" .
         '<span class="sa-item-domain-mail">"' . $variables['item']['gstr'] . "\"</span>\n";
}


/**
 * Returns HTML for the entire dashboard.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: A render element containing the properties of the dashboard
 *     region element, #dashboard_region and #children.
 *
 * @ingroup themeable
 */
function theme_sa_table_container($variables) {
  $element = $variables['element'];
  $rows = array();
  $header = $element['#header'];
  $colgroups = $element['#colgroups'];
  $tbodys = $element['#tbodys'];
  // Use each enabled realm controller to generate table rows.
  foreach(array_intersect_key(shared_access_get_access_controllers(), $element['#realms']) as $realm => $controller) {
    $rows = array_merge($rows, $controller->sa_table_add_rows($element));
  }
  return theme('sa_table', array('element' => $element, 'header' => $header, 'rows' => $rows, 'empty' => $element['#empty'], 'attributes' => $element['#attributes'], 'tbodys' => $tbodys, 'colgroups' => $colgroups));
}

/**
 * Callback for themeing a table.
 *
 * Pretty much a clone of theme_table() except multiple body tags are possible.
 *
 * @see theme_table().
 */
function theme_sa_table($variables) {
  $element = $variables['element'];
  $header = $variables['header'];
  $rows = $variables['rows'];
  $attributes = $variables['attributes'];
  $caption = $element['#title'];
  $colgroups = $variables['colgroups'];
  $sticky = $variables['sticky'];
  $empty = $variables['empty'];

  $tbodys = $variables['tbodys'];
  foreach ($tbodys as $key => &$tbody) {
    $tbody += array('weight' => 0, 'output' => '');
  }
  $tbodys += array('main' => array('weight' => 0, 'output' => ''));


  // Add sticky headers, if applicable.
  if (count($header) && $sticky) {
    drupal_add_js('misc/tableheader.js');
    // Add 'sticky-enabled' class to the table to identify it for JS.
    // This is needed to target tables constructed by this function.
    $attributes['class'][] = 'sticky-enabled';
  }

  $output = '<table' . drupal_attributes($attributes) . ">\n";

  if (isset($caption)) {
    $output .= '<caption>' . $caption . "</caption>\n";
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
    $rows[] = array(array(
      'data' => $empty,
      'colspan' => $header_count,
      'class' => array('empty', 'message'),
    ));
  }

  // Format the table header:
  if (count($header)) {
    $ts = tablesort_init($header);
    // HTML requires that the thead tag has tr tags in it followed by tbody
    // tags. Using ternary operator to check and see if we have any rows.
    $output .= (count($rows) ? ' <thead><tr>' : ' <tr>');
    foreach ($header as $cell) {
      $cell = tablesort_header($cell, $header, $ts);
      $output .= _theme_table_cell($cell, TRUE);
    }
    // Using ternary operator to close the tags based on whether or not there
    // are rows
    $output .= (count($rows) ? " </tr></thead>\n" : "</tr>\n");
  }
  else {
    $ts = array();
  }

  // Format the table rows:
  if (count($rows)) {
    $flip = array(
      'even' => 'odd',
      'odd' => 'even',
    );
    $class = 'even';
    foreach ($rows as $number => $row) {
      $attributes = array();
      $tbody_group = 'main';
      // Check if we're dealing with a simple or complex row
      if (isset($row['data'])) {
        foreach ($row as $key => $value) {
          if ($key == 'data') {
            $cells = $value;
          } elseif ($key == 'tbody') {
            if (!isset($tbodys[$value])) $tbodys[$value] = array('weight' => 0, 'output' => '');
            $tbody_group = $value;
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
        }

        // Build row
        $tbodys[$tbody_group]['output'] .= ' <tr' . drupal_attributes($attributes) . '>';
        $i = 0;
        foreach ($cells as $cell) {
          $cell = tablesort_cell($cell, $header, $ts, $i++);
          $tbodys[$tbody_group]['output'] .= _theme_table_cell($cell);
        }
        $tbodys[$tbody_group]['output'] .= " </tr>\n";
      }
    }

    uasort($tbodys, 'drupal_sort_weight');
    foreach ($tbodys as $key => $tbody) {
      if (!empty($tbody['output'])) $output .= "<tbody>\n" . $tbody['output'] . "</tbody>\n";
    }
  }

  $output .= "</table>\n";
  return $output;
}
