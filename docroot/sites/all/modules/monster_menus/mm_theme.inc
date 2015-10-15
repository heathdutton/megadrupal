<?php

/**
 * @file
 * Theme functions for Monster Menus
 */

/**
 * Theme a category list form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #mm_list_popup_root, #mm_list_max
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_catlist($variables) {
  $element = $variables['element'];
  return _mm_theme_mm_list(MM_BROWSER_MODE_PAGE, $element, isset($element['#mm_list_popup_root']) ? $element['#mm_list_popup_root'] : '',
      !empty($element['#title']) ? $element['#title'] :
      ($element['#mm_list_max'] == 1 ? t('Page:') : t('Pages:')),
      t('Path:'), '_mm_theme_list_info_cat');
}

/**
 * Theme a group list form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #mm_list_popup_root, #mm_list_max
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_grouplist($variables) {
  $element = $variables['element'];
  return _mm_theme_mm_list(MM_BROWSER_MODE_GROUP, $element, mm_content_groups_mmtid(),
      !empty($element['#title']) ? $element['#title'] :
      ($element['#mm_list_max'] == 1 ? t('Group:') : t('Groups:')),
      t('Users in selected group:'), '_mm_theme_list_info_group');
}

/**
 * Theme a user list form element.
 *
 * @param $variables
 *   An associative array containing:
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #mm_list_popup_root, #mm_list_max
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_userlist($variables) {
  $element = $variables['element'];
  return _mm_theme_mm_list(MM_BROWSER_MODE_USER, $element, 0,
      !empty($element['#title']) ? $element['#title'] :
      ($element['#mm_list_max'] == 1 ? t('User:') : t('Users:')),
      t('Path:'), NULL);
}

/**
 * Theme a node list form element.
 * @param $variables
 *   - element: An associative array containing the properties of the element.
 *     Properties used: #title, #mm_list_popup_root, #mm_list_max
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_nodelist($variables) {
  $element = $variables['element'];
  return _mm_theme_mm_list(MM_BROWSER_MODE_NODE, $element, 0,
      !empty($element['#title']) ? $element['#title'] :
      ($element['#mm_list_max'] == 1 ? t('Node:') : t('Nodes:')),
      t('Path:'), '_mm_theme_list_info_cat');
}

/**
 * Generate the form code to allow the user to add pages/groups to a node
 *
 * @param $mode
 *   The MM_BROWSER_X constant mode
 * @param $elt
 *   An associative array containing the properties of the element.
 *   Properties used: #default_value, #description, #field_name,
 *   #mm_list_autocomplete_name, #mm_list_browser, #mm_list_buttons_underneath,
 *   #mm_list_enabled, #mm_list_max, #mm_list_min, #mm_list_no_info,
 *   #mm_list_other_callback, #mm_list_other_name, #mm_list_popup_root,
 *   #mm_list_readonly, #mm_list_selectable, #name, #parents, #required, #title,
 *   #type, #value
 * @param $start_mmtid
 *   The location in the tree at which to default the tree view
 * @param $cat_label
 *   The label appearing above the list
 * @param $info_label
 *   The label appearing above the item information line
 * @param $info_func
 *   Callback used to generate the default information about each item
 *   pre-populated in the list
 * @return
 *   The HTML code for the results
 */
