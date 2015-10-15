<?php
/**
 * @file
 *  GitRepository class
 */

class GitRepository extends Entity {

  public $is_new = FALSE;

  public $bundle = 'repository';

  public $name;

  public $title;

  public $server_name;

  public $log;

  /**
   * Creates a new entity.
   *
   * @see entity_create()
   */
  public function __construct(array $values = array(), $entityType = NULL) {
    parent::__construct($values, 'git_repository');
  }

  /**
   * Generates the URI for the entity.
   */
  public function defaultUri() {
    return array('path' => 'git/repository/' . $this->identifier(), 'options' => array());
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
    if (empty($this->url) || !empty($this->is_new)) {
      $this->created = REQUEST_TIME;
    }
    parent::save();
  }

  /**
   * Get this repo's git_server.
   *
   * @return GitServer
   */
  public function server() {
    return git_server_name_load($this->server_name);
  }


  /**
   * Provides the url of this repository for use as a git remote: eg. git@localhost:repo.git
   *
   * @return string
   */
  public function remoteUrl() {
    return $this->server()->remoteUrl() . ':' . $this->name;
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
      '#items' => array(array('value' => $this->rid)),
      '#formatters' => 'text_default',
    );

    $content['title'] = $defaults + array(
      '#title' => 'Title',
      0 => array('#markup' => check_plain(entity_label('git_repository', $this))),
    );

    $content['url'] = $defaults + array(
      '#title' => 'Machine Name',
      0 => array('#markup' => check_plain($this->name)),
    );

    $git_server = git_server_name_load($this->server_name);
    $git_server_uri = entity_uri('git_server', $git_server);
    $git_server_uri['options'] += array('attributes' => array(), 'html' => FALSE);
    $content['server'] = $defaults + array(
      '#title' => 'Git server',
      0 => array(
        '#theme' => 'link',
        '#text' => entity_label('git_server', $git_server),
        '#path' => $git_server_uri['path'],
        '#options' => $git_server_uri['options'],
      ),
    );

    $content['server_url'] = $defaults + array(
      '#title' => 'Repo URL',
      0 => array('#markup' => check_plain("{$git_server->user}@{$git_server->host}:{$this->name}.git")),
    );

    return $content;
  }
}
