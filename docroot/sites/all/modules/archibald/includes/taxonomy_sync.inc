<?php

/**
 * @file
 * this file contains all methodes to sync local taxonomy with ontology server
 */

/**
 * Imports Taxonomies from Ontology Server into the drupal database.
 * As an alternative the data can be importer from the CSV file
 * (param $source = 'csv').
 *
 * The Terms in the drupal taxonomies are:
 *  - deleted in case they no longer provided by ontology server
 *  - added in case they are new provided by ontology server
 *  - replace in case ontology server provides new terms with existing keys
 *
 * @param string $source ontology or csv
 *
 * @todo identifier (key) of the ontology to import is hard coded ! (lom-v1.0)
 * Find a way how the identifier is configurable.
 */
function archibald_sync_taxonomy($source = 'csv') {
  $vocabularies = &archibald_static(__FUNCTION__, NULL);

  // load vocabulary vid s
  if (is_null($vocabularies)) {
    $vocabularies = array();
    foreach (taxonomy_get_vocabularies() as $vocabularie) {
      $vocabularies[$vocabularie->machine_name] = $vocabularie->vid;
    }
  }

  if ($source == 'ontology') {
    // load data from ontology server
    $taxonomy = archibald_get_taxonomy();

    if ($taxonomy == NULL) {
      echo "Ontology Service provides unuseable taxonomy data.";
      return;
    }

    $new_data = array();
    foreach ($taxonomy->vocabularies as $vocabulary) {
      foreach ($vocabulary->terms as $term) {
        $identifier = $vocabulary->identifier;
        $new_data[$identifier][$term->context][$term->identifier] = (object)array(
          'de' => mb_convert_encoding(
            $term->name->de, 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'it' => mb_convert_encoding(
            $term->name->it, 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'fr' => mb_convert_encoding(
            $term->name->fr, 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'en' => mb_convert_encoding(
            $term->name->en, 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'rm' => mb_convert_encoding(
            $term->name->rm, 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'value' => mb_convert_encoding(
            $term->identifier, 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'source' => mb_convert_encoding(
            $term->context, 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'is_new' => TRUE,
        );
      }
    }
  }
  else {
    // load data from csv
    $new_data = array();
    $handle = fopen(dirname(__FILE__) . '/../data/lom_taxonomyen.csv', 'r');
    while (($data = fgetcsv($handle, 1000, ';')) !== FALSE) {
      if (!empty($data[0]) && drupal_strtolower($data[0]) != 'catalog') {
        if (empty($data[7])) {
          $data[7] = 'none';
        }
        $new_data[$data[0]][$data[7]][$data[6]] = (object)array(
          'de' => mb_convert_encoding(
            $data[1], 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'it' => mb_convert_encoding(
            $data[2], 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'fr' => mb_convert_encoding(
            $data[3], 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'en' => mb_convert_encoding(
            $data[4], 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'rm' => mb_convert_encoding(
            $data[5], 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'value' => mb_convert_encoding(
            $data[6], 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'source' => mb_convert_encoding(
            $data[7], 'UTF-8', 'UTF-8,ISO-8859-1'
          ),
          'is_new' => TRUE,
        );
      }
    }
    fclose($handle);
  }

  // check if some data in db but not in csv and delete them from db
  foreach ($new_data as $vocab => $data) {
    $tree = taxonomy_get_tree(
      $vocabularies[strtr($vocab, array(' ' => '_'))],
      0,
      NULL,
      TRUE
    );

    foreach ($tree as $term) {
      $taxon_key = @$term->field_taxon_key['und'][0]['value'];
      $taxon_context = @$term->field_taxon_context['und'][0]['value'];
      if (empty($taxon_context)) {
        $taxon_context = 'none';
      }

      // in remote resource this term dosent exists anymore
      if (empty($data[$taxon_context][$taxon_key])) {
        taxonomy_term_delete($term->tid);

        watchdog(
          'taxonomy', t(
            'Delete term @term_name (@term_id)',
            array(
              '@term_name' => $term->name,
              '@term_id' => $term->tid,
            )
          )
        );

        /* drush_log(t('Delete term @term_name (@term_id)',
          array('@term_name' => $term->name,
          '@term_id'   => $term->tid
          )
          ),
          'status'
          ); */
      }
      elseif (!empty($taxon_key) &&
        $term->name != $data[$taxon_context][$taxon_key]->de
      ) {

        // update if name (german translation has changed)
        $term->name = $data[$taxon_context][$taxon_key]->de;
        taxonomy_term_save($term);

        watchdog(
          'taxonomy', t(
            'Update term @term_name (@term_id)',
            array(
              '@term_name' => $term->name,
              '@term_id' => $term->tid,
            )
          )
        );

        /* drush_log(t('Update term @term_name (@term_id)',
          array('@term_name' => $term->name,
          '@term_id'   => $term->tid
          )
          ),
          'status'
          ); */
      }

      $data[$taxon_context][$taxon_key]->is_new = FALSE;
    }
  }

  // insert new terms
  foreach ($new_data as $vocab => $data_context) {
    if (empty($vocabularies[$vocab])) {
      continue;
    }

    foreach ($data_context as $taxon_context => $data_keys) {
      foreach ($data_keys as $taxon_key => $data) {
        if ($data->is_new == TRUE) {
          $term = new stdClass();
          // couse german is set as default language in profile archibald profile
          $term->name = $data->de;
          $term->description = '';
          $term->format = 'filtered_html';
          $term->vid = $vocabularies[$vocab];
          $term->language = 'und';
          $term->i18n_tsid = 0;
          $term->field_taxon_key = array(
            'und' => array(
              0 => array(
                'value' => $taxon_key,
                'format' => '',
                'safe_value' => $taxon_key,
              ),
            ),
          );
          if ($taxon_context != 'none') {
            $term->field_taxon_context = array(
              'und' => array(
                0 => array(
                  'value' => $taxon_context,
                  'format' => '',
                  'safe_value' => $taxon_context,
                ),
              ),
            );
          }
          taxonomy_term_save($term);

          watchdog(
            'taxonomy', t(
              'Insert term @term_name',
              array(
                '@term_name' => $term->name,
              )
            )
          );
        }
      }
    }
  }

  // update translations
  foreach ($new_data as $vocab => $data) {
    $tree = taxonomy_get_tree(
      $vocabularies[strtr($vocab, array(' ' => '_'))],
      0,
      NULL,
      TRUE
    );

    foreach ($tree as $term) {
      $taxon_key = @$term->field_taxon_key['und'][0]['value'];
      $taxon_context = @$term->field_taxon_context['und'][0]['value'];
      if (empty($taxon_context)) {
        $taxon_context = 'none';
      }

      $element = &$data[$taxon_context][$taxon_key];

      $location = 'taxonomy:term:' . $term->tid . ':name';
      $lid = db_query(
        'SELECT lid  FROM {locales_source} WHERE location = :location',
        array(
          ':location' => $location,
        )
      )->fetchField();

      foreach (array('fr', 'it', 'en', 'rm') as $lang_key) {
        if (!empty($element->$lang_key)) {
          $translation = db_query(
            'SELECT translation FROM {locales_target} WHERE lid = :lid AND language = :language',
            array(
              ':lid' => $lid,
              ':language' => $lang_key,
            )
          )->fetchField();

          // Only update or insert if we have a value to use.
          if (!empty($translation)) {
            db_update('locales_target')
              ->fields(
                array(
                  'translation' => $element->$lang_key
                )
              )
              ->condition('lid', $lid)
              ->condition('language', $lang_key)
              ->execute();

            watchdog(
              'taxonomy',
              t(
                'Update translation @element -> @translation',
                array(
                  '@element' => $element->value,
                  '@translation' => $element->$lang_key,
                )
              )
            );

            /* drush_log(t('Update translation $element -> @translation',
              array('@element'     => $element,
              '@translation' => $elmnt->$lang_key)
              ),
              'status'
              ); */
          }
          else {
            if ($lid > 0) {
              db_insert('locales_target')->fields(
                array(
                  'lid' => $lid,
                  'translation' => $element->$lang_key,
                  'language' => $lang_key,
                )
              )->execute();
            }
          }
        }
      }
    }
  }
}

/**
 *
 * This function makes a HTTP-Request to the Ontology Server to receive the
 * taxonomy data as json string.
 * The URL of Ontology Server URL is gained from config
 * variable archibald_ontology_url.
 *
 * @param string $key the ontology identifier (key), default: lom-v1.0
 *
 * @return string taxonomy data as json
 */
function archibald_get_taxonomy($key = 'lom-ch-v1') {
  $url = variable_get('archibald_ontology_url');
  if (empty($url)) {
    return NULL;
  }
  $data = file_get_contents($url . '/json/' . $key);
  return json_decode($data);
}

/**
 * Calls watchdog function.
 *
 * @param string $text
 *   log text
 * @param int $severity
 *   WATCHDOG_NOTICE
 *   WATCHDOG_ERROR
 *   WATCHDOG_WARNING
 * @param array $parameters
 */
function archibald_log($text, $parameters = array(), $severity = WATCHDOG_NOTICE) {
  watchdog('taxonomy', $text, $parameters, $severity);
}

/**
 * This is the "main function" which performs the synchronization process.
 *
 * @param string $ontology_identifier
 *   when executed during unit test, data is given by parameter
 *   example: lom-v1-installation
 */
function archibald_sync_taxonomy_os($ontology_identifier = NULL) {
  $sync = new ArchibaldTaxonomyOntologyServerSync();
  $sync->sync($ontology_identifier);
}

/**
 * this class provids methodes to sync the local taxnomy`s with the
 * onotology server
 */
class ArchibaldTaxonomyOntologyServerSync {

  /**
   * list of vocabular vid mapping
   * global cuase it will be accesed by more then 1 methode
   * @var array
   */
  protected $m_vocabulary_vids = NULL;

  /**
   * This is the "main function" which performs the synchronization process.
   *
   * @param string $ontology_identifier
   *   when executed during unit test, data is given by parameter
   *   options:
   *     lom-v1
   *     lom-v1-installation
   *     educa_standard_curriculum
   */
  public function sync($ontology_identifier = NULL) {
    $language_content_backup = $GLOBALS['language_content']->language;
    $GLOBALS['language_content']->language = i18n_string_source_language();

    $ontology = $this->loadDataFromOntologyServer($ontology_identifier);

    if ($ontology === NULL) {
      return;
    }

    if ($ontology == FALSE) {
      archibald_log(
        'Error when retrieving data from Ontology Service',
        array(),
        WATCHDOG_ERROR
      );
      return;
    }

    $this->loadTaxonomieVids();

    foreach ($ontology->vocabularies as $vocabulary) {
      if (function_exists('drush_log')) {
        drush_log('Import: ' . $vocabulary->identifier, 'ok');
      }

      $this->importTerms($vocabulary);
    }

    if (function_exists('drush_log')) {
      drush_log('Import done', 'ok');
    }

    $GLOBALS['language_content']->language = $language_content_backup;
  }

  /**
   * This function create an associative array which maps all vocabulary
   * machine names to its vid.
   *
   * @return array
   *  associative array: key: vocab machine name, value: vid
   */
  protected function loadTaxonomieVids() {
    if (is_null($this->m_vocabulary_vids)) {
      $this->m_vocabulary_vids = array();
      foreach (taxonomy_get_vocabularies() as $vocabulary) {
        $this->m_vocabulary_vids[$vocabulary->machine_name] = $vocabulary->vid;
      }
    }
  }

  /**
   * This function creates an array which contains all term
   * identifiers of a vocab which
   * ist subpplied by the ontology service.
   *
   * @param object $vocabulary
   *   vocab object supplied by ontology service
   *
   * @return array
   *   simple array which term identifiers
   */
  protected function fetchTermIdentifiers($vocabulary) {
    $identifiers = array();
    foreach ($vocabulary->terms as $term) {
      $identifiers[$term->identifier] = $term->identifier;
    }
    return $identifiers;
  }

  /**
   * This function loads all data of a vocabulary from the LOCAL database.
   *
   * @param string $vocabulary_name
   *
   * @return object
   */
  protected function loadLocalVocabulary($vocabulary_name) {
    if (isset($this->m_vocabulary_vids[$vocabulary_name])) {
      $vid = $this->m_vocabulary_vids[$vocabulary_name];
      $return = taxonomy_get_tree($vid, 0, NULL, TRUE);
      return $return;
    }
    else {
      return FALSE;
    }
  }

  /**
   * Imports all terms of the given vocabulary.
   *
   * @param object $vocabulary
   *   the vocabulary object supplied by ontology service
   */
  protected function importTerms($vocabulary) {
    $local_vocab = $this->loadLocalVocabulary($vocabulary->identifier);
    $identifiers = $this->fetchTermIdentifiers($vocabulary);

    if ($local_vocab === FALSE) {
      archibald_log(
        'Ontology Service supplied vocabulary @identifier which is not available in local taxonomy database',
        array(
          '@identifier' => $vocabulary->identifier,
        ),
        WATCHDOG_WARNING
      );
      return FALSE;
    } else {
      // Check each term of the vocabulary from the LOCAL DB
      foreach ($local_vocab as $term) {
        // if term is no longer supplied by ontology service, remove it
        $key = $term->field_taxon_key['und'][0]['value'];

        if (!in_array($key, $identifiers)) {
          taxonomy_term_delete($term->tid);
          $this->removeTranslation($term->tid);

          archibald_log(
            'Term @identifier in vocabulary @vocab deleted.',
            array(
              '@identifier' => $key,
              '@vocab' => $vocabulary->identifier,
            ),
            WATCHDOG_WARNING
          );
        }
        else {
          $os_term = $vocabulary->terms->$key;

          $this->setTermAttributes($term, $os_term);
	  //print_r( $os_term );

	  unset($identifiers[$key]);

          taxonomy_term_save($term);

          $this->updateTranslation($term->tid, $vocabulary->terms->$key);
        }
      }
    }

    // Now insert all terms which are supplied by ontology service but
    // not yet exist in the LOCAL DB.
    foreach ($identifiers as $identifier) {
      $tid = $this->insertTerm(
        $vocabulary->terms->$identifier, $vocabulary->identifier
      );

      $this->updateTranslation(
        $tid, $vocabulary->terms->$identifier
      );
    }

    // Finally, set all parent terms
    $this->setParentTerms($vocabulary->identifier, $vocabulary->terms);
  }

  /**
   * Update all translations (name and description) for a given term
   *
   * @param int $tid term ID (LOCAL database)
   * @param object $term term object which is supplied by the ontology service
   */
  protected function updateTranslation($tid, $term) {
    $lang_code_trans = array(
      'fr' => 'fr',
      'it' => 'it',
      'rm' => 'rm',
      'en' => 'en',
      'de' => 'de',
    );

    $fields = array(
      'name',
      'description',
    );

    foreach ($fields as $field) {
      $locales_source_exists = TRUE;

      $context = 'term:' . $tid . ':' . $field;

      $query = "SELECT lid FROM {locales_source} WHERE location = :location";
      $lid = db_query(
        $query, array(
          ':location' => 'taxonomy:' . $context,
        )
      )->fetchField();

      // FIRST STEP: Update the translation source table (locales_source)
      // If german translation is supplied, update translation source
      if (drupal_strlen($term->$field->$lang_code_trans[i18n_string_source_language()]) > 0) {
        $source            = new stdClass();
        $source->location  = 'taxonomy:' . $context;
        $source->source    = $term->$field->$lang_code_trans[i18n_string_source_language()];
        $source->context   = $context;
        $source->textgroup = 'taxonomy';

        if ($lid != FALSE) {
          $source->lid = $lid;
          drupal_write_record('locales_source', $source, 'lid');
        }
        else {
          $locales_source_exists = FALSE;
        }
      }
      else {
        // If NO german translation is supplied
        // should not occur on field name but description !),
        // remove translation source and target
        db_delete('locales_source')->condition('lid', $lid)->execute();

        $locales_source_exists = FALSE;
      }

      // SECOND STEP: Update translation target table (locales_target)

      foreach ($lang_code_trans as $iso_639_1 => $iso_639_2) {
        if (drupal_strlen($term->$field->$iso_639_2) > 0 && $locales_source_exists) {
          $count = db_query(
            'SELECT count(*) FROM {locales_target} WHERE language = :language AND lid = :lid',
            array(
              ':language' => $iso_639_1,
              ':lid' => $lid,
            )
          )->fetchField();

          $target              = new stdClass();
          $target->language    = $iso_639_1;
          $target->translation = $term->$field->$iso_639_2;
          $target->plid        = 0;
          $target->plural      = 0;
          $target->lid         = $lid;

          if ($count == 1) {
            drupal_write_record(
              'locales_target', $target, array(
                'language',
                'lid',
                'plural',
              )
            );
          }
          else {
            drupal_write_record(
              'locales_target', $target
            );
          }
        }
        else {
          db_delete('locales_target')
            ->condition('language', $iso_639_1)
            ->condition('lid', $lid)
            ->execute();
        }
      }
    }
  }

  /**
   * Removes translation source and target from LOCAL database.
   *
   * @param int $tid
   */
  protected function removeTranslation($tid) {
    foreach (array('name', 'description') as $field) {
      // Remove translations for term name
      $location = 'taxonomy:term:' . $tid . ':' . $field;
      $lid = db_query(
        'SELECT lid FROM {locales_source} WHERE location = :location',
        array(
          ':location' => $location,
        )
      )->fetchField();

      db_delete('locales_source')->condition('location', $location)->execute();

      db_delete('locales_target')->condition('lid', $lid)->execute();
    }
  }

  /**
   * Insert new term to database.
   *
   * @param object $term term
   *  object which is supplied from ontology service
   * @param string $vocabulary_identifier
   */
  protected function insertTerm($term, $vocabulary_identifier) {
    $obj_term = new stdClass();

    $this->setTermAttributes($obj_term, $term);

    $obj_term->vid = $this->m_vocabulary_vids[$vocabulary_identifier];
    $obj_term->i18n_tsid = 0;
    $obj_term->field_taxon_key['und'][0]['value'] = $term->identifier;
    $obj_term->field_taxon_key['und'][0]['safe_value'] = $term->identifier;
    $obj_term->field_taxon_key['und'][0]['format'] = '';

    $obj_term->field_taxon_url['und'][0]['value'] = $term->url;
    $obj_term->field_taxon_url['und'][0]['safe_value'] = $term->url;
    $obj_term->field_taxon_url['und'][0]['format'] = '';

    taxonomy_term_save($obj_term);

    return $this->getTidByKey($vocabulary_identifier, $term->identifier);
  }

  /**
   * Checks each term of a vocab if it has a parent term.
   * In case term has parent term(s) sets tids of parent terms and update term.
   *
   * @param string $vocab_identifier
   * @param object $terms terms
   *  of vocabulary supplied by ontology service
   */
  protected function setParentTerms($vocab_identifier, $terms) {
    $local_vocab = $this->loadLocalVocabulary($vocab_identifier);
    $key_tid_map = $this->getKeyTidMap($vocab_identifier);

    foreach ($local_vocab as $term) {
      $key = $term->field_taxon_key['und'][0]['value'];
      if (isset($terms->$key)) {
        if (isset($terms->$key->parents)) {
          $tids = array();
          foreach ($terms->$key->parents as $parent) {
            if (isset($key_tid_map[$parent])) {
              $tids[] = $key_tid_map[$parent];
            }
          }
          if (count($tids) == 0) {
            $tids = 0;
          }
          $term->parent = $tids;
        } else {
	  $term->parent = 0;
	}
      }
      else {
        $term->parent = 0;
      }

                //if( $vocab_identifier == 'educa_school_subjects' && $key == 'sport' ) {
                  //      print_r( $term );
                //}

      taxonomy_term_save($term);
    }
  }

  /**
   * Reads the Term ID from LOCAL DB for a given term identifier.
   *
   * @param srting $vocabulary_identifier
   * @param string $term_identifier
   *
   * @return int
   */
  protected function getTidByKey($vocabulary_identifier, $term_identifier) {
    $terms = $this->loadLocalVocabulary($vocabulary_identifier);
    foreach ($terms as $term) {
      if ($term->field_taxon_key['und'][0]['value'] == $term_identifier) {
        return $term->tid;
      }
    }

    return 0;
  }

  /**
   * Creates a map (array) which maps the tid and key (identifiers)
   * of all terms in a vocabulary.
   *
   * @param string $vocabulary_identifier
   *
   * @return array
   */
  protected function getKeyTidMap($vocabulary_identifier) {
    $terms = $this->loadLocalVocabulary($vocabulary_identifier);
    $map = array();
    foreach ($terms as $term) {
      $map[$term->field_taxon_key['und'][0]['value']] = $term->tid;
    }

    return $map;
  }

  /**
   * This function takes the term attributes provided by ontology server
   * ($os_term_object)
   * and sets it to the drupal term object($drupal_term_object)
   *
   * @param object $drupal_term_obj by reference
   * @param object $os_term_obj by object
   */
  protected function setTermAttributes(&$drupal_term_obj, $os_term_obj) {
    switch (i18n_string_source_language()) {
      case 'fr':
        $drupal_term_obj->name = $os_term_obj->name->fr;
        $drupal_term_obj->description = $os_term_obj->description->fr;
        break;

      case 'it':
        $drupal_term_obj->name = $os_term_obj->name->it;
        $drupal_term_obj->description = $os_term_obj->description->it;
        break;

      case 'rm':
        $drupal_term_obj->name = $os_term_obj->name->rm;
        $drupal_term_obj->description = $os_term_obj->description->rm;
        break;

      case 'en':
        $drupal_term_obj->name = $os_term_obj->name->en;
        $drupal_term_obj->description = $os_term_obj->description->en;
        break;

      case 'de':
      default:
        $drupal_term_obj->name = $os_term_obj->name->de;
        $drupal_term_obj->description = $os_term_obj->description->de;
        break;
    }

    $drupal_term_obj->format = 'plain_text';
    $drupal_term_obj->weight = $os_term_obj->order;

    $drupal_term_obj->language = 'und';

    $drupal_term_obj->field_taxon_context['und'][0]['value'] = $os_term_obj->context;
    $drupal_term_obj->field_taxon_context['und'][0]['safe_value'] = $os_term_obj->context;
    $drupal_term_obj->field_taxon_context['und'][0]['format'] = '';

    $drupal_term_obj->field_taxon_deprecated['und'][0]['value'] = $os_term_obj->deprecated;
  }

  /**
   *
   * This function makes a HTTP-Request to the Ontology Server to receive the
   * taxonomy data as json string.
   * The URL of Ontology Server URL is gained from config variable
   *  archibald_ontology_url.
   *
   * @param string $key the ontology identifier (key), default: lom-v1.0
   *
   * @return string taxonomy data as json
   */
  public function loadDataFromOntologyServer($key = 'lom-v1') {
    $url = variable_get('archibald_ontology_url');

    if (empty($url)) {
      return NULL;
    }

    $path = $url . '/json/' . $key;
    $data = file_get_contents($path);
    return json_decode($data);
  }
}

