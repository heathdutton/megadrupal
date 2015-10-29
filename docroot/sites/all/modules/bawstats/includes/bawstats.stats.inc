<?php

/**
 * BAWstats drupal module
 *
 * BAWstats is a drupal module written in php for integration of
 * the BetterAWstats presentation and processing of AWstats data into drupal.
 * Copyright (C) 2008  Andrew Gillies (anaru@equivocation.org)
 *
 * For more information about BetterAWstats see:
 * http://betterawstats.com/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or, at
 * your option, any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

include DRUPAL_ROOT . '/' . drupal_get_path('module', 'bawstats') . '/includes/bawstats.config.inc';

/**
 * Form for the bawstats
 */

/**
 *
 * Form that displays the statistics. If access to all sites is allowed
 * (bawstats_admin_access is 1), the site name is taken from the $_SESSION
 * variable.
 *
 * @return
 *   The form
 */
function bawstats_view_selection($form) {
  global $BAW_CURR, $BAW_CONF, $BAW_DFILES;
  $site_OK = True;
  $date_OK = True;

  // Is access to all sites allowed?
  $site_access = variable_get('bawstats_admin_access', 0);

  // check we are configured...
  if ((strlen($BAW_CONF['path_data']) > 0) && (strlen($BAW_CONF['path_lib']) > 0) && (strlen($BAW_CONF['path_lang']) > 0)) {

    // If recall the site we are looking at from the SESSION
    if ($site_access && isset($_SESSION['bawstats'])) {
      $BAW_CURR['site_name'] = $_SESSION['bawstats']['site_name'];
    }
    else {
      // its not set in the session, so set it here to the default (and record it in the session for the future)
      $oursite = $_SERVER['HTTP_HOST'];
      if (strpos(strtolower($oursite), "www.") === 0) {
        $oursite = substr($oursite, 4); // remove www. if any
      }
      // this can be overriden only in the settings.php file using conf
      $BAW_CURR['site_name'] = variable_get('bawstats_defsite', $oursite);
      $_SESSION['bawstats']['site_name'] = $BAW_CURR['site_name'];
    }

    // record what was requested before checks so we can make a nice message
    $rec_year = $BAW_CURR['year'];
    $rec_month = $BAW_CURR['month'];

    // sanity check on date data:
    if ((!is_numeric($BAW_CURR['month'])) || (intval($BAW_CURR['month']) < 1) || (intval($BAW_CURR['month']) > 12)) {
      $site_OK = False;
      $date_OK = False;
      // drupal_set_message(t('Invalid month %name.', array('%name' => $BAW_CURR['month'])));
      $BAW_CURR['month'] = '01';
    }
    $curyear = date('Y');
    if ((!is_numeric($BAW_CURR['year'])) || (intval($BAW_CURR['year']) > intval($curyear))) {
      $site_OK = False;
      $date_OK = False;
      //drupal_set_message(t('Invalid year %name.', array('%name' => $BAW_CURR['year'])));
      $BAW_CURR['year'] = $curyear;
    }

//    $BAW_CURR['monthyear'] = $BAW_CURR['month'].$BAW_CURR['year'];
    $BAW_CURR['yearmonth'] = $BAW_CURR['year'] . $BAW_CURR['month'];

    // set key request variables in the session for use in the, only if sane!
    if ($site_OK) {
      $_SESSION['bawstats']['month'] = $BAW_CURR['month'];
      $_SESSION['bawstats']['year']  = $BAW_CURR['year'];
    }

    baw_match_files();

    // some simple validation
    if (count($BAW_DFILES) > 0) {

      // build a 'stats selection' form
      $form['bawstats'] = array(
        '#type' => 'fieldset',
        '#title' => t('Statistics selection'),
        '#prefix' => '<div class="container-inline">',
        '#suffix' => '</div>',
      );

      // check year is in list of options:

      // FIX, this doesn't work - range of years is nolonger filled, as baw_match_files doesn't do it!
      if (!isset($BAW_CURR['years'][$BAW_CURR['year']])) {
        $site_OK = False;
        drupal_set_message(t('No data for year %name.', array('%name' => $BAW_CURR['year'])));
        $BAW_CURR['year'] = $curyear;
        // re-read data for current year, so that the forms are correctly built
        // need new function! baw_read_data();
      }

      $options = bawstats_sites_dropdown();

      // make sure stats exist for this site
      if ((!isset($options[$BAW_CURR['site_name']])) &&
          (!(($BAW_CURR['site_name'] == 'all_days') || ($BAW_CURR['site_name'] == 'all_months')))) {
        $site_OK = False;
      }

      // only admin has the option of sites
      if ($site_access > 0) {
        if (count($options) == 1 ) {
          $BAW_CURR['site_name'] = $options[0];
        }
        $form['bawstats']['site'] = array(
          '#type' => 'select',
          '#options' => $options,
          '#default_value' => $BAW_CURR['site_name'],
        );
      }

      $options = bawstats_months_dropdown();

      $form['bawstats']['month'] = array(
        '#type' => 'select',
        '#options' => $options,
        '#default_value' => $BAW_CURR['months'][$BAW_CURR['month']],
      );

      $form['bawstats']['year'] = array(
        '#type' => 'select',
        '#options' => $BAW_CURR['years'],
        '#default_value' => $BAW_CURR['year'],
      );

      $form['bawstats']['submit'] = array(
        '#type' => 'submit',
        '#value' => t('View'),
      );
    }
    else {
      drupal_set_message(t('No AWStat files found in %path', array('%path' => $BAW_CONF['path_data'] )), 'error');
      $site_OK = False;
    }

    if ($site_OK) {
      $form['bawstats_results'] = array(
        '#markup' => bawstats_display(),
      );
    }
    else {
      if ($date_OK) {
        $form['bawstats_results'] = array(
          '#markup' => "No statistics found for site <em>" . $BAW_CURR['site_name'] . "</em> " . $rec_month . "/" . $rec_year,
        );
      }
      else {
        $form['bawstats_results'] = array(
          '#markup' => "No statistics found for site <em>" . $BAW_CURR['site_name'] . "</em> ",
        );
      }

    }
  }
  else {
    $form['bawstats_results'] = array(
      '#markup' => t('No statistics found.  Please check the <a href="@url">configuration</a> of the AWstats data, library and language locations.',
                 array('@url' => url('admin/config/system/bawstats')))
      );
  }

  return $form;
}

