<?php

/**
 * @file
 * Contains the content type plugin for a Ajax tabs container.
 *
 */

$plugin = array(
  'title'            => t('AJAX Tab - Container'),
  'admin title'      => 'panels_ajax_tab_container_content_type_admin_title',
  'description'      => t('The AJAX content will be loaded within this pane'),
  'single'           => TRUE,
  'category'         => t('AJAX Container'),
  'required context' => array(
    new ctools_context_optional(t('Entity Context'), 'entity'),
  ),
  'render callback'  => 'panels_ajax_tab_container_render',
  'edit form'        => 'panels_ajax_tab_container_edit_form'
);

/**
 * Provide the administrative title of a Ajax tab container.
 */
function panels_ajax_tab_container_content_type_admin_title($subtype, $conf) {
  return "Ajax Tab Container - " . $conf['container_id'];
}

function panels_ajax_tab_container_render($subtype, $conf, $panel_args, &$contexts) {
  static $default_tab_container_id = 0;
  ctools_include('context');
  ctools_include('plugins', 'panels');
  $block = new stdClass();
  $block->title = '';
  $block->content = '';

  // Add required JavaScript and CSS
  $path = drupal_get_path('module', 'panels_ajax_tab');
  drupal_add_js(array('panel_ajax_tab' => array('path' => $path)), 'setting');
  drupal_add_js($path . '/js/panels_ajax_tab.js');

  // Generate the context string
  if (!$contexts[0]->data) {
    $context_string = 'none';
  }
  else {
    $entity_type = $contexts[0]->type[2];
    $entity_id = array_shift(entity_extract_ids($entity_type, $contexts[0]->data));
    $context_string = $entity_type . ':' . $entity_id;
  }

  // Get the tab-container-id
  $tab_container_id = $conf['container_id'];

  // If we have clean-urls turned on we can pre-load the content
  $settings = variable_get('panels_ajax_tab', array('clean_url' => FALSE, 'panes' => array()));
  $tabs = isset($settings['panes'][$tab_container_id]) ? $settings['panes'][$tab_container_id] : array();
  $clean_url = isset($settings['clean_url']) ? $settings['clean_url'] : FALSE;

  $loading_img = array(
    'path' => drupal_get_path('module', 'panels_ajax_tab') . '/images/loading.gif', 
    'alt' => t('Loading'),
    'title' => t('Loading'),
    'attributes' => array('class' => 'loading'),
  );
  $content = '<div class="panels-ajax-tab-loading" style ="display:none">' . theme('image', $loading_img) . '</div>';
  $preloaded = '';
  if ($tabs) {
    $preloaded = reset(array_keys($tabs));
    if ($clean_url && isset($_GET['panels_ajax_tab_tab'])) {
      if ($tabs[$_GET['panels_ajax_tab_tab']]) {
        $preloaded = $_GET['panels_ajax_tab_tab'];
      }
    }

    $mini = panels_mini_load($preloaded);
    $mini = panels_ajax_tab_prepare_mini($mini, $context_string);
    $layout = panels_get_layout($mini->display->layout);
    $content .= '<div class="panels-ajax-tab-wrap-' . $preloaded . '">' . panels_render_display($mini->display) . '</div>';
  }

  // Render the container
  $block->content .= theme('panels_ajax_tab_container', array('tab_container_id' => $tab_container_id, 'context_string' => $context_string, 'content' => $content, 'preloaded' => $preloaded, 'clean_url' => $clean_url));

  $default_tab_container_id++;

  return $block;
}

function panels_ajax_tab_container_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];

  $panels = panels_mini_load_all();

  $options = array();
  foreach ($panels as $panel) {
    $options[$panel->name] = $panel->admin_title;
  }

  $form['container_id'] = array(
    '#title' => t('Container ID'),
    '#type' => 'textfield',
    '#description' => 'If you are using tabs in seperate pane, you must supply a unique container id string.',
    '#default_value' => $conf['container_id'],
    '#states' => array(
      'invisible' => array(
        ':input[name="display_tabs"]' => array('checked' => TRUE),
      ),
    )
  );

  return $form;
}

function panels_ajax_tab_container_edit_form_submit(&$form, &$form_state) {
  $form_state['conf']['container_id'] = $form_state['values']['container_id'];
}
