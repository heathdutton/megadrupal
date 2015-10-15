<?php

/**
 * @file
 * Contains TMGMTBulkSelectorLocale class.
 */

/**
 * Selector for the "locale" source plugin.
 */
class TMGMTBulkSelectorLocale extends TMGMTBulkSelectorBase implements TMGMTBulkSelectorInterface {

  /**
   * {@inheritdoc}
   */
  public function searchItems($item_type, $translated_to, $not_exists_in_jobs, array $specific) {
    $query = db_select('locales_source', 'ls')
      ->condition('ls.textgroup', $item_type)
      ->fields('ls', array('lid'));

    if ($translated_to) {
      $translation_table_alias = $query->leftJoin(
        'locales_target',
        NULL,
        "%alias.lid = ls.lid AND %alias.language = :language",
        array(':language' => $translated_to)
      );
      $query->isNotNull("$translation_table_alias.lid");
      if (module_exists('l10n_update') && !empty($specific['filter_l10n_update'])) {
        module_load_include('inc', 'l10n_update');
        $query->condition("$translation_table_alias.l10n_status", L10N_UPDATE_STRING_CUSTOM);
      }
    }

    $item_ids = $query->execute()->fetchCol();
    if ($not_exists_in_jobs) {
      $item_ids = $this->filterExistingInJobs('locale', $item_type, $item_ids, $not_exists_in_jobs);
    }
    return $item_ids;
  }

  /**
   * {@inheritdoc}
   */
  public function getFormSpecificFilters() {
    if (module_exists('l10n_update')) {
      return array(
        'filter_l10n_update' => array(
          '#type' => 'checkbox',
          '#title' => t('Filter out strings translated with the Localization update module'),
          '#description' => t('When checked: the result will only include manually translated strings.'),
          '#states' => array(
            'invisible' => array(
              'select[name="plugins[locale][filters][translated_to]"]' => array('value' => ''),
            ),
          ),
        ),
      );
    }
    return parent::getFormSpecificFilters();
  }
}
