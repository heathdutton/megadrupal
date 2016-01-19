<?php

/**
 * @file
 * Et_action plugin class.
 */

interface EntityTranslationActionsInterface {

  /**
   * Function checks if plugin available for selected entity type.
   */
  public function available();

  /**
   * Function function executes plugin actions.
   */
  public function action($entity, $context, $handler = NULL);

  /**
   * Function builds form elements for action.
   */
  public function formBuild(&$form, &$form_state);

  /**
   * Function validates form data.
   */
  public function formValidate($form, &$form_state);

  /**
   * Function process form data.
   */
  public function formSubmit($form, &$form_state, $options);

  /**
   * Function returns entity language.
   */
  public function entityLanguage($entity, $handler, $default);
}

class EntityTranslationActionsBasic implements EntityTranslationActionsInterface {

  public $entityType;
  public $options;

  /**
   * Class constructor.
   */
  public function __construct($entity_type, $options) {
    $this->entityType = $entity_type;
    $this->options = $options;
  }

  /**
   * Function checks if plugin available for selected entity type.
   */
  public function available() {
    return FALSE;
  }

  /**
   * Function function executes plugin actions.
   */
  public function action($entity, $context, $handler = NULL) {

  }

  /**
   * Function builds form elements for action.
   */
  public function formBuild(&$form, &$form_state) {

  }

  /**
   * Function validates form data.
   */
  public function formValidate($form, &$form_state) {

  }

  /**
   * Function process form data.
   */
  public function formSubmit($form, &$form_state, $options) {
    $this->options = $options;
  }

  /**
   * Function returns entity language.
   */
  public function entityLanguage($entity, $handler, $default) {

    // Get name of language field.
    $language_key = $handler->getLanguageKey();
    if ($language_key) {
      if (isset($entity->{$language_key}) && !empty($entity->{$language_key})) {

        // Return entity language from a propertie.
        return $entity->{$language_key};
      }
    }

    // Get all entity translations.
    $translations = $handler->getTranslations();

    if (!empty($translations->original)) {

      // Return entity original language.
      return $translations->original;
    }
    else {

      // Default fallback.
      return $default;
    }
  }

  /**
   * Function returns list of available languages for translations.
   */
  protected function languagesList() {

    $languages = entity_translation_languages();

    $language_none = array(LANGUAGE_NONE => t('Language neutral'));
    $languages_list = $language_none;

    foreach ($languages as $language) {
      $languages_list[$language->language] = $language->name;
    }

    return $languages_list;
  }

  /**
   * Function returns options for action form.
   */
  protected function languagesOptions() {

    $languages_options = array(
      '-entity-' => t('Entity language'),
    ) + $this->languagesList();

    return $languages_options;
  }

}