function _mm_theme_mm_list($mode, $elt, $start_mmtid, $cat_label, $info_label, $info_func = NULL) {
  $_mmlist_instance = &drupal_static('_mmlist_instance', 0);

  if (!user_access('use tree browser') || !module_exists('thickbox')) {
    $elt['#mm_list_readonly'] = TRUE;
  }
  $max = intval($elt['#mm_list_max']);
  $min = intval($elt['#mm_list_min']);
  $exactly1 = $max == 1 && $min == 1;
  $flags = array();
  $js_flags = array(
    '#mm_list_buttons_underneath' => 'action_under',
    '#mm_list_submit_on_add' => 'submit_on_add',
    '#mm_list_initial_focus' => 'initial_focus',
    '#mm_list_hide_left_pane' => 'hide_left_pane',
  );
  foreach ($js_flags as $api_flag => $js_flag) {
    if (!empty($elt[$api_flag])) {
      $flags[$js_flag] = $elt[$api_flag];
    }
  }
  $flags = $flags ? drupal_json_encode($flags) : '{}';
  $select_callback = 'catSelectCallback';
  if (!empty($elt['#mm_list_readonly'])) {
    $label_above_actions = $label_add_cat = $label_replace = $label_delete = "''";
    if ($elt['#mm_list_no_info']) {
      $select_callback = 'null';
    }
  }
  else {
    $label_above_actions = drupal_json_encode(t('Action:'));
    $label_add_cat = drupal_json_encode($max == 1 || $mode == MM_BROWSER_MODE_USER ? '' : t('Add...'));
    $label_replace = drupal_json_encode($mode == MM_BROWSER_MODE_USER ? '' : ($max == 1 ? t('Choose...') : t('Replace...')));
    $label_delete = drupal_json_encode($exactly1 ? '' : ($min == 0 && $max == 1 ? t('Clear') : t('Delete')));
  }
  $label_above_info = !empty($elt['#mm_list_no_info']) ? "''" : drupal_json_encode($info_label);

  if ($mode == MM_BROWSER_MODE_USER) {
    $popup_base = '';
  }
  else {
    $root = $elt['#mm_list_browser'];
    if ($elt['#type'] == 'mm_nodelist') {
      $field_id = !empty($elt['#mm_list_field_name']) && !empty($elt['#mm_list_bundle_name']) ? $elt['#mm_list_field_name'] .',' . $elt['#mm_list_bundle_name'] : '';
      $popup_base = base_path() . "$root/$start_mmtid-$mode-$_mmlist_instance-" .
        $elt['#mm_list_enabled'] . '-' . $elt['#mm_list_selectable'] . '--' . $field_id . '/';
    }
    else {
      $popup_base = base_path() . "$root/$start_mmtid-$mode-$_mmlist_instance-" .
        (isset($elt['#mm_list_enabled']) ? $elt['#mm_list_enabled'] : '') . '-' . $elt['#mm_list_selectable'] . '/';
    }
  }
  $popup_URL = $elt['#mm_list_popup_start'];

  if ($max == 1 && !$elt['#value']) {
    $elt['#value'] = count($elt['#default_value']) ? $elt['#default_value'] : array('' => t('(choose a location)'));
  }

  $adds = array();
  if (!mm_ui_is_search()) {
    if (is_array($elt['#value'])) {
      foreach ($elt['#value'] as $mmtid => $name) {
        if (!$mmtid || empty($info_func)) {
          $url = $exactly1 ? $popup_URL : '';
          $info = '';
        }
        else {
          $info_func($mmtid, $name, $url, $info, $popup_URL);
        }

        $adds[] = _mm_theme_add_row_js(array($name, $mmtid, $url, $info));
      }
    }
    else if (!empty($elt['#value'])) {
      foreach (_mm_ui_split_mmlist($elt['#value']) as $m) {
        $name = $m[2];
        if (!empty($info_func)) {
          $info_func($m[1], $name, $url, $info, $popup_URL);
        }
        list($mmtid, $nid) = explode('/', $m[1]);

        if ($mode != MM_BROWSER_MODE_USER && !$nid && !mm_content_user_can($mmtid, $elt['#mm_list_selectable'])) {
          if ($exactly1) {
            $name = t('(choose)');
            $info = '';
          }
          else {
            if ($url == $popup_URL) {
              unset($popup_URL);
            }
            continue;
          }
        }

        $adds[] = _mm_theme_add_row_js(array($name, $mmtid, $url, $info));
      }
    }
  }
  else {
    if ($exactly1) $adds[] = _mm_theme_add_row_js(array('', '', '', ''));
    $adds[] = '$(obj.p.hiddenElt).change(function() { mmListImport(obj, this.value); });';
  }

  if (empty($popup_URL)) {
    $popup_URL = $start_mmtid;
  }
  $popup_label = t('Tree Browser');

  if (isset($elt['#name'])) {
    $name = $elt['#name'];
  }
  else {
    $name = $elt['#parents'][0];
    if (count($elt['#parents']) > 1) {
      $name .= '[' . join('][', array_slice($elt['#parents'], 1)) . ']';
    }
  }

  $other_name = isset($elt['#mm_list_other_name']) ? $elt['#mm_list_other_name'] : '';
  $other_callback = isset($elt['#mm_list_other_callback']) ? $elt['#mm_list_other_callback'] : 'null';

  $cat_label .= !empty($elt['#required']) ? ' <span class="form-required" title="' . t('This field is required.') . '">*</span>' : '';
  $cat_label = drupal_json_encode($cat_label);

  $class = _mm_theme_add_class($elt);
  $desc = !empty($elt['#description']) ? "\n" . '<div class="description">' . $elt['#description'] . '</div>' : '';
  $auto = isset($elt['#mm_list_autocomplete_name']) ? $elt['#mm_list_autocomplete_name'] : '';
  $tag = $exactly1 || $elt['#mm_list_readonly'] && $elt['#mm_list_no_info'] ? '<span></span>' : '<a></a>';
  if (mm_ui_is_search()) {
    $startup = "\nDrupal.settings.MMSR.fixups['[name=\"mm_list_obj$_mmlist_instance\"]'] = function() { mmListInit$_mmlist_instance(this); };";
  }
  else {
    $startup = <<<JS
Drupal.behaviors.mmListInit$_mmlist_instance = {
  attach: function () {
    if (mmListInit$_mmlist_instance) {
      mmListInit$_mmlist_instance();
      mmListInit$_mmlist_instance = null;
    }
  }
};
JS;
  }

  $adds = join("\n    ", $adds);
  $js = <<<JS
(function (\$) {
mmListInit$_mmlist_instance = function(where) {
  var obj = mmListGetObj(where,
    'div[name=mm_list_obj$_mmlist_instance]',
    'div[name=mm_list_obj$_mmlist_instance] + div[class="$class"]',
    '$name',
    '$auto', {
      minRows: $min,
      maxRows: $max,
      popupBase: '$popup_base',
      popupURL: '$popup_URL',
      popupLabel: '$popup_label',
      flags: $flags,
      addCallback: catAddCallback,
      replaceCallback: catReplCallback,
      selectCallback: $select_callback,
      dataCallback: catDataCallback,
      labelAboveList: $cat_label,
      labelAboveActions: $label_above_actions,
      labelAddCat: $label_add_cat,
      labelReplace: $label_replace,
      labelDelete: $label_delete,
      labelAboveInfo: $label_above_info,
      updateOnChangeName: '$other_name',
      updateOnChangeCallback: $other_callback
    }
  );
  if (obj && obj.addItem) {
    $adds
    obj.enableOpts();
  }
}; $startup
})(jQuery);
JS;
  $js = drupal_get_js('inline', array(array('scope' => 'inline', 'type' => 'inline', 'defer' => FALSE, 'data' => $js)));
  $output = <<<HTML
$js
<div name="mm_list_obj$_mmlist_instance" style="display: none">$tag</div><div class="$class"></div>$desc
HTML;
  $_mmlist_instance++;

  return $output;
}

/**
 * Theme the list of blocks
 * @param $variables
 *   - form: The form element
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_admin_list_blocks($variables) {
  $form = $variables['form'];
  $rows = array();
  if (isset($form['name']) && is_array($form['name'])) {
    foreach (element_children($form['name']) as $key) {
      $row = array();
      $row[] = drupal_render($form['name'][$key]);
      $row[] = drupal_render($form['title'][$key]);
      $row[] = drupal_render($form['flags'][$key]);
      $row[] = drupal_render($form['operations'][$key][0]);
      $row[] = drupal_render($form['operations'][$key][1]);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('No blocks defined.'), 'colspan' => '5'));
  }

  $output = theme('table', array(
    'header' => $form['header']['#value'],
    'rows' => $rows,
  ));
  if (isset($form['pager']['#markup'])) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render_children($form);
  $output .= t('<p>To control where these blocks appear on the page, you must visit !link.', array('!link' => l(t('the main Blocks editor'), 'admin/structure/block')));

  return $output;
}

/**
 * Theme the sites list
 *
 * @param $variables
 *   - form: The form element
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_admin_list_sites($variables) {
  $form = $variables['form'];
  $rows = array();
  if (isset($form['name']) && is_array($form['name'])) {
    foreach (element_children($form['name']) as $key) {
      $row = array();
      $row[] = drupal_render($form['name'][$key]);
      $row[] = array('data' => drupal_render($form['current'][$key]), 'align' => 'center');
      $row[] = drupal_render($form['operations'][$key][0]);
      $row[] = drupal_render($form['operations'][$key][1]);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('No sites defined.'), 'colspan' => '3'));
  }

  $output = theme('table', array(
    'header' => $form['header']['#value'],
    'rows' => $rows,
  ));
  if ($form['pager']['#markup']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render_children($form);

  return $output;
}

/**
 * Theme the integrity verification
 *
 * @param $variables
 *   - form: The form element
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_admin_verify_integrity($variables) {
  $form = $variables['form'];
  $passed = t('All tests passed.');
  foreach (element_children($form) as $fieldset) {
    if (substr($fieldset, 0, 2) == 'fs') {
      $rows = array();
      $collapsed = TRUE;
      foreach (element_children($form[$fieldset]) as $key) {
        if (is_numeric($key)) {
          $append = '';
          if ($form[$fieldset][$key]['result']['#markup'] === '0') {
            $form[$fieldset][$key]['result']['#markup'] = t('PASS');
          }
          else {
            $collapsed = $passed = FALSE;
            $append = '<input type="text" size="80" value="SELECT * FROM ' . drupal_render($form[$fieldset][$key]['test']) . '">';
            $form[$fieldset][$key]['result']['#markup'] = t('<strong>FAIL</strong>');
            if (!empty($form[$fieldset][$key]['error']['#markup'])) {
              $append .= '<br /><input type="text" size="80" value="' . drupal_render($form[$fieldset][$key]['error']) . '">';
            }
          }

          $row = array();
          $row[] = '<div class="description">' . drupal_render($form[$fieldset][$key]['description']) . '</div>' . $append;
          $row[] = drupal_render($form[$fieldset][$key]['result']);
          $rows[] = $row;
        }
      }

      $form[$fieldset] = array(
        '#type' => 'fieldset',
        '#value' => theme('table', array(
          'header' => $form[$fieldset]['header']['#value'],
          'rows' => $rows
        )),
        '#title' => $form[$fieldset]['#title'],
        '#collapsible' => TRUE,
        '#collapsed' => $collapsed,
      );
    }
  }

  return $passed . drupal_render_children($form);
}

/**
 * Format a tooltip activation string
 *
 * @param $variables
 *   An associative array containing:
 *   - text: The text of the link that starts the tooltip
 *   - title: The popup tooltip's title
 *   - tip: The popup tooltip's text
 *   - html: If TRUE, $text should be raw HTML, and check_plain() is not run
 *
 * @return
 *   The HTML code for the results
 *
 * @ingroup themeable
 */
