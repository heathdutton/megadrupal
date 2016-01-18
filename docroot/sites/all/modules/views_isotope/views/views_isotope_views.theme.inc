<?php
/**
 * @file
 * Theme callbacks.
 */

/**
 * Preprocess function to build the isotope grid.
 */
function template_preprocess_views_isotope_views_grid(&$vars) {
  template_preprocess_views_view_list($vars);

  $items = array();
  foreach ($vars['view']->result as $key => $value) {
    $item['value'] = $vars['rows'][$key];
    foreach ($vars['options']['data_fields'] as $fieldname) {
      if (!empty($fieldname)) {
        $item['data'][$fieldname] = views_isotope_views_helper_get_field_value($value, $fieldname);
      }
    }
    $items[] = $item;
  }

  $output = array(
    '#theme' => 'isotope_grid',
    '#items' => $items,
    '#instance' => $vars['options']['instance_id'],
    '#config' => !empty($vars['options']['config']) ? $vars['options']['config'] : 'isotope_default_config',
  );
  $vars['isotope_grid'] = render($output);
}

/**
 * Preprocess function to build isotope filter blocks.
 */
function template_preprocess_views_isotope_views_filter(&$vars) {
  template_preprocess_views_view_list($vars);

  $items = array();
  foreach ($vars['view']->result as $key => $value) {
    $label = $vars['rows'][$key];
    $fieldname = $vars['options']['data_fields'];
    $filter_value = views_isotope_views_helper_get_field_value($value, $fieldname);
    $items[$filter_value] = $label;
  }

  $output = array(
    '#theme' => 'isotope_filter',
    '#items' => $items,
    '#instance' => $vars['options']['instance_id'],
    '#filtername' => $vars['options']['filter_group'],
    '#filtertitle' => $vars['options']['filter_group'],
  );

  $vars['isotope_filter'] = render($output);
}

/**
 * Preprocess function for isotope sort blocks.
 */
function template_preprocess_views_isotope_views_sorter(&$vars) {
  template_preprocess_views_view_list($vars);

  $items = array();
  foreach ($vars['options']['data_fields'] as $fieldname) {
    if (!empty($fieldname)) {
      $field = $vars['view']->field[$fieldname];
      $label = $field->options['label'];
      $items[$label] = $fieldname;
    }
  }

  $output = array(
    '#theme' => 'isotope_sorter',
    '#sorts' => $items,
    '#instance' => $vars['options']['instance_id'],
    '#original' => $vars['options']['original'],
  );

  $vars['isotope_sorter'] = render($output);
}

/**
 * Helper function to get a field value.
 *
 * @todo: this seems very messy - there must be a better way to do it?
 *
 * @param object $entity
 *   The entity.
 * @param string $fieldname
 *   The field name.
 *
 * @return string
 *   Value.
 */
function views_isotope_views_helper_get_field_value($entity, $fieldname) {
  if (!empty($entity->{'field_' . $fieldname})) {
    foreach($entity->{'field_' . $fieldname} as $a) {
      if(!empty($a['rendered'])) {
        $field[] = $a['rendered'];
      }
    }
  }
  elseif (!empty($entity->{'taxonomy_term_data_' . $fieldname})) {
    // Term title is not a real field in D7.
    $field[$fieldname] = $entity->{'taxonomy_term_data_' . $fieldname};
  }

  if (isset($field)) {
    $r = array();
    if(!is_array($field)) {
      $field = array($field);
    }
    foreach($field as $f) {
      $filter_value = render($f);
      $r[] = trim(check_plain(strip_tags($filter_value)));
    }
    if(count($r) == 1){
      return $r[0];
    }
    return $r;
  }

  return '';
}
