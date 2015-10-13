<?php
/**
 * @file
 * all general theems for archibald module.
 *
 */

/**
 * Process variables for archibald_content_overview_row.tpl.php
 *
 * @param array $variables
 *   called by reference
 *
 * The $variables array contains the following arguments:
 * - $row
 *    lom_id
 *    version
 *    title
 *    description
 *    default_language
 *    status
 *    preview_image
 *
 * see archibald_content_overview_row.tpl.php
 */
function template_preprocess_archibald_content_overview_row(&$variables) {
  global $language;

  foreach ($variables['row'] as $k => $v) {
    $variables[$k] = $v;
  }
  unset($variables['row']);
  if ($variables['title'] instanceof ArchibaldLomDataLangString) {
    $variables['title'] = ArchibaldLom::getLangstringText(
      $variables['title'], $variables['default_language']
    );
  }
  if ($variables['description'] instanceof ArchibaldLomDataLangString) {
    $variables['description'] = ArchibaldLom::getLangstringText(
      $variables['description'], $variables['default_language']
    );
  }

  if (!isset($variables['preview_image_fid'])) {
    preg_match('/archibald_file\/([0-9]+)\//', $variables['preview_image'], $old_fid);
    $variables['preview_image_fid'] = isset($old_fid[1]) ? $old_fid[1] : NULL;
  }

  // load logo file from der db
  if (!empty($variables['company_logo'])) {
    $variables['company_logo'] = file_load($variables['company_logo']);
  }

  $variables['img'] = '';
  $file = NULL;
  if (!empty($variables['preview_image_fid'])) {
    $file = file_load((INT)$variables['preview_image_fid']);
  }

  if (!empty($file->uri)) {
    $variables['preview_image_img'] = theme(
      'image_style',
      array(
        'style_name' => 'archibald_preview',
        'path' => $file->uri,
        'getsize' => FALSE,
      )
    );
  }
  else {
    $variables['preview_image_img'] = theme(
      'image',
      array(
        'style_name' => 'archibald_preview',
        'path' => drupal_get_path('module', 'archibald') . '/images/standardbild_' . $language->language . '_90x90.png',
      )
    );
  }
  if (drupal_strlen($variables['title']) > 39) {
    $variables['title'] = truncate_utf8($variables['title'], 35, TRUE) . ' ...';
  }

  if (!empty($variables['company_logo'])) {
    $variables['company_logo'] = theme(
      'image_style',
      array(
        'style_name' => 'archibald_contributor',
        'path' => $variables['company_logo']->uri,
        'getsize' => FALSE,
        'attributes' => array(
          'class' => 'archibald_content_overview_company_logo',
          'fid' => $variables['company_logo']->fid,
        ),
      )
    );
  }

  $variables['description'] = truncate_utf8(
    check_plain(
      strtr(
        $variables['description'],
        array(
          "\n" => ' ',
          "\t" => '',
        )
      )
    ),
    200,
    TRUE,
    TRUE
  );
}


/**
 * Process variables for archibald_view_curriculum_educa.tpl.php.
 *
 * @param array $variables
 *  caled by reference
 */
function template_preprocess_archibald_content_view_curriculum_educa(&$variables) {
  $variables['educa_curriculum'] = array();
  // echo '<pre>';
  // print_r( $variables['entrys'] );
  // die();
  foreach ($variables['entrys'] as $entry) {
    $level0 = (!empty($entry->educationalLevel[0])) ? trim($entry->educationalLevel[0]) : '-';
    $level1 = (!empty($entry->educationalLevel[1])) ? trim($entry->educationalLevel[1]) : '-';
    $level2 = (!empty($entry->educationalLevel[2])) ? trim($entry->educationalLevel[2]) : '-';

    // remove "school grade" sufix and place it as "Class" if it is empty
    $level1_sufix = '';
    if (preg_match('/\((.+)\)$/', $level1, $matches)) {
      $level1_sufix = $matches[1];
      $level1 = trim(strtr($level1, array($matches[0] => '')));
    }
    if ($level2 == '-' && !empty($level1_sufix)) {
      $level2 = $level1_sufix;
    }

    // build up recursiv array
    $variables['educa_curriculum'][$level0]['data'][$level1]['data'][$level2]['data'][] = $entry;

    // calculate entrys
    @$variables['educa_curriculum'][$level0]['data'][$level1]['data'][$level2]['count']++;
    @$variables['educa_curriculum'][$level0]['data'][$level1]['count']++;
    @$variables['educa_curriculum'][$level0]['count']++;
  }
}

