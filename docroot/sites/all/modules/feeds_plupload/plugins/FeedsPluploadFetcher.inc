<?php

/**
 * @file
 * Home of the FeedsPluploadFetcher and related classes.
 */

/**
 * Definition of the import batch object created on the fetching stage by
 * FeedsPluploadFetcher.
 */
class FeedsPluploadFetcherResult extends FeedsFetcherResult {

  /**
   * The file record as provided by the plupload form element.
   *
   * @var array
   */
  protected $file;

  /**
   * Constructor.
   */
  public function __construct(array $file) {
    parent::__construct('');
    $this->file = $file;
  }

  /**
   * Overrides parent::getRaw().
   */
  public function getRaw() {
    return $this->sanitizeRaw(file_get_contents($this->file['tmppath']));
  }

  /**
   * Overrides parent::getFilePath().
   */
  public function getFilePath() {
    if (!file_exists($this->file['tmppath'])) {
      throw new Exception(t('File @filepath is not accessible.', array('@filepath' => $this->file['tmppath'])));
    }
    return $this->sanitizeFile($this->file['tmppath']);
  }

}

/**
 * Fetches data using the plupload form element.
 */
class FeedsPluploadFetcher extends FeedsFetcher {

  /**
   * Implements FeedsFetcher::fetch().
   */
  public function fetch(FeedsSource $source) {
    $source_config = $source->getConfigFor($this);
    $state = $source->state(FEEDS_FETCH);

    if (!isset($state->files)) {
      // Extract files from plupload element.
      $state->files = array();
      foreach (unserialize($source_config['source']) as $file) {
        $state->files[] = $file;
      }
      $state->total = count($state->files);
    }

    if (count($state->files)) {
      $file = array_shift($state->files);
      $state->progress($state->total, $state->total - count($state->files));
      return new FeedsPluploadFetcherResult($file);
    }
  }

  /**
   * Source form.
   */
  public function sourceForm($source_config) {
    $form = array();

    $form['source'] = array(
      '#type' => 'plupload',
      '#title' => t('Upload files'),
      '#required' => TRUE,
      '#autoupload' => $this->config['autoupload'],
      '#autosubmit' => $this->config['autosubmit'],
      '#upload_validators' => array(
        'file_validate_extensions' => array($this->config['allowed_extensions']),
      ),
    );

    return $form;
  }

  /**
   * {@inheritdocs}
   */
  public function sourceFormValidate(&$values) {
    $values['source'] = serialize($values['source']);
  }

  /**
   * Overrides parent::configDefaults().
   */
  public function configDefaults() {
    return array(
      'allowed_extensions' => 'txt csv tsv xml opml',
      'autoupload' => FALSE,
      'autosubmit' => FALSE,
    );
  }

  /**
   * Overrides parent::configForm().
   */
  public function configForm(&$form_state) {
    $form = array();
    $form['allowed_extensions'] = array(
      '#type' => 'textfield',
      '#title' => t('Allowed file extensions'),
      '#description' => t('Allowed file extensions for upload.'),
      '#default_value' => $this->config['allowed_extensions'],
    );
    $form['autoupload'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto upload'),
      '#description' => t('Start uploading immediately after files are added.'),
      '#default_value' => $this->config['autoupload'],
    );
    $form['autosubmit'] = array(
      '#type' => 'checkbox',
      '#title' => t('Auto submit'),
      '#description' => t('Submit the form after automatic upload has finished.'),
      '#default_value' => $this->config['autosubmit'],
      '#states' => array(
        'enabled' => array(
          'input[name="autoupload"]' => array('checked' => TRUE),
        ),
      ),
    );

    return $form;
  }

}
