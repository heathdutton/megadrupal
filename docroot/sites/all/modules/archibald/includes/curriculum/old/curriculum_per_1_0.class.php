<?php

/**
 * @file
 * hnalder for per curriculum
 * old version 1.0
 * Only used within update script
 */

/**
 * in ArchibaldLom-CH all curriculums will stored in 9. Classification
 *
 * Example Data:
 *
 * Schoolgrade:
 *   Cycle 1
 *    3e – 4e années
 *
 * Discipline:
 *   Mathématiques
 *    Opérations (MSN 13)
 *
 * Competency:
 *  en utilisant la commutativité et l’associativité de l’addition
 *  en choisissant l’outil de calcul le mieux adapté à la situation proposée
 *  en construisant, en exerçant et en utilisant des procédures de calcul avec
 *    des nombres naturels
 *    (calcul réfléchi, calculatrice, répertoires mémorisés)
 *
 *
 * will be stored in 2 Classifcation tree`s,
 * first for Schoolgrade and the second for Discipline
 *
 * 1.)
 * ArchibaldLomDataClassification(
 *   purpose => "educational level"
 *   taxon path => array(
 *      0 => ArchibaldLomDataTaxonPath(
 *             source => "per" // name of this currculum
 *             taxon => array(
 *                0 => ArchibaldLomDataTaxon(
 *  // it is empty cause in per only the depest level has an id,
 *  // with this id you are able to identify the full path
 *                         id    => "",
 *                         entry => ArchibaldLomDataLangString(
 *  // in per only existing french lang strings
 *                                          fr => "Cycle 1"
 *                                     )
 *                       )
 *                1 => ArchibaldLomDataTaxon(
 *                         id    => "",
 *                         entry => ArchibaldLomDataLangString(
 *                                          fr => "3e – 4e années",
 *                                     )
 *                       )
 *               )
 *          )
 *   )
 * )
 *
 * 2.)
 * Now we have a complete path to school grade "3e – 4e années".
 *
 * In other Curriculums we have our problems begins if we have 2 or
 * more school grades and discipline.
 * Cause those dosent have an unique id
 * But to be equal we use here the same way to store the data.
 * This means we put the "educational level" infront of the
 * For further informations have a look at educa curriculum "discipline"
 *
 * ArchibaldLomDataClassification(
 *   purpose => "discipline"
 *   taxon path => array(
 *      0 => ArchibaldLomDataTaxonPath(
 *             source => "per" // name of this currculum
 *             taxon => array(
 *                0 => ArchibaldLomDataTaxon(
 *                         id    => "",
 *                         entry => ArchibaldLomDataLangString(
 *                                          fr => "Cycle 1"
 *                                     )
 *                       )
 *                1 => ArchibaldLomDataTaxon(
 *                         id    => "",
 *                         entry => ArchibaldLomDataLangString(
 *                                          fr => "3e – 4e années",
 *                                     )
 *                       )
 *               )
 *                2 => ArchibaldLomDataTaxon(
 *                         id    => "",
 *                         entry => ArchibaldLomDataLangString(
 *                                          fr => "Mathématiques"
 *                                     )
 *                       )
 *                3 => ArchibaldLomDataTaxon(
 *  // with this id you can load the full path from curriclum
 *  // with out all previos data
 *                         id    => "MSN 13",
 *                         entry => ArchibaldLomDataLangString(
 *                                          fr => "Opérations"
 *                                     )
 *                       )
 *               )
 *          )
 *   )
 * )
 *
 * 3.)
 * At least we needs to store the competencys.
 * Here we have one taxonPath for each competency. Identified by the id
 *
 * ArchibaldLomDataClassification(
 *   purpose => "competency"
 *   taxon path => array(
 *      0 => ArchibaldLomDataTaxonPath(
 *             source => "per" // name of this currculum
 *             taxon => array(
 *                0 => ArchibaldLomDataTaxon(
 *  // with this id you can load the full path from curriclum with
 *  // out all previos data
 *                         id    => "MSN 13",
 *                         entry => ArchibaldLomDataLangString(
 *                                          fr => "Opérations"
 *                                     )
 *                       )
 *                1 => ArchibaldLomDataTaxon(
 *                         id    => "",
 *                         entry => ArchibaldLomDataLangString(
 *                           fr => "en utilisant la commutativité et
 * l’associativité de l’addition"
 *                          )
 *                       )
 *               )
 * ...............
 *
 * The complete curriculum could be found here,
 * It will cached and all 12 hours refreched
 * const CURRICULUM_URL
 *
 * In the currisculum you will find additional information for id`s
 * Like:
 *  short description
 *  url to full descriptions at www.plandetudes.ch
 *
 * This class will extract for display, add, remove ...
 * the per curriculum from lom classifcation
 * @example  for display curriculum, all values will displayed in french
 *  (the one and only language for per)
 *  $curr_handler = new curriculum_per();
 *  $curr_handler->setLomClassification(  $lom->getClassification()  );
 // Default currciulum
 *  echo $curr_handler->getName();
 *  print_r( $curr_handler->getEntrys() );
 *
 *  stdClass(
 *       educational_level => array("Cycle 1",
 *                                  "3e – 4e années"),
 *       discipline =>        array("Mathématiques",
 *                                  "Opérations"),
 *       discipline_ids =>    array("MSN 13"),
 *       competences =>       array("en utilisant la commutativité et
 * l’associativité de l’addition",
 *                                  "en choisissant l’outil de calcul le
 *  mieux adapté à la situation proposée",
 *                                  "en construisant, en exerçant et en
 *  utilisant des procédures de calcul avec des nombres naturels
 * (calcul réfléchi, calculatrice, répertoires mémorisés)"),
 *    );
 *
 *  // get additional infomations directly from the curriculum by discipline_id
 *  $discipline_id = reset( $curr_handler->getEntrys()->discipline_ids ):
 *  echo $curr_handler->getUrlById($discipline_id);
 *  echo $curr_handler->getDescriptionById($discipline_id);
 *
 */