function theme_tooltip($variables) {
  if (empty($variables['html'])) {
    $variables['text'] = check_plain($variables['text']);
  }
  if (module_exists('qtip')) {
    $text = $variables['text'];
    $title = $variables['title'];
    $tip = $variables['tip'];
    return <<<HTML
      <span class="qtip-link tooltip-link">
        <span class="qtip-header">
          $title
        </span>
        <span class="qtip-tooltip">
          $tip
        </span>
        $text
      </span>
HTML;
  }
  else {
    $variables['tip'] = str_replace(array('</tr>', '<br />', '</td>'), array("\n", "\n", '  '), $variables['tip']);
    $attrib = array('title' => strip_tags($variables['tip']));
  }
  $attrib['onclick'] = 'return false';

  drupal_add_css(drupal_get_path('module', 'monster_menus') . '/css/mm.css');
  return '<a href="#" class="tooltip-link" ' . drupal_attributes($attrib) . '>' . $variables['text'] . '</a>';
}

function theme_mm_repeatlist_div($elt) {
  $elt['#attributes']['style'] .= '; display: none';
  return '<div id="' . $elt['#parents'][0] . '"' . drupal_attributes($elt['#attributes']) . '>' . $elt['#children'] . "</div>\n";
}

/**
 * Theme a form input element with the 'This field is required' notation, but
 * which does not get the built-in validation. Instead, it is assumed that
 * validation code will be provided in a custom function. This is helpful when
 * the element is dynamically hidden to the user using Javascript, because it
 * prevents validation errors on elements which aren't visible.
 *
 * If validation is needed, the function mm_ui_validate_fake_required() can be
 * called during the form's validation callback.
 *
 * @param $variables
 *   - elt: The element
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_fake_required($variables) {
  $info = element_info($variables['elt']['#mm_orig_type']);
  $elt = array_merge($info, $variables['elt']);
  $elt['#required'] = TRUE;
  $elt['#type'] = $elt['#mm_orig_type'];
  unset($elt['#mm_orig_type']);
  if (isset($info['#theme'])) {
    $elt['#theme'] = $info['#theme'];
  }
  else {
    unset($elt['#theme']);
  }
  unset($elt['#theme_wrappers']);
  unset($elt['#suffix']);
  return drupal_render($elt);
}

/**
 * Add Javascript code to a page, allowing the user to repeat a set of form
 * fields any number of times without having to submit the form multiple times.
 *
 * To specify which data is repeatable, use the mm_repeatlist_div type to set
 * up a new form group and pass its ID in the mm_repeatlist form element using
 * the #mm_list_id parameter. In order for the code to work properly, the
 * mm_repeatlist_div MUST come before the mm_repeatlist form element in the
 * form array.
 *
 * Default values for the data fields can be passed using the #default_value
 * parameter. It should contain an array of arrays, one per row of data to
 * pre-populate in the form. To parse the data after it has been submitted by
 * the user, call mm_ui_parse_repeatlist().
 *
 * Required fields in the form element:
 *   #mm_list_id                 Form ID of repeatable DIV (mm_repeatlist_div
 *                               type)
 *   #mm_list_inputs_per_row     Number of values per row in repeatable DIV and
 *                               #default_value
 *
 * Optional fields in the form element:
 *   #mm_list_min                Min number of rows (default: 1)
 *   #mm_list_max                Max number of rows (default: 0)
 *   #mm_list_buttons_underneath Show the action buttons underneath the data
 *                               rows, instead of to the right (default: FALSE)
 *   #mm_list_reorder            Show up/down arrows in each row, allowing user
 *                               to change the order (default: FALSE)
 *
 * Caveats:
 *   - In order for the code to work properly, the mm_repeatlist_div MUST come
 *     before the mm_repeatlist form element in the form array
 *   - The '#multiple' option is not supported for the 'select' type.
 *   - The 'file' and 'password' types are not supported.
 *   - If using the 'date' type, you must be sure to allocate three elements per
 *     row in the '#default_value' field, and increase the
 *     '#mm_list_inputs_per_row' accordingly.
 *
 * Example: Create a form element that allows the user to enter up to 10 rows of
 * data in two textfields per row. The form is pre-populated with some default
 * values:
 *
 *     $form['name_age'] = array(
 *       '#type' => 'mm_repeatlist_div',
 *       '#title' => t('Name and age'));
 *     $form['name_age']['name'] = array(
 *       '#type' => 'textfield',
 *       '#title' => t('Your first name'),
 *       '#description' => t('What is your first name?'));
 *     $form['name_age']['age'] = array(
 *       '#type' => 'textfield',
 *       '#title' => t('Your age'),
 *       '#description' => t('What is your age?'));
 *
 *     $form['grouping'] = array(
 *       '#type' => 'fieldset',
 *       '#collapsible' => TRUE, '#collapsed' => FALSE);
 *     $form['grouping']['data'] = array(
 *       '#type' => 'mm_repeatlist',
 *       '#title' => t('Tell us about yourself'),
 *       '#mm_list_id' => 'name_age',
 *       '#mm_list_inputs_per_row' => 2,
 *       '#default_value' => array(
 *           array('name1', '18'),
 *           array('name2', '26')));
 *
 * @param $elt
 *   The form element to display
 * @return
 *   The HTML code for the form element
 */