/**
 * Process variables for archibald_view_curriculum_per.tpl.php.
 *
 * @param array $variables
 *  caled by reference
 */
function template_preprocess_archibald_content_view_curriculum_per(&$variables) {
  module_load_include('php', 'archibald', 'includes/curriculum/curriculum_per.class');

  $variables['per_curriculum'] = array();
  $variables['level0_rows'] = 0;
  foreach ($variables['entrys'] as $cycle => $domains) {
    $level1 = $cycle;
    foreach ($domains as $domain => $disciplines) {
      //$level2 = $domain;
      foreach ($disciplines as $objectiv_code => $objectiv) {

        $possible_school_year = array();
        foreach ($objectiv->object_elements as $subtitle1) {
          if (!empty($subtitle1->details)) {
            foreach ($subtitle1->details as $detail) {
              if (!empty($detail->school_year)) {
                $possible_school_year[$detail->school_year] = $detail->school_year;
              }
            }
          }

          foreach ($subtitle1->childs as $subtitle2) {
            if (!empty($subtitle2->details)) {
              foreach ($subtitle2->details as $detail) {
                if (!empty($detail->school_year)) {
                  $possible_school_year[$detail->school_year] = $detail->school_year;
                }
              }
            }
          }
        }

        if (empty($possible_school_year)) {
          $level2 = '-';
        }
        else {
          $level2 = implode(' <br /> ', $possible_school_year);
        }

        // build up recursiv array
        @$variables['per_curriculum'][$level1]['data'][$level2]['data'][] = $objectiv;

        // calculate entrys
        @$variables['per_curriculum'][$level1]['data'][$level2]['count']++;
        @$variables['per_curriculum'][$level1]['count']++;
        $variables['level0_rows'] ++;
      }
    }
  }
}

/**
 * Process variables for archibald_view_curriculum_per.tpl.php.
 *
 * @param array $variables
 *  caled by reference
 */
function template_preprocess_archibald_content_view_curriculum_per_details(&$variables) {
  drupal_add_library('system', 'ui.dialog');
  $variables['curriculum_per'] = new ArchibaldCurriculumPer();

  $variables['possible_school_year'] = array();
  foreach ($variables['objectiv']->object_elements as $subtitle1) {
    if (!empty($subtitle1->details)) {
      foreach ($subtitle1->details as $detail) {
        if (!empty($detail->school_year)) {
          $variables['possible_school_year'][$detail->school_year] = $detail->school_year;
        }
      }
    }

    foreach ($subtitle1->childs as $subtitle2) {
      if (!empty($subtitle2->details)) {
        foreach ($subtitle2->details as $detail) {
          if (!empty($detail->school_year)) {
            $variables['possible_school_year'][$detail->school_year] = $detail->school_year;
          }
        }
      }
    }
  }

  sort($variables['possible_school_year']);

  preg_match('/^([A-Z]+)/', $variables['objectiv']->code, $matches);
  $variables['css_base_code'] = $matches[1];
}


/**
 * Process variables for archibald_content.tpl.php
 *
 * Most themes utilize their own copy of archibald_content.tpl.php.
 * The default is located inside
 *  "site/all/modules/archibald/archibald_content.tpl.php".
 * Look in there for the full list of variables.
 *
 * The $variables array contains the following arguments:
 * - $lom  the full lom objct
 * - $lom_general
 * - $lom_lifeCycle
 * - $lom_metaMetadata
 * - $lom_technical
 * - $lom_education
 * - $lom_rights
 * - $lom_relations
 * - $lom_annotation
 * - $lom_classification
 *
 * see archibald_content.tpl.php
 */
