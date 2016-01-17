<?php

/**
 * @file
 * handler class for per curriculum
 */

/**
 * in ArchibaldLom-CH all basic curriculums will stored in 9. Classification
 * all advanced curriculums will stored in 10. Curriculum
 *
 * Type: basic
 *
 * Example Data:
 *
 * Schoolgrade:  (compulsory education|cycle_3|9th_year)
 *   Compulsory education
 *    3rd cycle (9th to 11th school year)
 *     9th year
 *
 * Discipline:
 *  (mathematics and natural science|mathematics|applied mathematics)
 *
 *   Mathematics and Natural Science
 *    Mathematics
 *     Applied mathematics
 *
 * will be stored in 2 Classifcation tree`s,
 * first for Schoolgrade and the second for Discipline
 *
 * 1.)
 * ArchibaldLomDataClassification(
 *   purpose => "educational level"
 *   taxon path => array(
 *      0 => ArchibaldLomDataTaxonPath(
 *             source => "educa" // name of this currculum
 *             taxon => array(
 *                0 => ArchibaldLomDataTaxon(
 *                         // most the lower case english term.
 *                         // Its defined in ontology serivce
 *                         // further informations below
 *
 *                         id    => "compulsory education",
 *                         entry => ArchibaldLomDataLangString(
 *                            de => "Obligatorische Schule",
 *                            fr => "École obligatoire",
 *                            it => "Scuola obligatoria",
 *                            rm => "Scola obligatorica",
 *                            en => "Compulsory education"
 *                            )
 *                       )
 *                1 => ArchibaldLomDataTaxon(
 *                         id    => "3rd cycle",
 *                         entry => ArchibaldLomDataLangString(
 *                            de => "3. Zyklus (9. bis 11. Schuljahr)",
 *                            fr => "3e cycle (9ème à 11ème année scolaire)",
 *                            it => "3° ciclo",
 *                            rm => "Terz ciclus",
 *                            en => "3rd cycle (9th to 11th school year)"
 *                            )
 *                       )
 *                2 => ArchibaldLomDataTaxon(
 *                         id    => "9th_year",
 *                         entry => ArchibaldLomDataLangString(
 *                            de => "9. Schuljahr",
 *                            fr => "9e année",
 *                            it => "9° anno scolastico",
 *                            rm => "9. classa",
 *                            en => "9th year"
 *                            )
 *                       )
 *               )
 *          )
 *   )
 * )
 *
 * 2.)
 * Now we have a complete path to school grade "9th year".
 * If we store only 1 path and 1 discipline all works well.
 * Problems begins if we have 2 or more school grades and discipline.
 * We can not determine which discipline is linked to which school grade.
 * For Example
 *  9th year -> Applied mathematics
 *  and
 *  11th year -> Technical design
 *
 *  If we store it plain with no additional information,
 *  we do not know if the data was
 *
 *  11th year -> Applied mathematics
 *  9th year -> Technical design
 *  or
 *  11th year -> Technical design
 *  9th year -> Applied mathematics
 *
 * To solve this problem we store the hole school-grade
 * path within the discipline path.
 *
 * so plain discipline is:
 *  mathematics and natural science|mathematics|applied mathematics
 * But we store
 *  compulsory education|cycle_3|9th_year|mathematics and
 *  natural science|mathematics|applied mathematics
 *
 * With this method we have the correct link between school grade and
 * discipline.
 *
 * But also we got a new Problem.
 * How can we find out what part is school-grade and what is discipline.
 * This we can solve with a method called "findBestMatch"
 * We build up an array with all school-grades within the resource
 * The array includes all possible depths within the path so for example:
 *
 * compulsory education|cycle_3|9th_year
 * compulsory education|cycle_3|11th_year
 *
 * Now we walk though our discpline array and
 * check the string against the splittet school-grade array.
 * if the string is found in the school-grade array than
 * this string is the "school-grade" part and the other is the discipline.
 *
 * Explanation: why we do what we do
 *  The reason we not easily can say the fist 3 taxons are "educational level"
 *  and the rest ist "discipline",
 *  is that we never know if it are realy 3. It is possible to
 *  choose the whole "3rd cycle".
 *  Why we dont have 2 taxon pathes in 1 classifiaction is,
 *  that we think this function was intendet for describing
 *  multiply way`s to this subject.
 *  Or only have one classification for each purpose.
 *  There we got big problems with lom.
 *  Its not well defined, you have to interpret it.
 *  We tryed to use the max possible way of interoperability
 *  with other lom using system.
 *  To be able to exchange data without converting it.
 *
 * ArchibaldLomDataClassification(
 *   purpose => "discipline"
 *   taxon path => array(
 *      0 => ArchibaldLomDataTaxonPath(
 *             source => "educa" // name of this currculum
 *             taxon => array(
 *                0 => ArchibaldLomDataTaxon(
 *                         // most the lower case english term.
 *                         // Its defined in ontology serivce
 *                         // further informations below
 *                         id    => "compulsory education",
 *                         entry => ArchibaldLomDataLangString(
 *                            de => "Obligatorische Schule",
 *                            fr => "École obligatoire",
 *                            it => "Scuola obligatoria",
 *                            rm => "Scola obligatorica",
 *                            en => "Compulsory education"
 *                            )
 *                       )
 *                1 => ArchibaldLomDataTaxon(
 *                         id    => "3rd cycle",
 *                         entry => ArchibaldLomDataLangString(
 *                            de => "3. Zyklus (9. bis 11. Schuljahr)",
 *                            fr => "3e cycle (9ème à 11ème année scolaire)",
 *                            it => "3° ciclo",
 *                            rm => "Terz ciclus",
 *                            en => "3rd cycle (9th to 11th school year)"
 *                            )
 *                       )
 *                2 => ArchibaldLomDataTaxon(
 *                         id    => "9th_year",
 *                         entry => ArchibaldLomDataLangString(
 *                            de => "9. Schuljahr",
 *                            fr => "9e année",
 *                            it => "9° anno scolastico",
 *                            rm => "9. classa",
 *                            en => "9th year"
 *                            )
 *                       )
 *                3 => ArchibaldLomDataTaxon(
 *                         id    => "mathematics and natural science",
 *                         // not all languges must be set
 *                         entry => ArchibaldLomDataLangString(
 *                            de => "Mathematik und Naturwissenschaften",
 *                            fr => "Mathématiques et sciences de la nature",
 *                            en => "Mathematics and Natural Science"
 *                            )
 *                       )
 *                4 => ArchibaldLomDataTaxon(
 *                         id    => "mathematics",
 *                         entry => ArchibaldLomDataLangString(
 *                            de => "Mathematik",
 *                            fr => "Mathématiques",
 *                            it => "Mathematics",
 *                            rm => "Mathematics",
 *                            en => "Mathematics"
 *                            )
 *                       )
 *                5 => ArchibaldLomDataTaxon(
 *                         id    => "applied mathematics",
 *                         entry => ArchibaldLomDataLangString(
 *                             de => "Anwendungen der Mathematik",
 *                             fr => "Applications des mathématiques",
 *                             en => "Applied mathematics"
 *                             )
 *                       )
 *               )
 *          )
 *   )
 * )
 *
 * The complete curriculum could be found here,
 * It will synced with the drupal taxonomy every few hours
 * (if the configured URL to the ontology service is valid)
 *  within the section "educa: Standard Curriculum"
 *
 * This class will extract for
 * display, add, remove ...
 * the educa curriculum from lom classifcation
 *
 * @example  for display curriculum,
 *   all values will given in current language (if possible) by using
 *    ArchibaldLom::getLangstringText()
 *
 *  $curr_handler = new ArchibaldCurriculumEduca();
 *  $curr_handler->setLomClassification(  $lom->getClassification()  );
 // Default currciulum
 *  echo $curr_handler->getName();
 *  print_r( $curr_handler->getEntrys() );
 *
 *  stdClass(
 *       educationalLevel => array(
 *          "Compulsory education",
 *          "3rd cycle (9th to 11th school year)",
 *          "9th year"
 *          ),
 *       discipline => array(
 *          "Mathematics and Natural Science",
 *          "Mathematics",
 *          "Applied mathematics"
 *          )
 *    );
 *
 */
