<?php

/**
 * @file
 * abstract handler class and interfaces for curricula
 *
 * there are two kinds of curriculum
 * basic: will be stored in ArchibaldLom 9. classification
 * advanced: will be stored in ArchibaldLom 10. curriculum
 *
 * cause 10. is in inidivual solution for ArchibaldLom-CH cause of
 * high complexity of some curricula
 * to have some international interoperability,
 * we will store curricula with are simple enough in 9.
 */

/**
 * interface for curricula width type: basic
 */
interface ArchibaldInterfaceCurriculumBasic extends ArchibaldInterfaceCurriculumGeneral {

  /**
   * set current classification object
   *
   * @param array $lom_classification
   *   list of ArchibaldLomDataClassification
   */
  public function setLomClassification(array &$lom_classification);

  /**
   * figure out if the given classification is one for the current curriculum
   * if not it will return a empty object
   *
   * @param string $source
   * @param string $purpose
   * @param array $taxons
   *
   * @return object
   *   key=>md5,
   *   val=>human_reable_string
   */
  public function getEntry($source, $purpose, array $taxons);

  /**
   * return a array of array(md5_uniquy_ident=>human_readable_string, ....)
   * for display within form
   *
   * @return array
   */
  public function getEntrys();
}

/**
 * abstract class for curricula with type: advanced
 */
interface ArchibaldInterfaceCurriculumAdvanced extends ArchibaldInterfaceCurriculumGeneral {

  /**
   * return a array of array(md5_uniquy_ident=>human_readable_string, ....)
   * for display within form
   *
   * @return array
   */
  public function getEntrys();

  /**
   * set current curriculum object
   *
   * @param array $lom_curriculum
   *   list of ArchibaldLomDataCurriculum
   */
  public function setLomCurriculum(array &$lom_curriculum);
}

/**
 * interface for curricula width type: basic or advanced
 */
interface ArchibaldInterfaceCurriculumGeneral {
  /**
   * get the add form
   *
   * @param $form_state
   *
   * @return array
   *   drupal form object
   */
  public function getAddForm(&$form_state);

  /**
   * form submit handler to add new items
   *
   * @param array $form_state
   */
  public function submitAddForm(&$form_state);

  /**
   * get settings configuration form
   *
   * @return array
   *   drupal form
   */
  public function getSettingsForm();

  /**
   * validate configuration form
   *
   * @param array $form_values
   *   The list of values that was choosen by user
   *
   * @param array $form_state
   *   The complete fomr_state called by reference
   */
  public function validateSettingsForm($form_values, &$form_state);

  /**
   * submit configuration form
   *
   * @param array $form_values
   *   The list of values that was choosen by user
   *
   * @param array $form_state
   *   The complete fomr_state called by reference
   */
  public function submitSettingsForm($form_values, &$form_state);

  /**
   * return TRUE if this curriculum is active
   * @return boolean
   */
  public function isActive();

  /**
   * returns a array of 5.6 educational.context entetys where it is valid for
   * or array('all') when it is for all of them
   *
   * @return array
   */
  public function isValidFor();

  /**
   * return human readable name of this curriculum
   *
   * @return string
   */
  public function getName();

  /**
   * remove one item
   */
  public function remove($key);

}

/**
 * abstract class for curricula with type: basic
 */
abstract class ArchibaldCurriculumBasic extends ArchibaldCurriculumGeneral {

  /**
   * lom classifications
   * @var array
   *   list of ArchibaldLomDataClassification
   */
  protected $lomClassification = array();

  /**
   * set current classification object
   *
   * @param array $lom_classification
   *   list of ArchibaldLomDataClassification
   */
  public function setLomClassification(array &$lom_classification) {
    $this->lomClassification = &$lom_classification;

    $this->renderEntrys();
  }

  /**
   * get current classification object
   *
   * @return array
   *   list of ArchibaldLomDataClassification
   */
  public function getLomClassification() {
    return $this->lomClassification;
  }

  /**
   * return a array of array(
   *   md5_uniquy_ident=> stdClass(
   *    key->md5_uniquy_ident,
   *    val->human_readable_string,
   *    educational_level->array_of_strings,
   *    discipline->array_of_strings
   *   ),
   *   ....
   * )
   *
   * for rendering list in own format
   *
   * @return array
   */
  public function getEntryObjects() {
    $entrys = array();
    foreach ($this->walkTroughClassifcations() as $data) {
      $entry = $this->getEntry($data->source, $data->purpose, $data->taxons);
      if (!empty($entry->key)) {
        $entrys[] = $entry;
      }
    }
    return $entrys;
  }

