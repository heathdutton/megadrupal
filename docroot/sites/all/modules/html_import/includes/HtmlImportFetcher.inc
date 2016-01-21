<?php

/**
 * @file
 * Fetches an HTML file for import.
 */

class HtmlImportFetcher extends FeedsFileFetcher {

  protected $imageFormElement = 'feeds_images';
  protected $log;

  /**
   * Override parent::configForm().
   */
  public function sourceForm($source_config) {
    $form = parent::sourceForm($source_config);
    $form['heading_level'] = array(
      '#type' => 'select',
      '#title' => t('Heading level depth'),
      '#description' => t('The maximum depth of heading levels that will be imported as a separate book page'),
      '#options' => array(
        '1' => 'h1',
        '2' => 'h2',
        '3' => 'h3',
        '4' => 'h4',
        '5' => 'h5',
        '6' => 'h6',
      ),
      '#default_value' => empty($source_config['heading_level']) ? '2' : $source_config['heading_level'],
    );

    $form['page_start_heading_level'] = array(
      '#type' => 'select',
      '#title' => t('Page title heading level'),
      '#access' => html_import_perm('administer content'),
      '#description' => t('The heading level of the book page title. Headings of the imported content will be bumped to minimise skipped heading levels.'),
      '#options' => array(
        '0' => 'Do not bump heading levels',
        '1' => 'h1',
        '2' => 'h2',
        '3' => 'h3',
      ),
      '#default_value' => empty($source_config['page_start_heading_level']) ? '1' : $source_config['page_start_heading_level'],
    );

    $form['allowed_html_tags'] = array(
      '#type' => 'textfield',
      '#maxlength' => 512,
      '#title' => t('Allowed HTML tags'),
      '#access' => html_import_perm('administer content'),
      '#description' => t('Enter allowed HTML tags. Do not leave space between tags.'),
      '#default_value' => empty($source_config['allowed_html_tags']) ? '<h1><h2><h3><h4><h5><h6><p><em><strong><img><a><table><caption><thead><tbody><tr><th><td><blockquote><sup><sub><div><b><ol><ul><li><i><br>' : $source_config['allowed_html_tags'],
    );

    $form['footnote_link_pattern'] = array(
      '#type' => 'textfield',
      '#title' => t('Footnote link pattern'),
      '#access' => html_import_perm('administer content'),
      '#description' => t('<strong>Caution expert only!</strong> E.g. if links to footnotes in the text are like #fnr_123, then enter <em>fnr_</em>. Escape special characters such as dash using a back slash.'),
      '#default_value' => empty($source_config['footnote_link_pattern']) ? 'fn_' : $source_config['footnote_link_pattern'],
    );

    $form['footnote_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Footnote class'),
      '#access' => html_import_perm('administer content'),
      '#description' => t('<strong>Caution!</strong> The CSS class of a footnote\'s parent P tag. Escape special characters such as dash using a back slash.'),
      '#default_value' => empty($source_config['footnote_class']) ? 'footnote\-text' : $source_config['footnote_class'],
    );

    $form['title_all_caps'] = array(
      '#type' => 'radios',
      '#title' => t('Convert book page title in ALL CAPS to'),
      '#access' => html_import_perm('administer content'),
      '#description' => t('How to handle section titles in ALL CAPS that will become the title of a book page'),
      '#options' => array(
        '0' => 'Do not convert',
        '1' => 'Convert to "Sentence case"',
        '2' => 'Convert to "Title Case"',
      ),
      '#default_value' => empty($source_config['title_all_caps']) ? '0' : $source_config['title_all_caps'],
    );

    //generic container for passing settings
    $form['settings'] = array(
      '#type' => 'value',
      '#value' => array(),
    );
    return $form;
  }

  /**
   * Override parent::sourceFormValidate().
   */
  public function sourceFormValidate(&$values) {
    $values['source'] = trim($values['source']);
    $feed_dir = 'public://feeds';
    file_prepare_directory($feed_dir, FILE_CREATE_DIRECTORY | FILE_MODIFY_PERMISSIONS);

    // If there is a file uploaded, save it, otherwise validate input on
    // file.
    if ($file = file_save_upload('feeds', array('file_validate_extensions' => array(0 => $this->config['allowed_extensions'])), $feed_dir)) {
      $values['source'] = $file->uri;
      $values['file'] = $file;
    }
  }
}
