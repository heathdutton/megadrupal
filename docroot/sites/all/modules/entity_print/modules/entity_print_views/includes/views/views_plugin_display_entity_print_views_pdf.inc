<?php

/**
 * @file
 * The PDF display handler.
 */

class views_plugin_display_entity_print_views_pdf extends views_plugin_display_page {

  /**
   * {@inheritdoc}
   */
  function execute() {

    $library = libraries_load('phpwkhtmltopdf');
    if (!empty($library['loaded'])) {

      $pdf = new WkHtmlToPdf(array('binary' => variable_get('entity_print_wkhtmltopdf', '/usr/local/bin/wkhtmltopdf')));
      $html = $this->view->render();

      // Inject some generic CSS across all templates.
      if (variable_get('entity_print_default_css', TRUE)) {
        entity_print_add_css(drupal_get_path('module', 'entity_print') . '/css/entity-print.css');
      }

      // Wrap the output with the appropriate template.
      $html = theme('entity_print_views__' . $this->view->name, array(
        'view_html' => $html,
        'view' => $this->view,
        'entity_print_css' => _entity_print_views_get_css($this->view),
      ));

      // Add the page of HTML.
      $pdf->addPage($html);

      // Allow other modules to alter the generated PDF object.
      drupal_alter('entity_print_pdf_view', $pdf, $this->view);

      if (!$pdf->send()) {
        return $pdf->getError();
      }
    }
    else {
      return $library['error message'];
    }
  }

}