function theme_mm_repeatlist($elt) {
  $_mmlist_instance = &drupal_static('_mmlist_instance', 0);

  $max = intval($elt['#mm_list_max']);
  $min = intval($elt['#mm_list_min']);

  $label_above_actions = $elt['#mm_list_buttons_underneath'] ? '&nbsp;' : '';
  $cat_label = $elt['#title'] . (!empty($elt['#required']) ? ' <span class="form-required" title="' . t('This field is required.') . '">*</span>' : '');
  $cat_label = drupal_json_encode($cat_label);

  $flags = array('narrow_actions' => TRUE);
  if ($elt['#mm_list_buttons_underneath']) {
    $flags['action_under'] = TRUE;
  }

  if ($elt['#mm_list_readonly']) {
    $labels = array();
    $label_add_set = "''";
    $flags['readonly'] = TRUE;
  }
  else {
    $labels = mm_ui_mmlist_labels();
    $labels[5] = '';  // no edit
    if (!$elt['#mm_list_reorder']) $labels[0] = $labels[1] = $labels[3] = $labels[4] = '';
    $label_add_set = drupal_json_encode($max == 1 ? '' : $elt['#mm_list_add_button']);
  }
  $flags = drupal_json_encode($flags);

  if (isset($elt['#name'])) {
    $name = $elt['#name'];
  }
  else {
    $name = $elt['#parents'][0];
    if (count($elt['#parents']) > 1)
        $name .= '[' . join('][', array_slice($elt['#parents'], 1)) . ']';
  }

  $class = _mm_theme_add_class($elt);
  $desc = $elt['#description'] ? "\n" . '<div class="description">' . $elt['#description'] . '</div>' : '';
  $objname = $elt['#mm_list_id'];

  $imgpath = base_path() . drupal_get_path('module', 'monster_menus') . '/images';
  $del_confirm = t("Are you sure you want to delete this row?\\n\\n(You can skip this alert in the future by holding the Shift key while clicking the Delete button.)");

  $adds = array();
  if (mm_ui_is_search()) {
    $startup = "\nDrupal.settings.MMSR.fixups['[name=\"mm_list_obj$_mmlist_instance\"]'] = 'function() { mmListInit$_mmlist_instance(this); }';";
  }
  else {
    $value = $elt['#value'];
    if (!is_array($value)) {
      $value = $value[0] == '{' ? mm_ui_parse_repeatlist($value, $elt['#mm_list_inputs_per_row']) : array();
    }
    if ($min) {
      $value = array_pad($value, $min, array());
    }
    foreach ($value as $row) {
      $adds[] = _mm_theme_add_row_js($row);
    }
    $startup = "\n\$(document).ready(mmListInit$_mmlist_instance);";
  }
  $adds = join("\n    ", $adds);

  $js = <<<JS
(function (\$) {
mmListInit$_mmlist_instance = function(where) {
  var obj = mmListGetObj(where,
    'div#$objname',
    'div[name=mm_list$_mmlist_instance]',
    '$name',
    null, {
      minRows: $min,
      maxRows: $max,
      flags: $flags,
      addCallback: listAddCallback,
      dataCallback: listDataCallback,
      labelAboveList: $cat_label,
      labelAboveActions: '$label_above_actions',
      labelAddList: $label_add_set,
      imgPath: '$imgpath',
      delConfirmMsg: '$del_confirm',
      labelTop: '$labels[0]',
      labelUp: '$labels[1]',
      labelX: '$labels[2]',
      labelBott: '$labels[3]',
      labelDown: '$labels[4]',
      labelEdit: '$labels[5]'
    }
  );
  if (obj && obj.addItem) {
    $adds
    obj.enableOpts();
    obj.setHiddenElt();
  }
}; $startup
})(jQuery);
JS;
  drupal_add_js($js, array('type' => 'inline', 'scope' => 'footer'));
  $output = "\n<div name=\"mm_list$_mmlist_instance\" class=\"$class\"></div>$desc";
  $_mmlist_instance++;

  return $output;
}

