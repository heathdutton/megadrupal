<?php

class dan_polaroid_row_plugin extends views_plugin_row {


  // Basic properties that let the row style follow relationships.
  var $base_table = 'node';
  var $base_field = 'nid';

  // Stores the nodes loaded with pre_render.
  var $entities = array();

  function options(&$options) {
    $options['height'] = array('default' => '300px');
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $image_fields = _dan_polaroid_style_get_image_fields();

    $image_options = array('none' => t('- None -'));
    foreach ($image_fields['name'] as $item => $value) {
      $image_options[$item] = $value['label'];
    }

    $form['image_field'] = array(
      '#type' => 'select',
      '#title' => t('Image field'),
      '#options' => $image_options,
      '#default_value' => $this->options['height'],
    );
  }

  function pre_render($values) {

   $ids = array();
    foreach ($values as $row) {
      // Use the $id as the key so we don't create more than one value per entity.
      $id = $row->{$this->field_alias};
      $ids[$id] = $id;
    }

    if (!empty($ids)) {
      $this->entities = entity_load($this->view->base_table, $ids);
    }

    //dpm($result, 'dan_polaroid_row_plugin::pre_render');
  }

  function render($row) {
    //dpm($row, 'dan_polaroid_row_plugin::render');

    $id = $row->{$this->field_alias};
    if (!is_numeric($id)) {
      return;
    }

    // Load the specified entity:
    $entity = $this->entities[$id];
    if (empty($entity)) {
      return;
    }

    // If the user specified a LOCATION field, pull that data from the entity.
    $image = '';
    if (!empty($this->options['image_field']) && $this->options['image_field'] != 'none') {
      $image_fields = _dan_polaroid_style_get_image_fields($this->base_table);
      $image_info = $image_fields['name'][$this->options['image_field']];
      $image_field_name = $image_info['real_field_name'];

      dpm($image_field_name, 'image_field_name');

      // Only attempt this is the entity actually has this field.

      $items = field_get_items($this->entity_type, $entity, $image_field_name);
      if ($items) {
        $image_field = $items[$delta];
        if ($image_info['type'] == 'image') {
        }
      }
    }

    dpm($items, 'items');

    return theme($this->theme_functions(), array(
      'view' => $this->view,
      'options' => $this->options,
      'row' => $row,
      'field_alias' => isset($this->field_alias) ? $this->field_alias : '',
    ));


  }

}

/**
 *
 */
function _dan_polaroid_style_get_image_fields($base = 'node') {
  // Make sure $base is never empty.
  if (empty($base)) {
    $base = 'node';
  }

  // Iterate over all the fields that Views knows about.
  $all_fields = views_fetch_fields($base, 'field');
  $fields = array();
  foreach ($all_fields as $alias => $val) {
    $name = $alias;
    $tmp = explode('.', $name);
    $field_name = $tmp[1];
    $table_name = $tmp[0];

    // Skip unsupported field types and fields that weren't defined through
    // the Field module.
    $info = field_info_field($field_name);
    if (!$info || !in_array($info['type'], array('image'))) {
      continue;
    }

    // Build an array of the field info that we'll need.
    $alias = str_replace('.', '_', $alias);
    $fields['name'][$name] = array(
      'label' => "{$val['group']}: {$val['title']} ($field_name)",
      'table_name' => $table_name,
      'field_name' => $field_name,
      'type' => $info['type'],
    );

    // These are here only to make this $field array conform to the same format
    // as the one returned by _date_views_fields(). They're probably not needed,
    // but I thought that consistency would be a good idea.
    $fields['name'][$name]['real_field_name'] = $field_name;
    $fields['alias'][$alias] = $fields['name'][$name];
  }

  return $fields;
}
