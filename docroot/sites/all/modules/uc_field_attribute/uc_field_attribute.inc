<?php
/**
 * @file uc_field_attribute.inc
 * Node functions for uc_field_attribute
 *
 * (C)2011 Michael Moritz miiimooo/at/drupal.org http://drupal.org/user/62954
 */

function _uc_field_attribute_nodeapi($node, $mode = NULL) {
  if (!array_key_exists($node->type, uc_product_class_load())) {
    return;
  }
  $attribute_settings = uc_field_attribute_load_settings($node->type);
  foreach($attribute_settings->fields as $fiid => $enabled) {
    if (!$enabled) {
      continue;
    }
    $bundle = field_extract_bundle('node', $node);
    $instances = field_info_instances('node', $bundle);
    $found = false;
    foreach($instances as $field_name => $instance) {
      if ($instance['id'] == $fiid) {
        $found = true;
        break;
      }
    }
    if (!$found) {
      continue; //TODO warning?
    }
    $instance = (object) $instance;
    $attribute = uc_field_attribute_load($node->nid, $instance->id);
    if (!$attribute) {
//       dsm($instance);
      $attribute = $instance;
      $attribute->display = 1;
      $attribute->name = $instance->field_name;
      $attribute->options = array();
      uc_attribute_save($attribute);
      $attribute = (object) $attribute;
      $field_attribute = array(
        "aid" => $attribute->aid,
        "nid" => $node->nid,
        "field_instance_id" => $instance->id,
      );
      drupal_write_record('uc_field_attribute_node', $field_attribute);
//       $attribute = (object) $field_attribute;
    }
    $product_attribute = (object) $attribute;
//     dsm($product_attribute);
    $attributes = uc_product_get_attributes($node->nid);
    // UPDATE or INSERT
    $exists = isset($attributes[$product_attribute->aid]);
//     $primary_keys = ($is_new ? NULL : array('aid', 'nid'));
    $product_attribute->nid = $node->nid;
    /// NOTE no API for this?
    if (!$exists) {
      drupal_write_record('uc_product_attributes', $product_attribute);
      $product_attribute = (object) $product_attribute;
    }
    $product_options = uc_field_attribute_options_load($product_attribute->aid);

    $items = field_get_items('node', $node, $field_name);

    foreach($items as $index => $item) {
      // this returns markup in an array
      // could probably make the $display configurable
      $value = field_view_value('node', $node, $field_name, $item);
      $value = strip_tags($value['#markup']);
      $product_attribute_oid = 0;
      foreach($product_attribute->options as $oid => $option) {
        if ($option->name == $value) {
          $product_attribute_oid = $oid;
          unset($product_attribute->options[$oid]);
          break;
        }
      }
      if (!$product_attribute_oid) {
        ///TODO could this already exist - should probably check through the table first
        $option = array(
          "aid" => $product_attribute->aid,
          'name' => $value,
          'ordering' => $index,
        );
        $option = (object) $option;
        uc_attribute_option_save($option);
        $option = (object) $option;
        $option->nid = $node->nid;
        drupal_write_record('uc_product_options', $option);
        drupal_write_record('uc_field_attribute_options', $option);
      }
    }
    //at this point $product_attribute->options holds all options that have "disappeared"
    //from the Field API field
    foreach($product_attribute->options as $oid => $option) {
      db_delete('uc_product_options')
        ->condition('oid', $oid)
        ->condition('nid', $node->nid)
        ->execute();
      db_delete('uc_attribute_options')
        ->condition('oid', $oid)
        ->execute();
      db_delete('uc_field_attribute_options')
        ->condition('aid', $product_attribute->aid)
        ->condition('oid', $oid)
        ->execute();
    }
  }
}
function _uc_field_attribute_node_delete($node) {
  $result = db_query("SELECT aid FROM {uc_field_attribute_node}
    WHERE nid = :nid",
    array(':nid' => $node->nid));
  foreach($result as $row) {
//     $options = uc_field_attribute_options_load($aid);
    db_delete('uc_attribute_options')
      ->condition('aid', $row->aid)
      ->execute();
    db_delete('uc_field_attribute_options')
      ->condition('aid', $row->aid)
      ->execute();
    db_delete('uc_attributes')
      ->condition('aid', $row->aid)
      ->execute();
  }
  db_delete('uc_field_attribute_node')
    ->condition('nid', $node->nid)
    ->execute();
}
function uc_field_attribute_options_load($aid) {
  $options = array();

  $result = db_query("SELECT * FROM {uc_field_attribute_options}
    WHERE aid = :aid", array(':aid' => $aid));
  foreach ($result as $option) {
    $options[$option->oid] = $option;
  }
  return $options;

}
function uc_field_attribute_save(&$attribute) {
  // Insert or update?
  $key = empty($attribute->aid) ? array() : 'aid';
  return drupal_write_record('uc_field_attribute_node', $attribute, $key);
}
function uc_field_attribute_load($nid, $fiid) {
  $aid = db_query("SELECT aid FROM {uc_field_attribute_node}
    WHERE nid = :nid AND field_instance_id = :fid",
    array(':nid' => $nid, ':fid' => $fiid))->fetchField();
  if (!$aid) {
    return false;
  }
  return uc_attribute_load($aid);
//   $aid = db_select('')
// //     ->fields('aid')
//     ->condition('nid', $nid)
//     ->condition('field_id', $fid)
//     ->execute();

}