function template_preprocess_archibald_content_view(&$variables) {
  global $language, $languages;
  include dirname(__FILE__) . '/languages/' . $language->language . '.php';

  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/archibald.css');
  drupal_add_css(drupal_get_path('module', 'archibald') . '/css/jquery.ui.tabs.css');
  drupal_add_js('misc/ui/jquery.ui.widget.min.js');
  drupal_add_js('misc/ui/jquery.ui.tabs.min.js');

  drupal_add_library('system', 'ui');

  archibald_get_lom_for_template($variables['lom'], $variables);

  $variables['curriculums'] = array();
  foreach (archibald_get_active_curriculums() as $source) {
    $curr_name = 'curriculum_' . $source;
    $class_name = 'ArchibaldCurriculum' . drupal_ucfirst($source);
    if (module_load_include('php', 'archibald', 'includes/curriculum/' . $curr_name . '.class') !== FALSE) {
      $curr_handler = new $class_name();
      if ($curr_handler instanceof ArchibaldCurriculumAdvanced) {
        $curriculum = $variables['lom']->getCurriculum();
        $curr_handler->setLomCurriculum(
          $curriculum
        );
      }
      else {
        // $curr_handler instanceof ArchibaldCurriculumBasic
        $tmp_classification = $variables['lom']->getClassification();
        $curr_handler->setLomClassification(
          $tmp_classification
        );
      }

      $variables['curriculums'][$source]['name'] = $curr_handler->getName();
      $variables['curriculums'][$source]['entrys'] = $curr_handler->getEntrys();
    }
  }

  $variables['useful_ressource_links'] = array();
  $variables['locations'] = $variables['lom']->getTechnical()->getLocation();

  $identifiers = $variables['lom']->getGeneral()->getIdentifier();

  foreach ($identifiers as $identifier) {
    if ($identifier->catalog == 'URL') {
      $variables['useful_ressource_links'][] = $identifier;
    }
  }

  // echo '<PRE>';
  // print_r( $variables['useful_ressource_links'] );

  if (empty($variables['useful_ressource_links']) && !empty($variables['locations'])) {
    foreach ($variables['locations'] as $lk => $location) {
      if (strtoupper($location->getType()) == 'URL') {
        $identifier =  new ArchibaldLomDataIdentifier(
            'URL',
            $location->getValue(),
            ''
        );
        $variables['useful_ressource_links'][] = $identifier;
        $variables['extra_identifier'] = $location->getValue();
        break;
      }
    }
  }

  // print_r( $variables['useful_ressource_links'] );
  // die();

  if (!empty($variables['preview_image_fid'])) {
    $file = file_load((INT) $variables['preview_image_fid']);


    if (count($variables['useful_ressource_links']) == 1 && (count($identifiers) == 1 || !empty($variables['extra_identifier']))) {
      reset($variables['useful_ressource_links']);

      $variables['preview_image_img'] = theme(
        'archibald_lom_identifier',
        array(
          'identifier' => current($variables['useful_ressource_links']),
          'just_link' => TRUE,
          'force_title' => theme(
            'image_style', array(
              'style_name' => 'archibald_details',
              'path' => $file->uri,
              'getsize' => FALSE,
              'attributes' => array(
                'fid' => $file->fid,
                'class' => 'preview_image_img',
              ),
            )
          ),
        )
      );
    }
    else {
      $variables['preview_image_img'] = theme(
        'image_style', array(
          'style_name' => 'archibald_details',
          'path' => $file->uri,
          'getsize' => FALSE,
          'attributes' => array(
            'fid' => $file->fid,
            'class' => 'preview_image_img',
          ),
        )
      );
    }
  }
  else {
    $variables['preview_image_img'] = theme(
      'image', array(
        'path' => drupal_get_path('module', 'archibald') . '/images/standardbild_' . $language->language . '_170x170.png',
        'attributes' => array(
          'class' => 'preview_image_img',
        ),
      )
    );
  }

  if (!empty($variables['company_logo'])) {
    $variables['company_logo'] = theme(
      'image_style', array(
        'style_name' => 'archibald_contributor',
        'path' => $variables['company_logo']->uri,
        'getsize' => FALSE,
        'attributes' => array(
          'class' => 'archibald_content_view_company_logo',
          'fid' => $variables['company_logo']->fid,
        ),
      )
    );
  }
  else {
    $variables['company_logo'] = '';
  }

  $variables['contributors_grouped'] = array();
  foreach ($variables['lifecycle_contributors'] as $contributor_id => $contributor) {
    $variables['contributors_grouped'][$contributor['role']->getValue()][$contributor_id] = $contributor;
  }

  $variables['version'] = check_plain(
    ArchibaldLom::getLangstringText(
      $variables['lom']->getLifeCycle()->getVersion(),
      '',
      TRUE
    )
  );


  $typical_age_range = $variables['lom']->getEducation()->getTypicalAgeRange();
  if (empty($typical_age_range)) {
    $variables['typical_age_range'] = '-';
  }
  else {
    foreach ($typical_age_range as $lang) {

      list($from, $to) = explode('-', ArchibaldLom::getLangstringText($lang), 2);
      if (empty($from) && empty($to)) {
        $variables['typical_age_range'] = '-';
      }
      elseif (empty($from)) {
        $variables['typical_age_range'] = '-' . $to;
      }
      elseif (empty($to)) {
        $variables['typical_age_range'] = $from . '+';
      }
      else {
        $variables['typical_age_range'] = $from . '-' . $to;
      }
      $variables['typical_age_range'] .= '<br />';
    }
  }

  /*
  // LOM 1.0
  .......##........#######..##.....##.......##.........#####..
  .......##.......##.....##.###...###.....####........##...##.
  .......##.......##.....##.####.####.......##.......##.....##
  .......##.......##.....##.##.###.##.......##.......##.....##
  .......##.......##.....##.##.....##.......##.......##.....##
  .......##.......##.....##.##.....##.......##...###..##...##.
  .......########..#######..##.....##.....######.###...#####..
  */
  $lizenz = ArchibaldLom::getLangstringText($variables['lom']->getRights()->getDescription());

  // If lizenz is empty, this is a description which last modified date is prior to changes in lom 1.0 that made this field mandatory. We used to show 'unknown'. Now should be 'All rights reserved' by default.
  if (empty($lizenz)) {
    $lizenz = archibald_get_term_by_key('all rights reserved', 'rights_licenses', TRUE);

    // Make sure the key exists in Drupal's taxonomies (may have been overwritten by the user)
    if (is_array($lizenz)) {
      $lizenz = $lizenz[$language->language];
    } else {
      $lizenz = t('All rights reserved');
    }

    $variables['lizenz'] = $lizenz;
  }
  else {
    $lizenz_title = archibald_get_term_by_key($lizenz, 'rights_licenses', FALSE, NULL, $lizenz_url);

    if ($lizenz == $lizenz_title) {
      $lizenz_title = '';
    }

    if (!empty($lizenz_title)) {
      if (drupal_substr($lizenz, 0, 1) == '/' || drupal_substr($lizenz, 0, 4) == 'http') {
        $variables['lizenz'] = '<a href="' . $lizenz . '" target="_blank">' . $lizenz_title . '</a>';
      }
      else {
        if (!empty($lizenz_url)) {
          $variables['lizenz'] = l($lizenz_title, $lizenz_url, array('attributes' => array('target' => '_blank')));
        }
        else {
          $variables['lizenz'] = $lizenz_title;
        }
      }
    }
    else {
      $variables['lizenz'] = check_markup($lizenz, 'markdown');
    }
  }

  $variables['relations'] = $variables['lom']->getRelation();

}

