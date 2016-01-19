<?php

/**
 * @file
 * Contains CloudwordsFileformatXLIFF.
 */

/**
 * Export to XLIFF format.
 */
class CloudwordsFileformatXLIFF extends XMLWriter {

  /**
   * Starts an export.
   *
   * @param CloudwordsDrupalProject $project
   *   A Cloudwords project
   * @param CloudwordsTranslatable $translatable
   *   A Cloudwords translatable.
   *
   * @return string
   *   The XML generated.
   */
  public function beginExport(CloudwordsDrupalProject $project, CloudwordsTranslatable $translatable) {

    $this->openMemory();
    $this->setIndent(TRUE);
    $this->setIndentString(' ');
    $this->startDocument('1.0', 'UTF-8');

    // Root element with schema definition.
    $this->startElement('xliff');
    $this->writeAttribute('version', '1.2');
    $this->writeAttribute('xmlns', 'urn:oasis:names:tc:xliff:document:1.2');
    $this->writeAttribute('xmlns:xsi', 'http://www.w3.org/2001/XMLSchema-instance');
    $this->writeAttribute('xmlns:html', 'http://www.w3.org/1999/xhtml');
    $this->writeAttribute('xsi:schemaLocation', 'urn:oasis:names:tc:xliff:document:1.2 xliff-core-1.2-strict.xsd');

    // File element.
    $this->startElement('file');
    $this->writeAttribute('original', 'xliff-core-1.2-strict.xsd');
    $this->writeAttribute('source-language', 'en');
    $this->writeAttribute('target-language', $translatable->cloudwordsLanguage());
    $this->writeAttribute('datatype', 'plaintext');

    // Date needs to be in ISO-8601 UTC.
    $this->writeAttribute('date', date('Y-m-d\Th:m:i\Z'));

    $this->startElement('header');
    $this->startElement('phase-group');
    $this->startElement('phase');
    $this->writeAttribute('tool-id', 'cloudwords-drupal');
    $this->writeAttribute('phase-name', 'extraction');
    $this->writeAttribute('process-name', 'extraction');
    $this->writeAttribute('job-id', $project->getId());

    $this->endElement();
    $this->endElement();
    $this->startElement('tool');
    $this->writeAttribute('tool-id', 'cloudwords-drupal');
    $this->writeAttribute('tool-name', 'Cloudwords for Drupal');
    $this->endElement();
    $this->endElement();

    return $this->outputMemory() . '<body>';
  }

  /**
   * Adds a project item to the xml export.
   *
   * @param CloudwordsTranslatable $translatable
   *   A Cloudwords translatable to serialize.
   *
   * @return string
   *   The generated XML.
   */
  public function exportTranslatable(CloudwordsTranslatable $translatable) {
    $this->openMemory();
    $this->setIndent(TRUE);
    $this->setIndentString(' ');

    $this->startElement('group');
    $this->writeAttribute('id', $translatable->ctid);
    $this->writeAttribute('restype', 'x-drupal-translatable');

    // @todo: Write in nested groups instead of flattening it.
    $data = array_filter(cloudwords_flatten_data($translatable->getData()), '_cloudwords_filter_data');
    foreach ($data as $key => $element) {
      $this->addTransUnit($translatable->ctid . '][' . $key, $element, $translatable);
    }
    $this->endElement();
    return $this->outputMemory();
  }

