<?php

/**
 * @file
 * PDF Generation
 *
 * This file included the logic needed to generate the temporary PDF out of each
 * nodes with the print module as well the final merge done by FPDI and FPDF
 */

/**
 * Generates a PDF from a path
 *
 * This functions copies the implementation on the print module but adds
 * information to the node object in order to be used in the print template
 * files
 *
 * @see print_pdf_generate_path().
 */
function pdfck_print_pdf_generate_path($path, $query = NULL, $cid = NULL, $pdf_filename = NULL, $startpage = 1) {
  global $base_url;

  $print = print_controller($path, $query, $cid, PRINT_PDF_FORMAT);
  if ($print === FALSE) {
    return;
  }

  // Img elements must be set to absolute
  $pattern = '!<(img\s[^>]*?)>!is';
  $print['content'] = preg_replace_callback($pattern, '_print_rewrite_urls', $print['content']);
  $print['logo'] = preg_replace_callback($pattern, '_print_rewrite_urls', $print['logo']);
  $print['footer_message'] = preg_replace_callback($pattern, '_print_rewrite_urls', $print['footer_message']);

  // Send to printer option causes problems with PDF
  $print['sendtoprinter'] = '';

  $node = $print['node'];
  $node->pdfck['startpage'] = $startpage;
  $html = theme('print', array('print' => $print, 'type' => PRINT_PDF_FORMAT, 'node' => $node));

  // Convert the a href elements, to make sure no relative links remain
  $pattern = '!<(a\s[^>]*?)>!is';
  $html = preg_replace_callback($pattern, '_print_rewrite_urls', $html);
  // And make anchor links relative again, to permit in-PDF navigation
  $html = preg_replace("!${base_url}/" . PRINTPDF_PATH . '/.*?#!', '#', $html);

  return print_pdf_generate_html($print, $html, $pdf_filename);
}

/**
 * Generates a temporary PDF out of a node and save it in the files database
 * as a temporary file
 *
 * @param type $nid
 *   Node ID
 * @param type $startpage
 *   (Optional) Initial starting page for the temporary PDF
 * @return mixed
 *   A valid file object or FALSE if error
 */
function pdfck_generate_node_temp_pdf($pdfck, $nid, $startpage = 1) {
  global $user;

  module_load_include('inc', 'print_pdf', 'print_pdf.pages');
  $node = node_load($nid);
  $pdf_filename = 'node_' . $nid . '.pdf';
  
  $pdf = pdfck_print_pdf_generate_path($node->nid, NULL, NULL, NULL, $startpage);

  if ($pdf) {
    // Build the destination folder tree if it doesn't already exists.
    $path = $pdfck->uri_scheme . '://' . $pdfck->path;
    if (!file_prepare_directory($path, FILE_CREATE_DIRECTORY|FILE_MODIFY_PERMISSIONS)) {
      watchdog('pdfck', 'Failed to create pdfck directory: %dir', array('%dir' => $path), WATCHDOG_ERROR);
      return FALSE;
    }
    $path = $pdfck->uri_scheme . '://' . $pdfck->path . '/temp';
    if (!file_prepare_directory($path, FILE_CREATE_DIRECTORY|FILE_MODIFY_PERMISSIONS)) {
      watchdog('pdfck', 'Failed to create pdfck directory: %dir', array('%dir' => $path), WATCHDOG_ERROR);
      return FALSE;
    }

    $file = file_save_data($pdf, $path . '/' . $pdf_filename, FILE_EXISTS_REPLACE);
    if (!$file) {
      watchdog('pdfck', 'Node %title PDF was not properly exported', array('%title' => $node->title));
      return FALSE;
    }
    else {
      // update file object.
      // Make it temporary
      $file->status &= ~FILE_STATUS_PERMANENT;
      file_save($file);

      return $file;
    }
  }
  else {
    drupal_set_message(t('Error generating the PDF for <a href="!node_uri">@node_title</a>, please check the print pdf module is working correctly and that you can view the PDF version of that node properly.', array('!node_uri' => url('node/' . $node->nid), '@node_title' => $node->title)), 'error');
  }

  return FALSE;
}

/**
 * Batch API finished callback.
 *
 * Build the final PDF and store it in both the files table and our files
 * mapping table
 */
