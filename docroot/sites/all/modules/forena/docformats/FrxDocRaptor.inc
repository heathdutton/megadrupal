<?php
/**
 * @file FrxPrincePDF
 * PDF document via Prince XML
 * @author davidmetzler
 *
 */
class FrxDocRaptor extends FrxDocument {

  private $api_key;
  private $url;
  private $test;
  private $access;

  public function __construct() {
    $this->content_type='application/pdf';
    $this->url = variable_get('forena_pdf_docraptor_url', 'https://docraptor.com/docs');
    $this->api_key = variable_get('forena_pdf_docraptor_key', '');
    $this->test = variable_get('forena_pdf_docraptor_test', TRUE);
    $this->access = user_access('use forena pdf document generation service');

  }


  public function render($r, $format, $options = array()) {
    if (!$this->access) {
      drupal_set_message(t('Not Authorized to use PDF Document Generation Service!'), 'error');

      drupal_goto('<front>');
    }
    $disable_links = variable_get('forena_pdf_disable_links', TRUE);
    $html = $r->html;
    if ($disable_links) {
      $html = preg_replace('/<a href=\"(.*?)\">(.*?)<\/a>/', "\\2", $html);
    }
    $link_class = $disable_links ? 'prince-disable-links': '';
    $output = '<html><head>';
    $output .= '<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>';
    $css = '';
    foreach (Frx::Skin()->stylesheets as $type => $sheets) {
      foreach ($sheets as $sheet) {
        switch ($type) {
          case 'all':
          case 'print':
          case 'screen':
          case 'pdf':
            if (strpos($sheet, 'http:') === 0 || strpos($sheet, 'https:') === 0) {
              $output .= "    <link rel='stylesheet' type='text/css' href='$sheet'> \n";
            }
            else {
              $css .= file_get_contents($sheet);
            }
            //echo $sheet;
            break;
        }
      }
    }
    if ( $css || isset($r->rpt_xml->head->style)) {
      $output .= '<style type="text/css">';
      $output .= $css;
      if (isset($r->rpt_xml->head->style) || isset($r->rpt_xml->head->style)) {
        $sheet = (string)$r->rpt_xml->head->style;
        $output .= $sheet;
      }
      $output .= '</style>';
    }

    $output .= '<title>' . $r->title . "</title></head><body class='forena-report $link_class'><h1>" . $r->title . '</h1>' . $html . '</body></html>';
    $api_key = $this->api_key;
    if ($api_key) {

      $service_url = $this->url;
      $url = "$service_url?user_credentials=$api_key";

      $name = 'report.pdf';
      $post_array = array(
        'doc[name]' => $name,
        'doc[document_type]' => 'pdf',
        'doc[test]' => $this->test ? 'true' : 'false',
        'doc[document_content]' => $output,
      );

      $postdata = http_build_query($post_array);

      $opts = array('http' =>
        array(
          'method'  => 'POST',
          'header'  => 'Content-type: application/x-www-form-urlencoded',
          'content' => $postdata
        )
      );

      $context = stream_context_create($opts);

      $doc = file_get_contents($url, false, $context);

      return $doc;
    }
    else {
      drupal_set_message(t('No Docraptor API Key COnfiguredd'), 'error');
      return ('');
    }
  }

  public function header($r, $print = FALSE) {
    if (!$this->access) return;
    $r->html = '';
    if ($print) {
      header('Content-Type: ' . $this->content_type);
    }
  }
}
