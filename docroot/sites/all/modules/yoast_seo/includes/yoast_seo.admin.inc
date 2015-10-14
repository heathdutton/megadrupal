<?php

/**
 * @file
 * Administration page callbacks for the Yoast SEO for Drupal module.
 */

/**
 * Main settings page.
 */
function yoast_seo_admin_settings_form() {
  $form = array();

  // Check if XML Sitemap is installed and enabled.
  if (module_exists('xmlsitemap')) {
    // Inform the user about altering the XML Sitemap configuration on the
    // module configuration page if he has access to do so.
    if (user_access('administer xmlsitemap')) {
      $xmlsitemap_description = t('You can configure the XML Sitemap settings at the !url.', array('!url' => l(t('XML Sitemap configuration page'), 'admin/config/search/xmlsitemap')));
    }
    else {
      $xmlsitemap_description = t('You do not have the permission to administer XML Sitemap.');
    }
  }
  else {
    // XML Sitemap is not enabled, inform the user he should think about
    // installing and enabling it.
    $xmlsitemap_description = t('You currently do not have XML Sitemap enabled. We strongly recommend you to install XML Sitemap. You can download the module from <a href="@project-page-url">@project-page-url</a>.', array('@project-page-url' => 'https://www.drupal.org/project/xmlsitemap'));
  }

  $form['xmlsitemap'] = array(
    '#type' => 'item',
    '#title' => t('XML Sitemap'),
    '#markup' => $xmlsitemap_description,
  );

  // Inform the user about altering the Metatag configuration on the module
  // configuration page if he has access to do so.
  // We do not check if the module is enabled since it is our dependency.
  if (user_access('administer meta tags')) {
    $metatag_description = t('You can configure and override the Metatag title & description default settings at the !url.', array('!url' => l(t('Metatag configuration page'), 'admin/config/search/metatags')));
  }
  else {
    $metatag_description = t('You currently do not have the permission to administer Metatag.');
  }

  $form['metatag'] = array(
    '#type' => 'item',
    '#title' => t('Configure Metatag default templates'),
    '#markup' => $metatag_description,
  );

  return $form;
}
