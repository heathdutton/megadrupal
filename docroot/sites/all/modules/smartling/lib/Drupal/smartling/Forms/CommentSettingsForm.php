<?php

namespace Drupal\smartling\Forms;

class CommentSettingsForm extends GenericEntitySettingsForm {

  public function __construct() {
    $this->entity_name_translated = t('Comment');
    $this->entity_key = '#comment';
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'smartling_get_comment_settings_form';
  }
}