/**
 * Theme one radio button with a Help tooltip.
 *
 * @param $variables
 *   - element: The element to theme
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_help_radio($variables) {
  $element = $variables['element'];
  $element['#attributes']['type'] = 'radio';
  element_set_attributes($element, array('id', 'name', '#return_value' => 'value'));

  if (isset($element['#return_value']) && $element['#value'] !== FALSE && $element['#value'] == $element['#return_value']) {
    $element['#attributes']['checked'] = 'checked';
  }
  _form_set_class($element, array('form-radio'));

  $output = '<input' . drupal_attributes($element['#attributes']) . ' />';

  if (!is_null($element['#title'])) {
    $output = '<label class="option">' . $output . ' ' . $element['#title'] . '</label>';
  }

  if (!empty($element['#help'])) {
    $output .= '&nbsp;&nbsp;&nbsp;' . theme('tooltip', array(
      'text' => t('help'),
      'title' => $element['#title'],
      'tip' => $element['#help']
    ));
  }

  return $output;
}

/**
 * Theme a list of radio buttons, each with a Help tooltip.
 *
 * @param $variables
 *   - element: The element to theme
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_help_radios($variables) {
  $element = $variables['element'];
  $attributes = array();
  if (isset($element['#id'])) {
    $attributes['id'] = $element['#id'];
  }
  $attributes['class'] = isset($element['#attributes']['class']) ? $element['#attributes']['class'] : array();
  $attributes['class'][] = array('form-radios');
  return '<div' . drupal_attributes($attributes) . '>' . (!empty($element['#children']) ? $element['#children'] : '') . '</div>';
}

/**
 * Theme the menu reorder form
 *
 * @param $variables
 *   - form: The form element
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_ui_menu_reorder($variables) {
  $form = $variables['form'];
  drupal_add_tabledrag('mm-reorder', 'order', 'sibling', 'menu-weight');

  $rows = array();
  foreach (element_children($form) as $mmtid) {
    $element = &$form[$mmtid];
    if (isset($element['mmtid'])) {
      // Add special classes to be used for tabledrag.js.
      $element['mmtid']['#attributes']['class'][] = 'menu-mmtid';
      $element['weight']['#attributes']['class'][] = 'menu-weight';

      $row = array();
      $row[] = drupal_render($element['title']);
      $row[] = drupal_render($element['weight']) . drupal_render($element['mmtid']);

      $row = array_merge(array('data' => $row), $element['#attributes']);
      $row['class'][] = 'draggable';
      $rows[] = $row;
    }
  }

  $output = '';
  if ($rows) {
    $output .= theme('table', array(
      'header' => array(),
      'rows' => $rows,
      'attributes' => array('id' => 'mm-reorder')
    ));
  }
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Theme the node reorder
 *
 * @param $variables
 *   - form: The form element
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_ui_node_reorder($variables) {
  $form = $variables['form'];
  $regions = $variables['form']['regions']['#value'];

  $rendered_rows = $region_targetable = array(0 => array(), 1 => array());
  foreach (element_children($form['nodes']) as $id) {
    $element = &$form['nodes'][$id];

    // Add special classes to be used for tabledrag.js.
    $element['nid']['#attributes']['class'][] = 'menu-nid';
    $region = $element['region']['#value'];
    $element['region']['#attributes']['class'] = array('nodes-region-select', 'nodes-region-' . $region);
    $element['weight']['#attributes']['class'] = array('nodes-weight', 'nodes-weight-' . $region);
    $element['weight']['#attributes']['class'][] = isset($element['stuck']) ? 'sticky-weight' : 'unsticky-weight';

    $row = array();
    $row[] = '';
    $row[] = drupal_render($element['title']) . drupal_render($element['nid']);
    $row[] = drupal_render($element['type']);
    $row[] = drupal_render($element['region']);
    $row[] = drupal_render($element['weight']);

    $row = array_merge(array('data' => $row), $element['#attributes']);
    $row['class'][] = 'draggable';
    $sticky = isset($element['stuck']) ? 1 : 0;
    $rendered_rows[$sticky][$region][] = $row;
    foreach (array_keys($element['region']['#options']) as $region) {
      $region_targetable[$sticky][$region] = TRUE;
    }
  }

  $output = '';
  $header = array('', t('Title'), t('Type'), t('Region'), t('Weight'));
  if ($rendered_rows[1]) {
    if ($rendered_rows[0]) {
      $output .= t('<h3>Sticky content</h3>');
    }
    $output .= theme('table', array(
      'header' => $header,
      'rows' => _mm_theme_mm_ui_node_reorder_table('mm-reorder1', $regions, $rendered_rows[1], $region_targetable[1]),
      'attributes' => array('id' => 'mm-reorder1')
    ));
  }
  if ($rendered_rows[0]) {
    if ($rendered_rows[1]) {
      $output .= t('<p></p><h3>Non-sticky content</h3>');
    }
    $output .= theme('table', array(
      'header' => $header,
      'rows' => _mm_theme_mm_ui_node_reorder_table('mm-reorder2', $regions, $rendered_rows[0], $region_targetable[0]),
      'attributes' => array('id' => 'mm-reorder2')
    ));
  }
  $output .= drupal_render_children($form);
  return $output;
}

function _mm_theme_mm_ui_node_reorder_table($table_id, $regions, $rows, $region_targetable) {
  $table = array();
  foreach ($regions as $region => $desc) {
    if (!empty($rows[$region]) || !empty($region_targetable[$region])) {
      drupal_add_tabledrag($table_id, 'match', 'sibling', 'nodes-region-select', 'nodes-region-' . $region);
      drupal_add_tabledrag($table_id, 'order', 'sibling', 'nodes-weight', 'nodes-weight-' . $region);
      $table[] = array(
        'class' => array('region-title', "region-title-$region"),
        'no_striping' => TRUE,
        'data' => array(
          array('colspan' => 5, 'data' => check_plain($desc)),
        ),
      );
      $table[] = array(
        'class' => array('region-message', "region-$region-message", !empty($rows[$region]) ? 'region-populated' : 'region-empty'),
        'no_striping' => TRUE,
        'data' => array(
          array('colspan' => 5, 'data' => t('<em>There is no content in this region.</em>')),
        ),
      );
      if (!empty($rows[$region])) {
        $table = array_merge($table, $rows[$region]);
      }
    }
  }
  return $table;
}

/**
 * Theme the node permissions
 *
 * @param $variables
 *   - form: The form element
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_ui_node_permissions($variables) {
  $form = $variables['form'];

  $form['indiv_tbl']['#type'] = 'value';
  $form['indiv_tbl']['#value'] = array(
    'title' => t('Individuals'),
    'action' => mm_ui_add_user_subform($form, 'settings-perms-indiv-add', t('add'), t('User(s) to add to permissions:'), t('Add users to permissions'), 'Drupal.MMSettingsPermsAddUsers'),
  );

  foreach (array('indiv_tbl', 'groups_tbl') as $type) {
    $kids = array();
    foreach ($form[$type] as $key => $item) {
      if (is_array($item) && isset($item['#mm_delete_link'])) {
        if (!isset($users)) {
          $users = $item['#mm_users'];
        }

        if ($type == 'indiv_tbl') {
          if (!isset($item_uid)) {
            $item_uid = $item['#mm_owner']['uid'];
          }

          if (isset($form['everyone']['owner'])) {
            $form['everyone']['owner']['#value'] = $item_uid;
          }

          if (!isset($item['#mm_owner']['show']) || !empty($item['#mm_owner']['show'])) {
            list($name, $msg, $owner_readonly) = mm_ui_owner_desc($form, array('@thing' => $item['#mm_owner']['type']), $item_uid);
            $kids[] = _mm_ui_perms_table_row(
              'user',
              'owner',
              t('<span class="settings-perms-owner-prefix">Owner: </span><span class="settings-perms-owner-name">!name</span>', array('!name' => $name)),
              $msg,
              $owner_readonly ? NULL : mm_ui_add_user_subform($form, 'settings-perms-indiv-owner', t('change'), t('Owner:'), t('Change the owner'), 'Drupal.MMSettingsPermsOwner', $item_uid, $name)
            );

            if (!$owner_readonly) {
              $kids[] = array(
                '#type' => 'hidden',
                '#name' => 'uid',
                '#value' => $item_uid,
                '#id' => "uid-$item_uid",
              );
            }
          }

          if (is_array($users)) {
            foreach ($users as $uid => $name) {
              $obj = array(
                "user-w-$uid" => array(
                  '#id' => "user-w-$uid",
                  '#name' => "user-w-$uid",
                  '#type' => 'hidden',
                  '#value' => $uid,
                ),
                array(
                  '#type' => 'item',
                  '#markup' => $name,
                ),
              );
              $kids[] = _mm_ui_perms_table_row('user', $uid, $obj, '', !empty($form['#readonly']) ? NULL : $item['#mm_delete_link']);
            }
          }

          if (empty($form['#readonly'])) {
            // Empty row to be used when adding new users
            $obj = array(
              'user-w-new' => array(
                '#attributes' => array('class' => array('user-w-new')),
                '#name' => 'user-w-new',
                '#type' => 'hidden',
                '#value' => 0,
              ),
              array(
                '#type' => 'item',
                '#markup' => '<div class="mm-permissions-user-new form-item"> </div>',
              ),
            );
            $kids[] = _mm_ui_perms_table_row('user', 'new', $obj, '', $item['#mm_delete_link']);
          }
        }
        else {
          if (!isset($groups)) {
            $groups = $item['#mm_groups'];
          }

          if (is_array($groups)) {
            foreach ($groups as $mmtid => $data) {
              $temp_elem = $item['mm_groups_elem'];
              $temp_elem[0]['#title'] = $data['name'];
              $temp_elem[0][0]['#markup'] = '<div class="form-item">' . $data['members'] . '</div>';
              $temp_elem[0]["group-w-$mmtid"] = array(
                '#id' => "group-w-$mmtid",
                '#name' => "group-w-$mmtid",
                '#type' => 'hidden',
                '#value' => $mmtid,
              );
              $kids[] = _mm_ui_perms_table_row('group', $mmtid, $temp_elem, '', !empty($form['#readonly']) ? NULL : $item['#mm_delete_link']);
            }
          }

          if (empty($form['#readonly'])) {
            // Empty row to be used when adding new groups
            $temp_elem = $item['mm_groups_elem'];
            $temp_elem[0]['#title'] = ' ';
            $temp_elem[0][0]['#attributes'] = array('class' => array('mm-permissions-group-new'));
            $temp_elem[0][0]['#markup'] = '<div class="mm-permissions-group-new form-item"> </div>';
            $temp_elem[0]['group-w-new'] = array(
              '#id' => 'group-w-new',
              '#name' => 'group-w-new',
              '#type' => 'hidden',
              '#value' => 0,
            );
            unset($form['groups_tbl'][$key]['mm_groups_elem']);
            $kids[] = _mm_ui_perms_table_row('group', 'new', $temp_elem, '', !empty($form['#readonly']) ? NULL : $item['#mm_delete_link']);
          }
        }
      }
      elseif (is_numeric($key)) {
        $kids[] = $item;
      }
      else {
        $kids[$key] = $item;
      }
    }
    $form[$type] = $kids;
  }

  $form['#attached']['js'][] = array('data' => mm_static('settings_perms', 'return_once'), 'type' => 'inline', 'scope' => 'footer');
  $form['#attached']['library'][] = array('monster_menus', 'xregexp');
  drupal_process_attached($form);
  return theme_mm_ui_permissions(array('form' => $form));
}

/**
 * Theme the mm permissions.
 *
 * @param $variables
 *   - form: The form element to theme
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_ui_permissions($variables) {
  $form = $variables['form'];
  $rows = array();
  foreach (element_children($form) as $section_id) {
    $colspan = 1;
    if (isset($form[$section_id]['#value']['types']) && is_array($form[$section_id]['#value']['types'])) {
      $colspan = count($form[$section_id]['#value']['types']) + 1;
    }

    $row = array(
      'class' => array('mm-permissions-header-row'),
      'data' => array(
        array(
          'header' => TRUE,
          'data' => '<h2>' . $form[$section_id]['#value']['title'] . '</h2>',
          'class' => array('first-col'),
        ),
      ),
    );

    if (isset($form[$section_id]['#value']['types']) && is_array($form[$section_id]['#value']['types']) && !empty($form[$section_id]['#value']['headings'])) {
      foreach ($form[$section_id]['#value']['types'] as $vals) {
        $row['data'][] = array(
          'header' => TRUE,
          'data' => '<h4>' . $vals[0] . '</h4>',
        );
      }
      $row['data'][] = array(
        'header' => TRUE,
        'data' => '',
      );
    }
    else {
      $row['data'][] = array(
        'header' => TRUE,
        'data' => '',
        'colspan' => $colspan,
      );
    }

    $rows[] = $row;
    foreach (element_children($form[$section_id]) as $row_id) {
      $row = !empty($form[$section_id][$row_id]['#mm_is_data_row']) ? array('class' => array('mm-permissions-data-row')) : array();
      foreach (element_children($form[$section_id][$row_id]) as $item_id) {
        $row['data'][] = drupal_render($form[$section_id][$row_id][$item_id]);
      }
      $rows[] = $row;
    }

    if (empty($form['#readonly']) && isset($form[$section_id]['#value']['action'])) {
      $rows[] = array(
        array('colspan' => $colspan, 'data' => ''),
        array('data' => $form[$section_id]['#value']['action']),
      );
    }
  }

  $output = '';
  if ($rows) {
    $output = theme('table', array(
      'rows' => $rows,
      'attributes' => array('class' => array('mm-permissions')),
    ));
  }
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Theme an mm_archive header
 *
 * @param $variables
 *   An associative array containing:
 *   - frequency: duration of time this archive references
 *   - date: date of this archive
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_archive_header($variables) {
  $format = array(
    'year' => t('Archive for %Y'),
    'month' => t('Archive for %B %Y'),
    'week' => t('Archive for the week of %B %e, %Y'),
    'day' => t('Archive for %A, %B %e, %Y'),
  );
  return strftime($format[$variables['frequency']], $variables['date']);
}

/**
 * Theme an mm_archive
 *
 * @param $variables
 *   An associative array containing:
 *   - list
 *   - frequency
 *   - this_mmtid
 *   - main_mmtid
 *   - archive_mmtid
 *   - date
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_archive($variables) {
  $format = array(
    '%Y',
    '%B',
    $variables['frequency'] == 'week' ? t('week of %b %e') : '%e',
  );
  $link_attributes = array('attributes' => array('class' => array('archive-link')));
  $out = array('header' => array(), 'links' => array());

  // remove unneeded upper levels
  $offset = 0;
  $list = $variables['list'];
  while (is_array($list) && count($list) == 1 && is_array($list[key($list)])) {
    $list = $list[key($list)];
    $offset++;
  }

  if (is_array($list) && !$list) {
    return $out;
  }

  $recent_link = '';
  if ($variables['this_mmtid'] == $variables['archive_mmtid']) {
    $recent_link = '<span class="archive-link">' . l(t('See the most recent entries'), mm_content_get_mmtid_url($variables['main_mmtid']), $link_attributes) . '</span>';
    $out['header'] = array(
      '#prefix' => '<h2 class="archive-header">',
      '#theme' => 'mm_archive_header',
      '#frequency' => $variables['frequency'],
      '#date' => $variables['date'],
      '#suffix' => '</h2>',
    );
    if (count($list) == 1) {
      $out['links'] = array(
        '#prefix' => '<div class="archive-links">',
        '#markup' => $recent_link,
        '#suffix' => '</div>',
      );
      return $out;
    }
  }

  if (!is_array($list) || !mm_content_user_can($variables['archive_mmtid'], MM_PERMS_READ)) {
    return $out;
  }

  if (count($list) == 1) {
    $out['links'] = array(
      '#prefix' => '<div class="archive-links">',
      '#markup' => l(t('See older entries'), 'mm/' . $variables['archive_mmtid'] . '/' . strftime('%Y-%m-%d', $list[key($list)]), $link_attributes),
      '#suffix' => '</div>',
    );
    return $out;
  }

  _mm_theme_archive_links_recurs($list, $offset, $format, $variables['archive_mmtid'], $out['links']);

  if ($out['links']) {
    $ul = array(
      '#prefix' => '<ul class="select-menu">',
      $out['links'],
      '#suffix' => '</ul>',
    );
    if ($variables['this_mmtid'] == $variables['archive_mmtid']) {
      $out['links'] = array(
        array(
          '#prefix' => '<label class="archive-link">',
          '#markup' => t('Choose another archive:'),
          '#suffix' => '</label>',
        ),
        $ul,
        array(
          '#markup' => $recent_link,
        ),
      );
    }
    else {
      $out['links'] = array(
        array(
          '#prefix' => '<label class="archive-link">',
          '#markup' => t('Choose an archive:'),
          '#suffix' => '</label>',
        ),
        $ul,
      );
    }
    $out['links']['#prefix'] = '<div class="archive-links">';
    $out['links']['#suffix'] = '</div>';

    mm_static('select_menu', TRUE);
  }

  return $out;
}

/**
 * Returns HTML for mm_page_wrapper
 *
 * @param $variables
 *   An associative array containing:
 *   - title: Title of the page
 *   - body: Body of the page
 *   - wrap_body_with_div: Self-explanatory
 *   - attributes: Miscellaneous attributes of the element.
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_page_wrapper($variables) {
  $head = drupal_get_html_head() . drupal_get_css() . drupal_get_js();
  $body_att = drupal_attributes($variables['attributes']);
  if ($variables['wrap_body_with_div']) $variables['body'] = '<div class="mm-page-wrapper">' . $variables['body'] . '</div>';
  $status = theme('status_messages');
  $footer = drupal_get_js('footer');
  $lang = $GLOBALS['language']->language;
  $title = $variables['title'];
  $body = $variables['body'];

  return <<<HTML
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="$lang"
  xml:lang="$lang">
<head>
  <meta http-equiv="Content-Style-Type" content="text/css" />
  <title>$title</title>
  $head
</head>
<body$body_att>
$status$body$footer
</body>
</html>
HTML;
}

/**
 * Theme the mm tabbed ui.
 *
 * @param $variables
 *   - title: Title of the tabbed ui
 *   - outer_id: ID tag of the outer div
 *   - tabs: The tabs to theme
 *   - selected: The default selected tab
 *   - before_tabs: Markup appearing before the tabs
 *   - show_by_default: Show (expand) the outer div by default
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_tabbed_ui($variables) {
  $title = $variables['title'];
  $outer_id = $variables['outer_id'];
  $tabs = $variables['tabs'];
  $selected = $variables['selected'];
  $before_tabs = $variables['before_tabs'];
  /* NOTE: Be careful not to have more than one form element with the same name
     on multiple tabs. If the element is required on one tab, and the user
     does not enter any value, it will be highlighted in red on EVERY tab on
     which it appears. */

  $output = "<div id=\"$outer_id\" class=\"mm-tabbed-ui\">";
  if (!empty($title)) $output .= "<h5><a href=\"#\">$title</a></h5>";
  $display = empty($variables['show_by_default']) ? ' style="display: none"' : '';
  $output .= "<div class=\"mm-tabs\"$display>$before_tabs<ul>";
  $forms = '';
  uasort($tabs, 'element_sort');  // sort by #weight
  $i = 0;
  foreach ($tabs as $id => $data) {
    $output .= '<li><a href="#' . $id . '"><span>' . $data['#title'] . '</span></a></li>';
    $forms .= '<div id="' . $id . '">' . $data['#data'] . '</div>';
    $tabs[$id]['#order'] = $i++;
  }

  drupal_add_library('system', 'ui.tabs');

  $selected_tab = 0;
  if ($selected && isset($tabs[$selected])) $selected_tab = $tabs[$selected]['#order'];
  mm_static('tabbed_ui_js', TRUE, $selected_tab);
  drupal_add_css(drupal_get_path('module', 'monster_menus') . '/css/mm_tabbed_ui.css');

  return $output . "</ul>$forms</div></div>";
}