  /**
   * return a array of array(md5_uniquy_ident=>human_readable_string, ....)
   * for display within form
   *
   * @return boolean
   *   Status of process, FALSE in case of error.
   */
  public function renderEntrys($key_to_delete = NULL) {
    $this->entrys = array();

    if (empty($this->lomClassification) || !is_array($this->lomClassification)) {
      return FALSE;
    }

    foreach ($this->walkTroughClassifcations() as $data) {
      $entry = $this->getEntry($data->source, $data->purpose, $data->taxons);
      if (!empty($entry->key)) {
        $this->entrys[$entry->key] = $entry;
      }
    }

    return TRUE;
  }

  /**
   * walk throug current classifications and try to clean it up
   * and return it in a simplyfied structur
   * @callback a callback funtion with will called
   * for each entry with the same paramters as the return of this function
   *
   * the callback funcktion can return a command string
   *  ignore  ignore this object in return
   *  delete  like ignore but delete it also forever
   *          from classification object
   *
   * @return array
   *  []=>(object)
   *    source->string,
   *    purpose->string,
   *    taxons->ArchibaldLomDataTaxonPath)
   */
  protected function walkTroughClassifcations($callback = NULL) {
    $data = array();
    foreach ($this->lomClassification as $classification_id => $classification) {

      if ($classification instanceof ArchibaldLomDataClassification) {
        $purpose = $classification->getPurpose()->getValue();
        $taxon_pathes = $classification->getTaxonPath();
        foreach ($taxon_pathes as $taxon_path_id => $taxon_path) {
          if ($taxon_path instanceof ArchibaldLomDataTaxonPath) {
            $source = $taxon_path->getSource()->getStrings();
            // get first element
            $source = reset($source);
            $taxons = $taxon_path->getTaxon();

            if (!empty($source) && !empty($purpose) && !empty($taxons)) {

              $add_to_array = TRUE;
              if (!empty($callback)) {
                if (!is_array($taxons)) {
                  print_r($this->lomClassification);
                  die();
                }

                $action = call_user_func($callback, $source, $purpose, $taxons);
                switch ($action) {
                  case 'ignore':
                    $add_to_array = FALSE;
                    break;

                  case 'delete':
                    unset($taxon_pathes[$taxon_path_id]);
                    if (count($taxon_pathes) < 1) {
                      // if this was the last taxonPath
                      // within this classification
                      unset($this->lomClassification[$classification_id]);
                    }
                    else {
                      $this->lomClassification[$classification_id]->setTaxonPath($taxon_pathes);
                    }
                    $add_to_array = FALSE;
                    break;
                }
              }
              if ($add_to_array == TRUE) {
                $data[] = (object)array(
                  'source' => $source,
                  'purpose' => $purpose,
                  'taxons' => $taxons,
                );
              }
            }
          }
        }
      }
    }

    return $data;
  }

  /**
   * add new ArchibaldLomDataClassification to lom_classification array
   *
   * @param string $source
   * @param string $purpose
   * @param array $taxonpath
   *  [](object)
   *    'entry'->string,
   *    'id'->string
   *
   * @param string $language
   *   default en to char for langstring object
   */
  protected function addTaxonpath($source, $purpose, $taxonpath, $lang = '') {

    $taxonpath_obj = new ArchibaldLomDataTaxonPath();
    $taxonpath_obj->setSource($this->buildLangstring($source, 'en'));

    foreach ($taxonpath as $taxon) {
      if (empty($taxon->entry)) {
        continue;
      }

      $taxon_obj = new ArchibaldLomDataTaxon();
      $taxon_obj->setEntry($this->buildLangstring($taxon->entry, $lang));

      if (!empty($taxon->id)) {
        $taxon_obj->setId($taxon->id);
      }

      $taxonpath_obj->addTaxon($taxon_obj);
    }

    $classification = new ArchibaldLomDataClassification();
    $classification->setPurpose(new ArchibaldLomDataVocabulary($purpose));
    $classification->addTaxonPath($taxonpath_obj);

    // check if we try to ad it twice
    $dont_add = FALSE;
    $ident = md5(json_encode($classification));
    foreach ($this->lomClassification as $tmp_classifcation) {
      if ($ident == md5(json_encode($tmp_classifcation))) {
        $dont_add = TRUE;
      }
    }

    if ($dont_add == FALSE) {
      $this->lomClassification[] = $classification;
    }

    $entry = $this->getEntry($source, $purpose, $taxonpath_obj->getTaxon());

    if (!empty($entry->key)) {
      $this->entrys[$entry->key] = $entry;
    }
  }

