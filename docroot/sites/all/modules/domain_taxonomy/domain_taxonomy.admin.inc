<?php
/**
 * @file
 */
// $Id$

function domain_taxonomy_configure_form($form_state, $user_submitted = FALSE) {
  $form = array();

  $form['domain_taxonomy_behavior'] = array(
    '#type' => 'fieldset',
    '#title' => t('Domain taxonomy module behaviors'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );

  $form['domain_taxonomy_behavior']['domain_taxonomy_options'] = array(
    '#type' => 'radios',
    '#title' => t('Term editing forms'),
    '#required' => TRUE,
    '#default_value' => variable_get('domain_taxonomy_options', 0),
    '#options' => array(
      0 => t('Pass the default form values as hidden fields'),
      1 => t('Take users to the default domain'),
      2 => t('Take users to their assigned domain'),
      3 => t('Show users their publishing options')),
  );

  $form['domain_taxonomy_behavior']['domain_taxonomy_debug'] = array(
    '#type' => 'radios',
    '#title' => t('Show Domain Taxonomy debugging information'),
    '#required' => TRUE,
    '#default_value' => variable_get('domain_taxonomy_debug', 0),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
  );

  $form['domain_taxonomy_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );

  $form['domain_taxonomy_advanced']['domain_taxonomy_seo'] = array(
    '#type' => 'radios',
    '#title' => t('Taxonomy search engine optimization'),
    '#default_value' => variable_get('domain_taxonomy_seo', 0),
    '#options' => array(0 => t('Do not rewrite URLs'), 1 => t('Rewrite all URLs to point to a single source')),
    '#description' => t('If rewrite is turned on, all term links will point to a single instance of the term. This option reduces the chance that search engines will recognize duplicate content.')
  );

  $form['domain_taxonomy_disable_vocs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Vocabularies without domain access rules'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );

  $form['domain_taxonomy_share_vocs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Share these vocabularies to all affiliates by default'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );

  $form['domain_taxonomy_node_inherit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Parent vocabularies for node types'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#description' => t('Inherit access rules for nodes from terms of selected vocabularies. If vocabulary is not selected, the node does not inherit domain rules from parent terms. <br />Example: choose vocabulary "Forum" for node type "Forum Topic" if you want nodes in forums to inherit parents forums domain access rules.')
  );

  $vocs = taxonomy_get_vocabularies();
  if (count($vocs)>0) {
    $voc_options[] = '';
    foreach ($vocs as $voc) {
      $voc_options[$voc->vid] = $voc->name;
      $form['domain_taxonomy_disable_vocs']['domain_disable_voc_' . $voc->vid] = array(
        '#type' => 'checkbox',
        '#title' => check_plain($voc->name),
        '#default_value' => variable_get('domain_disable_voc_' . $voc->vid, FALSE),
      );
      $form['domain_taxonomy_share_vocs']['domain_share_voc_' . $voc->vid] = array(
        '#type' => 'checkbox',
        '#title' => check_plain($voc->name),
        '#default_value' => variable_get('domain_share_voc_' . $voc->vid, variable_get('domain_taxonomy_behavior', DOMAIN_INSTALL_RULE)),
      );
    }
  }

  foreach (node_type_get_names() as $type => $name) {
    $form['domain_taxonomy_node_inherit']['domain_inherit_type_voc_' . $type] = array(
      '#type' => 'select',
      '#options' => $voc_options,
      '#title' => check_plain($name),
      '#default_value' => variable_get('domain_inherit_type_voc_' . $type, NULL),
    );
  }

  $form['domain_taxonomy_paths'] = array(
    '#type' => 'fieldset',
    '#title' => t('Term link patterns'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE
  );
  $form['domain_taxonomy_paths']['domain_taxonomy_paths'] = array(
    '#type' => 'textarea',
    '#rows' => 5,
    '#cols' => 40,
    '#default_value' => variable_get('domain_taxonomy_paths', "taxonomy/term/%t\r\ntaxonomy/edit/term/%t\r\nforum/%t"),
    '#description' => t('When using SEO or other path rewrites, the following link paths should be turned into absolute URLs. Enter the Drupal path of the link, using the <em>%t</em> placeholder to represent the term id. Enter one path per line. See the README for more details.')
  );

  return system_settings_form($form);
}
