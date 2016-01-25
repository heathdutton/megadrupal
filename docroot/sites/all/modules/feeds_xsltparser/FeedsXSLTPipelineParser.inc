<?php
/**
 * @file
 * This module implements a simple XSLT pipeline for feeds
 *
 * Written by Sebastian Leu, MD-Systems 2011
 */

require_once DRUPAL_ROOT . '/' . drupal_get_path('module', 'feeds_xsltparser') . '/feeds_xsltparser.config.inc';


class FeedsXSLTPipelineParser extends FeedsParser {
  public function configGetXsltDirectory() {
    $xslt_dir = $this->config['xslt_dir'];
    if ($xslt_dir[0] != '/') {
      $xslt_dir = DRUPAL_ROOT . '/' . $xslt_dir;
    }

    if (is_dir($xslt_dir)) {
      return $xslt_dir;
    }
    else {
      drupal_set_message(t('The XSLT pipeline directory does not exist. Please check your feed settings.'), 'warning');
      return FALSE;
    }
  }

  /**
   * Execute the XSLT pipeline on the current fetcher result
   */
  public function parse(FeedsSource $source, FeedsFetcherResult $fetcher_result) {
    $xslt_dir = $this->configGetXsltDirectory();
    if ($xslt_dir === FALSE) {
      $message = t('The XSLT pipeline directory does not exist. Please check your feed settings.');
      $source->log('import', $message, array(), WATCHDOG_ERROR);
      throw new Exception($message);
    }

    $config = feeds_xsltparser_config_generate_from_directory($xslt_dir);

    $markup = $fetcher_result->getRaw();

    $logproxy = new FeedsXSLTChainLogProxy($source, $config, $xslt_dir . '/');
    try {
      $pipe = feeds_xsltparser_config_construct_pipe($config, $logproxy);
      $rows = $pipe->run($markup);
    }
    catch (Exception $e) {
      $message = $e->getMessage();
      list($xmlError, $xmlPrefix, $xsltPath) = $logproxy->getOriginalError();

      if ($xmlError) {
        $message .= '<br/>';
        $message .= $logproxy->formatXmlError($xmlError, $xmlPrefix, $xsltPath);
      }

      $source->log('import', $message, array(), WATCHDOG_ERROR);
      throw new Exception($message);
    }

    return new FeedsParserResult($rows);
  }


  /**
   * Return the list of XSLT pipelines found in the pipelines directory
   */
  public function getMappingSources() {
    $xslt_dir = $this->configGetXsltDirectory();
    if ($xslt_dir === FALSE) {
      return array();
    }

    $config = feeds_xsltparser_config_generate_from_directory($xslt_dir);
    $fieldmap = feeds_xsltparser_config_extract_fieldmap($config);

    $fieldLabels = array_keys($fieldmap['mapper']['items']);

    $map = array();
    foreach ($fieldLabels as $label) {
      $map[$label] = array(
        'name' => $label,
        'description' => t('A field value extracted using the @label XSLT pipeline.', array('@label' => $label)),
      );
    }

    return $map + parent::getMappingSources();
  }


  /**
   * Default configuration
   */
  public function configDefaults() {
    return array(
      'xslt_dir' => 'sites/all/libraries/xslt_pipelines',
    );
  }


  /**
   * Build configuration form
   */
  public function configForm(&$form_state) {
    $form = array();

    $form['xslt_dir'] = array(
      '#type' => 'textfield',
      '#title' => t('XSLT Pipelines Path'),
      '#description' => t('A path on the server to the directory where your XSLT pipelines are stored'),
      '#size' => 60,
      '#default_value' => $this->config['xslt_dir'],
    );

    $xslt_dir = $this->configGetXsltDirectory();
    if ($xslt_dir === FALSE) {
      $count = 0;
    }
    else {
      $config = feeds_xsltparser_config_generate_from_directory($xslt_dir);
      $fieldmap = feeds_xsltparser_config_extract_fieldmap($config);
      $fieldLabels = array_keys($fieldmap['mapper']['items']);
      $count = count($fieldLabels);
    }

    if ($count == 0) {
      $form['xslt_help'] = array(
        '#type' => 'markup',
        '#markup' => t('No pipelines found in XSLT pipeline path.') . '<br/>' .
          t('In order to create pipelines upload your XSLT files into the specified directory. If you place folders containing XSLT files under the XSLT pipeline path, the stylesheets will be executed in alphabetic order, each operating on the result of its predecessor.') . '<br/>' .
          t('Each pipeline is exposed as a source field in the mapping section'),
      );
    }
    else {
      $items = $fieldmap['mapper']['items'];
      $pipes = array();
      foreach ($fieldLabels as $label) {
        $pipes[$label] = $this->describeFieldExtractor($items[$label], $xslt_dir);
      }

      $variables = array('pipes' => $pipes);
      $form['xslt_help'] = array(
        '#type' => 'markup',
        '#markup' => theme('feeds_xsltparser_pipelist', $variables),
      );
    }

    return $form;
  }


  /**
   * Returns a string describing the given field extractor fragemnt from a
   * pipeline config. Typically you use this to generate help text. See
   * FeedsXSLTPipelineParser:configForm for an example of how this function
   * is called.
   */
  private function describeFieldExtractor($item, $base_dir) {
    $reloffset = drupal_strlen($base_dir) + 1;

    $output = t('Unknown extractor type');
    if ($item['type'] == 'XSLTMap' && $item['mapper']['type'] == 'XSLTRepeat') {
      $extractors = substr_replace(
        $item['mapper']['pipe']['stylesheets'], '', 0, $reloffset);
      $counters = substr_replace(
        $item['mapper']['counter']['stylesheets'], '', 0, $reloffset);
      $output = t('Extract multiple field values by applying @extracttext %extractors repeatedly. The XSLT parameter %paramname is set to the current iteration (0 in the first pass, 1 in the second, etc.). The number of repetitions is determined using @countertext %counters, which must return an integer value.', array(
        '@extracttext' => format_plural(count($extractors), 'stylesheet', 'stylesheet chain comprised of the extractors'),
        '%paramname' => $item['paramKey'],
        '%extractors' => implode(', ', $extractors),
        '@countertext' => format_plural(count($counters), 'stylesheet', 'stylesheet chain comprised of the counters'),
        '%counters' => implode(', ', $counters),
      ));

      if (isset($item['preproc'])) {
        $preprocs = substr_replace($item['preproc']['stylesheets'], '', 0, $reloffset);
        $output .= ' ';
        $output .= t('Input gets preprocessed using @filetext %files, counting and value extraction is done on the result obtained form the preprocessor.', array(
          '@filetext' => format_plural(count($preprocs), 'stylesheet', 'stylesheet chain comprised of the files'),
          '%files' => implode(', ', $preprocs)));
      }
    }
    elseif ($item['type'] == 'XSLTChain') {
      $extractors = substr_replace($item['stylesheets'], '', 0, $reloffset);
      $output = t('Extract one field value using @filetext %files.', array(
        '@filetext' => format_plural(count($extractors), 'stylesheet', 'stylesheet chain comprised of the files'),
        '%files' => implode(', ', $extractors)));
    }

    return $output;
  }
}
