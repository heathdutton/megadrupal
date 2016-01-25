<?php

namespace Drupal\maps_import\Mapping\Library;

use Drupal\maps_import\Profile\Profile;
use Drupal\maps_import\Operation\Operation;
use Drupal\maps_suite\Log\Logger;
use Drupal\maps_suite\Log\Context\Context;
use Drupal\maps_import\Mapping\Target\Drupal\TaxonomyTerm;

class Library extends Operation {

	/**
	 * @inheritdoc
	 */
	public function __construct(Profile $profile) {
		$this->profile = $profile;
	}

  /**
   * @inheritdoc
   */
  public function batchOperations() {
    $operations = array();
    $total = $this->getTotal();

    $maxEntities = $this->profile->getMaxObjectsPerOp();
$maxEntities = 1;
    $count = ceil($total / $maxEntities);

    for ($i = 0; $i < $count; $i++) {
      $operations[] = array(
        // Callback function
        array($this, 'process'),
        // Callback arguments
        array(
          $i * $maxEntities,
          $maxEntities,
        )
      );
    }

    return $operations;
  }

  /**
   * Get the total number of MaPS libraries for this profile.
   *
   * @return int
   *   The total numbver of libraries.
   */
	public function getTotal() {
    return db_query('SELECT COUNT(*) FROM {maps_import_library_map} WHERE pid = :pid ', array(':pid' => $this->getProfile()->getPid()))->fetchField();
  }

	/**
   * @inheritdoc
   */
	protected function processRun($start = 0, $range = 1000, $last = FALSE){
    if ($start == 0) {
      $this->context['results']['count_total'] = $this->getTotal();
      $this->context['results']['count_processed'] = 0;
    }

    // Message displayed under the progressbar.
    $this->context['message'] = t('Mapping libraries for profile "@title" (@current/@total)', array(
      '@title' => $this->getProfile()->getTitle(),
      '@current' => ($this->context['results']['count_processed'] + 1),
      '@total' => $this->context['results']['count_total'],
    ));

    Logger::getLog($this->getType())
      ->setCurrentOperation($start)
      ->setTotalOperations($this->getTotal());

    $currentLibraries = $this->getCurrentLibraries($start, $range);
    if ($this->isBatch()) {
      $this->context['results']['count_processed'] += count($currentLibraries);
    }

    // Get all libraries for the current profile.
    foreach ($currentLibraries as $vid => $library) {
      $vocabulary = taxonomy_vocabulary_load($vid);
    	Logger::getLog($this->getType())
        ->moveToContentRoot()
        ->addContext(new Context('library', array('id' => $vocabulary->name)), 'child');

      $this->createTaxonomyTerm($vid, $this->getLibraryValues($library['id_attribute']));
    	Logger::getLog($this->getType())->incrementCurrentOperation();
    }

    Logger::getLog($this->getType())->save();

    return TRUE;
  }

  /**
   * Delete all mapped libraries and the related taxonomy terms.
   */
  protected function deleteMappedLibraries() {
  	$request = db_select('maps_import_library_index', 'li')
  	  ->fields('li')
  	  ->condition('pid', $this->profile->getPid())
  	  ->execute();

  	while ($term = $request->fetchAssoc()) {
  		taxonomy_term_delete($term['tid']);
  	}

  	db_delete('maps_import_library_index')
  	  ->condition('pid', $this->profile->getPid())
  	  ->execute();
  }

  /**
   * Save the library index in {maps_import_library_index} table.
   *
   * @param $id_attribute
   *   The attribute ID.
   * @param $id
   *   The MaPS System® ID.
   * @param $id_language
   *   The language ID.
   * @param $tid
   *   The taxonomy term ID.
   */
  protected function saveLibraryIndex($id_attribute, $id, $id_language, $tid) {
    $record = array(
      'pid' => $this->getProfile()->getPid(),
      'id_attribute' => $id_attribute,
      'id' => $id,
      'id_language' => $id_language,
      'tid' => $tid,
    );

    $mappedTerm = $this->getMappedTerm($id);
    $pk = $mappedTerm ? 'tid' : array();

    drupal_write_record('maps_import_library_index', $record, $pk);
  }

  protected function getMappedTerm($id) {
  	$libraryIndex = db_select('maps_import_library_index')
  	  ->fields('maps_import_library_index')
      ->condition('pid', $this->getProfile()->getPid())
      ->condition('id', $id)
  	  ->execute()
  	  ->fetchAssoc('id', \PDO::FETCH_ASSOC);

  	return !empty($libraryIndex['tid']) ? $libraryIndex['tid'] : NULL;
  }

