<?php

/**
 * @file placeholder.inc
 * This provides a content pane for placeholder adverts for your development site.
 */


/**
 * Define the plugin
 */
$plugin = array(
  'title' => t('IAB Placeholder'),
  'defaults' => array(
    'size' => '',
  ),
  'icon' => 'placeholder.png',
  'description' => t('An IAB compliant ad placeholder'),
  'category' => t('Advertising'),
  'top level' => TRUE,
);


/**
 * Pane Render Callback
 */
function panels_iab_placeholder_content_type_render($subtype, $conf, $panels_args) {
  $block = (object)array(
    'module' => 'panels_iab',
    'delta' => $conf['size'],
    'title' => ($conf['override_title'] ? check_plain($conf['override_title_text']) : ''),
    'content' => theme('panels_iab_placeholder', array('size' => $conf['size'])),
  );

  return $block;
}


/**
 * Pane Settings Form
 */
function panels_iab_placeholder_content_type_edit_form($form, &$form_state) {
  // Get the configuration into a handy variable
  $conf = $form_state['conf'];

  // Build an array of options
  $options = array();
  $sizes = panels_iab_available_iab_sizes();
  foreach ($sizes as $provider => $provider_info) {
    foreach ($provider_info['groups'] as $group => $group_info) {
      $optgroup = "{$provider_info['title']} -- {$group_info['title']}";
      // Init the group
      $options[$optgroup] = array();

      foreach ($group_info['items'] as $file => $item_info) {
        // Note: The key and value do not need check_plain() protection as
        // Drupal does this for us in form_select_options().
        $key = "{$provider}__{$group}__{$file}";
        $options[$optgroup][$key] = t('!title (!width x !height)', array(
          '!title' => $item_info['title'],
          '!width' => $item_info['width'],
          '!height' => $item_info['height'],
        ));
      }
    }
  }

  // Generate a select box for the sizes
  $form['size'] = array(
    '#type' => 'select',
    '#title' => t('Advert Size'),
    '#default_value' => $conf['size'],
    '#description' => t('Pick and ad size'),
    '#required' => TRUE,
    '#options' => $options,
  );

  return $form;
}


/**
 * Pane Settings Form submit Handler
 */
function panels_iab_placeholder_content_type_edit_form_submit(&$form, &$form_state) {
  foreach (array('size') as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}


/**
 * Provide an Admin Title for a placeholder pane.
 */
function panels_iab_placeholder_content_type_admin_title($subtype, $conf) {
  return t('IAB Placeholder');
}


/**
 * Provide some admin info for the pane
 */
function panels_iab_placeholder_content_type_admin_info($subtype, $conf) {
  // Get the sizes
  $sizes = panels_iab_available_iab_sizes();

  // Extract the provider, group and filename
  list($provider, $group, $file) = explode('__', $conf['size']);

  // Validate Provider
  if (!isset($sizes[$provider])) {
    return t('Missing Provider @provider', array('@provider' => $provider));
  }
  else {
    $provider_title = $sizes[$provider]['title'];
  }

  // Validate Group
  if (!isset($sizes[$provider]['groups'][$group])) {
    return t('Missing Group @group', array('@group' => $group));
  }
  else {
    $group_title = $sizes[$provider]['groups'][$group]['title'];
  }


  // Validate the placeholder item / file
  if (!isset($sizes[$provider]['groups'][$group]['items'][$file])) {
    return t('Missing Placeholder Item @item', array('@item' => $file));
  }
  else {
    $placeholder = $sizes[$provider]['groups'][$group]['items'][$file];
  }

  // Render a block
  $block = panels_iab_placeholder_content_type_render($subtype, $conf, array());

  // Now we need to override the title
  $block->title = t('IAB Placeholder: @provider > @group > @title (@size)', array(
    '@provider' => $provider_title,
    '@group'    => $group_title,
    '@title'    => $placeholder['title'],
    '@size'     => $placeholder['width'] .' x '. $placeholder['height'],
  ));

  return $block;
}
