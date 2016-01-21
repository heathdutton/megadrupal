<?php

/**
 * @file
 * Implement Wysiwyg SEO form.
 */

/**
 * Helper that returns urls form or a custom form.
 */
function wysiwyg_smart_paging_seo_form() {
  $form = array();

  if (module_exists('metatag')) {
    module_load_include('inc', 'metatag', 'metatag.admin');
    drupal_add_css(drupal_get_path('module', 'metatag') . '/metatag.admin.css');
    drupal_add_js(drupal_get_path('module', 'metatag') . '/metatag.admin.js');

    $config = new stdClass();
    $config->cid = NULL;
    $config->instance = 'node:page';
    $config->config = array();

    $form = drupal_get_form('metatag_config_edit_form', $config);
  }
  else {
    $form = drupal_get_form('_wysiwyg_smart_paging_metatag_form');
  }

  $description = t('Add custom meta tags for this page by using the form below.<br />
      If you just want a page break and do not want to add custom meta tags, simply press Insert.<br />
      Cancel takes you back and does not add a pagebreak as well.');

  // Modify the form.
  $form['#prefix'] = '<div class="wysiwyg-sp-seo-popup">' . '<h2>Smart Paging SEO</h2>' . $description;
  $form['#suffix'] = '</div>';

  $form['url'] = array(
    '#type' => 'textfield',
    '#name' => 'url',
    '#title' => 'Fragment URL',
    '#size' => 100,
    '#weight' => 99,
    '#description' => t('A Fragment URL specifies a location within a page. i.e. http://example.com/help#main'),
  );

  $form['actions'] = array('#weight' => 100);
  $form['actions']['insert'] = array(
    '#type' => 'button',
    '#value' => t('Insert'),
    '#name' => 'insert',
    '#attributes' => array('class' => array('insert'),
      'onclick' => 'return (false);'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'button',
    '#value' => t('Cancel'),
    '#name' => 'cancel',
    '#attributes' => array('class' => array('cancel'),
      'onclick' => 'return (false);'),
  );

  // Allow other modules to alter this form.
  drupal_alter('wysiwyg_smart_paging_seo_form', $form);

  return $form;
}

/**
 * Implements hook_wysiwyg_smart_paging_seo_form_alter().
 */
function wysiwyg_smart_paging_seo_wysiwyg_smart_paging_seo_form_alter(&$form) {
  // Alter the form here.
}

/**
 * Implements hook_form().
 */
function _wysiwyg_smart_paging_metatag_form() {
  $tags = array(
    'global' => array(
      'title', 'description', 'abstract', 'keywords',
    ),
    'open-graph' => array(
      'fb:admins', 'fb:app_id', 'og:title', 'og:description',
      'og:image', 'og:url', 'og:latitude', 'og:longitude',
      'og:street-address', 'og:locality', 'og:region', 'og:postal-code',
      'og:country-name', 'og:email', 'og:phone_number', 'og:fax_number',
      'og:video', 'og:video:secure_url', 'og:video:width', 'og:video:height',
    ),
    'advanced' => array(
      'news_keywords', 'copyright', 'image_src',
      'canonical', 'shortlink', 'publisher', 'author',
      'original-source_content',
    ),
  );

  $form = array();
  $weight = 0;
  foreach ($tags as $group_name => $group) {
    $form[$group_name] = array(
      '#type' => 'fieldset',
      '#title' => ucfirst($group_name),
      '#collapsible' => TRUE,
      '#collapsed' => ($group_name != 'global') ? TRUE : FALSE,
      '#weight' => $weight,
    );
    foreach ($group as $tag) {
      $form[$group_name][$tag] = array(
        '#type' => 'textfield',
        '#name' => 'meta_name_' . $tag . '_content',
        '#title' => ucfirst($tag),
        '#size' => 100,
      );
    }
    $weight++;
  }

  return $form;
}
