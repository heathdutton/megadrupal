<?php
/**
 * @file
 * Git Entity classes.
 */

class GitServer extends Entity {

  public $is_new = FALSE;

  public $name;

  public $title;

  public $log;

  /**
   * Creates a new entity.
   *
   * @see entity_create()
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, 'git_server');
  }

  /**
   * Generates the URI for the entity.
   */
  public function defaultUri() {
    return array('path' => 'git/server/' . $this->identifier(), 'options' => array());
  }

  /**
   * Generates the title for the entity.
   */
  public function label() {
    $label = t('Invalid');
    if (isset($this->title)) {
      return $this->title;
    }
    return $label;
  }

  /**
   * Saves the entity.
   */
  public function save() {
    $this->modified = REQUEST_TIME;
    if (empty($this->sid) || !empty($this->is_new)) {
      $this->created = REQUEST_TIME;
    }
    parent::save();
  }

  /**
   * Provides the url of this server for use as a git remote: eg. git@localhost
   *
   * @return string
   */
  public function remoteUrl() {
    return $this->user . '@' . $this->host;
  }

  /**
   * Build the view page for a git server.
   */
  public function buildContent($view_mode = 'full', $langcode = NULL) {
    $content = array();
    $defaults = array(
      '#theme' => 'field',
      '#access' => TRUE,
      '#view_mode' => $view_mode,
      '#language' => LANGUAGE_NONE,
      '#field_name' => 'field_fake_description',
      '#field_type' => 'text',
      '#label_display' => 'above',
      '#entity_type' => 'git_server',
      '#bundle' => 'server',
      '#items' => array(array('value' => $this->sid)),
      '#formatters' => 'text_default',
    );

    $content['url'] = $defaults + array(
      '#title' => 'Server URL',
      0 => array('#markup' => check_plain("{$this->user}@{$this->host}")),
    );

    $content += parent::buildContent($view_mode, $langcode);

    return $content;
  }
}