/**
 * Returns the yes/no checkmarks.
 *
 * @param $variables
 *   An associative array containing:
 *   - yes: Whether it should be yes or no.
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_ui_mark_yesno($variables) {
  drupal_add_css(drupal_get_path('module', 'monster_menus') . '/css/mm.css');
  if ($variables['yes']) {
    return '<span class="mm-yesno-yes">&#10004;</span>';
  }
  return '<span class="mm-yesno-no">&#10007;</span>';
}

/**
 * Returns a new instance of the mmRenderer
 */
function theme_mm_get_renderer() {
  module_load_include('inc', 'monster_menus', 'mm_renderer');
  return new mmRenderer();
}

/**
 * Add custom code to the bottom of every page
 */
function theme_mm_page_footer() {
  $footer = mm_node_alert_add_to_footer();
  if (function_exists('AMH_debug_footer')) {
    $footer .= AMH_debug_footer();
  }
  $footer_array = mm_add_page_footer($footer);
  if ($footer_array) {
    return drupal_render($footer_array);
  }
}

/**
 * Return a text string for autocomplete #description fields
 */
function theme_mm_autocomplete_desc() {
  return t('Type part of the person\'s name, then choose a name from the list.');
}

/**
 * Returns HTML for a "you can't post comments" notice. Overrides
 * theme_comment_post_forbidden().
 *
 * @param $variables
 *   An associative array containing:
 *   - node: The comment node.
 *   - read: If TRUE, display a message assuming the user only wants to read
 *           comments. Otherwise, the message assumes read/write.
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_comment_post_forbidden($variables) {
  $node = $variables['node'];
  $read = !empty($variables['read']);
  global $user;

  // Since this is expensive to compute, we cache it so that a page with many
  // comments only has to query the database once for all the links.
  $authenticated_post_comments = &drupal_static(__FUNCTION__, NULL);

  if (!$user->uid) {
    if (!isset($authenticated_post_comments)) {
      // We only output a link if we are certain that users will get permission
      // to post comments by logging in.
      $comment_roles = user_roles(TRUE, 'post comments');
      $authenticated_post_comments = isset($comment_roles[DRUPAL_AUTHENTICATED_RID]);
    }

    if ($authenticated_post_comments) {
      // We cannot use drupal_get_destination() because these links
      // sometimes appear on /node and taxonomy listing pages.
      if (variable_get('comment_form_location_' . $node->type, COMMENT_FORM_BELOW) == COMMENT_FORM_SEPARATE_PAGE) {
        $destination = array('destination' => "comment/reply/$node->nid#comment-form");
      }
      else {
        $destination = array('destination' => "node/$node->nid#comment-form");
      }

      // Amherst: Change the login link for custom auth if possible
      if (function_exists('localmods_user_login_url')) {
        $dest = isset($destination) ? $destination['destination'] : NULL;
        $login = localmods_user_login_url($dest);
        $destination = NULL;
      }
      else {
        $login = 'user/login';
      }

      if (variable_get('user_register', USER_REGISTER_VISITORS_ADMINISTRATIVE_APPROVAL)) {
        // Users can register themselves.
        // Amherst: include $read
        $msg = $read ? '<a href="@login">Log in</a> or <a href="@register">register</a> to read or post comments' : '<a href="@login">Log in</a> or <a href="@register">register</a> to post comments';
        return t($msg, array('@login' => url($login, array('query' => $destination)), '@register' => url('user/register', array('query' => $destination))));
      }
      else {
        // Only admins can add new users, no public registration.
        // Amherst: include $read
        $msg = $read ? '<a href="@login">Log in</a> to read or post comments' : '<a href="@login">Log in</a> to post comments';
        return t($msg, array('@login' => url($login, array('query' => $destination))));
      }
    }
  }
}

/**
 * Display a list of files as thumbnails.
 *
 * @param $variables
 *   An associative array containing:
 *   - files: An array of file objects to display.
 *   - mode: MM_BROWSER_MODE_GALLERY or another mode.
 *   - mmtid: When mode = MM_BROWSER_MODE_GALLERY, the mmtid of the page.
 *   - (optional) style_name: The name of the image style to use, such as 'thumbnail'.
 * @return string
 *   HTML code
 * @ingroup themeable
*/
function theme_mm_browser_thumbnails($variables) {
  $style_name = empty($variables['style_name']) ? 'thumbnail' : $variables['style_name'];
  $output = '';
  foreach ($variables['files'] as $file) {
    $output .= theme('mm_browser_thumbnail', array('file' => $file, 'style_name' => $style_name, 'mode' => $variables['mode'], 'mmtid' => $variables['mmtid']));
  }
  // Remove the link around any images so the onclick Javascript executes correctly
  $output .= '<script>(function ($) {$(".mmtree-browse-imagepreview img").unwrap();})(jQuery);</script>';
  return $output;
}