/**
 * Menu callback to view the statitics.
 *
 * @param month The number of the  month to view
 * @param year The number of the year to view
 * @param section The section to view
 *
 * @return
 *   The form
 */
function bawstats_view_theform($month = '-1', $year = '-1', $section = '') {
  global $BAW_CURR, $BAW_CONF_DIS_DEF, $BAW_CONF, $BAW_LIB;

  // get configuration from drupal variables
  $BAW_CONF['path_data'] = variable_get('bawstats_data', '');
  $BAW_CONF['path_lib'] = variable_get('bawstats_lib', '');
  $BAW_CONF['path_lang'] = variable_get('bawstats_lang', '');

  if (is_numeric($year) && is_numeric($month)) {
    if (intval($month) >= 0) {
      //bawstats_view_selection will check validity of month
      $BAW_CURR['month'] = $month;
    }
    else {
      $BAW_CURR['month'] = date('m');
    }

    if (intval($year) >= 0) {
      //bawstats_view_selection will check validity of year
      $BAW_CURR['year'] = $year;
    }
    else {
      $BAW_CURR['year'] = date('Y');
    }
  }

  if (strlen($section) > 1) {
    // validate sections (ignore all that are not recognised from the default configuration)
    $found = False;
    // NOTE, must ensure group and section names are different!
    // first, is it a group?
    foreach ($BAW_LIB['item_groups'] as $group => $group_details) {
      if ($section == check_plain($group)) {
        $found = True;
        break;
      }
    }
    if ($found) {
      $BAW_CURR['drupal_group'] = $section;
    }
    else {
      // second, is it a section
      foreach ($BAW_CONF_DIS_DEF as $key => $value) {
        if ($section == $key) {
          $found = True;
          break;
        }
      }
      if ($found) {
        $BAW_CURR['drupal_section'] = $section;
      }
    }
  }
  return drupal_get_form('bawstats_view_selection');
}

