<?php

/**
 * Menu callback; Listing of all current widget sets.
 */
function cta_selector_list() {
  $page = array();

  $cta_selectors = cta_selectors();

  $page['cta_selector_list'] = array(
    //'#markup' => theme('cta_selector_list', array('cta_blocks' => $cta_blocks)),
    '#markup' => theme_cta_selector_list(array('cta_selectors' => $cta_selectors)),
    '#attached' => array(
      'css' => array(drupal_get_path('module', 'cta') . '/cta.admin.css' => array()),
    ),
  );

  return $page;
}

/**
 * Form builder; Form for adding a new widget set.
 *
 * @ingroup forms
 * @see widgets_set_add_form_submit()
 * @see widgets_set_name_validate()
 */
function cta_selector_form($form, &$form_state, $selector) {
  $title = t('Edit %name CTA block', array('%name' => $selector['name']));
  drupal_set_title($title, PASS_THROUGH); 
  
  $form_state['selector'] = $selector;

  $form['name'] = array(
    '#type' => 'item',
    '#size' => '30',
    '#title' => t('Block name'),
    '#markup' => $selector['name'],
    //'#description' => t('The name is used to identify the CTA block. Use only lowercase alphanumeric characters, underscores (_), and hyphens (-).'),
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Block description'),
    '#default_value' => $selector['description'], //$cta_selector->description,
    '#maxlength' => 64,
    '#description' => t('A brief description of the selector block, e.g. "CTA: sidebar selector". Used for administrative purposes.'),
  );

  $form['cta_description'] = array(
    '#type' => 'textfield',
    '#title' => t('CTA description'),
    '#default_value' => isset($selector['data']['cta_description']) ? $selector['data']['cta_description'] : '', //$cta_selector->description,
    '#maxlength' => 64,
    '#description' => t('A description for the CTAs added to this selector, e.g. "sidebar CTA". Used for administrative purposes.'),
  );
  
  $header = array(
    t('Category'),
    t('Preset'),
    t('Operations'),
  );
  $selections = array(
    '@default' => t('Default'),
  );

  $preset_vocab = '';

  if (isset($selector['data']['preset_vocabulary'])) {
    $preset_vocab = $selector['data']['preset_vocabulary'];
    $vocab = taxonomy_vocabulary_machine_name_load($preset_vocab);
    if (!empty($vocab)) {
      $terms = taxonomy_get_tree($vocab->vid);
      foreach ($terms AS $id => $term) {
        $selections["@" . drupal_html_class($term->name)] = t("Category: %name", array('%name' => $term->name));
      }
    }
  }


  $rows = array();
  foreach ($selections AS $id => $name) {
    $path = '@' . drupal_html_class($id);
    $selection = cta_selection_load($id, $selector['name'], TRUE);
    $ops = l(t('change'), 'admin/structure/cta/select/' . $selector['name'] . '/' . urlencode($path), array('query' => drupal_get_destination())) . ' ';
    $stitle = '-';
    if (isset($selection['bean'])) {
      $stitle = $selection['bean']->label;
      $ops .= l(t('remove'), 'admin/structure/cta/select/bean/' . $selector['name'] . '/' . urlencode($path) . '/' . urlencode('@default') , array('query' => drupal_get_destination()));
    }
    else if (isset($selection['block'])) {
      $block_info = call_user_func($selection['block_module'] . "_block_info", $selection['block_delta']);
      $stitle = $block_info[$selection['block_delta']]['info'];
      $ops .= l(t('remove'), 'admin/structure/cta/select/bean/' . $selector['name'] . '/' . urlencode($path) . '/' . urlencode('@default') , array('query' => drupal_get_destination()));
      
      //$ops .= l(t('remove'), 'admin/structure/cta/select/block/' . $selector['name'] . '/' . urlencode($id) . '/' . urlencode('@default') . '/-' , array('query' => drupal_get_destination()));
    }
    
    $rows[] = array(
      $name,
      $stitle,
      $ops,
    );
  }
  $vars = array(
    'header' => $header,
    'rows' => $rows,
  );
  $markup = theme('table', $vars);

  $form['presets'] = array(
    '#type' => 'item',
    '#title' => t('CTA presets'),
    '#markup' => $markup,
  );
  
  $vocabs = taxonomy_get_vocabularies();
  $options = array(
    '' => t('-')
  );
  foreach ($vocabs AS $vocab) {
    $options[$vocab->machine_name] = $vocab->name;
  }
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE, 
  );
  $form['advanced']['preset_vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Preset vocabulary'),
    '#description' => t('Provide a vocabulary to sync multiple defaults. Must click Save changes for default presents to be available on this form.'),
    '#options' => $options,
    '#default_value' => $preset_vocab,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  return $form;
}
/**
 * Submit handler for saving an widget set.
 */
