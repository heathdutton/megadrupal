<?php

/**
 * @file
 * Custom content type.
 *
 * This content type is nothing more than a title and a body that is entered
 * by the user and run through standard filters. The information is stored
 * right in the config, so each custom content is unique.
 */

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
  'title' => t('New custom content (i18n)'),
  'no title override' => TRUE,
  'defaults' => array(array('general' => array('admin_title' => ''), 'languages' => array())),
  'js' => array('misc/autocomplete.js', 'misc/textarea.js', 'misc/collapse.js'),
  // Make sure the edit form is only used for some subtypes.
  'description' => t('Create a completely custom piece of HTML content.'),
  'edit form' => 'i18n_custom_content_i18n_custom_content_type_edit_form',
  'all contexts' => TRUE,
  'edit text' => t('Edit'),
  'single' => TRUE,
  'icon' => 'icon_block_custom.png',
  'category' => t('Custom'),
  'top level' => TRUE,
);

/**
 * Output function for the 'custom' content type. Outputs a custom
 * based on the module and delta supplied in the configuration.
 */
function i18n_custom_content_i18n_custom_content_type_render($subtype, $conf, $args, $contexts) {
  static $delta = 0;

  $block          = new stdClass();
  $block->subtype = ++$delta;

  global $language;
  if (!empty($conf['languages'][$language->language])) {
    $block->title = filter_xss_admin($conf['languages'][$language->language]['title']);

    $content = $conf['languages'][$language->language]['body']['value'];
    if (!empty($contexts) && !empty($conf['languages'][$language->language]['substitute'])) {
      $content = ctools_context_keyword_substitute($content, array(), $contexts);
    }
    $block->content = check_markup($content, $conf['languages'][$language->language]['body']['format']);
  }

  return $block;
}

/**
 * Callback to provide the administrative title of the custom content.
 */
function i18n_custom_content_i18n_custom_content_type_admin_title($subtype, $conf) {
  $output = t('i18n custom content');
  $title = !empty($conf['general']['admin_title']) ? $conf['general']['admin_title'] : NULL;
  if ($title) {
    $output = t('i18n custom contents: @title', array('@title' => $title));
  }

  return $output;
}

/**
 * Callback to provide administrative info. In this case we'll render the
 * content as long as it's not PHP, which is too risky to render here.
 */
function i18n_custom_content_i18n_custom_content_type_admin_info($subtype, $conf) {
  $block = new stdClass();
  $block->title = filter_xss_admin($conf['general']['admin_title']);
  return $block;

  // We don't want to render php output on preview here, because if something is
  // wrong the whole display will be borked. So we check to see if the php
  // evaluator filter is being used, and make a temporary change to the filter
  // so that we get the printed php, not the eval'ed php.
  $php_filter = FALSE;
  foreach (filter_list_format($settings['format']) as $filter) {
    if ($filter->module == 'php') {
      $php_filter = TRUE;
      break;
    }
  }
  // If a php filter is active, just print the source, but only if the current
  // user has access to the actual filter.
  if ($php_filter) {
    $filter = filter_format_load($settings['format']);
    if (!filter_access($filter)) {
      return NULL;
    }
    $block->content = '<pre>' . check_plain($settings['body']) . '</pre>';
  }
  else {
    // We also need to filter through XSS admin because <script> tags can
    // cause javascript which will interfere with our ajax.
    $block->content = filter_xss_admin(check_markup($settings['body'], $settings['format']));
  }
  return $block;
}

/**
 * Returns an edit form for the custom type.
 */
function i18n_custom_content_i18n_custom_content_type_edit_form($form, &$form_state) {
  $general_settings = $form_state['conf']['general'];
  $languages_settings = $form_state['conf']['languages'];

  $form_all = $form;
  $languages = language_list();
  $form_all['languages_tabs'] = array(
    '#type' => 'vertical_tabs',
  );

  $form_all['general'] = array(
    '#type' => 'fieldset',
    '#title' => t('General'),
    '#collapsible' => TRUE,
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#group' => 'languages_tabs',
    '#weight' => -99,
  );
  $form_all['general']['admin_title'] = array(
    '#type' => 'textfield',
    '#default_value' => isset($general_settings['admin_title']) ? $general_settings['admin_title'] : '',
    '#title' => t('Administrative title'),
    '#description' => t('This title will be used administratively to identify this pane. If blank, the regular title will be used.'),
  );
  $form_all['languages'] = array(
    '#tree' => TRUE,
  );

  foreach ($languages as $langcode => $language) {
    $settings = $languages_settings[$langcode];
    $form = array();

    $form['title'] = array(
      '#type' => 'textfield',
      '#default_value' => $settings['title'],
      '#title' => t('Title'),
    );

    $form['body'] = array(
      '#type' => 'text_format',
      '#title' => t('Body'),
      '#default_value' => $settings['body']['value'],
      '#format' => isset($settings['body']['format']) ? $settings['body']['format'] : filter_default_format(),
    );

    if (!empty($form_state['contexts'])) {
      // Set extended description if both CCK and Token modules are enabled, notifying of unlisted keywords
      if (module_exists('content') && module_exists('token')) {
        $description = t('If checked, context keywords will be substituted in this content. Note that CCK fields may be used as keywords using patterns like <em>%node:field_name-formatted</em>.');
      }
      elseif (!module_exists('token')) {
        $description = t('If checked, context keywords will be substituted in this content. More keywords will be available if you install the Token module, see http://drupal.org/project/token.');
      }
      else {
        $description = t('If checked, context keywords will be substituted in this content.');
      }

      $form['substitute'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use context keywords'),
        '#description' => $description,
        '#default_value' => !empty($settings['substitute']),
      );
      $form['contexts'] = array(
        '#title' => t('Substitutions'),
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
      );

      $rows = array();
      foreach ($form_state['contexts'] as $context) {
        foreach (ctools_context_get_converters('%' . check_plain($context->keyword) . ':', $context) as $keyword => $title) {
          $rows[] = array(
            check_plain($keyword),
            t('@identifier: @title', array('@title' => $title, '@identifier' => $context->identifier)),
          );
        }
      }
      $header = array(t('Keyword'), t('Value'));
      $form['contexts']['context'] = array(
        '#markup' => theme('table', array('header' => $header, 'rows' => $rows)),
        '#group' => 'form',
      );
    }
    $form_all['languages'][$langcode] = array(
      '#type' => 'fieldset',
      '#title' => $language->name,
      '#collapsible' => TRUE,
      '#collapsible' => FALSE,
      '#tree' => TRUE,
      '#group' => 'languages_tabs'
    ) + $form;
  }

  return $form_all;
}

/**
 * The validate form to ensure the custom content data is okay.
 */
function i18n_custom_content_i18n_custom_content_type_edit_form_validate(&$form, &$form_state) {
}

/**
 * The submit form stores the data in $conf.
 */
function i18n_custom_content_i18n_custom_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['general'] = $form_state['values']['general'];
  $form_state['conf']['languages'] = $form_state['values']['languages'];
}
