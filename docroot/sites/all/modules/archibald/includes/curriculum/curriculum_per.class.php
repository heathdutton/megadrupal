<?php

/**
 * @file
 * handler class for per curriculum
 */

/**
 * in ArchibaldLom-CH
 * all basic    curriculums will stored in  9. Classification
 * all advanced curriculums will stored in 10. Curriculum
 *
 * Type: advanced
 *
 * will be stored in Curriculum tree`s,
 *
 * ...............
 *
 * The complete curriculum could be found here,
 * It will cached and all 12 hours refreched
 * const CURRICULUM_URL
 *
 *
 * This class will extract for display, add, remove ...
 * the per curriculum from lom curriculum
 * @example  for display curriculum, all values will displayed
 * in french (the one and only language for per)

 $curr_handler = new ArchibaldCurriculumPer();
 $curr_handler->setLomCurriculum(  $lom->getCurriculum()  );
 // Default currciulum
 echo $curr_handler->getName();
 foreach ($curr_handler->getEntrys() as $cycle=>$domains) {
   // @var string $cycle
   foreach ($domains as $domain=>$disciplines) {
     // @var string $domain
     foreach ($disciplines as $objectiv_code=>$objectiv) {
       // @var string $objectiv->code
       // @var string $objectiv->object
       // @var string $objectiv->discipline
       // @var string $objectiv->description
       // @var string $objectiv->url_part
       if (!empty($objectiv->object_elements)) {
         foreach ($objectiv->object_elements as $subtitle1) {
           // @var string $subtitle1->subtitle  'GENERAL'
           // @var string $subtitle1->url_part
           if (!empty($subtitle1->details)) {
             foreach ($subtitle1->details as $detail) {
               // @var string $detail->text
               // @var string $detail->school_year
               // @var string $detail->url_part
             }
           }
           if (!empty($subtitle1->childs)) {
             foreach ($subtitle1->childs as $subtitle2) {
               // @var string $subtitle2->subtitle  'GENERAL'
               // @var string $subtitle2->url_part
               if (!empty($subtitle2->details)) {
                 foreach ($subtitle2->details as $detail) {
                   // @var string $detail->text
                   // @var array $detail->school_year
                   // @var string $detail->url_part
                 }
               }
             }
           }
         }
       }
     }
   }
 }
 *
 *
 */
class ArchibaldCurriculumPer extends ArchibaldCurriculumAdvanced implements ArchibaldInterfaceCurriculumAdvanced {
  /**
   * basic url buidl from url_part and full url
   */
  const PER_BASE_URL = 'http://www.plandetudes.ch/web/guest/';

  /**
   * recursiv array with curriculum
   * @var array
   */
  private $curriculum = array();

  /**
   * The machine_name of the current curriculum
   * @var string
   */
  protected $curriculumIdentKey = 'per';

  public function getName() {
    return "Plan d'études romand (PER)";
  }

  public function __construct() {
    parent::__construct();
  }

