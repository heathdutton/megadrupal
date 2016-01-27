<?php

/**
 * Theming function : Overview of all labels
 * 
 * @param $vars
 */
function theme_nedstat_label_overview($vars) {
  $rows = array();
  $header = array(
    t('Label'),
    t('Description'),
    t('Options'),
    t('Storage'),
    t('Operations')
  );
  
  foreach ((array)$vars['data'] as $label) {
    $total = count($label->options);
    $label->options = $total > 5 ? array_splice($label->options, 0, 5) : $label->options;
    
    $options = array();
    foreach ((array)$label->options as $option) {
      $options[] = $option->name;
    }
    
    $rows[] = array(
      '0' => $label->name,
      '1' => $label->description,
      '2' => implode(', ', $options) . ($total > 5 ? ', ...' : ''),
      '3' => $label->storage == STORED_IN_DATABASE ? 'db' : 'code',
      '4' => !empty($label->links) ? implode(' - ', $label->links) : '-',
    );
  }
  
  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Theming function : Overview of options in label form
 * 
 * @param $vars
 */
function theme_nedstat_label_options($vars) {
  $form = $vars['form'];
  
  $header = array(
    array('data' => t('Name'), 'title' => t('The name of the option.')),
    array('data' => t('Is default'), 'title' => t('Is this the default selected option.')),
  );
  
  $rows = array();
  foreach (element_children($form) as $key) {
    //unset checkbox title
    unset($form[$key]['is_default']['#title']);
    
    // Build the table row.
    $rows[$key] = array(
      'data' => array(
        array('data' => drupal_render($form[$key]['name']), 'class' => 'label-option-name'),
        array('data' => drupal_render($form[$key]['is_default']), 'class' => 'label-option-is-default'),
      ),
    );
    // Add any attributes on the element to the row, such as the ahah class.
    if (array_key_exists('#attributes', $form[$key])) {
      $rows[$key] = array_merge($rows[$key], $form[$key]['#attributes']);
    }
  }
  
  $output = '';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'nedstat-label-options-wrapper')));
  $output .= drupal_render_children($form);
  
  return $output;
}

/**
 * Theming function : Overview of all paths
 *
 * @param $vars
 */
function theme_nedstat_path_overview($vars) {
  $rows = array();
  $header = array(
    t('Url'),
    t('Countername'),
    t('Storage'),
    t('Operations')
  );

  foreach ((array)$vars['data'] as $path) {
    $rows[] = array(
      '0' => $path->url,
      '1' => $path->countername,
      '2' => $path->storage == STORED_IN_DATABASE ? 'db' : 'code',
      '3' => !empty($path->links) ? implode(' - ', $path->links) : '-',
    );
  }

  return theme('table', array('header' => $header, 'rows' => $rows));
}

/**
 * Theming function : path form
 *
 * @param $vars
 */
function theme_nedstat_path_add_form($vars) {
  $form = $vars['form'];
  
  $header = array(
    array('data' => t('Url'), 'title' => t('The url for which you want to create a countername.')),
    array('data' => t('Countername'), 'title' => t('The countername for this url.')),
  );
  
  $rows = array();
  foreach (element_children($form['paths']) as $key) {
    // Build the table row.
    $rows[$key] = array(
      'data' => array(
        array('data' => drupal_render($form['paths'][$key]['url']), 'class' => 'path-url'),
        array('data' => drupal_render($form['paths'][$key]['countername']), 'class' => 'path-countername'),
      ),
    );
    // Add any attributes on the element to the row, such as the ahah class.
    if (array_key_exists('#attributes', $form['paths'][$key])) {
      $rows[$key] = array_merge($rows[$key], $form['paths'][$key]['#attributes']);
    }
  }
  

  $output = '';
  $output .= theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'nedstat-path-wrapper')));
  $output .= drupal_render_children($form);
  
  return $output;
}