  /**
   * add new ArchibaldLomDataClassification to lomClassification array
   *
   * @param string $source
   * @param string $purpose
   * @param array $taxonpath
   *  [](object)
   *    'entry'->string,
   *    'id'->string
   * @param string $language
   *  default en to char for langstring object
   */
  protected function delTaxonpath($source, $purpose, $taxonpath, $lang = 'en') {
    $taxonpath_obj = new ArchibaldLomDataTaxonPath();
    $taxonpath_obj->setSource(new ArchibaldLomDataLangString($source, $lang));

    foreach ($taxonpath as $taxon) {
      if (empty($taxon->entry)) {
        continue;
      }

      $taxon_obj = new ArchibaldLomDataTaxon();
      $taxon_obj->setEntry(new ArchibaldLomDataLangString($taxon->entry, $lang));

      if (!empty($taxon->id)) {
        $taxon_obj->setId($taxon->id);
      }

      $taxonpath_obj->addTaxon($taxon_obj);
    }

    $classification = new ArchibaldLomDataClassification();
    $classification->setPurpose(new ArchibaldLomDataVocabulary($purpose));
    $classification->addTaxonPath($taxonpath_obj);

    $this->lomClassification[] = $classification;

    $entry = $this->getEntry($source, $purpose, $taxonpath_obj->getTaxon());

    if (!empty($entry->key)) {
      $this->entrys[$entry->key] = $entry;
    }
  }
}

/**
 * interface for curricula width type: advanced
 */
abstract class ArchibaldCurriculumAdvanced extends ArchibaldCurriculumGeneral {

  /**
   * lom curriculum
   * @var array of ArchibaldLomDataCurriculum
   */
  protected $lomCurriculum = array();

  /**
   * @var stdClass
   */
  protected $entrys = NULL;

  /**
   * source within ArchibaldLomDATA_currculum
   * @var string
   */
  protected $curriculumIdentKey = '';

  public function __construct() {
    $this->entrys = new stdClass();
  }

  /**
   * set current curriculum object
   *
   * @param array $lom_curriculum
   *   list of ArchibaldLomDataCurriculum
   */
  public function setLomCurriculum(array &$lom_curriculum) {
    $this->lomCurriculum = &$lom_curriculum;

    $this->renderEntrys();
  }

  /**
   * extract entry from ArchibaldLomDataCurriculum
   *
   * @return boolean
   */
  public function renderEntrys($key_to_delete = NULL) {
    if (empty($this->lomCurriculum) || !is_array($this->lomCurriculum)) {
      return FALSE;
    }

    foreach ($this->lomCurriculum as $curriculum_id => $curriculum) {
      if ($curriculum instanceof ArchibaldLomDataCurriculum) {
        if ($curriculum->getSource() == $this->curriculumIdentKey) {
          $this->entrys = json_decode($curriculum->getEntity());
          return TRUE;
        }
      }
    }

    return FALSE;
  }

  /**
   * get current curriculum object
   *
   * @return array $lom_curriculum
   *   list of ArchibaldLomDataCurriculum
   */
  public function getLomCurriculum() {
    $hit = FALSE;
    foreach ($this->lomCurriculum as $curriculum_id => $curriculum) {
      if ($curriculum instanceof ArchibaldLomDataCurriculum) {
        if ($curriculum->getSource() == $this->curriculumIdentKey) {
          $curriculum->setEntity(json_encode($this->entrys));
          $hit = TRUE;
          break;
        }
      }
    }

    if ($hit == FALSE) {
      $curriculum = new ArchibaldLomDataCurriculum();
      $curriculum->setSource($this->curriculumIdentKey);
      $curriculum->setEntity(json_encode($this->entrys));
      $this->lomCurriculum[] = $curriculum;
    }

    return $this->lomCurriculum;
  }

  /**
   * index current curriculum into solr index
   *
   * @param ApacheSolrDocument $document
   */
  public function solrIndex(ApacheSolrDocument &$document) {}
}

/**
 * genreal abstract class with methodes shared by both curricula types
 */
abstract class ArchibaldCurriculumGeneral {

  /**
   * a array of array(md5_uniquy_ident=>human_readable_string, ....)
   * @var array
   */
  protected $entrys = array();

  /**
   * session ident string for direct session access
   * @var string
   */
  public $sessionIdent = '';

  /**
   * return a array of array(md5_uniquy_ident=>human_readable_string, ....)
   * for display within form
   *
   * @return array
   */
  public function getEntrys() {
    return $this->entrys;
  }

  /**
   * returns a array of 5.6 educational.context entetys where it is valid for
   * or array('all') when it is for all of them
   *
   * @return array
   */
  public function isValidFor() {
    return array('all');
  }