  /**
   * add new entry to the curriculum
   *
   * @param string $education_level_cycle
   *   cycle
   * @param string $education_level_class
   *   années
   * @param string $discipline_a
   *   Domaines
   * @param string $discipline_b
   *   Diciplines
   * @param string $discipline_c
   *   Axe thématique
   * @param string $discipline_c_key
   *   Code objectif d'apprentissage
   * @param string $subtitle_key
   *   key off ous-titres 1/2
   * @param array $details
   *   code lisable par ordinateur/url_part
   *   for texte progression des apprentissages
   *
   * @return boolean
   */
  public function add($education_level_cycle, $education_level_class, $discipline_a, $discipline_b, $discipline_c, $discipline_c_key, $subtitle_key = '', $details = array()) {
    /*
     * $education_level_cycle: Cycle 1
     * $education_level_class: 3e – 4e années
     * $discipline_a: Langues
     * $discipline_b: Français
     * $discipline_c: Compréhension de l'écrit
     * $discipline_c_key: L1 11-12
     * $subtitle_key: ad29b5d30fe1c19815759a3ae5782823
     * $details: array('L1_13-14#219',
     *                 'L1_13-14#222',
     *                 'L1_13-14#226'
     *                )
     */

    if (empty($this->entrys->$education_level_cycle)) {
      $this->entrys->$education_level_cycle = new stdClass();
    }

    if (empty($this->entrys->$education_level_cycle->$discipline_a)) {
      $this->entrys->$education_level_cycle->$discipline_a = new stdClass();
    }

    if (empty($this->entrys->$education_level_cycle->$discipline_a->$discipline_c_key)) {

      $objectiv = $this->getObjectivByCode($discipline_c_key);
      $this->entrys->$education_level_cycle->$discipline_a->$discipline_c_key = (object)array(
        'discipline' => $discipline_b,
        'object' => $discipline_c,
        'code' => $discipline_c_key,
        'url_part' => @$objectiv->url_part,
        'description' => @$objectiv->description,
        'object_elements' => array(),
      );
    }

    if (!empty($subtitle_key)) {
      $subtitle_data = $this->getSubtitleByUuid($discipline_c_key, $subtitle_key);

      if (!empty($subtitle_data)) {
        // if it is an object, normalize it to an array
        $this->entrys->$education_level_cycle->$discipline_a->$discipline_c_key->object_elements = (array) $this->entrys->$education_level_cycle->$discipline_a->$discipline_c_key->object_elements;

        $discipline_c_key_value = $this->entrys->$education_level_cycle->$discipline_a->$discipline_c_key->object_elements;
        // now we add subtitle 1
        $key = $this->arraySearch('uuid', $subtitle_data->uuid, $discipline_c_key_value);

        if ($key === FALSE) {
          // ellement actualy not exist, now we will create it
          $current_subittle = (object)array(
            'uuid' => $subtitle_data->uuid,
            'url_part' => $subtitle_data->url_part,
            'subtitle' => $subtitle_data->title,
            'childs' => array(),
            'details' => array(),
          );
          $count = array_push($this->entrys->$education_level_cycle->$discipline_a->$discipline_c_key->object_elements, $current_subittle);
          $current_subittle = &$this->entrys->$education_level_cycle->$discipline_a->$discipline_c_key->object_elements[($count - 1)];
        }
        else {
          // use existing
          $current_subittle = &$this->entrys->$education_level_cycle->$discipline_a->$discipline_c_key->object_elements[$key];
        }


        if (!empty($subtitle_data->childs)) {
          // now we add subtitle 2, cause it exists
          $key = $this->arraySearch('uuid', $subtitle_data->uuid, $current_subittle->childs);

          if ($key === FALSE) {
            // ellement actualy not exist, now we will create it
            $new_current_subittle = (object)array(
              'uuid' => $subtitle_data->childs[0]->uuid,
              'url_part' => $subtitle_data->childs[0]->url_part,
              'subtitle' => $subtitle_data->childs[0]->title,
              'childs' => array(),
              'details' => array(),
            );
            $count = array_push($current_subittle->childs, $new_current_subittle);
            $new_current_subittle = &$current_subittle->childs[($count - 1)];
          }
          else {
            $new_current_subittle = &$current_subittle->childs[$key];
          }
          unset($current_subittle);
          $current_subittle = &$new_current_subittle;
        }

        if (!empty($details)) {
          $possible_details = array();
          foreach ($subtitle_data->details as $d) {
            $possible_details[$d->url_part] = $d;
          }
          if (!empty($subtitle_data->childs)) {
            foreach ($subtitle_data->childs[0]->details as $d) {
              $possible_details[$d->url_part] = $d;
            }
          }
          $current_subittle->details = (array) $current_subittle->details;

          // there are details, bow we add it to $current_subittle
          // independent of if it is subtitle1 or subtitle2
          foreach ($details as $detail_url_part) {
            $key = $this->arraySearch('ident', $education_level_class . '/' . $detail_url_part, $current_subittle->details);

            if ($key === FALSE) {
              // ellement actualy not exist, now we will create it
              array_push(
                $current_subittle->details, (object)array(
                  'ident' => $education_level_class . '/' . $detail_url_part,
                  'url_part' => $detail_url_part,
                  'text' => $possible_details[$detail_url_part]->text,
                  'school_year' => $education_level_class,
                )
              );
            }
          }
        }
      }
    }

    return TRUE;
  }

