<?php

/**
 * @file
 * Component Destination classes.
 */

use \ChapterThree\AppleNewsAPI\Document\Metadata;

/**
 * Base class for metadata destinations.
 */
class ApplenewsDestinationMetadata extends ApplenewsDestinationSingleton {

  /**
   * {@inheritdoc}
   */
  public function settings() {
    return [
      'date_published'     => NULL,
      'date_created'       => NULL,
      'date_modified'      => NULL,
      'authors'            => NULL,
      'thumbnailURL'       => NULL,
      'keywords'           => NULL,
      'excerpt'            => NULL,
      'transparentToolbar' => NULL,
    ] + parent::settings();
  }

  /**
   * {@inheritdoc}
   */
  public function getConfigForm(array $values = []) {
    $form = parent::getConfigForm($values);

    $element = parent::getConfigFormSourceElement('date_published');
    $element['#title'] = t('Date Published');
    $form['settings']['date_published'] = $element;

    $element = parent::getConfigFormSourceElement('date_created');
    $element['#title'] = t('Date Created');
    $form['settings']['date_created'] = $element;

    $element = parent::getConfigFormSourceElement('date_modified');
    $element['#title'] = t('Date Modified');
    $form['settings']['date_modified'] = $element;

    $element = parent::getConfigFormSourceElement('authors');
    $element['#title'] = t('Author');
    $form['settings']['authors'] = $element;

    $element = parent::getConfigFormSourceElement('thumbnailURL', TRUE, TRUE);
    $element['#title'] = t('Thumbnail');
    $form['settings']['thumbnailURL'] = $element;

    $element = parent::getConfigFormSourceElement('keywords');
    $element['#title'] = t('Keywords');
    $form['settings']['keywords'] = $element;

    $element = parent::getConfigFormSourceElement('excerpt');
    $element['#title'] = t('Excerpt');
    $form['settings']['excerpt'] = $element;

    $element = parent::getConfigFormSourceElement('transparentToolbar');
    $element['#title'] = t('Transparent Toolbar');
    $form['settings']['transparentToolbar'] = $element;

    return $form;
  }

  /**
   * {@inheritdoc}
   *
   * @return \ChapterThree\AppleNewsAPI\Document\Metadata
   *   Apple News Document layout object.
   */
  public function export() {
    $metadata = new Metadata();

    $date_string = function($string) {
      if (intval($string) == $string) {
        $date = date_timestamp_set(new DateTime(), $string);
      }
      else {
        $date = date_create($string);
      }
      if ($date) {
        $date->setTimeZone(new DateTimeZone('UTC'));
        return $date->format(DateTime::ISO8601);
      }
      watchdog('applenews', 'Could not decode value as datetime.');
      return FALSE;
    };

    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('date_published'));
    if (!empty($values[0][$key]) && $date = $date_string($values[0][$key])) {
      $metadata->setDatePublished($date);
    }
    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('date_created'));
    if (!empty($values[0][$key]) && $date = $date_string($values[0][$key])) {
      $metadata->setDateCreated($date);
    }
    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('date_modified'));
    if (!empty($values[0][$key]) && $date = $date_string($values[0][$key])) {
      $metadata->setDateModified($date);
    }

    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('authors'));
    foreach ($values as $value) {
      if (!empty($value[$key])) {
        $metadata->addAuthor($value[$key]);
      }
    }

    $metadata->setGeneratorName('Chapter Three Apple News Module')
      ->setGeneratorIdentifier('com.chapterthree.drupal.applenews');
    $module = system_list('module_enabled')['applenews'];
    if (!empty($module->info['version'])) {
      $metadata->setGeneratorVersion($module->info['version']);
    }

    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('thumbnailURL'));
    if (!empty($values[0][$key])) {
      $url = @$this->getSetting('thumbnailURL')['bundle']
        ? $this->export->addAsset($values[0][$key]) : $values[0][$key];
      if ($url) {
        $metadata->setThumbnailURL($url);
      }
    }

    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('keywords'));
    foreach ($values as $value) {
      if (!empty($value[$key])) {
        foreach (preg_split('/\s+/', $value[$key]) as $keyword) {
          $metadata->addKeyword($keyword);
        }
      }
    }

    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('excerpt'));
    if (!empty($values[0][$key])) {
      $metadata->setExcerpt($values[0][$key]);
    }

    list($key, $values) = $this->getSourceValues($this->export->source,
      $this->getSetting('transparentToolbar'));
    if (!empty($values[0][$key])) {
      $metadata->setTransparentToolbar(TRUE);
    }

    return $metadata;
  }

}
