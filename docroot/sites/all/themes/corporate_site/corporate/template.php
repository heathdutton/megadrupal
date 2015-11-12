<?php 

/**
 * Preprocess theme function to print a single record from a row, with fields
 */
function corporate_preprocess_views_view_fields(&$vars) {
  if (isset($vars['view']->style_plugin->definition['module']) && $vars['view']->style_plugin->definition['module'] == 'views_slideshow') {
    $fields = $vars['fields'];
    if (count($fields) >= 2) {
      $fields_key = array_keys($fields);
      $fields[$fields_key[1]]->wrapper_prefix = '<div class="slideshow-group-fields-wrapper">' . $fields[$fields_key[1]]->wrapper_prefix;
      $fields[$fields_key[count($fields_key) - 1]]->wrapper_suffix = $fields[$fields_key[count($fields_key) - 1]]->wrapper_suffix . '</div>';
    }
  }
}

/**
 * Override or insert variables into the block template.
 */
function corporate_preprocess_block(&$vars) {
  $block = $vars['block'];
  $subject = $block->subject;
  $parts = explode(" ", $subject);
  $vars['corporate_block_subject'] = false;
  if (count($parts) > 1) {
    $pre_subject = $parts[0];
    unset($parts[0]);
    $subject = implode(" ", $parts);
    $vars['corporate_block_subject'] = "<span class='first-word'>" . $pre_subject . "</span> " . $subject;
  }
}

function corporate_preprocess_page(&$vars) {
  if (isset($vars['node'])) {
    if ($vars['node']->type != 'page') {
      $result = db_select('node_type', NULL, array('fetch' => PDO::FETCH_ASSOC))
        ->fields('node_type', array('name'))
        ->condition('type', $vars['node']->type)
        ->execute();
      foreach ($result as $item) {
        $vars['title'] = $item['name'];
      }
    }
  }
}

function corporate_preprocess_node(&$vars) {
  $vars['page'] = ($vars['type'] == 'page') ? TRUE : FALSE;
}
