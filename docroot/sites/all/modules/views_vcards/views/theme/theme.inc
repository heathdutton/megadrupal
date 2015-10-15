<?php

/**
 * @file
 * Preprocessors and helper functions to make theming easier.
 */

/**
 * Preprocess a vCards list.
 */
function template_preprocess_views_vcards_view_vcard(&$vars) {
  $view = &$vars['view'];
  $options = &$vars['options'];
  $items = &$vars['rows'];

  $style = &$view->style_plugin;

  // Figure out which display which has a path we're using for this feed. If
  // there isn't one, use the global $base_url.
  $link_display_id = $view->display_handler->get_link_display();
  if ($link_display_id && !empty($view->display[$link_display_id])) {
    $path = $view->display[$link_display_id]->handler->get_path();
  }

  if ($path) {
    $path = $view->get_url(NULL, $path);
    $url_options = array('absolute' => TRUE);
    if (!empty($view->exposed_raw_input)) {
      $url_options['query'] = $view->exposed_raw_input;
    }

    // Compare the link to the default home page; if it's the default home
    // page, just use $base_url.
    if ($path == variable_get('site_frontpage', 'node')) {
      $path = '';
    }

    $vars['link'] = check_url(url($path, $url_options));
  }

  // During live preview we don't want to output the header since the contents
  // of the feed are being displayed inside a normal HTML page.
  if (empty($vars['view']->live_preview)) {

    // Only build the zip if we are not watching the preview.
    if (count($items) > 1) {

      // Try to load the library and check if that worked.
      if (($library = libraries_load('zipstream')) && !empty($library['loaded'])) {
        $zip = new ZipStream($view->human_name . '.zip');
        foreach ($items as $name => $item) {
          $zip->add_file($name . '.vcf', $item);
        }
        $zip->finish();
      }
      else {
        drupal_set_message(t("The zipstream library was not loaded"), 'error');
      }
      // Even though we present a zip with download headers, the view needs
      // something otherwise it crashes.
      $vars['items'] = 'foo';

      // Add headers for zip file. Content is already built by $zip->finish().
      drupal_add_http_header('Content-Type', 'application/zip');
      drupal_add_http_header('Content-Disposition', 'attachment; filename=' .
        $view->human_name . '.zip');
      drupal_add_http_header('Pragma', 'no-cache');
      drupal_add_http_header('Expires', '0');
    }
    else {
      // Only one item? Add headers for vCard and output single (first) item.
      drupal_add_http_header('Content-Type', 'text/vcard; charset=utf-8');
      drupal_add_http_header('Content-Disposition', 'attachment; filename=' .
        key($items) . '.vcf');

      $vars['items'] = array_shift($items);
    }
  }
  else {
    // If we are in preview mode, just place all vCards under each other.
    $vars['items'] = implode("\n", $items);
  }
}

/**
 * Default theme function for all vCards entries.
 *
 * Sanitizes all items on a row.
 */
function template_preprocess_views_vcards_view_row_vcard(&$vars) {
  $view = &$vars['view'];
  $options = &$vars['options'];
  $item = &$vars['row'];

  $vars['first_name'] = check_plain($item->first_name);
  $vars['middle_name'] = check_plain($item->middle_name);
  $vars['last_name'] = check_plain($item->last_name);
  $vars['full_name'] = check_plain($item->full_name);
  $vars['title'] = check_plain($item->title);
  $vars['email'] = check_plain($item->email);
  $vars['email2'] = check_plain($item->email2);
  $vars['email3'] = check_plain($item->email3);
  $vars['home_address'] = check_plain($item->home_address);
  $vars['home_city'] = check_plain($item->home_city);
  $vars['home_state'] = check_plain($item->home_state);
  $vars['home_zip'] = check_plain($item->home_zip);
  $vars['home_country'] = check_plain($item->home_country);
  $vars['home_phone'] = check_plain($item->home_phone);
  $vars['home_cellphone'] = check_plain($item->home_cellphone);
  $vars['home_website'] = check_plain($item->home_website);
  $vars['work_title'] = check_plain($item->work_title);
  $vars['work_company'] = check_plain($item->work_company);
  $vars['work_address'] = check_plain($item->work_address);
  $vars['work_city'] = check_plain($item->work_city);
  $vars['work_state'] = check_plain($item->work_state);
  $vars['work_zip'] = check_plain($item->work_zip);
  $vars['work_country'] = check_plain($item->work_country);
  $vars['work_phone'] = check_plain($item->work_phone);
  $vars['work_fax'] = check_plain($item->work_fax);
  $vars['work_website'] = check_plain($item->work_website);
}