class ArchibaldCurriculumPer10 extends ArchibaldCurriculumBasic implements ArchibaldInterfaceCurriculumBasic {

  const CURRICULUM_URL = 'http://ontology.biblio.educa.ch/curriculum/curriculum_per.xml';

  /**
   * recursiv array with curriculum
   * @var array
   */
  private $curriculum = array();

  public function getName() {
    return "Plan d'études romand (PER)";
  }

  public function getEntry($source, $purpose, array $taxons) {
    $entry = (object)array('key' => '', 'val' => '');

    if ($purpose == 'discipline' && $source == 'per') {
      $match = $this->findBestMatch($taxons);

      // should never happen
      if (empty($match)) {
        return $entry;
      }

      // second run to split at $match in educational_level and discipline
      $educational_level_id = array();
      $educational_level_val = array();
      $discipline_val = NULL;
      $educational_level_ids = array();
      $discipline_val_ids = array();
      foreach ($taxons as $taxon) {
        if ($taxon instanceof ArchibaldLomDataTaxon) {

          $id = $taxon->getId();
          $val_entry = $taxon->getEntry()->getString('fr');

          $educational_level_id[] = $this->taxonGetId($taxon);

          if ($discipline_val === NULL) {
            $educational_level_val[] = $val_entry;
          }
          else {
            $discipline_val[] = $val_entry;
            if (!empty($id)) {
              $discipline_val_ids[] = $id;
            }
          }

          if (implode('|', $educational_level_id) == $match) {
            $discipline_val = array();
          }
        }
      }


      $entry->val = $this->themeVal($educational_level_val, $discipline_val);
      $entry->educational_level = $educational_level_val;
      $entry->discipline = $discipline_val;
      $entry->discipline_ids = $discipline_val_ids;
      $entry->competences = array();

      if (!empty($discipline_val_ids[0])) {
        $entry->competences = $this->findCompeteneces($discipline_val_ids[0]);
      }

      $entry->key = md5($purpose . $source . json_encode($taxons));
      $this->getCurriculum();
    }

    return $entry;
  }