  /**
   * add new curriculum entry by object code
   * @param string $code
   *   code d objective
   * @return boolean
   */
  public function addByObjectCode($code, $subtitle_key = NULL, $details = NULL) {
    $discipline_c_key = '';
    foreach ($this->getCurriculum()->matrix as $cycle => $domains) {
      foreach ($domains as $domain => $disciplines) {
        foreach ($disciplines as $discipline => $themes) {
          foreach ($themes as $theme => $objectiv) {
            if ($code == $objectiv->code) {
              $discipline_c_key = $objectiv->code;
              $discipline_a = '';
              foreach ($this->getCurriculum()->matrix as $education_level_cycle => $educational_level_content) {

                foreach ($educational_level_content as $discipline_a => $discipline_a_content) {
                  $discipline_a_options[$discipline_a] = $discipline_a;
                  foreach ($discipline_a_content as $discipline_b => $discipline_b_content) {
                    foreach ($discipline_b_content as $discipline_c => $code) {
                      if ($discipline_c_key == $code->code) {
                        break 4;
                      }
                    }
                  }
                }
              }

              $education_level_class = '';

              $this->add(
                $education_level_cycle,
                $education_level_class,
                $discipline_a,
                $discipline,
                $theme,
                $discipline_c_key,
                $subtitle_key,
                $details
              );
              return TRUE;
            }
          }
        }
      }
    }
    return FALSE;
  }

  /**
   * return the full lvl 1 path
   *
   * @param string $discipline_c_key
   * @param string $uuid
   *
   * @return object
   */
  protected function getSubtitleByUuid($discipline_c_key, $uuid) {
    foreach ($this->getCurriculum()->subtitle[$discipline_c_key] as $item_lvl_1) {
      if ($item_lvl_1->uuid == $uuid) {
        // remoe all irrelevant childs
        $item_lvl_1->childs = array();

        return $item_lvl_1;
      }

      if (!empty($item_lvl_1->childs)) {
        foreach ($item_lvl_1->childs as $item_lvl_2) {
          if ($item_lvl_2->uuid == $uuid) {
            // remoe all irrelevant childs
            $item_lvl_2->childs = array();
            $item_lvl_1->childs = array($item_lvl_2);
            return $item_lvl_1;
          }
        }
      }
    }
    return NULL;
  }

  /**
   * delete one object
   *
   * @param string $to_delete
   */
  public function remove($to_delete) {
    $found_match = FALSE;
    foreach ($this->entrys as $cycle => $domains) {
      foreach ($domains as $domain => $disciplines) {
        foreach ($disciplines as $objectiv_code => $objectiv) {
          foreach ($objectiv->object_elements as $sk1 => $subtitle) {
            $subtitle_found_match = FALSE;
            $objectiv->object_elements[$sk1] = $this->removeSubtitle(
              $subtitle,
              $to_delete,
              $subtitle_found_match
            );

            if ($subtitle_found_match == TRUE) {
              if (empty($objectiv->object_elements[$sk1])) {
                unset($objectiv->object_elements[$sk1]);
              }
              // we wont delete the full tree
              // if user like to delete also Code objectif d'apprentissage
              // he have to click againe
              return TRUE;
            }
          }

          if ($found_match == FALSE && html_entity_decode($objectiv->code) == html_entity_decode($to_delete)) {
            unset($this->entrys->$cycle->$domain->$objectiv_code);
            $found_match = TRUE;
            break;
          }
        }

        if ($found_match == TRUE) {
          if (empty($this->entrys->$cycle->$domain)) {
            unset($this->entrys->$cycle->$domain);
          }
          break;
        }
      }
      if ($found_match == TRUE) {
        if (empty($this->entrys->$cycle)) {
          unset($this->entrys->$cycle);
        }
        break;
      }
    }
  }