/**
 * set the desired viewing section using the current view
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function bawstats_set_section($section = '') {
  global $BAW_CONF;

  $month = '0';
  $year = '0';

  if (isset($_SESSION['bawstats'])) {
    if (isset($_SESSION['bawstats']['month'])) {
      $month = $_SESSION['bawstats']['month'];
    }
    if (isset($_SESSION['bawstats']['year'])) {
      $year  = $_SESSION['bawstats']['year'];
    }
  }

  if ((strlen($section) > 1) && (intval($month) > 0) && (intval($year) > 0)) {
    drupal_goto($BAW_CONF['drupal_base'] . '/' . $section . '/' . $month . '/' . $year);
  }
  else {
    drupal_goto('admin/bawstats');
  }
}

/**
 * perform the chosen display settings
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function bawstats_view_selection_submit($form, &$form_state) {
  global $BAW_CURR, $BAW_MES, $BAW_LIB, $BAW_CONF;

  $site_access = variable_get('bawstats_admin_access', 0);

  $form_values = $form_state['values'];

  if (isset($form_values['year'])) {
    $year = $form_values['year'];
  }
  else {
    $year = date('Y');
  }

  if (isset($form_values['month'])) {
    $month = array_search($form_values['month'], $BAW_CURR['months']);
  }
  else {
    $month = date('m');
  }

  if ($site_access > 0) {
    // ensure we are allowed to change sites
    if (isset($form_values['site'])) {
      // alter the session site record
      $_SESSION['bawstats']['site_name'] = $form_values['site'];
    }
  }

  if (variable_get('bawstats_page_view', 0) == 0) {
    $redirect = $BAW_CONF['drupal_base'] . '/all/' . $month . '/' . $year;
  }
  else {
    $statgroups = bawstats_groups($BAW_CONF, $BAW_LIB);
    $keys = array_keys($statgroups);
    $redirect = $BAW_CONF['drupal_base'] . '/' . $keys[0]  . '/' . $month . '/' . $year;
  }
  $form_state['redirect'] = $redirect;
}

/**
 * mirror function to betterawstats baw_sites_dropdown() [engine/display_helpers.php]
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function bawstats_sites_dropdown() {
  global $BAW_CURR, $BAW_DFILES, $BAW_MES;
  $sites = array();

  $detected = array_keys($BAW_DFILES);
  $site_access = variable_get('bawstats_admin_access', 0);

  if (count($detected) > 0) {
    foreach ($detected as $site) {
      $sites[$site] = $site;
    }
    if ($site_access > 0) {
      # add the "all" options
      $sites['all_months'] = $BAW_MES['all_months'];
      $sites['all_days'] = $BAW_MES['all_days'];
    }
  } // else there is a problem - no sites data!

  return $sites;
}

/**
 * mirror function to betterawstats baw_date_dropdown()
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function bawstats_months_dropdown() {
  global $BAW_CURR, $BAW_MES;

  $months = array();

  foreach ($BAW_CURR['months'] as $month) {
    $months[$month] = $month;
  }
  return $months;
}

/**
 * perform the display, main calls to the betterawstats library
 *
 */

/**
 * @todo Please document this function.
 * @see http://drupal.org/node/1354
 */
function bawstats_display() {
  global $BAW_CURR, $BAW_LIB;

  $out = "<div class=\"contentbox\">\n";

  if (@$BAW_CURR['site_name'] == 'all_months') {
    $settings['section'] = "months";
    $out .= baw_display_drupal($settings);
  }
  else if (@$BAW_CURR['site_name'] == 'all_days') {
    $settings['section'] = "days";
    $out .= baw_display_drupal($settings);
  }
  else if (@strlen($BAW_CURR['drupal_section']) > 1) {
    $settings['section'] = $BAW_CURR['drupal_section'];
    $out .= baw_display_drupal($settings);
  }
  else {
    if (variable_get('bawstats_page_view', 0) > 0) { // sub-pages
      if (@strlen($BAW_CURR['drupal_group']) > 1) {
        $settings['group'] = $BAW_CURR['drupal_group'];
      }
      else {
        // as we are in multi-page view, produce the first entry only
        $groupkeys = array_keys($BAW_LIB['item_groups']);
        $settings['group'] = $groupkeys[0];
      }
      $out .= baw_display_drupal($settings);
    }
    else {
      $out .= baw_display_drupal();
    }
  }

  $out .= "</div>\n";

  return $out;
}

/**
 * mirror function to, and based on, betterawstats baw_display_index()
 */

