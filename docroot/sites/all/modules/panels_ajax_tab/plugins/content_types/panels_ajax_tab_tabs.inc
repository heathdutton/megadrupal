<?php

/**
 * @file
 * Contains the content type plugin for a Ajax tabs.
 *
 */

$plugin = array(
  'title'            => t('AJAX Tab - Tabs'),
  'admin title'      => 'panels_ajax_tab_tabs_content_type_admin_title',
  'admin info'       => 'panels_ajax_tab_tabs_content_type_admin_info',
  'description'      => t('List of Tabs to control a container'),
  'single'           => TRUE,
  'category'         => t('AJAX Tabs'),
  'required context' => array(
    new ctools_context_optional(t('Entity Context'), 'entity'),
  ),
  'render callback'  => 'panels_ajax_tab_tabs_render',
  'edit form'        => 'panels_ajax_tab_tabs_edit_form'
);

function panels_ajax_tab_tabs_render($subtype, $conf, $panel_args, &$contexts) {
  static $default_tab_container_id = 0;
  ctools_include('context');
  ctools_include('plugins', 'panels');
  $block = new stdClass();
  $block->title = '';
  $block->content = '';

  // Add required JavaScript and CSS
  drupal_add_js(drupal_get_path('module', 'panels_ajax_tab') . '/js/panels_ajax_tab.js');
  drupal_add_css(drupal_get_path('module', 'panels_ajax_tab') . '/css/panels_ajax_tab.css');

  // Add in default url_enabled
  if (!isset($conf['url_enabled'])) {
    $conf['url_enabled'] = 1;
  }

  // Generate the context string
  if (!$contexts[0]->data) {
    $context_string = 'none';
  }
  else {
    $entity_type = $contexts[0]->type[2];
    $entity_ids = entity_extract_ids($entity_type, $contexts[0]->data);
    $entity_id = array_shift($entity_ids);
    $context_string = $entity_type . ':' . $entity_id;
  }

  $settings = variable_get('panels_ajax_tab', array('clean_url' => FALSE, 'clean_url_delim' => '/', 'panes' => array()));
  $clean_url = $settings['clean_url'];
  $clean_url_delim = isset($settings['clean_url_delim']) ? $settings['clean_url_delim'] : '/';

  // sort and load the mini-panels
  $tabs = array();
  uasort($conf['mini_panels'], 'panels_ajax_tab_compare_weight');
  $i = 0;
  foreach ($conf['mini_panels'] as $panel_name => $panel_conf) {
    if ($panel_conf['selected']) {

      // Set the URL ID - note the first one never has one regardless of what has been set
      if ($i) $url_id = $panel_conf['url_id'] ? $panel_conf['url_id'] : FALSE;
      else $url_id = '';

      if (!$url_id || !$conf['url_enabled']) {
        $href = url($_GET['q']);
      }
      else if ($clean_url) {
        $href = url($_GET['q']) . $clean_url_delim . $url_id;
      }
      else {
        $href = url($_GET['q']) . '#' . $url_id;
      }

      $mini = panels_mini_load($panel_name);
      $tabs[] = array(
        'mini_panel' => $mini,
        'title' => $panel_conf['tab_title'] ? $panel_conf['tab_title'] : $mini->admin_title,
        'url_id' => $url_id,
        'url_enabled' => !empty($panel_conf['url_id']) ? $conf['url_enabled'] : 0,
        'href' => $href
      );

      $i++;
    }
  }

  // Render the tabs
  $block->content .= theme('panels_ajax_tab_tabs', array('tabs' => $tabs, 'tab_container_id' => $conf['container_id'], 'context_string' => $context_string, 'clean_url' => $clean_url));

  return $block;
}

/**
 * Provide the administrative title of a Ajax tab.
 */
function panels_ajax_tab_tabs_content_type_admin_title($subtype, $conf) {

  return "Ajax Tab - " . $conf['container_id'];
}

/**
 * Display the administrative information for a Ajax tab .
 */
function panels_ajax_tab_tabs_content_type_admin_info($subtype, $conf, $contexts) {
  //return panels_ajax_tab_tabs_render($subtype, $conf, array(), $contexts);

  $data = new stdClass();
  $data->content = '';

  // sort and load the mini-panels
  $minipanels = array();
  uasort($conf['mini_panels'], 'panels_ajax_tab_compare_weight');
  foreach ($conf['mini_panels'] as $panel_name => $panel_conf) {
    if ($panel_conf['selected']) {
      $minipanels[] = panels_mini_load($panel_name)->admin_title;
    }
  }

  $content = '';
  $data->title = 'Panels Ajax Tabs - ' . implode(', ', $minipanels);

  return $data;
}