  /**
   * figure out best matching "education level"
   * match for the whoole "discipline" path
   *
   * @param array $taxons
   * @param boolean $reset default=FALSE   clear internal cache if TRUE
   *
   * @return string @ implodet options
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


    $educational_levels = array();

    // get all available education levels for this source,
    // cause we need it to be able to split discipline path
    foreach ($curriculum_formated as $data) {
      if ($data->source == 'per' && $data->purpose == 'educational level') {
        $educational_level_ids = $this->extractTaxonIds($data->taxons);
        $educational_levels[implode('|', $educational_level_ids)] = $educational_level_ids;
      }
    }

    $found_matches = array();
    $educational_level_id = array();

    // the first run we need to find the longest matching string
    foreach ($taxons as $taxon) {
      if ($taxon instanceof ArchibaldLomDataTaxon) {
        $educational_level_id[] = $this->taxonGetId($taxon);

        $k = implode('|', $educational_level_id);
        if (isset($educational_levels[$k])) {
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
        $taxon_ids[] = $this->taxonGetId($taxon);
      }
    }

    return $taxon_ids;
  }

  /**
   * get id by taxon
   * use entry as id if no id exists
   *
   * @param ArchibaldLomDataTaxon $taxon
   *
   * @return string
   */
  public function taxonGetId(ArchibaldLomDataTaxon $taxon) {
    $id = $taxon->getId();
    if (empty($id)) {
      $id = ArchibaldLom::getLangstringText($taxon->getEntry());
    }
    return $id;
  }

  /**
   * the the path for frontend
   *
   * @param array $educational_level_val list of strings with display the path
   * @param array $discipline_val list of strings with display the path
   *
   * @return string html
   */
  public function themeVal($educational_level_val, $discipline_val) {
    return implode(' / ', $educational_level_val) . '<br />' . implode(' / ', $discipline_val);
  }

