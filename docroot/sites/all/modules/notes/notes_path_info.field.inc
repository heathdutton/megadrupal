<?php

/**
 * Implements hook_field_info().
 */
function notes_path_info_field_info() {
  return array(
    'notes_path_info' => array(
      'label' => t('Note Path Info'),
      'description' => t('Information about what a note references'),
      'default_widget' => 'notes_path_info_widget',
      'default_formatter' => 'notes_path_info_default_formatter',
    ),
  );
}

/**
 * Implements hook_field_validate()
 */
function notes_path_info_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {
  // TODO: review if we want to validate anything... probably not. Leaving this as a stub for now
  foreach ($items as $delta => $item) {
  }
}

/**
 * Implements hook_field_is_empty().
 */
function notes_path_info_field_is_empty($item, $field) {
  return (empty($item['rpath']) && empty($item['cpath']));
}

/**
 * Implements hook_field_formatter_info().
 */
function notes_path_info_field_formatter_info() {
  return array(
    // This formatter provides a link to the referenced item(s)
    'notes_path_info_default_formatter' => array(
      'label' => t('Link to referenced item(s)'),
      'field types' => array('notes_path_info'),
    ),
  );
}

/**
 * Implements hook_field_formatter_view().
 */
function notes_path_info_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();

  switch ($display['type']) {
    case 'notes_path_info_default_formatter':
      foreach($items as $delta => $item) {
        $citem = menu_get_item($item['cpath']);
        $ritem = menu_get_item($item['rpath']);
        if ($citem === $ritem) {
          $links['rpath'] = array(
            'title' => isset($ritem['title']) ? $ritem['title'] : t('Refers to'),
            'href' => $item['rpath'],
          );
        }
        else {
          $links['rpath'] = array(
            'title' => isset($ritem['title']) ? $ritem['title'] : t('Refers to'),
            'href' => $item['rpath'],
          );
          $links['cpath'] = array(
            'title' => isset($citem['title']) ? $citem['title'] : t('Related to'),
            'href' => $item['cpath'],
          );
        }
        $element[$delta] = array(
          '#type' => 'html_tag',
          '#tag' => 'p',
          '#value' => theme('links', array('links' => $links)),
        );
      }
    break;
  }
  return $element;
}

/**
 * Implements hook_field_widget_info().
 */
function notes_path_info_field_widget_info() {
  return array(
    'notes_path_info_widget' => array(
      'label' => t('Managed Path Info'),
      'field types' => array('notes_path_info'),
    ),
  );
}

/**
 * Implements hook_field_widget_form().
 */
function notes_path_info_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  // if there is no value, set it to the request_path()
  // TODO find a better way to avoid cached paths insted of detecting empty nids (this only works on node entities as is);
  $rpath = isset($items[$delta]['rpath']) ? $items[$delta]['rpath'] : '';
  $cpath = isset($items[$delta]['cpath']) ? $items[$delta]['cpath'] : '';
  if (empty($rpath) || (isset($form['nid']) && empty($form['nid']['#value']))) {
    // TODO how do we take a generic note? 
    // node/add/CONTENTYPE probably isn't sufficient 
    // whitelist urls in config?
    $rpath = request_path();
    $cpath = current_path();
  }
  
  $element['rpath']['#type'] = 'value';
  $element['rpath']['#value'] = $rpath;
  $element['cpath']['#type'] = 'value';
  $element['cpath']['#value'] = $cpath;
  return $element;
}