class ArchibaldCurriculumEduca extends ArchibaldCurriculumBasic implements ArchibaldInterfaceCurriculumBasic {

  /**
   * recursiv array with curriculum
   * @var array
   */
  private $curriculum = array();

  /**
   * The machine_name of the current curriculum
   * @var string
   */
  protected $curriculumIdentKey = 'educa';

  /**
   * return human readable name of this curriculum
   *
   * @return string
   */
  public function getName() {
    return t('Default curriculum');
  }

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
  public function getEntry($source, $purpose, array $taxons) {
    $entry = (object)array('key' => '', 'val' => '');

    if ($purpose == 'discipline' && $source == 'educa') {
      $match = $this->findBestMatch($taxons, TRUE);

      // should never happen
      if (empty($match)) {
        // because we ever first addTaxonpath() for "educational level" and then for "discipline"
        return $entry;
      }

      // second run to split at $match in educationalLevel and discipline
      $educationalLevel_id = array();
      $educationalLevel_val = array();
      $discipline_val = NULL;
      $educationalLevelIds = array();
      $discipline_val_ids = array();
      foreach ($taxons as $taxon) {
        if ($taxon instanceof ArchibaldLomDataTaxon) {

          $id = $taxon->getId();
          $val_entry = ArchibaldLom::getLangstringText($taxon->getEntry());

          $educationalLevel_id[] = $id;

          if ($discipline_val === NULL) {
            $educationalLevel_val[] = $val_entry;
            // @var $val_entry (object)array('id'=>$id, 'entry'=>$val_entry);
            $educationalLevel_val_ids[] = $id;
          }
          else {
            $discipline_val[] = $val_entry;
            // @var $val_entry (object)array('id'=>$id, 'entry'=>$val_entry);
            $discipline_val_ids[] = $id;
          }

          if (implode('|', $educationalLevel_id) == $match) {
            $discipline_val = array();
          }
        }
      }


      $entry->val = $this->themeVal($educationalLevel_val, $discipline_val);
      $entry->educationalLevel = $educationalLevel_val;
      $entry->discipline = $discipline_val;
      $entry->educationalLevelIds = $educationalLevel_val_ids;
      $entry->disciplineIds = $discipline_val_ids;

      $entry->key = md5($purpose . $source . json_encode($taxons));

      //var_dump($entry);
      //die();
    }

    return $entry;
  }