/**
 * Theme a thumbnail.
 *
 * @param $variables
 *   An associative array containing:
 *   - file: A file object to display.
 *   - mode: MM_BROWSER_MODE_GALLERY or another mode.
 *   - mmtid: When mode = MM_BROWSER_MODE_GALLERY, the mmtid of the page.
 *   - (optional) style_name: The name of the image style to use, such as 'thumbnail'.
 *   - (optional) onclick: Javascript onclick even code for MM_BROWSER_MODE_NODE.
 * @return string
 *   HTML code
 * @ingroup themeable
 */
function theme_mm_browser_thumbnail($variables) {
  $file = $variables['file'];
  $style_name = empty($variables['style_name']) ? 'thumbnail' : $variables['style_name'];
  if (!isset($file)) {
    return '';
  }

  if (strstr($file->filemime, 'image')) {
    if (module_exists('styles')) {
      $thumbnail = theme('styles', array(
        'field_type' => 'file',
        'style_name' => $style_name,
        'description' => t('Thumbnail for !filename.', array('!filename' => $file->filename)),
        'object' => $variables['file'],
      ));
    }
    else {
      // Display a thumbnail for images.
      $thumbnail = theme('image_style', array(
        'path' => $file->uri,
        'style_name' => 'thumbnail',
        'alt' => t('Thumbnail for !filename.', array('!filename' => $file->filename)),
      ));
    }
  }
  // Display the file icon for other file types.
  else {
    $thumbnail = theme('file_icon', array(
      'file' => $file,
    ));
  }

  switch ($variables['mode']) {
    case MM_BROWSER_MODE_GALLERY:
      $link = "Drupal.mm_browser_gallery_add($variables[mmtid], '" . mm_ui_js_escape($file->filename) . "', $file->nid, $file->fid);";
      break;

    case MM_BROWSER_MODE_WYSIWYG:
      $link = "Drupal.mm_browser_change_parent_url('" . url("mm_media_assist/properties/$file->fid/$file->nid", array('query' => array('instanceId' => empty($_GET['browserInstanceId']) ? 0 : $_GET['browserInstanceId']))) . "');";
      break;

    case MM_BROWSER_MODE_NODE:
      $link = $variables['onclick'];
      break;

    default:
      $link = '#';
      break;
  }

  $short_title = check_plain($file->filename);
  $long_title = empty($file->title) ?
    t('@name (@size)', array('@name' => $short_title, '@size' => format_size($file->filesize))) :
    t('@name (@size) on page "@title"', array('@name' => $file->filename, '@title' => $file->title, '@size' => format_size($file->filesize)));
  return '<a href="#" onclick="' . $link . '" title="' . check_plain($long_title) . '"><div class="mmtree-browse-imagesingle ui-state-default ui-widget-content"><div class="mmtree-browse-caption">' . $short_title . '</div><div class="mmtree-browse-imagepreview" onclick="' . $link . '">' . $thumbnail . '</div></div></a>';
}