  /**
   * process a subtitle and search elements to delete
   *
   * @param object $subtitle
   * @param string $to_delete
   * @param boolean $found_match
   *   called by refference
   *
   * @return object
   */
  protected function removeSubtitle($subtitle, $to_delete, &$found_match = FALSE) {

    if (html_entity_decode($subtitle->url_part) == html_entity_decode($to_delete)) {
      $found_match = TRUE;
      return NULL;
    }

    $subtitle->details = (array) $subtitle->details;
    usort($subtitle->details, array($this, 'removeSubtitleSort'));
    foreach ($subtitle->details as $skd => $detail) {
      if ($detail->url_part == $to_delete) {
        unset($subtitle->details[$skd]);
        $found_match = TRUE;
        break;
      }
    }
    usort($subtitle->details, array($this, 'removeSubtitleSort'));

    if ($found_match == FALSE) {
      $subtitle->childs = (array) $subtitle->childs;
      usort($subtitle->childs, array($this, 'removeSubtitleSort'));
      foreach ($subtitle->childs as $sk => $subtitle_child) {
        $found_match_child = FALSE;
        $subtitle->childs[$sk] = $this->removeSubtitle($subtitle_child, $to_delete, $found_match_child);

        if ($found_match_child == TRUE) {
          $found_match = TRUE;
          break;
        }
      }
      usort($subtitle->childs, array($this, 'removeSubtitleSort'));
    }

    if ($found_match == TRUE && empty($subtitle->details) && empty($subtitle->childs)) {
      // here we have no childs or details, cause of this we delete recursiv
      return NULL;
    }

    return $subtitle;
  }

  /**
   * strnatcmp for subtitle objects
   *
   * @param object $a
   * @param object $b
   *
   * @return integer
   */
  protected function removeSubtitleSort($a, $b) {
    return strnatcmp($a->url_part, $b->url_part);
  }

