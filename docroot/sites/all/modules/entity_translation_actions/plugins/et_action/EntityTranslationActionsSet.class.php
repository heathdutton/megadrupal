<?php

/**
 * @file
 * Et_action plugin class.
 */

class EntityTranslationActionsSet extends EntityTranslationActionsBasic {

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

    $lang_source = $this->entityLanguage($entity, $handler, FALSE);

    if ($lang_source == $this->options['target']) {
      return ENTITY_TRANSLATION_ACTIONS_RESULT_EQUAL;
    }

    $translations = $handler->getTranslations();

    // Add translation for new language if it doesn`t exist.
    if (!isset($translations->data[$this->options['target']]) && $lang_source && $this->options['target'] != LANGUAGE_NONE) {

      $options_add = array(
        'source' => $lang_source,
        'language' => $this->options['target'],
        'translation_exists' => ENTITY_TRANSLATION_ACTIONS_TRANSLATION_REPLACE,
      );

      $add_plugin = entity_translation_actions_plugin('EntityTranslationActionsAdd');
      $add_class = ctools_plugin_get_class($add_plugin, 'handler');
      if ($add_class) {
        $add_handler = new $add_class($this->entityType, $options_add);
        $add_handler->action($entity, $context, $handler);
      }
    }

    // Set original translation for entity.
    $language_key = $handler->getLanguageKey();
    $translations->original = $this->options['target'];
    $entity->{$language_key} = $this->options['target'];

    // Process entity translations.
    foreach ($translations->data as &$translation) {

      if ($translation['language'] == LANGUAGE_NONE && $this->options['target'] != LANGUAGE_NONE) {

        // Remove translation for language_none if new language is not empty.
        $handler->removeTranslation($translation['language']);
      }
      elseif ($translation['language'] == $this->options['target']) {

        // Remove source from translation for new language. It will be original.
        $translation['source'] = FALSE;
        $handler->setTranslation($translation);
      }
      else {

        // Set source language for other translations.
        $translation['source'] = $this->options['target'];
        $handler->setTranslation($translation);
      }

      // Delete reference to translation in order to prevent mix of data.
      unset($translation);
    }

    return ENTITY_TRANSLATION_ACTIONS_RESULT_REPLACED;
  }

  /**
   * Function builds form elements for action.
   */
  public function formBuild(&$form, &$form_state) {

    $form['target'] = array(
      '#type' => 'radios',
      '#options' => $this->languagesList(),
      '#title' => t('Select language:'),
      '#required' => TRUE,
    );
  }

}