  /**
   * delete one classification
   *
   * @param string $key
   */
  public function remove($key) {
    $data = $this->walkTroughClassifcations(
      create_function('$source, $purpose, $taxons', '
            if ("' . $key . '" == md5($purpose.$source.json_encode($taxons))) {
                $GLOBALS["educationalLevel_to_delete"] = $taxons;

                return "delete";
            } ')
    );

    // the $key was not found, also it was allready deleted
    if (!is_array($GLOBALS['educationalLevel_to_delete'])) {
      return FALSE;
    }

    // now we deleted the 'discipline' but not the 'educational level'
    // be we can not delete it simply,
    // cause we dont know if there are other with same 'education level'
    $hit_count = 0;
    $match = $this->findBestMatch($GLOBALS['educationalLevel_to_delete']);
    foreach ($data as $dat) {
      if ($dat->source == 'educa' && $dat->purpose == 'discipline') {
        if ($match == $this->findBestMatch($dat->taxons, TRUE)) {
          $hit_count++;
        }
      }
    }
    $GLOBALS['educationalLevel_to_delete'] = $match;

    // ok we found no other disciplines within the same educational level
    if ($hit_count == 0) {
      $data = $this->walkTroughClassifcations(
        create_function('$source, $purpose, $taxons', '
            if ($source=="educa" && $purpose=="educational level") {
                $educationalLevelIds =
                  ArchibaldCurriculumEduca::extractTaxonIds($taxons);
                if (implode("|", $educationalLevelIds)=="' .
          $GLOBALS['educationalLevel_to_delete'] . '") {
                    return "delete";
                }
            } '
        )
      );
    }

    unset($this->entrys[$key]);
  }

  /**
   * form handler to add new items
   *
   * @param array $form_state
   *
   * @return array
   *   drupal form object
   */
  public function getAddForm(&$form_state) {
    $educationalLevels = array_keys($this->curriculum);

    // $form['test'] = array(
    //   '#type' => 'item',
    //   '#markup' => '<pre>' . print_r( $this->archibaldGetTaxonomyOptions('educa_school_levels'), 1 ) . '</pre>',
    //   '#weight' => 9999
    // );
    $form['educationalLevel'] = array(
      '#type' => 'select',
      '#title' => t('Education level'),
      '#options' => $this->archibaldGetTaxonomyOptions('educa_school_levels'),
      '#empty_option' => ' - ' . t('Please choose') . ' - ',
      '#required' => TRUE,
      '#size' => 15,
    );

    $form['discipline'] = array(
      '#type' => 'select',
      '#title' => t('Discipline'),
      '#options' => $this->archibaldGetTaxonomyOptions('educa_school_subjects'),
      '#empty_option' => ' - ' . t('Please choose') . ' - ',
      '#required' => TRUE,
      '#size' => 15,
    );

    $form['close'] = array(
      '#type' => 'button',
      '#value' => t('Close'),
    );

    return $form;
  }

  /**
   * form submit handler to add new items
   *
   * @param array $form_state
   */
  public function submitAddForm(&$form_state) {
    $educationalLevel = explode('|', $form_state['values']['educationalLevel']);
    $discipline = explode('|', $form_state['values']['discipline']);

    $this->addTaxonPathEduca($educationalLevel, $discipline, $this);
    $form_state['was_saved'] = TRUE;
  }

  /**
   * add taxonpath from submitted form
   *
   * @param array $educationalLevel
   * @param array $discipline
   * @param ArchibaldCurriculumBasic $object_to_use
   */
  public function addTaxonPathEduca($educationalLevel, $discipline, &$object_to_use = NULL) {
    if (empty($object_to_use)) {
      $object_to_use = &$this;
    }

    // here we dont needs to check if we have it twice or not,
    // this does addTaxonpath()
    $taxonpath_el = array();
    foreach ($educationalLevel as $id) {
      $term_names = archibald_get_term_by_key($id, 'educa_school_levels', TRUE);
      $taxonpath_el[] = (object)array('entry' => $term_names, 'id' => $id);
    }
    $object_to_use->addTaxonpath('educa', 'educational level', $taxonpath_el);

    $taxonpath_di = $taxonpath_el;
    foreach ($discipline as $id) {
      $term_names = archibald_get_term_by_key($id, 'educa_school_subjects', TRUE);
      $taxonpath_di[] = (object)array('entry' => $term_names, 'id' => $id);
    }
    $object_to_use->addTaxonpath('educa', 'discipline', $taxonpath_di);

    if ($object_to_use instanceof ArchibaldCurriculumEduca) {
      #$object_to_use->findBestMatch(array(), TRUE);
      $object_to_use->renderEntrys();
    }
  }

  /**
   * get by taxonomy vocabulary machine_name a flat select options list
   * @staticvar string $vocabularies
   *
   * @param string $vocab
   *
   * @return array
   */
  private function archibaldGetTaxonomyOptions($vocab) {
    $options = array();

    if ($vocabulary = taxonomy_vocabulary_machine_name_load($vocab)) {
      if ($terms = taxonomy_get_tree($vocabulary->vid, 0, NULL, TRUE)) {
        $key_list = array();
        foreach ($terms as $term) {
          $taxon_key = @$term->field_taxon_key['und'][0]['value'];
          if (@$term->field_taxon_deprecated['und'][0]['value']) {
            continue;
          }
          $key_list[$term->depth] = $taxon_key;
          $key = array();
          for ($i = 0; $i <= $term->depth; $i++) {
            $key[] = $key_list[$i];
          }

          $term->name = i18n_taxonomy_term_name($term);
          $options[implode('|', $key)] = str_repeat('-', $term->depth) . $term->name;
        }
      }
    }

    return $options;
  }

  /**
   * get path by flat list
   *
   * @param string $vocab
   * @param string $key
   *   key to find
   * @param string $return
   *   key=keylist, name=namelist
   *
   * @return array
   */
  public function getTaxonomyPathByKey($vocab, $key, $return = 'key') {
    global $language;

    $key_list   = array();
    $name_list  = array();
    $filling_up = FALSE;
    $last_depth = 9999;

    if ($vocabulary = taxonomy_vocabulary_machine_name_load($vocab)) {
      if ($terms = taxonomy_get_tree($vocabulary->vid, 0, NULL, TRUE)) {
        $terms_reverse = array_reverse($terms);

        $key_list = array();
        foreach ($terms_reverse as $term) {
          $taxon_key = @$term->field_taxon_key['und'][0]['value'];

          if ($taxon_key == $key) {
            $filling_up = TRUE;
          }

          if ($filling_up == TRUE && $last_depth > $term->depth) {
            $key_list[] = $taxon_key;

            $name_list[] = archibald_get_term_by_key($taxon_key, $vocab, FALSE, $language->language);

            if ($term->depth == 0) {
              if ($return == 'key') {
                return array_reverse($key_list);
              }
              else {
                return array_reverse($name_list);
              }
            }

            $last_depth = $term->depth;
          }
        }
      }
    }

    return FALSE;
  }

  /**
   * figure out best matching "education level"
   * match for the whoole "discipline" path
   *
   * @param array $taxons
   * @param boolean $reset
   *   default=FALSE
   *   clean internal cache if TRUE
   *
   * @return string
   *   implodet options
   */
  protected function findBestMatch(array $taxons, $reset = FALSE) {
    $curriculum_formated = &archibald_static(__FUNCTION__, NULL);

    if ($reset == TRUE) {
      $curriculum_formated = NULL;

      if (empty($taxons)) {
        return;
      }
    }

    if (empty($curriculum_formated)) {
      $curriculum_formated = $this->walkTroughClassifcations();
    }


    $educationalLevels = array();

    // get all available education levels for this source,
    // cause we need it to be able to split discipline path
    foreach ($curriculum_formated as $data) {
      if ($data->source == 'educa' && $data->purpose == 'educational level') {
        $educationalLevelIds = $this->extractTaxonIds($data->taxons);
        $educationalLevels[implode('|', $educationalLevelIds)] = $educationalLevelIds;
      }
    }

    $found_matches = array();
    $educationalLevel_id = array();

    // the first run we need to find the longest matching string
    foreach ($taxons as $taxon) {
      if ($taxon instanceof ArchibaldLomDataTaxon) {
        $educationalLevel_id[] = $taxon->getId();

        $k = implode('|', $educationalLevel_id);
        if (isset($educationalLevels[$k])) {
          $found_matches[$k] = drupal_strlen($k);
        }
      }
    }
    arsort($found_matches);
    $found_matches = array_keys($found_matches);
    return @$found_matches[0];
  }

  /**
   * extract all ids from a taxon path
   *
   * @param array $taxons
   *
   * @return array
   */
  public function extractTaxonIds($taxons) {
    $taxon_ids = array();
    foreach ($taxons as $taxon) {
      if ($taxon instanceof ArchibaldLomDataTaxon) {
        $taxon_ids[] = $taxon->getId();
      }
    }

    return $taxon_ids;
  }

  /**
   * the the path for frontend
   *
   * @param array $educationalLevel_val
   *   list of strings with display the path
   * @param array $discipline_val
   *   list of strings with display the path
   *
   * @return string
   *   html
   */
  public function themeVal($educationalLevel_val, $discipline_val) {
    return implode(' / ', $educationalLevel_val) . '<br />' . implode(' / ', $discipline_val);
  }

  /**
   * remove objects form path where are high level descriptions exists for
   */
  public function cleanUp() {
    $entrys = array();
    foreach ($this->walkTroughClassifcations() as $data) {
      $entry = $this->getEntry($data->source, $data->purpose, $data->taxons);
      if (!empty($entry->key)) {
        $entrys[] = $entry;
      }
    }

    foreach ($entrys as $a => $entry) {
      $educationalLevel_path = implode('|', $entry->educationalLevel);
      $discipline_path = implode('|', $entry->discipline);

      $elp_length = drupal_strlen($educationalLevel_path);
      $dp_length = drupal_strlen($discipline_path);

      foreach ($entrys as $b => $mapper) {
        if ($a != $b) {
          $educationalLevel_path_mapper = implode('|', $mapper->educationalLevel);
          $discipline_path_mapper = implode('|', $mapper->discipline);

          $elpm_length = drupal_strlen($educationalLevel_path_mapper);
          $dpm_length = drupal_strlen($discipline_path_mapper);

          // if education level`s are the same and
          // mapper discipline path is shorte but equal to entry path
          $level_is_mapper = $educationalLevel_path == $educationalLevel_path_mapper;
          $discipline_subpath_is_mapper = (drupal_substr($discipline_path, 0, $dpm_length) == $discipline_path_mapper);
          if ($level_is_mapper && $dp_length > $dpm_length && $discipline_subpath_is_mapper) {
            $this->remove($mapper->key);
          }

          // if disciplin`s are the same and
          // mapper education level path is shorte but equal to entry path
          $discipline_path_is_mapper = ($discipline_path == $discipline_path_mapper);
          $educational_subpath_is_mapper = (drupal_substr($educationalLevel_path, 0, $elpm_length) == $educationalLevel_path_mapper);
          if ($discipline_path_is_mapper && $elp_length > $elpm_length && $educational_subpath_is_mapper) {
            $this->remove($mapper->key);
          }
        }
      }
    }
  }
}