function baw_display_drupal($settings = array()) {
  global $BAW_CONF, $BAW_CONF_DIS, $BAW_CONF_DIS_DEF, $BAW_D, $BAW_MES, $BAW_CURR, $BAW_LIB, $BAW_DFILES;

  // sort the charts according to order in config
  $BAW_CONF_DIS = baw_array_sorting($BAW_CONF_DIS, 'order', SORT_ASC);

  // ---------- CONTENT BOX --------------------------------------------------
  // create the main content
  $content = "<div class=\"baw-content\">\n";
  $nodata = 0;
  $stats_html = '';

  if (isset($settings['section'])) {
    $section = $settings['section'];
    $function = "baw_display_$section";
    $temp_conf = $BAW_CONF_DIS[$section];
    $temp_conf['top_x'] = false;
    $temp_conf['chart'] = false;
    $temp_conf['table'] = true;
    if (($section == 'months') || ($section == 'days')) {
      $temp_conf['chart'] = true;
      $temp_conf['table'] = false;
    }
    else {
      $temp_conf['chart'] = false;
      $temp_conf['table'] = true;
    }
    $temp_conf['name'] = $BAW_CONF_DIS_DEF[$section]['name'];
    if (function_exists($function)) {
      $stats_html .= $function($temp_conf);
      if ((strlen($stats_html) > 1) || (!$BAW_CONF['hideempty'])) {
        $content .= $stats_html;
        $nodata += 1;
      }
    }
    if ($nodata == 0) {
      $content .= 'No data found for site <em>' . $BAW_CURR['site_name'] . '</em> during ' . $BAW_CURR['month'] . '/' . $BAW_CURR['year'];
    }
  }
  else {
    $singlegroup = false;
    if (isset($settings['group'])) {
      $showonly = $settings['group'];
      $singlegroup = true;
    }
    else {
      $showonly = '';
    }
    $content .= "\n<!-- CONTENT START ++++++++++++++++++++++++++++++++++++++ -->\n";
    foreach ($BAW_LIB['item_groups'] as $group => $group_details) {
      if (($showonly == $group) || ($showonly == '')) {
        $group_items = $group_details['members'];
        $has_overview = in_array('overview', $group_items);
        if ($singlegroup && (!$has_overview)) {
          // pretend to display the overview, as it sets some BAW_CURR variables that may be needed
          // by other groups (namely, session)
          $sect_data = $BAW_CONF_DIS['overview'];
          $sect_data['name'] = 'overview';
          $junk = baw_display_overview($sect_data);
        }
        $menubuttons = '';
        $stats_section = '';
        $item_id = 0;
        $count_members = count($group_details['members']);
        $content .= "\n<!-- GROUP $group START ++++++++++++++++++++++++++++++++++++++ -->\n";

        foreach ($group_details['members'] as $item) {
          $sect_data = $BAW_CONF_DIS[$item];
          $sect_data['name'] = $item;
          //if ($item == 'weekdays' && !isset($BAW_CURR['wdays']['count'])) {
//  $setarr = array('collapse' => false, 'chart' => false,'table' => false);
//  baw_display_weekdays($setarr);
          //}
          $class = 'aws_data';
          $function = "baw_display_$item";
          $title = $BAW_CONF_DIS_DEF[$item]['name'];
          if (($sect_data['show'] == true) && (function_exists($function))) {
            $stats_html = $function($sect_data);
            if ((strlen($stats_html) > 1) || (!$BAW_CONF['hideempty'])) {
              $stats_section .= "\n<!-- ITEM {$title} START from function {$item}++++++++++++++++++++++++++++++++++++++ -->\n";
              $collapse = false;
              $name = "{$group}_$item_id";
              $stats_section .= "<h3 class=\"$class\" id=\"h2_$name\">$title</h3>\n"
                             . "<div class=\"$class\" id=\"box_$name\">\n"
	  	             . $stats_html
                             . "</div>";
              $stats_section .= "\n<!-- ITEM $title END ++++++++++++++++++++++++++++++++++++++ -->\n";
            }
          }
          $item_id++;
        }
        if ((strlen($stats_section) > 1) || (!$BAW_CONF['hideempty'])) {
          $content .= '<h2>' . $group_details['title'] . '</h2>';
          $content .= $stats_section;
          $nodata += 1;
        }
        $content .= "\n<!-- GROUP $group END ++++++++++++++++++++++++++++++++++++++ -->\n";
      }
    }
    $content .= "\n<!-- CONTENT END ++++++++++++++++++++++++++++++++++++++ -->\n";
    if ($nodata == 0) {
      $content .= 'No data found for site <em>' . $BAW_CURR['site_name'] . '</em> during ' . $BAW_CURR['month'] . '/' . $BAW_CURR['year'];
    }
  }
  $content .=  "</div>";
  if (function_exists('mb_detect_encoding')) {
    $content = drupal_convert_to_utf8($content, mb_detect_encoding($content, 'UTF-8, ISO-8859-1, ISO-8859-2'));
  }
  else {
    $content = drupal_convert_to_utf8($content, 'ISO-8859-1');
  }
  $content = filter_xss($content, array('a', 'em', 'strong', 'cite', 'code', 'ul', 'ol', 'li', 'dl', 'dt', 'dd', 'table', 'td', 'tr', 'th', 'div', 'img', 'br', 'h1', 'h2', 'h3'));
  // imgheight=\"([0-9\.]+)\"
  $content = preg_replace('|imgwidth=\"([0-9\.]+)\"\s+imgheight=\"([0-9\.]+)\"|',
                          'style="width: $1px; height: $2px"', $content);
  return($content);
}
