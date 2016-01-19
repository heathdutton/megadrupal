<?php

/**
 * @file
 * Generates the PDF version using Docraptor.
 *
 * This file is included by the print_pdf_docraptor module and includes the
 * functions that interface with the Docraptor library.
 *
 * @ingroup print
 */

/**
 * Implements hook_print_pdf_generate().
 */
function print_pdf_docraptor_print_pdf_generate($html, $meta, $paper_size = NULL, $page_orientation = NULL) {
  module_load_include('inc', 'print', 'includes/print');

  $pdf_tool = explode('|', variable_get('print_pdf_pdf_tool', PRINT_PDF_PDF_TOOL_DEFAULT));
  if (empty($paper_size)) {
    $paper_size = variable_get('print_pdf_paper_size', PRINT_PDF_PAPER_SIZE_DEFAULT);
  }
  if (empty($page_orientation)) {
    $page_orientation = variable_get('print_pdf_page_orientation', PRINT_PDF_PAGE_ORIENTATION_DEFAULT);
  }
  $images_via_file = variable_get('print_pdf_images_via_file', PRINT_PDF_IMAGES_VIA_FILE_DEFAULT);

  require_once DRUPAL_ROOT . '/' . $pdf_tool[1];

  // Try to use local file access for image files.
  $html = _print_access_images_via_file($html, $images_via_file);
  $print_pdf_docraptor_api_key = variable_get('print_pdf_docraptor_api_key', '');
  $print_pdf_docraptor_test = variable_get('print_pdf_docraptor_test', FALSE);
  $print_pdf_docraptor_filename = variable_get('print_pdf_docraptor_filename', 'output');
  if (!empty($print_pdf_docraptor_filename) && isset($meta['node'])) {
    $print_pdf_docraptor_filename = token_replace($print_pdf_docraptor_filename, array('node' => $meta['node']), array('clear' => TRUE));
  }
  else {
    $print_pdf_docraptor_filename = 'output';
  }

  // Set Document Information.
  $docraptor = new DocRaptor($print_pdf_docraptor_api_key);
  $docraptor->setDocumentContent($html)->setDocumentType('pdf')->setTest($print_pdf_docraptor_test)->setName($print_pdf_docraptor_filename . '.pdf')->setStrict('none');

  $file = $docraptor->fetchDocument();
  return $file;
}