function cta_selector_form_submit($form, &$form_state) {
  // Update the widget set name if it has changed.
  $selector = $form_state['selector'];
  $preset_vocab0 = isset($selector['data']['preset_vocabulary']) ? $selector['data']['preset_vocabulary'] : '';
  if (isset($form_state['values']['name']) && $set['name'] != $form_state['values']['name']) {
    $selector['name'] = $form_state['values']['name'];
  }

  $selector['description'] = $form_state['values']['description'];
  $selector['data'] = array();

  if (!empty($form_state['values']['cta_description'])) {
    $selector['data']['cta_description'] = $form_state['values']['cta_description'];
  }

  if (!empty($form_state['values']['preset_vocabulary'])) {
    $selector['data']['preset_vocabulary'] = $form_state['values']['preset_vocabulary'];
  }
  cta_selector_save($selector);

  // if preset_vocabulary change, stay on edit form
  if ($preset_vocab0 == $form_state['values']['preset_vocabulary']) {
    $form_state['redirect'] = 'admin/structure/cta/selector';
  }
}

/**
 * Form builder; Form for adding a new widget set.
 *
 * @ingroup forms
 * @see widgets_set_add_form_submit()
 * @see widgets_set_name_validate()
 */
function cta_selector_add_form($form, &$form_state) {
  $form['name'] = array(
    '#type' => 'textfield',
    '#size' => '30',
    '#title' => t('Block name'),
    '#default_value' => '',
    '#description' => t('The name is used to identify the CTA block. Use only lowercase alphanumeric characters, underscores (_), and hyphens (-).'),
    '#element_validate' => array('cta_selector_name_validate'),
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Block description'),
    '#default_value' => '', //$cta_selector->description,
    '#maxlength' => 64,
    '#description' => t('A brief description of your block. Used for administrative purposes.'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Create new CTA block'),
  );

  return $form;
}

/**
 * Validate handler for cta block delta.
 */
function cta_selector_name_validate($element, &$form_state) {
  if (!preg_match('!^[a-z0-9_]+$!', $element['#value'])) {
    form_error($element, t('The machine-readable name must contain only lowercase letters, numbers, and underscores.'));
  }
  //if ((strpos($element['#value'], 'boxes_add__') === 0) || boxes_box_load($element['#value'])) {
  //  form_error($element, t('The machine-readable name is already taken.'));
  //}
}

/**
 * Submit handler for adding a new widget set.
 */
function cta_selector_add_form_submit($form, &$form_state) {
  $cta_selector = array(
    'name' => $form_state['values']['name'],
    'description' => $form_state['values']['description'],
  );
  $cta_selector = cta_selector_save($cta_selector);
  drupal_set_message(t('CTA block %name was created.', array('%name' => $cta_selector['name'])));
  $form_state['redirect'] = 'admin/structure/cta/selector/edit/' . $cta_selector['name'];
}

/**
 * Returns HTML for the page containing the list of widget sets.
 *
 * @param $variables
 *   An associative array containing:
 *   - sets: An array of all the widget sets returned by widgets_get_sets().
 *
 * @see widgets_get_sets()
 * @ingroup themeable
 */
function theme_cta_selector_list($variables) {  
  $cta_selectors = $variables['cta_selectors'];

  $header = array(t('Block name'), t('Description'), array('data' => t('Operations'), 'colspan' => 3));
  $rows = array();
  foreach ($cta_selectors as $set) {
    $row = array();
    $row[] = l($set['name'], 'admin/structure/cta/block/edit/' . $set['name']);
    $row[] = $set['description'];
    $link_attributes = array(
      'attributes' => array(
        'class' => array('widgets-set-link'),
      ),
    );
    $row[] = l(t('edit'), 'admin/structure/cta/selector/edit/' . $set['name'], $link_attributes);
    $row[] = l(t('delete'), 'admin/structure/cta/selector/delete/' . $set['name'], $link_attributes);
    $rows[] = $row;
  }

  if (empty($rows)) {
    $rows[] = array(array(
      'colspan' => 4,
      'data' => t('There are currently no CTA selector blocks. <a href="!url">Add a new one</a>.', array('!url' => url('admin/structure/cta/selector/add'))),
    ));
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/********************************************************************
 * CTA content functions
 * 
 */

function cta_cta_list() {
  $rows = array();

  // Build the sortable table header.
  $header = array(
    'title' => array(
      'data' => 'Title',
      'type' => 'property',
      'specifier' => 'title',
      'sort' => 'asc'
    ),
    'type' => array(
      'data' => 'Type',
      'type' => 'entity',
      'specifier' => 'bundle',
    ),
    'available' => array(
      'data' => t('Available in'),
    ),
    'selectors' => array('data' => t('Operations')),
  );

  //$filters = bean_get_filter();
  $filters['per_page'] = 50;
  $filters['types'] = array('cta');

  $query = new EntityFieldQuery();
  $query->entityCondition('entity_type', 'bean')
    ->tableSort($header)
    ->pager($filters['per_page']);

  if (!empty($filters['types'])) {
    $query->entityCondition('bundle', $filters['types'], 'IN');
  }

  $result = $query->execute();
  if (!empty($result)) {
    $beans = bean_load_multiple(array_keys($result['bean']));
  }
  else {
    $beans = array();
  }
  
  $selectors = cta_selectors();
  $metas = cta_bean_metas();
//dsm($selectors);
//dsm($metas);

  foreach ($beans as $bean) {
    $sstr = '';
    foreach ($metas[$bean->delta]['selectors'] AS $name) {
      $sstr  = (($sstr ) ? ', ' : '') . $selectors[$name]['description'];
    }
    $rows[$bean->bid] = array(
      'title' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => $bean->label,
          '#href' => 'block/' . $bean->delta,
          '#attributes' => array(
            'data-preview' => 'test',
          )
        ),
      ),
      'type' => (substr($bean->label, 0, 7) == 'custom ') ? t('custom') : check_plain($bean->type),
      'selectors' => $sstr,
    );

    $destination = drupal_get_destination();

    // Build a list of all the accessible operations for the current bean.
    $operations = array();
    $operations['edit'] = array(
      'title' => t('edit'),
      'href' => 'block/' . $bean->delta . '/edit',
      'query' => $destination,
    );
    $operations['delete'] = array(
      'title' => t('delete'),
      'href' => 'block/' . $bean->delta . '/delete',
      'query' => $destination,
    );
    // Render an unordered list of operations links.
    $rows[$bean->bid]['operations'] = array(
      'data' => array(
        '#theme' => 'links__bean_operations',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      ),
    );
  }

  $output = array(
    //'bean_filter_form' => drupal_get_form('bean_filter_form'),
    'bean_content' => array(
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#empty' => t('There are no Block Available'),
    ),
    'pager' => array(
      '#theme' => 'pager',
    ),
  );

  return $output;
}


/********************************************************************
 * Select CTA functions
 * 
 */

function cta_select_cta_list($selector, $path) {
  if ($lpath = libraries_get_path('jquery.qtip')) {
    //drupal_add_js($lpath . '/jquery.qtip.js');
    //drupal_add_css($lpath . '/jquery.qtip.css');
    //drupal_add_js(drupal_get_path('module', 'cta') . '/cta.cta_list.js');
  }
  
  $output = '';
  
  $destination = drupal_get_destination();
  $link_options = array(
    'query' => array('destination' => $_GET['destination'])
  );
  
  $rows = array();
  // Build the sortable table header.
  $header = array(
    'title' => array(
      'data' => t('Title (click to select)'),
    ),
    'type' => array(
      'data' => t('Type'),
    ),
    'operations' => array('data' => t('Operations')),
  );
  
  $selection = cta_selection_load(urldecode($path), $selector);
  
  $row = array(
    'data' => array(
      l('[' . t('Default') . ']', "admin/structure/cta/select/bean/$selector/$path/" . urlencode("@default"), $link_options),
      t('-'),  
      l(t('set defaults'), "admin/structure/cta/selector/edit/$selector", $link_options),  
    )
  );
  if (empty($selection)) {
    $row['class'][] = 'selected';
    $row['style'] = 'font-weight: bold;';     
  }
  $rows[] = $row;
  
  $nrows = cta_get_select_cta_list_beans($selector, $path, $selection, $link_options);
  
  $rows = array_merge($rows, $nrows);
  
  $nrows = cta_get_select_cta_list_blocks($selector, $path, $selection, $link_options);
  
  $rows = array_merge($rows, $nrows);
  
  ksort($rows);

  $row = '';
  $fieldset = cta_select_custom_fieldset($selector, $path, $selection, $link_options, $row);
 
  if (!empty($row)) {
    $rows['custom'] = $row;
  }

  $vars = array(
    'header' => $header,
    'rows' => $rows,
  );
  $output .= theme('table', $vars); 
  
  
  //$output .= drupal_render(drupal_get_form('cta_select_custom_form', $selector, $path, $selection, $link_options));
  $output .= cta_select_custom_fieldset($selector, $path, $selection, $link_options, $row);
  //$form = drupal_get_form('cta_select_custom_fieldset_form', $selector, $path, $selection, $link_options);
  //$output .= render($form);

  /*
    'ops' => array(
      '#type' => 'markup',
      '#markup' => l(t('Add CTA'), 'block/add/cta', $options) . ' | ' . l(t('All CTAs'), 'admin/content/blocks', $options), 
    ),
  );
  */
  return $output;
  
}

function cta_get_select_cta_list_beans($selector, $path, $selection, $link_options) {
  global $base_path;
  
  // list beans
  $query = db_select('bean', 'b')
    ->fields('b')
    ->condition('type', 'cta');
    //->extend('TableSort')
    //->orderByHeader($header);
//dsm($selector);  
  $alias = $query->innerJoin('cta_bean_selector', 's', 'b.delta = s.delta');
  $query->condition("{$alias}.selector", $selector);  

  $result = $query->execute();

  if (!isset($link_options['attributes'])) {
    $link_options['attributes'] = array();
  }
  /*
  if (!isset($link_options['attributes']['class'])) {
    $link_options['attributes']['class'] = array('qtip-bootstrap');
  }
  */
  
  $destination = drupal_get_destination();
  $rows = array();
  while ($bean = $result->fetchObject()) {
    $link_options['attributes']['data-tooltip-url'] = $base_path . 'cta/render/bean/' . $bean->delta;
    //$link_options['attributes']['data-tooltip-url'] = $base_path . 'block/' . $bean->delta;
   
    $row = array(
      'data' => array(
        l($bean->label, 'admin/structure/cta/select/bean/' . $selector . '/' . $path . '/' . $bean->delta, $link_options),
        t('bean') . ' (' . check_plain($bean->type) . ')',
        l(t('edit'),'block/' . $bean->delta . '/edit'),
      ),
    );
    if ($selection['bean_delta'] == $bean->delta) {
      $row['class'][] = 'selected';
      $row['style'] = 'font-weight: bold;';
    }
    $rows[$bean->label] = $row;
  } 
  //dsm($rows); 
  return $rows;
}

function cta_get_select_cta_list_blocks($selector, $path, $selection, $link_options) {
  global $base_path;
  $rows = array();
  $destination = drupal_get_destination();
  
  if (!isset($link_options['attributes'])) {
    $link_options['attributes'] = array();
  }
  
  $block_selectors = cta_blocks_by_selector($selector);
  $block_infos = array();
  
  foreach($block_selectors AS $block_selector) {
    if (!isset($block_infos[$block_selector['module']])) {
      $block_infos[$block_selector['module']] = call_user_func($block_selector['module'] . "_block_info");
      if (empty($block_infos[$block_selector['module']][$block_selector['delta']]['info'])) {
        continue;
      }
    }
    $link_options['attributes']['data-tooltip-url'] = $base_path . 'cta/render/' . $block_selector['module'] . '/' . $block_selector['delta'];
    $title = $block_infos[$block_selector['module']][$block_selector['delta']]['info'];
    $row = array(
      'data' => array(
        l($title, 'admin/structure/cta/select/block/' . $selector . '/' . $path . '/' . $block_selector['module'] . '/' . $block_selector['delta'], $link_options),
        t('block') . ' (' . $block_selector['module'] . ', ' . $block_selector['delta'] . ')',
        l(t('edit'), 'admin/structure/block/manage/' . $block_selector['module'] . '/' . $block_selector['delta'], $link_options),
      ),
    );
    if (($selection['block_module'] == $block_selector['module']) && ($selection['block_delta'] == $block_selector['delta'])) {
      $row['class'][] = 'selected';
      $row['style'] = 'font-weight: bold;';
    }     

    $rows[$title] = $row;
  }
//dsm($block_selectors);
//dsm($block_infos);
//dsm($blocks);
  return $rows;
}

function cta_select_custom_fieldset($selector, $path, $selection, $link_options, &$row = '') {
  include_once drupal_get_path('module', 'bean') . '/includes/bean.pages.inc';
  $out = '';
  
  $meta = cta_bean_meta_load_by_path_selector(urldecode($path), $selector); 
  
  if ($meta) {
    $bean = bean_load_delta($meta['delta']);
    $row = array(
      'data' => array(
        l('[' . t('Custom') . ']', 'admin/structure/cta/select/bean/' . $selector . '/' . $path . '/' . $bean->delta, $link_options),
        t('bean') . ' (' . check_plain($bean->type) . ')',
        l(t('delete'),'block/' . $bean->delta . '/delete'),
      ),
    );
    if ($selection['bean_delta'] == $bean->delta) {
      $row['class'][] = 'selected';
      $row['style'] = 'font-weight: bold;';
    }
  }
  else {
    $bean = bean_create(array('type' => 'cta'));
  }  
  
  $form = drupal_get_form('bean_form', $bean, 'cta');
  
  // auto generate label
  //$form['label']['#type'] = 'value';
  $form['label']['#value'] = t('custom') . ' ' . urldecode($path) . '#' . $selector; 
  //$form['label']['#value'] = $form['label']['#default_value']; 
  $form['label']['#theme'] = 'hidden';
  $form['label']['#theme_wrappers'][0] = 'hidden';
   
  $form['actions']['submit']['#value'] = t('Save and select');
  //$form['custom']['#submit'] = array('cta_select_custom_submit');
  unset($form['actions']['submit']['#submit']); 

  $out = drupal_render($form);
  
  $element = array(
    '#type' => 'fieldset',
    '#title' => t('Custom CTA'),
    '#attached' => array(
      'js' => array(
        'misc/form.js',
        'misc/collapse.js',
      ),
    ),
    '#attributes' => array(
      'class' => array('collapsible'),
    ),
    '#value' => $out,
    '#children' => array(),
  );
  // collapse fieldset if custom block is not selected
  if (!(substr($selection['bean_delta'], 0, 7) == 'custom-')) {
    $element['#attributes']['class'][] = 'collapsed';
  }
  $vars = array(
    'element' => $element,
  );
  $output = theme('fieldset', $vars);

  return $output;
}

function cta_select_bean($selector, $path, $delta) {
  $sel = cta_selector_load($selector);
  if (urldecode($delta) == "@default") {
    cta_selection_delete(urldecode($path), $selector);
    $vars = array(
      '%selector' => $sel['description'],
    );
    drupal_set_message(t('The default CTA for the %selector has been set to display on this page.', $vars));
  }
  else {
    cta_selection_save(urldecode($path), $selector, $delta);
    if (substr($delta, 0, 7) == 'custom-') {
      $vars = array(
        '%selector' => $sel['description'],
      );
      drupal_set_message(t('A custom CTA has been set to display in the %selector on this page.', $vars));      
    }
    else {
      $selection = cta_selection_load(urldecode($path), $selector, TRUE);
      $vars = array(
        '%title' => $selection['bean']->label,
        '%selector' => $sel['description'],
      );
      drupal_set_message(t('The CTA %title has been set to display in the %selector on this page.', $vars));
    }
  }
  drupal_goto($_GET['destination']);
}

function cta_select_block($selector, $path, $module, $delta) {
  $sel = cta_selector_load($selector);
  if (urldecode($module) == "@default") {
    cta_selection_delete(urldecode($path), $selector);
  }
  else {
    cta_selection_save(urldecode($path), $selector, '', $module, $delta);
    $selection = cta_selection_load(urldecode($path), $selector, TRUE);
    $vars = array(
      '%title' => $selection['block']['subject'],
      '%selector' => $sel['description'],
      '@path' => urldecode($path),
    );
    drupal_set_message(t('The block %title has been set to display in the %selector on this page.', $vars));
  }
  drupal_goto($_GET['destination']);
}

function cta_admin_intel_admin_config($form, $form_state) {
  $form = array_merge($form, cta_admin_intel_admin_config_form($form, $form_state));
  return system_settings_form($form);
}

function cta_admin_intel_admin_config_form($form, $form_state) {
  $form = array();
 
  $form['cta_intel_selector_postfix_override_bean'] = array(
    '#type' => 'textfield',
    '#title' => t('Bean jQuery selector postfix'),
    '#default_value' => variable_get('cta_intel_selector_postfix_override_bean', ''),
    '#description' => t('Enter a string to override the default postfix for bean selectors of " .content".'),
  ); 
  $form['cta_intel_selector_postfix_override_block'] = array(
    '#type' => 'textfield',
    '#title' => t('Block jQuery selector postfix'),
    '#default_value' => variable_get('cta_intel_selector_postfix_override_block', ''),
    '#description' => t('Enter a string to override the default postfix for block selectors of " .content".'),
  );  
  return $form;
}