  /**
   * Adds a single translation unit for a data element.
   *
   * @param string $key
   *   The unique identifier for this data element.
   * @param string $element
   *   Array with the properties #text and optionally #label.
   * @param CloudwordsTranslatable $translatable
   *   A Cloudwords translatable object.
   *
   * @return string
   *   The generated XML.
   */
  protected function addTransUnit($key, $element, CloudwordsTranslatable $translatable) {
    $this->startElement('group');
    $this->writeAttribute('id', $key);
    $this->writeAttribute('resname', $key);
    $this->writeAttribute('restype', 'x-drupal-field');

    if (isset($element['#label'])) {
      $this->writeElement('note', $element['#label']);
    }

    //escape named html entities prior to conversion
    $list = get_html_translation_table(HTML_ENTITIES);
    $namedTable = array();
    foreach($list as $k=>$v){
      $namedTable[$v]= "&amp;".str_replace('&', '',$v);
    }
    $element['#text'] = strtr($element['#text'], $namedTable);

    try {
      $converter = new CloudwordsConverter($element['#text'], $translatable->cloudwordsLanguage());
      $this->writeRaw($converter->toXLIFF());
    }
    catch (Exception $e) {
      $this->startElement('trans-unit');
      $this->writeAttribute('id', uniqid('text-'));
      $this->writeAttribute('restype', 'x-drupal-failure');
      $this->startElement('source');
      $this->writeAttribute('xml:lang', 'en');
      $this->text($element['#text']);
      $this->endElement();

      $this->startElement('target');
      $this->writeAttribute('xml:lang', $translatable->cloudwordsLanguage());
      $this->text($element['#text']);
      $this->endElement();
      $this->endElement();
    }
    $this->endElement();
  }

  /**
   * Ends an export.
   *
   * @param CloudwordsDrupalProject $project
   *   A Cloudwords project.
   *
   * @return string
   *   The generated XML.
   */
  public function endExport(CloudwordsDrupalProject $project) {

return '  </body>
 </file>
</xliff>';
  }

  public function import($imported_file) {
    // It is not possible to load the file directly with simplexml as it gets
    // url encoded due to the temporary://. This is a PHP bug, see
    // https://bugs.php.net/bug.php?id=61469
    $xml_string = _cloudwords_filter_xml_control_characters(file_get_contents($imported_file));
    $xml = $this->serializer_simplexml_load_string($xml_string);

    // Register the xliff namespace, required for xpath.
    $xml->registerXPathNamespace('xliff', 'urn:oasis:names:tc:xliff:document:1.2');
    $xml->registerXPathNamespace('html', 'http://www.w3.org/1999/xhtml');

    $translatables = $xml->xpath('//xliff:group[@restype="x-drupal-translatable"]');
    if (empty($translatables)) {
      return $this->oldImport($xml);
    }

    $data = array();
    foreach ($translatables as $translatable) {
      foreach ($translatable->xpath('//*[@restype="x-drupal-field"]') as $field) {
        if (isset($field->{'trans-unit'}) && $field->{'trans-unit'}->attributes()->restype == 'x-drupal-failure') {
          $data[(string) $field['id']]['#text'] = (string) $field->{'trans-unit'}->target;
        }
        else {
          $converter = new CloudwordsConverterToHTML($field->saveXML());
          $data[(string) $field['id']]['#text'] = $converter->toHTML();
        }
      }
    }

    return cloudwords_unflatten_data($data);
  }

  /**
   * Imports a file.
   *
   * @param string $imported_file
   *   The path to the file to be imported.
   *
   * @return array
   *   A Cloudwords data array.
   */
  public function oldImport($xml) {

    // Register the xliff namespace, required for xpath.
    $xml->registerXPathNamespace('xliff', 'urn:oasis:names:tc:xliff:document:1.2');

    $data = array();
    foreach ($xml->xpath('//xliff:trans-unit') as $unit) {
      $data[(string) $unit['id']]['#text'] = (string) $unit->target;
    }
    return cloudwords_unflatten_data($data);
  }

  /**
   * Converts xml to string and handles entity encoding.
   *
   * @param string $xml_string
   *   The xml string to convert to xml.
   *
   * @return bool
   *   Returns SimpleXml object
   */

  public function serializer_simplexml_load_string($xml_string){
    $numericTable = array();
    //commonly present restricted characters that can safely be replaced
    $numericTable['&'] = '&#38;';
    $trans = get_html_translation_table(HTML_ENTITIES, ENT_QUOTES);
    foreach ($trans as $k=>$v){
      $numericTable[$v]= "&#".ord($k).";";
    }
    $xml_string = strtr($xml_string, $numericTable);
    return simplexml_load_string($xml_string);
  }
  