  /**
   * downlad a matrix csv and convert it to a recursiv array
   * header of csv needs to be the first row
   *
   * @param string $url
   * @param integer $first_data_row
   *   row number where the border between schema and data is
   *
   * @return array
   */
  protected function csvUrl2recursivArray($url, $first_data_row = 3) {
    $data    = array();
    $headers = array();
    $i       = 0;
    foreach (file($url) as $row) {
      $i++;
      $row = mb_convert_encoding($row, 'UTF-8', array('WINDOWS-1252', 'UTF-8', 'ISO-8859-1'));

      # str_getcsv kills some french letters dont know why
      # $row=str_getcsv(trim($row), ';', '###', '####');
      $row = explode(';', trim($row));

      if ($i == 1) {
        $headers = $row;
      }
      else {
        $tmp_data = &$data;
        for ($x = 0; $x < $first_data_row; $x++) {
          $tmp_data = &$tmp_data[$row[$x]];
        }

        for ($x = $first_data_row; $x < count($row); $x++) {
          if (!empty($row[$x]) && drupal_strtolower($row[$x]) != 'x') {
            $tmp_data = addslashes($row[$x]);
          }
        }
      }
    }

    for ($x = $first_data_row; $x < count($row); $x++) {
      $data[$headers[$x]] = $data[$x];
      unset($data[$x]);
    }

    return $data;
  }

  /**
   * generate a langstring
   * @example
   * buildLangstring('Wort, 'de')
   *
   * buildLangstring(array('de'=>'Wort', 'en'=>'Word'))
   *
   * @param mixed $string
   *   string/array
   * @param string $lang
   *
   * @return ArchibaldLomDataLangString
   */
  protected function buildLangstring($string, $lang = '') {
    if (is_string($string) && !empty($lang)) {
      return new ArchibaldLomDataLangString($string, $lang);
    }
    elseif (is_array($string)) {
      $ls = new ArchibaldLomDataLangString();
      foreach ($string as $lang => $s) {
        $ls->setString($s, $lang);
      }
      return $ls;
    }

    return FALSE;
  }

  /**
   * Search all objects values for given $key which are in the given $array
   * and check if the given $value matches the object value
   *
   * @param string $key
   *   sub element kex to match in
   * @param string $value
   *   sub element value to match
   * @param array $array
   *   array to search in
   *
   * @return int
   *   returns the matched array index or if not found FALSE
   */
  protected function arraySearch($key, $value, $array) {
    foreach ($array as $k => $v) {
      if ($v->$key === $value) {
        return $k;
      }
    }
    return FALSE;
  }

  /**
   * get settings configuration form
   *
   * @return array
   *   drupal form
   */
  public function getSettingsForm() {
    $form = array();

    $form['activate'] = array(
      '#type' => 'radios',
      '#options' => array(
        '1' => t('Curriculum activated'),
        '' => t('Curriculum disabled'),
      ),
      '#default_value' => variable_get('archibald_curriculum_' . $this->curriculumIdentKey . '_active', ''),
    );

    return $form;
  }
  /**
   * validate configuration form
   *
   * @param array $form_values
   *   The list of values that was choosen by user
   *
   * @param array $form_state
   *   The complete fomr_state called by reference
   */
  public function validateSettingsForm($form_values, &$form_state) {
  }

  /**
   * submit configuration form
   *
   * @param array $form_values
   *   The list of values that was choosen by user
   *
   * @param array $form_state
   *   The complete fomr_state called by reference
   */
  public function submitSettingsForm($form_values, &$form_state) {
    variable_set('archibald_curriculum_' . $this->curriculumIdentKey . '_active', $form_values['activate']);
  }

  /**
   * return TRUE if this curriculum is active
   * @return boolean
   */
  public function isActive() {
    return (boolean)variable_get('archibald_curriculum_' . $this->curriculumIdentKey . '_active', '');
  }
}

// if (PHP 5 < 5.3.0)
if (!function_exists('str_getcsv')) {

  /**
   * Parses a string input for fields in CSV format
   * and returns an array containing the fields read.
   *
   * @param string $input
   *   The string to parse.
   * @param string $delimiter
   *   Set the field delimiter (one character only).
   * @param string $enclosure
   *   Set the field enclosure character (one character only).
   * @param string $escape
   *   Set the escape character (one character only).
   *   Defaults as a backslash (\)
   *
   * @return array
   *   Returns an indexed array containing the fields read.
   */
  function str_getcsv($input, $delimiter = ',', $enclosure = '"', $escape    = NULL) {
    $five_mbs = 5 * 1024 * 1024;
    $fp = fopen('php://temp/maxmemory:' . $five_mbs, 'r+');
    fputs($fp, $input);
    rewind($fp);
    //  $escape only got added in 5.3.0
    $data = fgetcsv($fp, 1000, $delimiter, $enclosure);
    fclose($fp);
    return $data;
  }
}