/**
 * prepare lom object for to be displayed in template
 * whill called by template_preprocess_hook()
 *
 * @global Object $language
 *   drupal default language object
 *
 * @param ArchibaldLom $lom
 *   the lom object to read out (passed by reference)
 *
 * @param array $variables
 *   the array in which the data will be saved (passed by reference)
 */
function archibald_get_lom_for_template(&$lom, &$variables) {
  global $language;

  $variables['lom'] = $lom;
  $variables['lom_title'] = '';
  $lang_string = $lom->getGeneral()->getTitle();
  if ($lang_string instanceof ArchibaldLomDataLangString) {
    $archibald_lom = new ArchibaldLom();
    $variables['lom_title'] = filter_xss($archibald_lom->determinTitle($lang_string, $language->language));
  }

  $variables['lom_description'] = '';
  $lang_string = $lom->getGeneral()->getDescription();

  if ($lang_string instanceof ArchibaldLomDataLangString) {
    $variables['lom_description'] = $lang_string->getString($language->language);
  }

  preg_match('/archibald_file\/([0-9]+)\//', $lom->getTechnical()->getPreviewImage()->getImage(), $old_fid);

  $copy_desc = $lom->getTechnical()->getPreviewImage()->getCopyrightDescription();
  $variables['preview_image_description_full'] = $copy_desc;
  if (strlen($copy_desc) > 49) $copy_desc = truncate_utf8($copy_desc, 45, TRUE) . ' ...';
  $variables['preview_image_description'] = $copy_desc;
  $variables['preview_image_fid'] = isset($old_fid[1]) ? $old_fid[1] : NULL;
  $variables['lifecycle_contributors'] = array();
  $lifecycle_contribute = $lom->getLifeCycle()->getContribute();
  foreach ($lifecycle_contribute as $contributor_id => $contribute) {
    if (!isset($contribute->entity[0])) {
      continue;
    }

    $parser = new ArchibaldAppDataVcard_Parser($contribute->entity[0]);
    $vcard  = $parser->parse();
    $vcard  = $vcard[0];

    $logo = NULL;
    if (isset($vcard->logo->uri)) {
      preg_match('/archibald_file\/([0-9]+)\//', $vcard->logo->uri, $tmp_fid);
      if (!empty($tmp_fid[1])) {
        $logo = file_load($tmp_fid[1]);
      }
    }

    $variables['lifecycle_contributors'][$contributor_id] = array(
      'role' => $contribute->getRole(),
      'vcard' => $vcard,
      'logo' => $logo,
    );
  }

  archibald_get_lom_for_template_company_logo($variables);
  // load logo if found a fid
  if (!empty($variables['company_logo'])) {
    $variables['company_logo'] = file_load($variables['company_logo']);
  }
}