function pdfck_generate_finished($success, $results, $operations) {
  global $user;

  module_load_include('inc', 'pdfck', 'pdfck.admin');

  if ($success) {
    // Here we do something meaningful with the results.
    require_once DRUPAL_ROOT . '/' . libraries_get_path('fpdf') . '/fpdf.php';
    require_once DRUPAL_ROOT . '/' . libraries_get_path('fpdi') . '/fpdi.php';

    if (count($results['pdfs']) == 0) {
      $message = t('No PDF was generated because no elements for it has been processed correctly.');
      drupal_set_message($message, 'warning');
    }
    else {
      $pdfck = $results['pdfck'];
      $pdf = new FPDI();
      $pages = 0;
      $nodes = 0;
      foreach ($results['pdfs'] as $result) {
        $file = $result['file'];
        $pagecount = $pdf->setSourceFile(drupal_realpath($file->uri));
        // Add pages to the merged PDF
        for ($i = 1; $i <= $pagecount; $i++) {
          $tplidx = $pdf->ImportPage($i);
          $s = $pdf->getTemplateSize($tplidx);
          $pdf->AddPage('P', array($s['w'], $s['h']));
          $pages++;
          $pdf->useTemplate($tplidx);
        }
        $nodes++;
      }

      // We assume we have already created the dirs, as that's done in the batch
      // process
      $pdf_filename = $pdfck->filename;
      if (!empty($pdf_filename)) {
        $pdf_filename = token_replace($pdf_filename);
      }
      else {
        $pdf_filename = $pdfck->name;
      }
      if (function_exists('transliteration_clean_filename')) {
        $pdf_filename = transliteration_clean_filename($pdf_filename, language_default('language'));
      }

      $pdf_filename .= '.pdf';

      $path = $pdfck->uri_scheme . '://' . $pdfck->path . '/' . $pdf_filename;

      $primary_keys = array();
      $file = file_save_data($pdf->Output('', 'S'), $path, $pdfck->keep);
      if ($file === FALSE) {
        drupal_set_message(t('%title PDF was not properly generated', array('%title' => $pdfck->name)), 'error');
        watchdog('pdfck', '%title PDF was not properly generated', array('%title' => $pdfck->name));
      }
      else {
        if ($pdfck->keep == FILE_EXISTS_REPLACE) {
          foreach ($pdfck->files as $pdffile) {
            if ($file->fid != $pdffile->fid) {
              pdfck_pdf_file_delete($pdffile->fid);
            }
            else {
              $primary_keys = array('fid');
            }
          }
        }

        $pdffile = new stdClass();
        $pdffile->pdfid = $pdfck->pdfid;
        $pdffile->fid = $file->fid;
        drupal_write_record('pdfck_files', $pdffile, $primary_keys);

        $message = t('PDF successfully generated to %path (nodes: %nodes / pages: %pages).', array('%path' => $file->uri, '%pages' => $pages, '%nodes' => $nodes));
        drupal_set_message($message);
      }
    }

    foreach ($results['pdfs'] as $result) {
      $file = $result['file'];
      // We might have to do something special for temp file
      // but because of the code in delete, there shouldn't be aproblem
      // of removing an unexistant file from the pdfck_files table
      file_delete($file);
    }
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
    drupal_set_message($message, 'error');
  }
}

/**
 * Helper function to get a list of nodes out of a 'view' element
 */
function pdfck_generate_process_getnodes_view($element) {
  $nodes = array();
  $results = views_get_view_result($element->id);
  foreach ($results as $node) {
    $nodes[] = $node->nid;
  }
  return $nodes;
}

/**
 * Helper function to get a list of nodes out of a 'node' element
 */
function pdfck_generate_process_getnodes_node($element) {
  $nodes = array();
  $nodes[] = $element->id;
  return $nodes;
}

/**
 * Batch API process callback.
 *
 * Generates the temporary PDF of each node.
 */
function pdfck_generate_process($pdfck, &$context) {
  require_once DRUPAL_ROOT . '/' . libraries_get_path('fpdf') . '/fpdf.php';
  require_once DRUPAL_ROOT . '/' . libraries_get_path('fpdi') . '/fpdi.php';

  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_id'] = 0;
    $context['results']['pdfs'] = array();

    // Build the array of nodes
    $nodes = array();
    foreach ($pdfck->elements as $element) {
      $function = 'pdfck_generate_process_getnodes_' . $element->type;
      $nodes = array_merge($nodes, call_user_func($function, $element));
    }
    $context['sandbox']['nodes'] = $nodes;
    $context['sandbox']['startpage'] = 1;
    $context['sandbox']['max'] = count($nodes);
    $context['results']['pdfck'] = $pdfck;

    // If no elements, finish the batch process
    if ($context['sandbox']['max'] == 0) {
      $context['sandbox']['finished'] = 1;
      return;
    }
  }

  $nid_idx = $context['sandbox']['current_id'];
  $nid = $context['sandbox']['nodes'][$nid_idx];
  $node = node_load($nid);

  // With each pass through the callback, retrieve the next group of nids.
  $file = pdfck_generate_node_temp_pdf($pdfck, $nid, $context['sandbox']['startpage']);

  if ($file) {
    $pdf = new FPDI();
    $pagecount = $pdf->setSourceFile(drupal_realpath($file->uri));
    $context['sandbox']['startpage'] += $pagecount;
    //Store some result for post-processing in the finished callback.
    $context['results']['pdfs'][] = array(
      'file' => $file,
    );
  }

  // Update our progress information.
  $context['sandbox']['progress']++;
  $context['sandbox']['current_id']++;
  $items[] = $node->title;
  $context['message'] = t('%items', array('%items' => implode(', ', $items)));

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

/**
 * Menu callback. Start the batch process for generating the PDF
 *
 * @param type $pdfck
 *   PDFCK object
 */
function pdfck_generate($pdfck) {
  $batch = array(
    'operations' => array(
      array('pdfck_generate_process', array($pdfck)),
    ),
    'finished' => 'pdfck_generate_finished',
    'title' => t('Generating PDF'),
    'init_message' => t('Generate PDF is warming up.'),
    'progress_message' => t('Processing nodes:'),
    'error_message' => t('Generating PDF has encountered an error.'),
    'file' => drupal_get_path('module', 'pdfck') . '/pdfck.generate.inc',
  );
  batch_set($batch);
  batch_process('admin/content/pdfck');
}