function panels_ajax_tab_tabs_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $settings = variable_get('panels_ajax_tab', array('clean_url' => FALSE, 'clean_url_delim' => '/', 'panes' => array()));

  $panels = panels_mini_load_all();
  foreach ($panels as $panel_name => $panel) {
    $panels[$panel_name] = (array) $panel;
    $panels[$panel_name] = empty($conf['mini_panels'][$panel_name]) ? $panels[$panel_name] : array_merge($panels[$panel_name], $conf['mini_panels'][$panel_name]);
  }
  if (!empty($conf['mini_panels'])) {
    uasort($panels, 'panels_ajax_tab_compare_weight');
  }

  $form['mini_panels']['#tree'] = TRUE;
  foreach ($panels as $mini_panel) {
    $form['mini_panels'][$mini_panel['name']] = array(

      'selected' => array(
        '#type' => 'checkbox',
        '#default_value' => $mini_panel['selected'],
      ),

      'name' => array(
        '#markup' => $mini_panel['admin_title'],
      ),

      'tab_title' => array(
        '#type' => 'textfield',
        '#default_value' => $mini_panel['tab_title'],
      ),

      'url_id' => array(
        '#type' => 'textfield',
        '#default_value' => $mini_panel['url_id'],
      ),

      'weight' => array(
        '#type' => 'weight',
        '#title' => t('Weight'),
        '#default_value' => $mini_panel['weight'],
        '#delta' => 10,
        '#title-display' => 'invisible',
      ),
    );
  }

  $form['prev_container_id'] = array(
    '#type' => 'hidden',
    '#value' => $conf['container_id'],
  );

  $form['container_id'] = array(
    '#title' => t('Container ID'),
    '#type' => 'textfield',
    '#description' => 'If you are using tabs in separate pane, you must supply a unique container id string.',
    '#default_value' => $conf['container_id'],
    '#required' => TRUE,
  );

  $form['url_enabled'] = array(
    '#title' => t('URL integration'),
    '#type' => 'checkbox',
    '#default_value' => isset($conf['url_enabled']) ? $conf['url_enabled'] : 1,
    '#description' => 'Enabling URL integration will allow users to link to a specific ajax-tab opening first.'
  );

  $form['clean_url'] = array(
    '#title' => t('Clean URLs'),
    '#type' => 'checkbox',
    '#default_value' => $settings['clean_url'],
    '#description' => 'Checking "Clean URLs" will cause the URL identifiers to be used to give the tabs a clean URL, and also allows the user to link to a specific tab without using an anchor link. It also makes it possible for web crawlers to crawl your tabs. Be careful to use a unique-looking URL identifier, as panels-ajax-tabs is greedy about claiming ownership over URL patterns. For example don\'t use "contact" if you also have a page somewhere that is at "/about/contact", instead use something like "tab-contact". When using clean URLs and features, you will additionally need to export the variable <tt>panels_ajax_tab</tt>',
    '#states' => array(
      'invisible' => array(
       ':input[name="url_enabled"]' => array('checked' => FALSE),
      ),
    ),
  );

  $form['clean_url_delim'] = array(
    '#title' => t('Clean URL delimiter'),
    '#type' => 'textfield',
    '#size'=> 1,
    '#default_value' => isset($settings['clean_url_delim']) ? $settings['clean_url_delim'] : '/',
    '#description' => 'Delimiter to be used for clean URLs',
    '#states' => array(
      'invisible' => array(
        ':input[name="url_enabled"]' => array('checked' => FALSE),
      ),
    ),
  );

  // Set the theme
  $form['#theme'] = 'panels_ajax_tab_tabs_edit_form';

  return $form;
}

function panels_ajax_tab_tabs_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['mini_panels'] =  $form_state['values']['mini_panels'];
  $form_state['conf']['container_id'] = $form_state['values']['container_id'];
  $form_state['conf']['url_enabled'] =  $form_state['values']['url_enabled'];

  // Save the global settings into a variable
  $settings = variable_get('panels_ajax_tab', array('clean_url' => FALSE, 'clean_url_delim' => '/', 'panes' => array()));
  $settings['clean_url'] = $form_state['values']['clean_url'];
  $settings['clean_url_delim'] = $form_state['values']['clean_url_delim'];

  if (isset($settings['panes'][$form_state['values']['prev_container_id']])) {
    unset($settings['panes'][$form_state['values']['prev_container_id']]);
  }
  // Sort and save the tabs into a variable
  uasort($form_state['conf']['mini_panels'], 'panels_ajax_tab_compare_weight');
  foreach ($form_state['conf']['mini_panels'] as $name => $mini) {
    if ($mini['selected']) {
      $settings['panes'][$form_state['values']['container_id']][$name] = $mini['url_id'];
    }
  }

  variable_set('panels_ajax_tab', $settings);
}
