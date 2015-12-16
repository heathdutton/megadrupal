<?php

/**
 * @file
 * File Scanner theme functions.
 *
 * This file contains theme functions.
 */


/**
 * Returns HTML for the file_scanner overview form into a table.
 *
 * @param array $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_file_scanner_admin_overview_form($variables) {
  $output = '';
  $form = $variables['form'];

  // Build table headers
  $headers = array(
    '',
    t('Label'),
    t('Folders'),
    t('# Files parsed'),
    t('# Files analysed'),
    t('# Files selector matched'),
    t('Last run on'),
    array('data' => t('Operations'), 'colspan' => '4'),
  );

  // Build table rows
  $rows = array();
  foreach (element_children($form['analyses']) as $aid) {
    $analysis = $form['analyses'][$aid]['#item'];

    // Get analysis statistics
    $analysis_stats = file_scanner_get_analysis_statistics($analysis);

    $row = array(
      'data' => array(
        drupal_render($form['analyses'][$aid]['analysis-run-' . $aid]),
        check_plain($analysis->label),
        implode('<br />', $analysis->folders),
        is_null($analysis->last_run_files_parsed) ? '-' : $analysis->last_run_files_parsed,
        is_null($analysis_stats['files_count']) ? '-' : $analysis_stats['files_count'],
        is_null($analysis_stats['files_selector_matched']) ? '-' : $analysis_stats['files_selector_matched'],
        is_null($analysis->last_run) ? t('Never') : format_date($analysis->last_run, 'short'),
        l(t('view'), 'admin/config/development/file_scanner/view/' . $analysis->aid),
        l(t('edit'), 'admin/config/development/file_scanner/edit/' . $analysis->aid),
        l(t('clone'), 'admin/config/development/file_scanner/clone/' . $analysis->aid),
        l(t('delete'), 'admin/config/development/file_scanner/delete/' . $analysis->aid),
      ),
    );
    $rows[] = $row;
  }

  // Output form + table
  $output .= theme('table', array('header' => $headers, 'rows' => $rows));
  $output .= drupal_render_children($form);
  return $output;
}

/**
 * Returns HTML for the analysis view page.
 *
 * @param array $variables
 *   An associative array containing:
 *   - analysis: Analysis object
 *   - analysis_stats: All analysis statistics
 *
 * @ingroup themeable
 */
function theme_file_scanner_analysis_summary($variables) {
  $analysis = $variables['analysis'];
  $analysis_stats = $variables['analysis_stats'];
  $output = '';

  // Analysis information
  $header = array(
    t('Label'),
    t('Description'),
    t('Folders'),
    t('Allowed extensions'),
    t('Selector'),
    t('# Files parsed'),
    t('# Files analysed'),
    t('# Files selector matched'),
    t('Last run on'),
    t('Created on'),
    t('Updated on'),
  );
  $rows = array();

  // Folders
  $folders = array();
  foreach ($analysis->folders as $folder) {
    $folders[] = check_plain($folder);
  }

  $rows[] = array(
    check_plain($analysis->label),
    check_plain($analysis->description),
    implode('<br />', $folders),
    empty($analysis->extensions) ? '' : implode('<br />', $analysis->extensions),
    check_plain($analysis->selector),
    is_null($analysis->last_run_files_parsed) ? '-' : $analysis->last_run_files_parsed,
    is_null($analysis_stats['files_count']) ? '-' : $analysis_stats['files_count'],
    is_null($analysis_stats['files_selector_matched']) ? '-' : $analysis_stats['files_selector_matched'],
    is_null($analysis->last_run) ? t('Never') : format_date($analysis->last_run, 'short'),
    format_date($analysis->created, 'short'),
    format_date($analysis->changed, 'short'),
  );
  $vertical_rows = array();
  // Turn header and rows arrays to a vertical table input
  foreach ($header as $index => $h_item) {
    $vertical_rows[] = array(
      array(
        'header' => TRUE,
        'data' => $h_item,
        'width' => '20%',
      ),
      array(
        'data' => $rows[0][$index],
      ),
    );
  }
  $output .= theme('table', array('header' => array(), 'rows' => $vertical_rows));

  return $output;
}

/**
 * Returns HTML for the analysis results display.
 *
 * @param array $variables
 *   An associative array containing:
 *   - analysis: Analysis object
 *   - analysis_results: All analysis results
 *
 * @ingroup themeable
 */
function theme_file_scanner_analysis_results_display($variables) {
  $output = '';

  // Analysis results
  $header = array(
    t('File ID'),
    t('Path'),
    t('Filename'),
    t('# matches'),
  );
  $rows = array();
  foreach ($variables['analysis_results'] as $result) {
    $row = array(
      'data' => array(
        $result->fid,
        $result->path,
        $result->filename,
        array(
          'data' => $result->result,
          'class' => 'file_result',
        ),
      ),
      'class' => array(
        $result->result > 0 ? 'match' : 'no-match',
      ),
    );
    $rows[] = $row;
  }
  $attributes = array(
    'class' => array('file_scanner-analysis_results'),
  );
  $output .= theme('table', array(
    'header' => $header,
    'rows' => $rows,
    'attributes' => $attributes,
  ));

  return $output;
}
