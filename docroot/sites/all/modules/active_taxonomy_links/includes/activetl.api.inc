<?php
function activetl_node_process($node, $field) {
  $field_value = field_get_items('node', $node, $field);
  if (empty($field_value[0])) {
  	return FALSE;
  }
  $text = $field_value[0]['value'];
  $result = $text;
  $vocs = variable_get('activetl_vocs', array());
  // Replace taxonomy terms in text by links
  foreach ($vocs as $voc) {
    $terms = taxonomy_get_tree($voc);
    foreach ($terms as $term) {
      $result = activetl_text_term_process($result, $term);
    }
  }
  // Save results
  if (activetl_node_result_save($node, $field, $result)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

function activetl_text_term_process($text, $term) {
  require_once 'simple_html_dom.php';
  $html = new simple_html_dom();
  $html->load(nl2br($text));
  // Don't parse text from ignore tags array.
  if ($html && is_object($html) && isset($html->nodes)) { 
  	$ignore_tags = array('a', 'no-activetl');
    $term_count = variable_get('activetl_limit', 0);
    $term_unlimited = ($term_count == 0) ? TRUE : FALSE;
    foreach ($html->find('text') as $subtext) {
      if (!$term_unlimited && $term_count < 1) {
        break;
      }
      // Don't parse text if it's a link already.
      if (in_array($subtext->parent->tag, $ignore_tags)) {
        continue;
      }
      if (!empty($subtext->innertext)) {
        if (substr_count($subtext->innertext, $term->name)) {
          $href = 'taxonomy/term/' . $term->tid;
          if (!variable_get('activetl_first_case', true)) {
            $match = '/\b' . $term->name . '\b/u';
          }
          else {
            $match = array();
            $match[] = '/\b' . ucfirst($term->name) . '\b/u';
            $match[] = '/\b' . lcfirst($term->name) . '\b/u';
          }
          $term_replace = function($matches) use ($href, &$term_count, $term_unlimited) {
            if ($term_unlimited || $term_count>0) {
              $term_count--;
              return l($matches[0], $href);
            }
            return $matches[0];
          };
          $subtext->innertext = preg_replace_callback($match, $term_replace, $subtext->innertext);
        }
      }
    } 
    $text = $html->save();
    $html->clear();
  } 
  unset($html);
  return $text;
}

function activetl_node_result_save($node, $field, $new_value) {
  // Keep original field value in db
  module_load_include('inc', 'activetl', 'model/activetl.db');
  $original_field_value = field_get_items('node', $node, $field);
  $info = activetl_db_node_info_select($node, $field);
  if ($info) {
    $info['value'] = $original_field_value[0]['value'];
    $result = activetl_db_node_info_update($info, array('id'));
  }
  else {
    $info = array('nid' => $node->nid, 'field' => $field, 'value' => $original_field_value[0]['value']);
    $result = activetl_db_node_info_update($info);
  }
  if ($result) {
    $node->{$field}[$node->language][0]['value'] = $new_value;
    node_save($node);
    return TRUE;
  }
  else {
    drupal_set_message(t('Active taxonomy links process failed.'), 'error');
    return FALSE;
  }
}

function activetl_node_unprocess($node, $field) {
  module_load_include('inc', 'activetl', 'model/activetl.db');
  $info = activetl_db_node_info_select($node, $field);
  if ($info) {
    $node->{$info['field']}[$node->language][0]['value'] = $info['value'];
    node_save($node);
    activetl_node_result_delete($node, $info['field']);
  }
}

function activetl_node_result_delete($node, $field) {
  $info = activetl_db_node_info_select($node, $field);
  if ($info) {
    activetl_db_node_info_delete($node, $field);
  }
}