// ****************************************************************************
// * Private functions start here
// ****************************************************************************

function _mm_theme_archive_links_recurs($list, $offset, $format, $mmtid, &$out) {
  $return = NULL;
  ksort($list, SORT_NUMERIC);
  foreach ($list as $item) {
    if (is_array($item)) {
      $inner = array();
      $return = _mm_theme_archive_links_recurs($item, $offset + 1, $format, $mmtid, $inner);
      $out[] = array(
        '#prefix' => '<li>',
        '#markup' => trim(strftime($format[$offset], $return)),
        '#suffix' => '</li>',
      );
      $out[] = array(
        '#prefix' => '<ul>',
        $inner,
        '#suffix' => '</ul>',
      );
    }
    else {
      $out[] = array(
        '#prefix' => '<li>',
        '#markup' => l(trim(strftime($format[$offset], $item)), "mm/$mmtid/" . strftime('%Y-%m-%d', $item)),
        '#suffix' => '</li>',
      );
      $return = $item;
    }
  }
  return $return;
}

function _mm_theme_add_row_js($row) {
  $list = '';
  foreach ($row as $col) {
    $list .= ', ' . drupal_json_encode($col);
  }

  return "obj.addItem(false$list);";
}

// helper function for theme_mm_catlist when it calls _mm_theme_mm_list
function _mm_theme_list_info_cat($mmtid, $name, &$url, &$info, &$popup_URL) {
  $parts = explode('/', $mmtid);
  $mmtid = isset($parts[0]) ? $parts[0] : NULL;
  $node = isset($parts[1]) ? $parts[1] : NULL;

  $parents = mm_content_get_parents($mmtid);
  array_shift($parents);  // skip root
  if ($node) {
    $parents[] = $mmtid;
  }

  $url = implode('/', $parents);
  if ($mmtid) {
    $url .= "/$mmtid";
  }

  $path = array();
  foreach ($parents as $par) {
    if (!($tree = mm_content_get($par))) {
      break;
    }
    $path[] = mm_content_get_name($tree);
  }

  if (!$node) $path[] = $name;
  $info = implode('&nbsp;&raquo; ', $path);

  if (isset($popup_URL)) {
    $top = explode('/', $popup_URL, 2);
    if (($found = strstr($url, "/$top[0]/")) !== FALSE) {
      $url = substr($found, 1);
    }
  }
  else {
    $popup_URL = $url;
  }
}

// helper function for theme_mm_grouplist when it calls _mm_theme_mm_list
function _mm_theme_list_info_group($mmtid, $name, &$url, &$info, &$popup_URL) {
  $parents = mm_content_get_parents($mmtid);
  array_shift($parents);  // skip root
  $url = implode('/', $parents);

  if (!isset($popup_URL)) {
    $popup_URL = $url;
  }

  $url .= "/$mmtid";

  $info = mm_content_get_users_in_group($mmtid, '<br />', FALSE, 20, TRUE);
  if ($info == '') {
    $info = t('(none)');
  }
}

function _mm_theme_add_class($elt) {
  $list = isset($elt['#attributes']['class']) ? $elt['#attributes']['class'] : array();
  array_unshift($list, 'form-item');
  if (form_get_error($elt)) {
    $list[] = 'mm-list-error';
  }
  return implode(' ', $list);
}
