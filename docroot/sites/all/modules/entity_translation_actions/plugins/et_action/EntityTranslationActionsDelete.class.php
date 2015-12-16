<?php

/**
 * @file
 * Et_action plugin class.
 */

class EntityTranslationActionsDelete extends EntityTranslationActionsBasic {

  /**
   * Function checks if plugin available for selected entity type.
   */
  public function available() {
    return entity_translation_enabled($this->entityType);
  }

  /**
   * Function function executes plugin actions.
   */
  public function action($entity, $context, $handler = NULL) {
    $handler = $handler ? $handler : entity_translation_get_handler($this->entityType, $entity);

    $translations = $handler->getTranslations();
    // Set new original language if replaced language was original.
    $lang_entity = $this->entityLanguage($entity, $handler, FALSE);
    if ($lang_entity && in_array($lang_entity, $this->options['language'])) {

      $language_key = $handler->getLanguageKey();
      $translations->original = LANGUAGE_NONE;
      $entity->{$language_key} = LANGUAGE_NONE;

      foreach ($translations->data as &$translation) {
        if ($translation['language'] == LANGUAGE_NONE) {
          $translation['source'] = FALSE;
          $handler->setTranslation($translation);
        }
        elseif ($translation['source'] == $lang_entity) {
          $translation['source'] = LANGUAGE_NONE;
          $handler->setTranslation($translation);
        }
        unset($translation);
      }
    }

    foreach ($this->options['language'] as $language) {
      $handler->removeTranslation($language);
    }

    return ENTITY_ACTIONS_KIT_RESULT_DELETED;
  }

  /**
   * Function builds form elements for action.
   */
  public function formBuild(&$form, &$form_state) {
    $form['language'] = array(
      '#type' => 'checkboxes',
      '#options' => $this->languagesList(),
      '#title' => t('Select languages:'),
      '#required' => TRUE,
    );
  }

  /**
   * Function process form data.
   *
   * Filter selected language options.
   */
  public function formSubmit($form, &$form_state, $options) {
    parent::formSubmit($form, $form_state, $options);
    $this->options['language'] = array_filter($this->options['language']);
  }

}