  /**
   * form handler to add new items
   *
   * @param array $form_state
   *
   * @return array drupal form object
   */
  public function getAddForm(&$form_state) {
    $educational_levels = array();
    foreach ($this->getCurriculum()->education_levels as $education_level_cycle => $education_classes) {
      $educational_levels[$education_level_cycle] = $education_level_cycle;
      foreach ($education_classes as $education_class) {
        // dont change the "- " prefix, cause it will triggert in JS
        $educational_levels[$education_class] = '- ' . $education_class;
      }
    }

    $form['educational_level'] = array(
      '#type' => 'select',
      '#title' => t('Cycles'),
      '#options' => $educational_levels,
      '#empty_option' => ' - ' . t('Please choose') . ' - ',
      '#attributes' => array('class' => array('curr_per_education_level')),
      '#required' => TRUE,
    );

    // this options are only for the form validator
    $discipline_c_tmp_options = array();

    $discipline_a = array();
    foreach ($this->getCurriculum()->matrix as $educational_level_content) {

      foreach ($educational_level_content as $disciline_a => $disciline_a_content) {
        $discipline_a_options[$disciline_a] = $disciline_a;
        foreach ($disciline_a_content as $disciline_b_content) {
          foreach ($disciline_b_content as $disciline_c => $code) {
            $discipline_c_tmp_options[$code->code] = $disciline_c . ' (' . $code->code . ')';
          }
        }
      }
    }
    $form['discipline_a'] = array(
      '#type' => 'select',
      '#title' => t('Disciplines'),
      '#options' => $discipline_a_options,
      '#empty_option' => ' - ' . t('Please choose') . ' - ',
      '#attributes' => array('class' => array('curr_per_discipline_a')),
      '#required' => TRUE,
    );

    $form['discipline_c'] = array(
      '#type' => 'select',
      '#title' => t('Components'),
      '#options' => $discipline_c_tmp_options,
      '#size' => 10,
      '#attributes' => array('class' => array('curr_per_discipline_c')),
      '#required' => TRUE,
    );

    $form['subtitle'] = array(
      '#markup' => '<div ' .
      'class="form-item form-type-select form-item-subtitles" ' .
      'style="display: block;">' .
      '<label for="edit-subtitles">' . t('Objectives') . '</label>' .
      '<select size="10" name="subtitle" id="edit-subtitles" ' .
      'class="curr_per_subtitles form-select"></select>' .
      '</div>',
    );
    /*
      '#type' => 'select',
      '#title' => t('subtitles'),
      '#options'=> array(),
      '#attributes' => array('class' => array('curr_per_subtitles')),
      '#required' => FALSE,
      '#size' => 10,
     */


    $form['details'] = array(
      '#type' => 'textfield',
      '#title' => t('Competences'),
      '#attributes' => array(
        'class' => array(
          'curr_per_details',
        ),
        'style' => array(
          'display:none;',
        ),
      ),
      '#required' => FALSE,
    );

    $class_name = 'ArchibaldCurriculumEduca';
    $educa_handler = new $class_name();
    if ($educa_handler->isActive()) {
      $form['proposals'] = array(
        '#type' => 'textfield',
        '#title' => t('Recommendation'),
        '#attributes' => array('style' => array('display:none;')),
        '#description' => t('Confirm matching entries in the default curriculum'),
      );
    }

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
    $education_level_cycle = '';
    $education_level_class = $form_state['values']['educational_level'];
    $discipline_a = $form_state['values']['discipline_a'];
    $discipline_b = '';
    $discipline_c = '';
    $discipline_c_key = $form_state['values']['discipline_c'];
    $subtitle_key = '';
    if (!empty($_POST['subtitle'])) {
      $subtitle_key = filter_xss($_POST['subtitle']);
    }

    $form_state['values']['details'] = trim($form_state['values']['details']);
    if (empty($form_state['values']['details'])) {
      $details = array();
    }
    else {
      $details = explode('|$|', $form_state['values']['details']);
    }

    // determine first level of education_level
    foreach ($this->getCurriculum()->education_levels as $tmp_education_level_cycle => $education_classes) {

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
    foreach ($this->getCurriculum()->matrix[$education_level_cycle][$discipline_a] as $tmp_discipline_b => $tmp) {

      foreach ($tmp as $tmp_discipline_c => $objective) {
        if ($discipline_c_key == $objective->code) {
          $discipline_b = $tmp_discipline_b;
          $discipline_c = $tmp_discipline_c;
          break 2;
        }
      }
    }

    // MANDETORY
    // $education_level_cycle  // cycle
    // $education_level_class  // années
    // $discipline_a      // Domaines
    // $discipline_b      // Diciplines
    // $discipline_c      // Axe thématique
    // $discipline_c_key  // Code objectif d'apprentissage
    // OPTIONAL
    // $subtitle_key      // url_code off Sous-titres 1/2
    // $details           // array of: code lisable par ordinateur/url_part
    //                       for texte progression des apprentissages

    $this->add(
      $education_level_cycle,
      $education_level_class,
      $discipline_a,
      $discipline_b,
      $discipline_c,
      $discipline_c_key,
      $subtitle_key,
      $details
    );

    // add proposals choosen to the default curriculum
    $form_state['values']['proposals'] = trim($form_state['values']['proposals']);

    $educa_proposals = explode('|$|', $form_state['values']['proposals']);
    if (!empty($educa_proposals) && !empty($form_state['values']['proposals'])) {

      $lom_classification = unserialize($_SESSION['lom_classification'][$this->sessionIdent]);

      $class_name = 'ArchibaldCurriculumEduca';
      $educa_handler = new $class_name();
      if ($educa_handler->isActive()) {
        if (!empty($lom_classification)) {
          $educa_handler->setLomClassification($lom_classification);
        }
        /* @var $educa_handler curriculum_educa */


        foreach ($educa_proposals as $educa_proposal) {
          list($educational_level, $discipline) = explode('|', $educa_proposal, 2);
          if (!empty($educational_level) && !empty($discipline)) {
            $educational_level_list = $educa_handler->getTaxonomyPathByKey('educa_school_levels', $educational_level, 'key');
            $educa_school_subjects_list = $educa_handler->getTaxonomyPathByKey('educa_school_subjects', $discipline, 'key');
            $educa_handler->addTaxonPathEduca($educational_level_list, $educa_school_subjects_list);
          }
        }
        $_SESSION['lom_classification'][$this->sessionIdent] = serialize($educa_handler->getLomClassification());

        $form_state['reload_other_curriculums'] = array('educa');
      }
    }

    $form_state['was_saved'] = TRUE;
  }

  /**
   * ajax callback for filling up last form element
   *
   * @return array
   */
  public function getDisciplineC() {
    // determine first level of education_level
    $education_level = filter_xss($_POST['education_level']);
    foreach ($this->getCurriculum()->education_levels as $education_level_cycle => $education_classes) {

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
    return $this->getCurriculum()->matrix[$education_level][$discipline_a];
  }

  /**
   * ajax callback for getting proposals for default curriculum
   *
   * @return array
   */
  public function getProposals() {
    $proposals = array();

    $per2default = variable_get('archibald_per2default', array());
    $discipline_c = htmlspecialchars_decode(filter_xss($_POST['discipline_c']));

    if (empty($per2default[$discipline_c])) {
      return array('proposals' => $proposals);
    }

    module_load_include('php', 'archibald', 'includes/curriculum/curriculum_educa.class');
    $educa_handler = new ArchibaldCurriculumEduca();

    foreach ($per2default[$discipline_c] as $raw_mapping_obj) {
      $key = $raw_mapping_obj->educational_level . '|' . $raw_mapping_obj->discipline;
      $educational_level_list = $educa_handler->getTaxonomyPathByKey('educa_school_levels', $raw_mapping_obj->educational_level, 'name');
      $educa_school_subjects_list = $educa_handler->getTaxonomyPathByKey('educa_school_subjects', $raw_mapping_obj->discipline, 'name');
      $proposals[$key] = $educa_handler->themeVal($educational_level_list, $educa_school_subjects_list);
    }

    return array('proposals' => $proposals);
  }

  /**
   * get full curriculum dataset
   * gets curriculum from drupal variables db or
   * get it fresh from internet if the time has come
   *
   * @param $reset boolean
   *   default:FALSE
   */
  public function getCurriculum($reset = FALSE) {
    $cache_key = 'archibald_portal_curriculum_per';
    $cache = cache_get($cache_key, 'cache');
    if (!empty($cache->data) && $reset == FALSE) {
      $this->curriculum = $cache->data;
      return $this->curriculum;
    }

    $xml_file_url = variable_get('archibald_curriculum_' . $this->curriculumIdentKey . '_curriculum_url');

    if (empty($xml_file_url)) {
      throw new Exception('getCurriculum: no url was given');
    }

    $xml_string = @file_get_contents($xml_file_url);

    if (empty($xml_string)) {
      throw new Exception('getCurriculum: invalid url: ' . $xml_file_url);
    }

    $xml = simplexml_load_string(mb_convert_encoding($xml_string, 'UTF-8'));

    if (!$xml) {
      $error_messages = array();
      foreach (libxml_get_errors() as $error) {
        $error_messages[] = $error->message;
      }
      throw new Exception('getCurriculum: can not parse xml: ' . "\n" . implode("\n", $error_messages));
    }

    if (empty($xml->education_levels->education_level)) {
      throw new Exception('getCurriculum: invlaide xml $xml->education_levels->education_level not found');
    }

    if (empty($xml->subtitles->code)) {
      throw new Exception('getCurriculum: invlaide xml $xml->subtitles->code not found');
    }

    $this->curriculum = new stdClass();
    $this->curriculum->education_levels = array();
    foreach ($xml->education_levels->education_level as $education_level_cycle) {
      foreach ($education_level_cycle->education_level as $education_level_class) {
        $this->curriculum->education_levels[(string) $education_level_cycle->attributes()->name][] = (string) $education_level_class->attributes()->name;
      }
    }

    $this->curriculum->matrix = array();
    foreach ($xml->code_matrix->domain as $domain) {
      foreach ($domain->discipline as $discipline) {
        foreach ($discipline->theme as $theme) {
          foreach ($theme as $cycle) {
            $val = new stdClass();
            $val->code = (string) $cycle;
            $val->url_part = '';
            $val->description = '';
            if (!empty($val->code) && drupal_strtolower($val->code) != 'x') {
              $objectiv = $xml->xpath('//objectiv_codes/code[@code="' . $val->code . '"]');
              if (!empty($objectiv)) {
                $val->url_part = (string) $objectiv[0]->attributes()->url_part;
                $val->description = (string) $objectiv[0]->description;
              }

              $this->curriculum->matrix[(string) $cycle->attributes()->name][(string) $domain->attributes()->name][(string) $discipline->attributes()->name][(string) $theme->attributes()->name] = $val;
            }
          }
        }
      }
    }

    $this->curriculum->subtitle = array();
    foreach ($xml->subtitles->code as $code) {
      if (!empty($code->attributes()->code)) {
        $code_string = (string) $code->attributes()->code;
      }
      else {
        $code_string = NULL;
      }

      if (!empty($code->subtitle) && !empty($code_string)) {
        $this->curriculum->subtitle[$code_string] = $this->getCurriuculumSubtitle($code->subtitle, $code_string);
      }
    }

    // set cache for 12 hours
    cache_set($cache_key, $this->curriculum, 'cache', (time() + (60 * 60 * 12)));

    return $this->curriculum;
  }

  /**
   * extract a subtitle item from SimpleXMLElement
   *
   * @param SimpleXMLElement $items
   * @param string $tree_string
   *   string to identify the tree path
   *
   * @return array
   */
  protected function getCurriuculumSubtitle(SimpleXMLElement $items, $tree_string) {
    $subtitle_lvl_1 = NULL;

    $subtitles = array();
    foreach ($items as $item) {
      $subtitle           = new stdClass();
      $subtitle->title    = (string) $item->title;
      $subtitle->url_part = (string) $item->attributes()->url_part;
      $subtitle->uuid     = md5($tree_string . '/' . $item->title);
      $subtitle->childs   = array();
      $subtitle->details  = array();

      if (!empty($item->subtitle)) {
        $subtitle->childs = $this->getCurriuculumSubtitle($item->subtitle, $tree_string . '|' . $item->title);
      }

      if (!empty($item->details)) {
        foreach ($item->details as $detail_item) {
          $detail              = new stdClass();
          $detail->url_part    = (string) $detail_item->attributes()->url_part;
          $detail->text        = (string) $detail_item->text;
          $detail->school_year = array();
          if (!empty($detail_item->school_year)) {
            foreach ($detail_item->school_year as $school_year_item) {
              $detail->school_year[] = (string) $school_year_item;
            }
          }
          $subtitle->details[] = $detail;
        }
      }

      $subtitles[] = $subtitle;
    }

    return $subtitles;
  }

  /**
   * setup callback with will called from install process
   * this one installes the per to default curriculum mapping
   * from csv to database
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
    $xml_file_url = variable_get('archibald_curriculum_' . $this->curriculumIdentKey . '_curriculum_url');
    if (!empty($xml_file_url)) {
      try {
        $this->getCurriculum(TRUE);
      }
      catch (Exception $e) {

      }
    }
  }

  /**
   * aajx callback to load subtitles filtertd by Objectiv Code
   *
   * @return array
   */
  public function getSubtitles($discipline_id = NULL) {
    $subtitles = array();

    if ($discipline_id == NULL && !empty($_POST['discipline_c'])) {
      $discipline_id = filter_xss($_POST['discipline_c']);
    }

    if (empty($discipline_id)) {
      $discipline_id = '';
    }

    if (!empty($this->getCurriculum()->subtitle[$discipline_id])) {
      return array('subtitle' => $this->getCurriculum()->subtitle[$discipline_id]);
    }
    return array('subtitle' => array());
  }

  /**
   * get objectiv by code
   * @staticvar array $objectiv_codes
   *
   * @param string $code
   *
   * @return object
   *  url_part
   *  description
   *  code
   */
  public function getObjectivByCode($code) {
    $objectiv_codes = &archibald_static(__FUNCTION__, array());
    if (empty($objectiv_codes)) {
      foreach ($this->getCurriculum()->matrix as $cycles => $domains) {
        foreach ($domains As $domain => $disciplines) {
          foreach ($disciplines as $discipline => $themes) {
            foreach ($themes as $theme => $val) {
              $objectiv_codes[$val->code] = $val;
            }
          }
        }
      }
    }
    return $objectiv_codes[$code];
  }

  /**
   * build a full url from url_part
   *
   * @param string $url_part
   *
   * @return string
   */
  public function buildUrl($url_part) {
    $elements = explode('#', $url_part);

    $url = self::PER_BASE_URL . $elements[0] . '/';
    if (!empty($elements[1])) {
      $url .= '#' . $elements[1];
    }

    return $url;
  }

  /**
   * index current curriculum into solr index
   * for fitlering following multy strings will filled up
   *  sm_per_cycle
   *  sm_per_domain
   *  sm_per_objectiv_code
   *  sm_per_discipline
   *
   * all text elements will addet to vocabular fulltext collection
   *  tm_vocabular_text
   *
   * @param ApacheSolrDocument $document
   */
  public function solrIndex(ApacheSolrDocument &$document) {
    foreach ($this->getEntrys() as $cycle => $domains) {
      // @var string $cycle
      $document->setMultiValue('sm_per_cycle', trim($cycle));
      foreach ($domains as $domain => $disciplines) {
        // @var string $domain
        $document->setMultiValue('tm_vocabular_text', trim($domain));
        $document->setMultiValue('sm_per_domain', trim($domain));
        foreach ($disciplines as $objectiv_code => $objectiv) {
          // @var string $objectiv->code
          // @var string $objectiv->discipline
          // @var string $objectiv->description
          // @var string $objectiv->object
          // @var string $objectiv->url_part
          $document->setMultiValue('sm_per_objectiv_code', trim($objectiv->code));
          $document->setMultiValue('sm_per_discipline', trim($objectiv->discipline));
          $document->setMultiValue('tm_vocabular_text', trim($objectiv->object));
          $document->setMultiValue('tm_vocabular_text', trim($objectiv->discipline));
          $document->setMultiValue('tm_vocabular_text', trim($objectiv->description));

          if (!empty($objectiv->object_elements)) {
            foreach ($objectiv->object_elements as $subtitle1) {
              // @var string $subtitle1->subtitle  'GENERAL'
              // @var string $subtitle1->url_part

              if (!empty($subtitle1->subtitle) && $subtitle1->subtitle != 'GENERAL') {
                $document->setMultiValue('tm_vocabular_text', trim($subtitle1->subtitle));
              }


              if (!empty($subtitle1->details)) {
                foreach ($subtitle1->details as $detail) {
                  // @var string $detail->text
                  // @var string $detail->school_year
                  // @var string $detail->url_part


                  if (!empty($detail->text)) {
                    $document->setMultiValue('tm_vocabular_text', trim($detail->text));
                  }

                  if (!empty($detail->school_year)) {
                    $document->setMultiValue('tm_vocabular_text', trim($detail->school_year));
                  }
                }
              }
              if (!empty($subtitle1->childs)) {
                foreach ($subtitle1->childs as $subtitle2) {
                  // @var string $subtitle2->subtitle  'GENERAL'
                  // @var string $subtitle2->url_part

                  if (!empty($subtitle2->subtitle) && $subtitle2->subtitle != 'GENERAL') {
                    $document->setMultiValue('tm_vocabular_text', trim($subtitle2->subtitle));
                  }

                  if (!empty($subtitle2->details)) {
                    foreach ($subtitle2->details as $detail) {
                      // @var string $detail->text
                      // @var string $detail->school_year
                      // @var string $detail->url_part

                      if (!empty($detail->text)) {
                        $document->setMultiValue('tm_vocabular_text', trim($detail->text));
                      }

                      if (!empty($detail->school_year)) {
                        $document->setMultiValue('tm_vocabular_text', trim($detail->school_year));
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  /**
   * get settings configuration form
   *
   * @return array
   *   drupal form
   */
  public function getSettingsForm() {
    $form = parent::getSettingsForm();

    $form['curriculum_url'] = array(
      '#type' => 'textfield',
      '#title' => t('URL to curriculum XML'),
      '#default_value' => variable_get('archibald_curriculum_' . $this->curriculumIdentKey . '_curriculum_url', ''),
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
    parent::validateSettingsForm($form_values, $form_state);

    if (!empty($form_values['activate'])) {
      if (empty($form_values['curriculum_url']) || !valid_url($form_values['curriculum_url'])) {
        form_set_error(
          'curriculum_fieldset][' . $this->curriculumIdentKey . '][curriculum_url',
          t('Please enter a valid URL.')
        );
      }
      else {
        variable_set('archibald_curriculum_' . $this->curriculumIdentKey . '_curriculum_url', $form_values['curriculum_url']);

        try {
          $this->getCurriculum(TRUE);
        }
        catch (Exception $e) {
          form_set_error(
            'curriculum_fieldset][' . $this->curriculumIdentKey . '][curriculum_url',
            $e->getMessage()
          );

          variable_set('archibald_curriculum_' . $this->curriculumIdentKey . '_curriculum_url', NULL);
        }
      }
    }
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
    parent::submitSettingsForm($form_values, $form_state);

    variable_set('archibald_curriculum_' . $this->curriculumIdentKey . '_curriculum_url', $form_values['curriculum_url']);
  }
}

