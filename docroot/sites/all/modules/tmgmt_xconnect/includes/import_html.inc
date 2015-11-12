<?php
/**
 * @file
 * Importer to import HTML files.
 */

/**
 * Implements the import to extract the translation from a received HTML string.
 */
class TmgmtXConnectImportHtml implements TmgmtXConnectImportInterface {
  /**
   * {@inheritdoc}
   */
  public function import($content) {
    $job = $this->getJob($content);

    // Check if the Job can accept the import.
    $this->checkJobFound($job);
    $this->checkJobIsFinished($job);

    // Add the id to the job.
    $data = $this->extractData($content);
    $job->addTranslatedData($data);

    return $job;
  }

  /**
   * Helper to get the Job based on the content.
   *
   * @param string $content
   *   The content to get the Job for.
   *
   * @return TMGMTJob|FALSE
   *   The found job.
   */
  protected function getJob($content) {
    // Create the simpleXml element out of the provided HTML string.
    $dom = new DOMDocument();
    if (!$dom->loadHTML($content)) {
      return FALSE;
    }
    $xml = simplexml_import_dom($dom);

    // Collect meta information.
    $meta_tags = $xml->xpath('//meta');
    $meta = array();
    foreach ($meta_tags as $meta_tag) {
      $meta[(string) $meta_tag['name']] = (string) $meta_tag['content'];
    }

    // Check required meta tags.
    foreach (array('JobID', 'languageSource', 'languageTarget') as $name) {
      if (!isset($meta[$name])) {
        return FALSE;
      }
    }

    // Attempt to load job.
    if (!$job = tmgmt_job_load($meta['JobID'])) {
      return FALSE;
    }

    // Check language.
    $translator = $job->getTranslator();
    if ($meta['languageSource'] != $translator->mapToRemoteLanguage($job->source_language) ||
      $meta['languageTarget'] != $translator->mapToRemoteLanguage($job->target_language)) {
      return FALSE;
    }

    // Validation successful.
    return $job;
  }

  /**
   * Check if a valid job is found.
   *
   * @param mixed $job
   *   The job to validate (if any).
   *
   * @throws Exception
   *   When no job is found.
   */
  protected function checkJobFound($job) {
    if (!($job instanceof TMGMTJob)) {
      throw new Exception(
        'No job found for the given translation.'
      );
    }
  }

  /**
   * Check if the given job is not already processed.
   *
   * @param TMGMTJob $job
   *   The job to check.
   *
   * @throws Exception
   *   When the job is already finished.
   */
  protected function checkJobIsFinished(TMGMTJob $job) {
    if ($job->isFinished()) {
      throw new Exception(
        'The job is already finished for the given translation.'
      );
    }
  }

  /**
   * Extract the translated id from the HTML string.
   *
   * @param string $content
   *   The HTML string.
   *
   * @return array
   *   The extracted Data.
   */
  public function extractData($content) {
    $dom = new DOMDocument();
    $dom->loadHTML($content);
    $xml = simplexml_import_dom($dom);

    $data = array();
    foreach ($xml->xpath("//div[@class='atom']") as $atom) {
      // Assets are our strings (eq fields in nodes).
      $id = (string) $atom['id'];
      $key = $this->decodeId($id);
      $text = $this->extractInnerHtml($dom->getElementById($id));
      $data[$key]['#text'] = $text;
    }

    return tmgmt_unflatten_data($data);
  }

  /**
   * Helper to decode the XML id back to the field id.
   *
   * @param string $id
   *   The encoded id.
   *
   * @return string
   *   The decoded id.
   */
  protected function decodeId($id) {
    // Remove prefixed b.
    $id = substr($id, 1);

    // Convert back to raw encoded string.
    $raw = str_pad(strtr($id, '-_', '+/'), strlen($id) % 4, '=', STR_PAD_RIGHT);

    return base64_decode($raw);
  }

  /**
   * Get inner html of a DOM Element.
   *
   * @param DOMElement $element
   *   The dom element.
   *
   * @return string
   *   The inner html as string
   */
  protected function extractInnerHtml(DOMElement $element) {
    $inner_html = '';
    $children  = $element->childNodes;

    foreach ($children as $child) {
      $inner_html .= (isset($child->wholeText))
        ? $child->wholeText
        : $element->ownerDocument->saveHTML($child);
    }

    return $inner_html;
  }

}