  /**
   * Create a taxonomy term from libraries.
   *
   * @param $vid
   *   The vocabulary id.
   * @param $libraries
   *   The array of libraries.
   *
   * @todo Clean this function
   */
  protected function createTaxonomyTerm($vid, $libraries) {
    $vocabulary = taxonomy_vocabulary_load($vid);

    foreach ($libraries as $values) {
      if (empty($values[$this->profile->getDefaultLanguage()])) {
        continue;
      }

      Logger::getLog($this->getType())
        ->addContext(new Context('values'), 'child');

      foreach ($values as $id => $value) {
        Logger::getLog($this->getType())
          ->addContext(new Context('value', array('id' => $id)), 'child')
          ->addMessage($value['value'])
          ->moveToParent();
      }

      Logger::getLog($this->getType())->moveToParent();

      // Create the taxonomy term entity.
      $value = $values[$this->profile->getDefaultLanguage()];

      $mappedTerm = $this->getMappedTerm($value['id']);
      if(!is_null($mappedTerm)) {
        $term = taxonomy_term_load($mappedTerm);
      }
      else{
        $term = entity_create('taxonomy_term', array(
          'vid' => $vid,
          'name' => $value['value'],
          'language' => $this->profile->getLangcode($this->profile->getDefaultLanguage()),
          'parent' => 0,
          'vocabulary_machine_name' => $vocabulary->machine_name,
        ));
      }

      $wrapper = entity_metadata_wrapper('taxonomy_term', $term);
      $wrapper_language = $this->profile->getLangcode($this->profile->getDefaultLanguage());
      $wrapper->language($wrapper_language);
      $wrapper->name->set($value['value']);

      if (!isset($wrapper->name_field)) {
        $wrapper->save();
        $this->saveLibraryIndex($value['id_attribute'], $value['id'], $this->profile->getDefaultLanguage(), $term->tid);
        continue;
  	  }

  	  // Entity translation.
      if (module_exists('entity_translation') && entity_translation_enabled('taxonomy_term', $vocabulary->machine_name)) {
      	foreach ($values as $idLanguage => $value) {
      	  if (!$langcode = $this->profile->getLangcode($idLanguage)) {
            continue;
          }

          $wrapper->language($langcode)->name_field->set($value['value']);
        }
      }

      module_invoke_all('maps_import_entity_presave', $wrapper, 'taxonomy_term', $vocabulary->machine_name);

      $entity = $wrapper->value();
      if ($handler = entity_translation_get_handler('taxonomy_term', $entity)) {
      	$info = entity_get_info('taxonomy_term');

        foreach ($values as $language_id => $value) {
          $langcode = $this->profile->getLangcode($language_id);
          if (!$langcode) {
          	continue;
          }

          $translation = array(
            'translate' => 0,
            'status' => 1,
            'language' => $langcode,
          );

          if ($this->profile->getLangcode($language_id) == $wrapper_language) {
            $handler->setOriginalLanguage($wrapper_language);
          }
          else {
            $translation['source'] = $wrapper_language;
          }

          if (!empty($entity->uid)) {
            $translation['uid'] = $entity->uid;
          }
          $handler->setTranslation($translation);
        }

        $translationsKey = isset($info['entity keys']['translations']) ? $info['entity keys']['translations'] : NULL;
        $entity->{$translationsKey} = $handler->getTranslations();
      }

      $wrapper->save();

      $this->saveLibraryIndex($value['id_attribute'], $value['id'], $this->profile->getDefaultLanguage(), $term->tid);
  	}
  }

  /**
   * Get the library values for a given attribute.
   *
   * @param $id_attribute
   *   The library id attribute.
   *
   * @return array
   *   The library values.
   *   Values as stored in an array like array[code][id_language][value].
   */
  public function getLibraryValues($id_attribute) {
    $result = db_select('maps_import_libraries', 'l')
      ->fields('l')
      ->condition('id_attribute', $id_attribute)
      ->execute();

    $values = array();
    while ($row = $result->fetchAssoc()) {
    	$values[$row['code']][$row['id_language']] = $row;
    }

    return $values;
  }

  /**
   * Return the current libraries to process.
   *
   *  @param $start
   *    The index to start.
   *  @param $maxLibraries
   *    The max limit.
   *
   *  @return array
   *    The libraries to process.
   */
  protected function getCurrentLibraries($start = 0, $maxLibraries) {
    $query =  db_select('maps_import_library_map', 'lm')
      ->fields('lm')
      ->condition('pid', $this->profile->getPid());

    if ($maxLibraries > 0) {
    	$query->range($start, $maxLibraries);
    }

    return $query->execute()
      ->fetchAllAssoc('vocabulary', \PDO::FETCH_ASSOC);
  }

  /**
   * @inheritdoc
   */
  public function getTitle(){
    return t('Library mapping');
  }

  /**
   * @inheritdoc
   */
  public function getDescription(){
    return t('Mapping between MaPS System® libraries values and Drupal taxonomy terms.');
  }

  /**
   * @inheritdoc
   */
  public function getType(){
    return 'library_mapping';
  }

  /**
   * Delete all the taxonomy terms generated for the
   * given profile.
   *
   * @param Profile $profile
   *   The related profile.
   */
  public static function deleteTerms(Profile $profile) {
    $result = db_select('maps_import_library_index', 'library')
      ->fields('library')
      ->condition('pid', $profile->getPid())
      ->execute()
      ->fetchAllAssoc('tid', \PDO::FETCH_ASSOC);

    foreach ($result as $tid => $library_item) {
      taxonomy_term_delete($tid);
    }
  }

  /**
   * Delete all the taxonomy vocabularies mapped
   * with MaPS System libraries.
   */
  public static function deleteVocabularies(Profile $profile) {
    $result = db_select('maps_import_library_map', 'library')
      ->fields('library')
      ->condition('pid', $profile->getPid())
      ->execute()
      ->fetchAllAssoc('vocabulary', \PDO::FETCH_ASSOC);

    foreach ($result as $vid => $library_map) {
      taxonomy_vocabulary_delete($vid);
    }
  }

}