  /**
   * Validates an import.
   *
   * @param CloudwordsDrupalProject $project
   *   A Cloudwords project.
   * @param CloudwordsLanguage $language
   *   A Cloudwords language object.
   * @param string $imported_file
   *   The file path of the file to import.
   *
   * @return bool
   *   Returns TRUE on success or FALSE on failure.
   */
  public function validateImport(CloudwordsDrupalProject $project, CloudwordsLanguage $language, $imported_file) {
    // It is not possible to load the file directly with simplexml as it gets
    // url encoded due to the temporary://. This is a PHP bug, see
    // https://bugs.php.net/bug.php?id=61469
    $xml_string = _cloudwords_filter_xml_control_characters(file_get_contents($imported_file));

    $error = $this->errorStart();

    // XML does not support most named HTML entities (eg, &nbsp;), but should be
    // able to handle the UTF-8 uncoded entity characters just fine.
    $xml = $this->serializer_simplexml_load_string($xml_string);

    $this->errorStop($error);

    if (!$xml) {
      return FALSE;
    }

    // Register the xliff namespace, required for xpath.
    $xml->registerXPathNamespace('xliff', 'urn:oasis:names:tc:xliff:document:1.2');

    $error_args = array(
      '%file' => $imported_file,
      '%lang' => $language->getLanguageCode(),
      '%name' => $project->getName(),
      '%id' => $project->getId(),
    );

    // Check if our phase information is there.
    $phase = $xml->xpath("//xliff:phase[@phase-name='extraction']");
    if ($phase) {
      $phase = reset($phase);
    }
    else {
      drupal_set_message(t('Phase missing from %file', $error_args), 'error');
      return FALSE;
    }

    // Check if the project can be loaded.
    if (!isset($phase['job-id']) || ($project->getId() != (string) $phase['job-id'])) {
      drupal_set_message(t('The project id is missing in %file.', $error_args), 'error');
      return FALSE;
    }
    elseif ($project->getId() != (string) $phase['job-id']) {
      drupal_set_message(t('The project id is invalid in %file. Correct id: %id.', $error_args), 'error');
      return FALSE;
    }

    // Compare source language.
    if (!isset($xml->file['source-language'])) {
      drupal_set_message(t('The source language is missing in %file.', $error_args), 'error');
      return FALSE;
    }
    elseif ($xml->file['source-language'] != 'en') {
      drupal_set_message(t('The source language is invalid in %file. Correct langcode: en.', $error_args), 'error');
      return FALSE;
    }

    // Compare target language.
    if (!isset($xml->file['target-language'])) {
      drupal_set_message(t('The target language is missing in %file.', $error_args), 'error');
      return FALSE;
    }
    elseif ($language->getLanguageCode() != _cloudwwords_normalize_langcode($xml->file['target-language'])) {
      $error_args['%wrong'] = $xml->file['target-language'];
      drupal_set_message(t('The target language %wrong is invalid in %file. Correct langcode: %lang', $error_args), 'error');
      return FALSE;
    }

    // Validation successful.
    return TRUE;
  }

  /**
   * Starts custom error handling.
   *
   * @return bool
   *   The previous value of use_errors.
   */
  protected function errorStart() {
    return libxml_use_internal_errors(TRUE);
  }

  /**
   * Ends custom error handling.
   *
   * @param bool $use
   *   The return value of CloudwordsFileformatXLIFF::errorStart().
   */
  protected function errorStop($use) {
    foreach (libxml_get_errors() as $error) {
      switch ($error->level) {
        case LIBXML_ERR_WARNING:
        case LIBXML_ERR_ERROR:
          $type = 'warning';
          break;

        case LIBXML_ERR_FATAL:
          $type = 'error';
          break;
      }
      $message = t('%error on line %num. Error code: %code', array(
        '%error' => trim($error->message),
        '%num' => $error->line, '%code' => $error->code,
      ));
      drupal_set_message($message, $type, FALSE);
    }
    libxml_clear_errors();
    libxml_use_internal_errors($use);
  }

}

function _cloudwwords_normalize_langcode($langcode) {
  return str_replace('_', '-', strtolower($langcode));
}
