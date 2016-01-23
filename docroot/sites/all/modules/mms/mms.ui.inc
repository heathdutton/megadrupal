<?php
/**
 * @file
 * MMS module UI management.
 */

define('MMS_UI_NODES', 'mms_ui_nodes');
define('MMS_DATA_FILE', drupal_get_path('module', 'mms') . '/mms.data.inc');

/* --------------------------------------------------------------- MAIN FUNCS */
/**
 * _mms_ui_export()
 *
 * Creates a MMS_UI node for each function found in mms.data.inc.
 */
function _mms_ui_export() {
  $data = file_get_contents(MMS_DATA_FILE);
  if (preg_match_all(
      '`[\s]*(function[\s]+[^(]+\([^)]*\))[\s]+(\{[\s]*)`',
      $data, $matches, PREG_SET_ORDER | PREG_OFFSET_CAPTURE)
  ) {
    // Delete previously exported ui-nodes, if any.
    _mms_delete_uinodes();
    
    // Map data to export.
    $ui_nodes = [];
    foreach ($matches as $match) {
      $funcs[] = [
        'func_offset' => $match[0][1],
        'title' => $match[1][0],
        'body_offset' => $match[2][1] + 1,
      ];
    }
    
    // Create ui-nodes.
    foreach ($funcs as $row => $func) {
      if ($next_func = @$funcs[$row + 1]) {
        $body_width = $funcs[$row + 1]['func_offset'] - $func['body_offset'];
        $body = substr($data, $func['body_offset'], $body_width);
      } else {
        $body = substr($data, $func['body_offset']);
      }
      $body = htmlentities(
        trim(substr($body, 0, strrpos($body, '}'))),
        ENT_QUOTES | ENT_HTML401
      );
      if (!$node_id = _mms_create_node($func['title'], $body)) {
        // Error while creating nodes, cancel all.
        node_delete_multiple($ui_nodes);
        return FALSE;
      }
      $ui_nodes[] = $node_id;
    }
    
    // Update ui-nodes list.
    variable_set(MMS_UI_NODES, $ui_nodes);
    
    // Finally return the list of created nodes.
    return _mms_ui_show();
  }
}

/**
 * _mms_ui_show()
 *
 * Returns a list of links to the existing MMS_UI nodes.
 */
function _mms_ui_show() {
  module_load_include('inc', 'mms', 'mms.data');
  $ui_nodes = variable_get(MMS_UI_NODES);
  
  if (!$ui_nodes) {
    return _mms_admin_data('uitranslate_pages_none');
  }
  
  global $language, $base_path;
  $template = '
<li>
  <a href="' . $base_path . $language->language . '/node/%d/edit" target="_blank">
    %s
  </a>
</li>
    ';
  
  foreach (_mms_get_nodes_by_id($ui_nodes) as $nid => $title) {
    $return[] = sprintf($template, $nid, $title);
  }
  return
    _mms_admin_data('uitranslate_pages_title') .
    '<ul>' . implode("\n", $return) . '</ul>' .
    _mms_admin_data('uitranslate_pages_footer');
}

/**
 * _mms_ui_import()
 *
 * Creates a new version of mms.data.inc from the current set of MMS_UI nodes.
 */
function _mms_ui_import() {
  if ($ui_nodes = variable_get(MMS_UI_NODES)) {
    
    // Save original mms.data.inc if not yet.
    // (do it only once, in order to keep *really original* version)
    if (!file_exists(MMS_DATA_FILE . '.original')) {
      rename(MMS_DATA_FILE, MMS_DATA_FILE . '.original');
    }
    
    // Create new mms.data.inc from MMS_UI nodes.
    foreach (_mms_get_nodes_by_title($ui_nodes) as $title => $body) {
      $contents[] =
        $title . " {\n" .
        html_entity_decode(
          trim(preg_replace('`[\s]*</?p>[\s]*`', "\n", $body)),
          ENT_QUOTES | ENT_HTML401
        ) .
        "\n}";
    }
    file_put_contents(MMS_DATA_FILE, "<?php\n" . implode("\n", $contents));
    
    // Finally delete ui-nodes.
    _mms_delete_uinodes();
    
    return TRUE;
  }
}

/* ---------------------------------------------------------------- SUB FUNCS */
/**
 * _mms_create_node()
 *
 * Creates a node and returns its node_id (or NULL if not created).
 */
function _mms_create_node($title, $body, $type = 'page') {
  $node = new stdClass();
  $node->type = $type;
  node_object_prepare($node);
  $node->uid = 1;
  $node->status = 0;
  $node->language = LANGUAGE_NONE;
  $node->title = $title;
  $node->body[$node->language][0]['value'] = $body;
  $node->body[$node->language][0]['format']  = 'full_html'; # ??? plain text?
  $node->path['pathauto'] = FALSE;
  
  if($node = node_submit($node)) {
    node_save($node);
    return $node->nid;
  }
}
/**
 * _mms_delete_uinodes()
 */
function _mms_delete_uinodes() {
  if ($ui_nodes = variable_get(MMS_UI_NODES)) {
    node_delete_multiple($ui_nodes);
    variable_del(MMS_UI_NODES);
  }
}

/**
 * _mms_get_nodes_by_id()
 *
 * Return an array of array(nid => title) .
 */
function _mms_get_nodes_by_id($id_list) {
  return db_query("
SELECT `nid`, `title`
FROM {node}
WHERE `nid` IN (" . implode(",", $id_list) . ")
ORDER BY `title`
    ")->fetchAllKeyed();
}

/**
 * _mms_get_nodes_by_title()
 *
 * Return an array of array(title => body) .
 */
function _mms_get_nodes_by_title($id_list) {
  return db_query("
SELECT N.`title`, F.`body_value`
FROM {node} N JOIN {field_data_body} F ON
  F.`entity_type` = 'node' AND F.`bundle` = 'page' AND F.`entity_id` = N.`nid`
WHERE N.`nid` IN (" . implode(",", $id_list) . ")
ORDER BY N.`title`
    ")->fetchAllKeyed();
}
/* -------------------------------------------------------------------------- */