  /**
   * get from current classifcation the choosen cometency by disciline_id
   * @staticvar array $curriculum_formated
   *
   * @param string $discipline_id
   *
   * @return array of strings
   */
  private function findCompeteneces($discipline_id) {
    $curriculum_formated = &archibald_static(__FUNCTION__, array());

    if (empty($curriculum_formated)) {
      $curriculum_formated = $this->walkTroughClassifcations();
    }

    $competences = array();
    // get all availablecompetences for this source,
    foreach ($curriculum_formated as $data) {
      if ($data->source == 'per' && $data->purpose == 'competency' && count($data->taxons) == 2) {
        $competences[$data->taxons[0]->getId()][] = $data->taxons[1]->getEntry()->getString('fr');
      }
    }

    if (empty($competences[$discipline_id])) {
      return array();
    }
    else {
      return $competences[$discipline_id];
    }
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
                $GLOBALS["educational_level_to_delete"] =
                $taxons[0]->getEntry()->getString("fr");

                return "delete";
            } ')
    );

    // now we deleted the 'discipline' but not the 'educational level'
    // be we can not delete it simply, cause we dont know
    // if there are other with same 'education level'
    $hit_count = 0;
    foreach ($data as $dat) {
      if ($dat->source == 'per' && $dat->purpose == 'discipline') {
        if ($GLOBALS['educational_level_to_delete'] == $dat->taxons[0]->getEntry()->getString('fr')) {
          $hit_count++;
        }
      }
    }

    // ok we found no other disciplines within the same educational level
    if ($hit_count == 0) {
      $data = $this->walkTroughClassifcations(
        create_function('$source, $purpose, $taxons', '
            if ($source=="per" && $purpose=="educational level") {
                if ($taxons[0]->getEntry()->getString("fr")=="' .
          $GLOBALS['educational_level_to_delete'] . '") {
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
   * @return array drupal form object
   */
  public function getAddForm(&$form_state) {
    $this->getCurriculum();

    $educational_levels = array();
    foreach ($this->curriculum->education_levels as $education_level_cycle => $education_classes) {
      $educational_levels[$education_level_cycle] = $education_level_cycle;
      foreach ($education_classes as $education_class) {
        $educational_levels[$education_class] = '- ' . $education_class;
      }
    }

    $form['educational_level'] = array(
      '#type' => 'select',
      '#title' => t('Education level'),
      '#options' => $educational_levels,
      '#empty_option' => ' - ' . t('Please choose') . ' - ',
      '#attributes' => array('class' => array('curr_per_education_level')),
      '#required' => TRUE,
    );

    // this options are only for the form validator
    $discipline_c_tmp_options = array();

    $discipline_a = array();
    foreach ($this->curriculum->matrix as $educational_level_content) {
      foreach ($educational_level_content as $disciline_a => $disciline_a_content) {
        $discipline_a_options[$disciline_a] = $disciline_a;

        foreach ($disciline_a_content as $disciline_b_content) {
          foreach ($disciline_b_content as $disciline_c => $code) {
            $discipline_c_tmp_options[$code] = $disciline_c . ' (' . $code . ')';
          }
        }
      }
    }
    $form['discipline_a'] = array(
      '#type' => 'select',
      '#title' => t('Discipline'),
      '#options' => $discipline_a_options,
      '#empty_option' => ' - ' . t('Please choose') . ' - ',
      '#attributes' => array('class' => array('curr_per_discipline_a')),
      '#required' => TRUE,
    );

    $form['discipline_c'] = array(
      '#type' => 'select',
      '#title' => t('Subject'),
      '#options' => $discipline_c_tmp_options,
      '#size' => 15,
      '#attributes' => array('class' => array('curr_per_discipline_c')),
      '#required' => TRUE,
    );

    $competences_tmp_options = array();
    foreach ($this->curriculum->competences as $competences) {
      foreach ($competences->competence As $competence) {
        $competences_tmp_options[$competence] = $competence;
      }
    }

    $form['competences'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Competences'),
      '#options' => $competences_tmp_options,
      '#attributes' => array('class' => array('curr_per_competences')),
      '#required' => FALSE,
    );

    $form['proposals'] = array(
      '#type' => 'textfield',
      '#title' => t('Proposals'),
      '#attributes' => array('style' => array('display:none;')),
      '#description' =>
      t('Confirm matching entry in the default curriculum.'),
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
    $this->getCurriculum();

    $education_level_cycle = '';
    $education_level_class = $form_state['values']['educational_level'];
    $discipline_a = $form_state['values']['discipline_a'];
    $discipline_b = '';
    $discipline_c = '';
    $discipline_c_key = $form_state['values']['discipline_c'];
    $competences = $form_state['values']['competences'];

    // determine first level of education_level
    foreach ($this->curriculum->education_levels as $tmp_education_level_cycle => $education_classes) {

      if ($tmp_education_level_cycle == $education_level_class) {
        // it is allready a first level, nothing to do
        $education_level_cycle = $education_level_class;
        $education_level_class = '';
        break;
      }

      $educational_levels[$education_level_cycle] = $education_level_cycle;
      foreach ($education_classes as $tmp_education_level_class) {
        if ($tmp_education_level_class == $education_level_class) {
          // the matching first level element
          $education_level_cycle = $tmp_education_level_cycle;
          break 2;
        }
      }
    }

    // get $discipline_b and $discipline_c  by $discipline_c_key
    foreach ($this->curriculum->matrix[$education_level_cycle][$discipline_a] as $discipline_b => $tmp) {
      foreach ($tmp as $discipline_c => $key) {
        if ($discipline_c_key == $key) {
          break 2;
        }
      }
    }

    $education_taxon_path = array(
      (object)array(
        'entry' => $education_level_cycle,
      ),
    );

    if (!empty($education_level_class)) {
      $education_taxon_path[] = (object) array(
        'entry' => $education_level_class,
      );
    }

    // here we dont needs to check if we have it twice or not,
    // this does addTaxonpath()
    $this->addTaxonpath(
      'per',
      'educational level',
      $education_taxon_path,
      'fr'
    );

    $discipline_taxon_path = $education_taxon_path;
    $discipline_taxon_path[] = (object) array(
      'entry' => $discipline_a,
    );

    $discipline_taxon_path[] = (object) array(
      'entry' => $discipline_b,
    );

    $discipline_taxon_path[] = (object) array(
      'entry' => $discipline_c,
      'id' => $discipline_c_key,
    );

    $this->addTaxonpath(
      'per',
      'discipline',
      $discipline_taxon_path,
      'fr'
    );

    if (!empty($competences)) {
      foreach ($competences as $competence => $val) {
        if (!empty($val)) {
          $this->addTaxonpath(
            'per',
            'competency',
            array(
              (object)array(
                'entry' => $discipline_c,
                'id' => $discipline_c_key,
              ),
              (object)array(
                'entry' => $competence,
              ),
            ),
            'fr'
          );
        }
      }
    }

    $educa_proposals = explode('##', $form_state['values']['proposals']);
    if (!empty($educa_proposals)) {
      $curr_name = 'curriculum_educa';
      $classname = 'ArchibaldCurriculumEduca';
      module_load_include('php', 'archibald', 'includes/curriculum/' . $curr_name . '.class');

      $educa_handler = new $classname();
      $educa_handler->setLomClassification($this->getLomClassification());

      foreach ($educa_proposals as $educa_proposal) {
        list($educational_level, $discipline) = explode('|', $educa_proposal, 2);
        if (!empty($educational_level) && !empty($discipline)) {
          $edu_lvl_list = $educa_handler->getTaxonomyPathByKey('educa_school_levels', $educational_level, 'key');
          $educa_scl_sub_list = $educa_handler->getTaxonomyPathByKey('educa_school_subjects', $discipline, 'key');
          $educa_handler->addTaxonPathEduca($edu_lvl_list, $educa_scl_sub_list, $this);
        }
      }

      $form_state['reload_other_curriculums'] = array('educa');
    }

    $form_state['was_saved'] = TRUE;
  }

  /**
   * ajax callback for filling up last form element
   *
   * @return array
   */
  public function getDisciplineC() {
    $this->getCurriculum();
    // determine first level of education_level
    $education_level = filter_xss($_POST['education_level']);
    foreach ($this->curriculum->education_levels as $education_level_cycle => $education_classes) {

      if ($education_level_cycle == $education_level) {
        // it is allready a first level, nothin to do
        break;
      }

      $educational_levels[$education_level_cycle] = $education_level_cycle;
      foreach ($education_classes as $education_class) {
        if ($education_class == $education_level) {
          // the matching second level element
          $education_level = $education_level_cycle;
          break 2;
        }
      }
    }
    $discipline_a = filter_xss($_POST['discipline_a']);
    return $this->curriculum->matrix[$education_level][$discipline_a];
  }

  /**
   * Menu callback
   * to load list of proposals
   * by post value: discipline_c
   */
  public function getProposals() {
    $proposals = array();

    $per2default = variable_get('archibald_per2default', array());
    $discipline_c = filter_xss($_POST['discipline_c']);
    if (!isset($per2default[$discipline_c]) || empty($per2default[$discipline_c])) {
      return array('proposals' => $proposals);
    }

    $curr_name = 'curriculum_educa';
    $classname = 'ArchibaldCurriculumEduca';
    module_load_include('php', 'archibald', 'includes/curriculum/' . $curr_name . '.class');

    $educa_handler = new $classname();

    foreach ($per2default[$discipline_c] as $raw_mapping_obj) {
      $key = $raw_mapping_obj->educational_level . '|' . $raw_mapping_obj->discipline;
      $edu_lvl_list = $educa_handler->getTaxonomyPathByKey('educa_school_levels', $raw_mapping_obj->educational_level, 'name');
      $educa_scl_sub_list = $educa_handler->getTaxonomyPathByKey('educa_school_subjects', $raw_mapping_obj->discipline, 'name');
      $proposals[$key] = $educa_handler->themeVal($edu_lvl_list, $educa_scl_sub_list);
    }

    return array('proposals' => $proposals);
  }

  /**
   *
   * @param $reset boolean  default:FALSE
   * curriculum handler
   * gets curriculum from drupal variables db or get it freh from
   * internet if the time has come
   */
  private function getCurriculum($reset = FALSE) {
    $cache_key = 'archibald_portal_curriculum_per';
    $cache = cache_get($cache_key, 'cache');
    if (!empty($cache->data) && $reset == FALSE) {
      $this->curriculum = $cache->data;
      return TRUE;
    }

    $xml = new SimpleXMLElement(mb_convert_encoding(file_get_contents(self::CURRICULUM_URL), 'UTF-8'));

    $this->curriculum = new stdClass();
    $this->curriculum->education_levels = array();
    foreach ($xml->education_levels->education_level as $education_level_cycle) {
      foreach ($education_level_cycle->education_level as $education_level_class) {
        $elc = (string) $education_level_cycle->attributes()->name;
        $this->curriculum->education_levels[$elc][] = (string) $education_level_class->attributes()->name;
      }
    }

    $this->curriculum->matrix = array();
    foreach ($xml->code_matrix->domain as $domain) {
      foreach ($domain->discipline as $discipline) {
        foreach ($discipline->thema as $thema) {
          foreach ($thema as $value) {
            $val = (string) $value;
            if (!empty($val) && drupal_strtolower($val) != 'x') {
              $this->curriculum->matrix[(string) $value->attributes()->name][(string) $domain->attributes()->name][(string) $discipline->attributes()->name][(string) $thema->attributes()->name] = $val;
            }
          }
        }
      }
    }

    $this->curriculum->competences = array();
    foreach ($xml->competences->code as $code) {
      $tmp = (object)array(
        'url' => (string) $code->url,
        'description' => (string) $code->description,
        'competence' => array(),
      );
      foreach ($code->competence as $competence) {
        $tmp->competence[] = (string) $competence;
      }
      $this->curriculum->competences[(string) $code->attributes()->code] = $tmp;
    }
    cache_set($cache_key, $this->curriculum, 'cache', (time() + (60 * 60 * 12)) /* set cache for 12 hours */ );
  }

  /**
   * per ident string
   *
   * @param string $ident
   */
  public function addTaxonPathByIdent($ident) {
    $this->getCurriculum();

    foreach ($this->curriculum->matrix as $educational_level => $tmpa) {
      foreach ($tmpa as $discipline_a => $tmpb) {
        foreach ($tmpb as $discipline_b => $tmpc) {
          foreach ($tmpc as $discipline_c => $id) {
            if ($id == $ident) {
              $this->addTaxonpath('per', 'educational level', array((object)array('entry' => $educational_level)), 'fr');
              $this->addTaxonpath('per', 'discipline',
                array(
                    (object)array('entry' => $educational_level),
                    (object)array('entry' => $discipline_a),
                    (object)array('entry' => $discipline_b),
                    (object)array('entry' => $discipline_c, 'id' => $id),
                ),
                'fr'
              );
            }
          }
        }
      }
    }
  }

  /**
   * setup callback with will called from install process
   * this one installes the per to default curriculum mapping from csv to databse
   */
  public function setupProcess() {
    $i           = 0;
    $per2default = array();
    $file        = DRUPAL_ROOT . '/' . drupal_get_path('module', 'archibald') . '/data/per2default.csv';

    if (is_file($file)) {
      foreach (file($file) as $row) {
        $i++;
        $row = mb_convert_encoding($row, 'UTF-8', array('WINDOWS-1252', 'UTF-8', 'ISO-8859-1'));
        $row = str_getcsv(trim($row), ';');

        if ($i == 1) {
          $headers = $row;
        }
        else {
          for ($cycle = 1; $cycle <= 3; $cycle++) {
            $cycle_val = trim(@$row[($cycle + 2)]);
            if (drupal_strtolower($cycle_val) == 'x') {
              $cycle_val = '';
            }

            if (!empty($cycle_val)) {
              for ($educa_fid = 7; $educa_fid <= 20; $educa_fid++) {
                if (!empty($row[$educa_fid])) {
                  $data = (object)array(
                    'educational_level' => 'cycle_' . $cycle,
                    'discipline' => $row[$educa_fid],
                  );
                  $per2default[$cycle_val][md5(json_encode($data))] = $data;
                }
              }
            }
          }
        }
      }

      if (!empty($per2default)) {
        variable_set('archibald_per2default', $per2default);
      }
    }

    // invalidate curriculum cache
    $this->getCurriculum(TRUE);
  }

  /**
   * aajx callback to load compenteces
   *
   * @return array
   */
  public function getCompetences() {
    $competences = array();

    $this->getCurriculum();
    $discipline_id = filter_xss($_POST['discipline_c']);

    if (!empty($this->curriculum->competences[$discipline_id]->competence)) {
      foreach ($this->curriculum->competences[$discipline_id]->competence as $competence) {
        $competences[] = drupal_html_id($competence);
      }
    }

    return array('competences' => $competences);
  }

  /**
   * get url by discipline_id
   *
   * @param string $discipline_id
   *
   * @return string
   */
  public function getUrlById($discipline_id) {
    $this->getCurriculum();
    if (!empty($this->curriculum->competences[$discipline_id]->url)) {
      return $this->curriculum->competences[$discipline_id]->url;
    }
    else {
      return NULL;
    }
  }

  /**
   * get description by discipline_id
   *
   * @param string $discipline_id
   *
   * @return string
   */
  public function getDescriptionById($discipline_id) {
    $this->getCurriculum();
    if (!empty($this->curriculum->competences[$discipline_id]->description)) {
      return $this->curriculum->competences[$discipline_id]->description;
    }
    else {
      return NULL;
    }
  }
}