/**
 * determine company logo
 * whill called by template_preprocess_hook()
 *
 * @param array $variables
 */
function archibald_get_lom_for_template_company_logo(&$variables) {
  // determine company logo by processing all metaMetadata.contribute
  $variables['company_logo'] = NULL;
  /**
   * HH 2012-05-04 vcard icons are not longer supported
   * CHA 2012-06-15 vcard enabled again
   */
  $mmd_contributes = array();
  try {
    $meta_meta_data = @$variables['lom']->getMetaMetadata();
    $meta_meta_data->sortContribute('date', 'asc');
    $mmd_contributes = @$meta_meta_data->getContribute();
  } catch (Exception $e) {
  }

  if (!empty($mmd_contributes)) {
    $mmd_contributes = array_reverse($mmd_contributes);
    foreach ($mmd_contributes as $mmd_contribute) {
      if ($mmd_contribute instanceof ArchibaldLomDataContribute) {
        if ($mmd_contribute->getRole()->getValue() == ArchibaldLomDataMetaMetadata::CONTRIBUTE_ROLE_CREATOR && !empty($mmd_contribute->entity[0])) {
          $parser = new ArchibaldAppDataVcard_Parser($mmd_contribute->entity[0]);
          $vcard = $parser->parse();
          if (!empty($vcard[0]) && !empty($vcard[0]->logo->uri) && preg_match('/archibald_file\/([0-9]+)\//', $vcard[0]->logo->uri, $tmp_fid)) {
            if (!empty($tmp_fid[1])) {
              $variables['company_logo'] = $tmp_fid[1];
              // Stop the foreach.
              break;
            }
          }
        }
      }
    }
  }

  // @var ArchibaldLom $variables['lom']
  $status = archibald_get_lom_stats($variables['lom']->getLomId());
  // check if there is a contributor logo
  if (!empty($status->content_partner['icon_fid'])) {
    $variables['company_logo'] = $status->content_partner['icon_fid'];
  }
}


/**
 * Process variables for archibald_search_block_form.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $form
 *
 * see archibald_search_block_form.tpl.php
 */
function template_preprocess_archibald_search_block_form(&$variables) {
  $variables['search'] = array();
  $hidden = array();
  // Provide variables named after form keys so themers can print
  // each element independently.
  foreach (element_children($variables['form']) as $key) {
    $type = $variables['form'][$key]['#type'];
    if ($type == 'hidden' || $type == 'token') {
      $hidden[] = drupal_render($variables['form'][$key]);
    }
    else {
      $variables['search'][$key] = drupal_render($variables['form'][$key]);
    }
  }
  // Hidden form elements have no value to themers. No need for separation.
  $variables['search']['hidden'] = implode($hidden);
  // Collect all form elements to make it easier to print the whole form.
  $variables['search_form'] = implode($variables['search']);
}


/**
 * Theme a ArchibaldLomDataIdentifier
 *
 * @param $variables
 *   An associative array containing:
 *
 *   ArchibaldLomDataIdentifier $identifier
 *
 *   boolean $just_link
 *     just return the link and not HTML
 *
 *   string $force_title
 *     force a special title
 *     optional
 *
 * @return string
 *   HTML
 */
function theme_archibald_lom_identifier($variables) {
  $value = $variables['identifier']->getEntry();
  $type  = $variables['identifier']->getCatalog();
  $title = $variables['identifier']->getTitle();

  if ($title instanceof ArchibaldLomDataLangString) {
    $title = check_plain(ArchibaldLom::getLangstringText($title, '', TRUE));
  }

  switch (drupal_strtoupper($type)) {
    default:
      $type = drupal_strtoupper($type);
      $value .= '<br>' . $title;
      break;

    case 'URL':
      if (empty($title)) {
        $title = t('Link to resource');
      }
      $type = t($type);

      if (strpos($value, '://') === FALSE && drupal_substr($value, 0, 1) != '/') {
        $value = url($value);
      }
      if (!empty($variables['force_title'])) {
        $title = $variables['force_title'];
      }
      $value = '<a href="' . $value . '" target="_blank">' . $title . '</a>';
      break;
  }
  if (isset($variables['just_link']) && $variables['just_link'] == TRUE) {
    return $value;
  }

  return '<tr><td valign="top" width="172">' . $type . '</td><td class="ressource_link">' . $value . '</td></tr>';
}