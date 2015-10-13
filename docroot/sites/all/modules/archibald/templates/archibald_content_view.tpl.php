<?php
  /**
   * @file
   * archibald_content_view.tpl.php
   * Display detail page of a lom ressource
   *
   * - $lom: ArchibaldLom Object
   * - $lom_title: ArchibaldLom general.title as string
   * - $lom_description: ArchibaldLom general.description as string (uncutted)
   * - $preview_image_fid: ArchibaldLom technical.preview_image Image fid (file id)
   * - $lifecycle_contributors: An multi array like (array( array('role', 'firstname', 'lastname') ))
   * - $is_part: an array with the same values as above (without is_part and has_part array)
   * - $has_part: an array with the same values as above (without is_part and has_part array)
   *
   */
  global $language;
?>

  <div class="lom_details_title">
    <table cellpadding="0">
      <tr>
        <td>
          <h1 class="title" id="page-title">
            <?php
              print $lom_title;
            ?>
          </h1>
        </td>
        <td valign="top">
          <?php
            print $company_logo;
          ?>
        </td>
      </tr>
    </table>
  </div>

  <div class="preview_image_parent">
    <div>
      <?php
        print $preview_image_img;
      ?>
    </div>
    <?php if (strlen($preview_image_description)) : ?>
      <div class="preview_image_copyright_description" title="<?php print htmlspecialchars($preview_image_description_full) ?>">
        <?php print $preview_image_description ?>
      </div>
    <?php endif; ?>
  </div>
  <div>
    <?php

    if (function_exists('archibald_ratings_show')):
      print archibald_ratings_show($lom->lomId);
    endif;

    print check_plain($lom_description);

    /*
      $return_vals = array();
      foreach ($lom->getGeneral()->getCoverage() as $langstring) {
      $return_vals[] = check_plain(ArchibaldLom::getLangstringText($langstring));
      }
      print implode(', ', $return_vals);
     */

    ?>
  </div>
  <div class="clear"></div>
</div>
<!-- /wboxinfo -->

<div class="wbox archibald_view_info_blocks">

  <div class="details">
    <p><a href="JavaScript:void(0);" id="archibald_view_general" class="btndetails inactive remember_choice"><span><?php print archibald_get_field_label('general'); ?></span></a></p>
    <div style="display: block;" class="detailscontainer archibald_view_general">

      <table class="view_general">
        <?php if (count($contributors_grouped)) :
            $types = archibald_get_taxonomy_options(
                'lc_cont_role'
            );
            foreach ($types as $ktype => $vtype) :
              $ktype = explode('|', $ktype);
              $ktype = array_pop($ktype);
              if ( empty($contributors_grouped[$ktype]) ) {
                continue;
              }
              $role = $ktype;
              $contributors = $contributors_grouped[$role];
            ?>
          <tr>
            <td width="172"><?php print archibald_get_term_by_key($role, 'lc_cont_role'); ?></td>
            <td>
              <?php foreach ($contributors as $contributor_id => $array): ?>
                <div class="contributor contributor_<?php print $role; ?>" contributor_id="<?php print $contributor_id; ?>">
                <?php
                  $name = trim($array['vcard']->lastname . ', ' . $array['vcard']->firstname . '; ' . $array['vcard']->organization, ' ,;');
                  if (variable_get('archibald_solr_search_activ', 0) == 1 && module_exists('apachesolr')){
                    switch ($role){
                      default:
                        $query = trim($array['vcard']->lastname . ' ' . $array['vcard']->firstname . ' ' . $array['vcard']->organization);
                        break;
                      case 'author':
                        $query = trim($array['vcard']->firstname . ' ' . $array['vcard']->lastname . ' ' . $array['vcard']->organization);
                        break;
                    }
                  }
                  else {
                    $query = trim($array['vcard']->firstname . ' ' . $array['vcard']->lastname . ' ' . $array['vcard']->organization);
                  }
                  print l($name, 'archibald', array('query' => array('query' => $role . ':"' . $query . '"')));
                ?>
                </div>
              <?php endforeach; ?>
            </td>
          </tr>
          <?php endforeach; ?>
        <?php else: ?>
          <tr>
            <td width="172"><?php print t('Contributors'); ?></td>
            <td>-</td>
          </tr>
        <?php endif; ?>
        <tr>
          <td width="172"><?php print archibald_get_field_label('lifecycle.version'); ?></td>
          <td>
            <?php print (empty($version)) ?  '-' : $version; ?>
          </td>
        </tr>
        <tr>
          <td><?php print archibald_get_field_label('general.language'); ?></td>
          <td>
            <?php
              $l = $lom->getGeneral()->getLanguage();
              $cl = $cs = array();
              foreach ($l as $lk => $lv) {
                switch ( $lv ) {
                  case 'none':
                    $cl[$lk] = $lv;
                    $cs[$lk] = 1;
                    unset( $l[$lk] );
                  break;
                  case 'de':
                    $cl[$lk] = $lv;
                    $cs[$lk] = 2;
                    unset( $l[$lk] );
                  break;
                  case 'fr':
                    $cl[$lk] = $lv;
                    $cs[$lk] = 3;
                    unset( $l[$lk] );
                  break;
                  case 'it':
                    $cl[$lk] = $lv;
                    $cs[$lk] = 4;
                    unset( $l[$lk] );
                  break;
                  case 'rm':
                    $cl[$lk] = $lv;
                    $cs[$lk] = 5;
                    unset( $l[$lk] );
                  break;
                  case 'rm2907':
                    $cl[$lk] = $lv;
                    $cs[$lk] = 6;
                    unset( $l[$lk] );
                  break;
                  case 'rm2908':
                    $cl[$lk] = $lv;
                    $cs[$lk] = 7;
                    unset( $l[$lk] );
                  break;
                  case 'rm16068':
                    $cl[$lk] = $lv;
                    $cs[$lk] = 8;
                    unset( $l[$lk] );
                  break;
                  case 'en':
                    $cl[$lk] = $lv;
                    $cs[$lk] = 9;
                    unset( $l[$lk] );
                  break;
                }
              }

              array_multisort($cs, $cl);
              asort($l);
              $langs = array_merge($cl, $l);

              $return_vals = array();
              foreach ($langs as $langstring) {
                $return_vals[] = archibald_get_language_by_code($langstring);
              }
              print implode(', ', $return_vals);
            ?>
          </td>
        </tr>
        <tr>
          <td><?php print archibald_get_field_label('general.keyword'); ?></td>
          <td>
            <?php
              $return_vals = array();
              foreach ($lom->getGeneral()->getKeyword() as $langstring) {
                $val = ArchibaldLom::getLangstringText($langstring, '', TRUE);
                if (!empty($val)) {
                  $val = l($val, 'archibald', array('query' => array('query' => 'keywords:"' . $val . '"')));
                  $return_vals[] = $val;
                };
              };

              if (empty($return_vals)) {
                print '-';
              }
              else {
                sort( $return_vals );
                print implode('<br />', $return_vals);
              }
            ?>
          </td>
        </tr>
        <tr>
          <td><?php print archibald_get_field_label('general.coverage'); ?></td>
          <td>
            <?php
              $return_vals = array();

              foreach ($lom->getGeneral()->getCoverage() as $langstring) {
                $val = ArchibaldLom::getLangstringText($langstring, '', TRUE);
                if (!empty($val)) {
                  $val = l($val, 'archibald', array('query' => array('query' => 'coverage:"' . $val . '"')));
                  $return_vals[] = $val;
                };
              };

              if (empty($return_vals)) {
                print '-';
              }
              else {
                sort( $return_vals );
                print implode('<br />', $return_vals);
              }
            ?>
          </td>
        </tr>
      </table>



    </div>
  </div>

  <div class="details">
    <p><a href="JavaScript:void(0);" id="archibald_view_education" class="btndetails inactive remember_choice"><span><?php print archibald_get_field_label('education'); ?></span></a></p>
    <div style="display: block;" class="detailscontainer archibald_view_education">
      <table class="view_education">
        <tr>
          <td width="172"><?php print archibald_get_field_label('education.description'); ?></td><td><?php
          $educational_description = check_plain(ArchibaldLom::getLangstringText($lom->getEducation()->getDescription(), '', TRUE));
          if (empty($educational_description)):
            print '-';
          else:
            print $educational_description;
          endif;
          ?></td>
        </tr>
        <tr>
          <td><?php print t('Learning resource type'); ?></td><td>
<?php

// We want to display types in the order specified by the Ontology Server.
$types = array(
  'pedagogical' => archibald_get_taxonomy_options(
    'learning_resourcetype', 'pedagogical'
  ),
  'documentary' => archibald_get_taxonomy_options(
    'learning_resourcetype', 'documentary'
  )
);
$opts = array('documentary', 'pedagogical');
foreach ($opts as $type):
  // $te (type external)
  // $tl (type local - means it is known to the Ontology Server)
  $te = $tl = array();
  $vocs = $lom->getEducation()->getLearningResourceType($type);
  foreach ($vocs as $kk => $vv) {
    $value = $vv->getValue();
    $kk = $vv->getFullKey();
    if( array_key_exists($kk, $types[$type]) ) {
      $tl[$kk] = l($types[$type][$kk], 'archibald', array('query' => array('query' => 'learning_resourcetype:"' . $value . '"')));
    } else {
      // This vocabulary is probably not in the LOM
      $te[] = $value;
    }
  }
  foreach ($types[$type] as $kk => $vv) {
    if( array_key_exists($kk, $tl) ) {
      print( $tl[$kk] ) . '<br>';
    }
  }
  sort( $te );
  foreach ($te as $kk => $vv) {
    print( $vv ) . '<br>';
  }
endforeach;

?>
          </td>
        </tr>
        <?php
          $duration_human = $lom->getTechnical()->duration->getDurationHuman();
        ?>
        <?php if (!is_null($duration_human)) : ?>
          <tr>
            <td><?php print archibald_get_field_label('technical.duration'); ?></td>
            <td><?php echo $duration_human ?></td>
          </tr>
        <?php endif; ?>
        <tr>
          <td><?php print archibald_get_field_label('education.intendedEndUserRole'); ?></td><td >
<?php
$enduser_roles = $lom->getEducation()->getIntendedEndUserRole();
if (empty($enduser_roles)):
  print '-';
else:
  $roles = archibald_get_taxonomy_options(
      'intended_enduserrole'
  );
  $sorter = array();
  $i = 0;
  foreach ($roles as $rkey=>$r):
    $rkey = explode('|', $rkey);
    $rkey = array_pop($rkey);
    $sorter[$rkey] = $i;
    $i++;
  endforeach;
  $sorted = array();
  foreach ($enduser_roles as $erkey=>$er):
    $sorted[] = $sorter[$er->value];
  endforeach;
  array_multisort($sorted, $enduser_roles);
  foreach ($enduser_roles as $voc):
    $value = $voc->getValue();
    $value_human = $value;
    switch ($voc->getSource()):
      case 'LOMv1.0':
      case 'LOM-CHv1.0':
      case 'LOM-CHv1.1':
      case 'LREv3.0':
      case 'LREv4.7':
        $value_human = archibald_get_term_by_key($value, 'intended_enduserrole');
        break;
    endswitch;
    print l($value_human, 'archibald', array('query' => array('query' => 'intendet_enduserrole:"' . $value . '"'))) . '<br />';
  endforeach;
endif;
?>
          </td>
        </tr>
        <tr>
          <td><?php print archibald_get_field_label('education.typicalAgeRange'); ?></td><td><?php print $typical_age_range;?></td>
        </tr>
        <tr>
          <td><?php print archibald_get_field_label('education.difficulty'); ?></td>
          <td>
            <?php
              $value = $lom->getEducation()->getDifficult()->getValue();
              if (empty($value)):
                print '-';
              else:
                $value_human = $value;
                switch ($lom->getEducation()->getDifficult()->getSource()):
                  case 'LOMv1.0':
                  case 'LOM-CHv1.0':
                  case 'LOM-CHv1.1':
                  case 'LREv3.0':
                  case 'LREv4.7':
                    $value_human = archibald_get_term_by_key($value, 'difficulty');
                    break;
                endswitch;
                print l($value_human, 'archibald', array('query' => array('query' => 'difficulty:"' . $value . '"')));
              endif;
            ?>
          </td>
        </tr>
        <tr>
          <td><?php print archibald_get_field_label('education.typicalLearningTime'); ?></td>
          <td>
            <?php
              $value = $lom->getEducation()->getTypicalLearningTime()->getValue();
              if (empty($value)):
                print '-';
              else:
                $value_human = $value;
                switch ($lom->getEducation()->getTypicalLearningTime()->getSource()):
                  case 'LOMv1.0':
                  case 'LOM-CHv1.0':
                  case 'LOM-CHv1.1':
                  case 'LREv3.0':
                  case 'LREv4.7':
                    $value_human = archibald_get_term_by_key($value, 'typical_learning_time');
                    break;
                endswitch;
                print ($value_human);
              endif;
            ?>
          </td>
        </tr>
        <tr>
          <td><?php print archibald_get_field_label('education.aggregationLevel'); ?></td><td><?php

$value = $lom->getGeneral()->getAggregationLevel()->getValue();
$value = explode('|', $value);
$value = ( count($value) > 1 ) ? $value[1] : $value[0];
if (empty($value)):
  print '-';
else:
  $value_human = $value;
  switch ($lom->getGeneral()->getAggregationLevel()->getSource()):
    case 'LOMv1.0':
    case 'LOM-CHv1.0':
    case 'LOM-CHv1.1':
    case 'LREv3.0':
    case 'LREv4.7':
      $value_human = archibald_get_term_by_key($value, 'aggregation_level');
      break;
  endswitch;
  print l($value_human, 'archibald', array('query' => array('query' => 'aggregation_level:"' . $value . '"')));
endif;
?></td>
        </tr>

      </table>



    </div>
  </div>

  <div class="details">
    <p><a href="JavaScript:void(0);" id="archibald_view_curriculum" class="btndetails inactive remember_choice"><span><?php print archibald_get_field_label('classification'); ?></span></a></p>
    <div style="display: block;" class="detailscontainer archibald_view_curriculum">

      <table class="view_curriculum">
        <tr>
          <td width="172"><?php print archibald_get_field_label('classification.context'); ?></td><td>
<?php

// We want to display contexts in the order specified by the Ontology Server.
$vocs = $lom->getEducation()->getContext();
$contexts = archibald_get_taxonomy_options('education_context');
// $ce (context external)
// $cl (context local - means it is known to the Ontology Server)
$cl = $ce = array();
foreach ($vocs as $kk => $vv) {
  $value = $vv->getValue();
  $kk = $vv->getFullKey();
  if( array_key_exists($kk, $contexts) ) {
    $cl[$kk] = l($contexts[$kk], 'archibald', array('query' => array('query' => 'education_context:"' . $value . '"')));
  } else {
    // This vocabulary is probably not in the LOM
    $ce[] = $value;
  }
}
foreach ($contexts as $kk => $vv) {
  if( array_key_exists($kk, $cl) ) {
    print( $cl[$kk] ) . '<br>';
  }
}
sort( $ce );
foreach ($ce as $kk => $vv) {
  print( $vv ) . '<br>';
}
?>
          </td>
        </tr>
      </table>

<?php

function archibald_check_if_curriculum_is_empty($tmp) {


  if (empty($tmp)) {
    return true;
  }

  if (is_object($tmp)) {
    $tmp = (array)$tmp;
  }

  foreach ($tmp AS $val) {
    if (is_object($val)) {
      $val = (array)$val;
    }
    if (!empty($val) && !is_array($val)) {
      return false;
    }

    if (is_array($val)) {
      if (!archibald_check_if_curriculum_is_empty($val)) {
        return false;
      }
    }
  }

  return true;

}

foreach ($curriculums as $cur_name => $curriculum):
  $tmp = (array) $curriculum['entrys'];
  if (!archibald_check_if_curriculum_is_empty($tmp)):
    drupal_add_css(drupal_get_path('module', 'archibald') . '/css/curriculum_' . $cur_name . '.css');
    ?>
      <strong><?php print $curriculum['name']; ?></strong>
    <?php
    print theme('archibald_content_view_curriculum_' . $cur_name, array('entrys' => $curriculum['entrys']));
  endif;
endforeach;
?>


    </div>
  </div>

  <div class="details">
    <p><a href="JavaScript:void(0);" id="archibald_view_access" class="btndetails inactive remember_choice"><span><?php print t('Relations'); ?></span></a></p>
    <div style="display: block;" class="detailscontainer archibald_view_relations">
      <?php
        if (count($relations)):
      ?>
        <?php
          foreach ($relations as $i => $relation):
        ?>
          <table class="archibald_view_relation">
            <tbody>
              <tr>
                <td width="172" valign="top">
                  <?php
                    $voc = $relation->getKind()->getValue();
                    $voc = explode('|', $voc);
                    $voc = array_pop($voc);
                    print archibald_get_term_by_key($voc, 'rel_kind');
                  ?>
                </td>
                <td>
                  <?php
                    $desc = ArchibaldLom::getLangstringText($relation->getDescription(), '', TRUE);
                    if (strlen($desc)):
                  ?>
                    <div>
                        <?php
                          print $desc;
                        ?>
                      </div>
                  <?php
                    endif;
                  ?>
                  <div>
                    <?php
                      $cat = $relation->getCatalog()->getValue();
                      $cat = explode('|', $cat);
                      $cat = strtoupper(array_pop($cat));
                      $value = $relation->getValue();
                      $prepend = $append = '';
                      switch ( $cat ){
                        case 'REL_URL':
                          $prepend = '<a target="_new" href="' . $value . '">';
                          $append = '</a>';
                          $value = t('Link to resource');
                        break;
                        case 'REL_DOI':
                          $prepend = '<span class="archibald_relation_catalog">DOI</span> ';
                        break;
                        case 'REL_ISBN':
                          $prepend = '<span class="archibald_relation_catalog">ISBN</span> ';
                        break;
                      }
                      print $prepend . $value . $append;
                    ?>
                  </div>
                </td>
              </tr>
            </tbody>
          </table>
        <?php
          endforeach;
        ?>
      <?php
        endif;
      ?>
    </div>
  </div>
  <div class="details">
    <p><a href="JavaScript:void(0);" id="archibald_view_access" class="btndetails inactive remember_choice"><span><?php print t('Access to resource'); ?></span></a></p>
    <div style="display: block;" class="detailscontainer archibald_view_access">
      <table class="view_access">
        <tr>
          <td colspan="2" class="archibald_table_heading"><?php print archibald_get_field_label('right'); ?></td>
        </tr>
        <tr>
          <td width="172"><?php print archibald_get_field_label('right.cost'); ?></td><td><?php print t(ucfirst($lom->getRights()->getCost()->getValue())); ?></td>
        </tr>
        <tr>
          <td><?php print archibald_get_field_label('right.description'); ?></td>
          <td>
            <?php print $lizenz; ?>
          </td>
        </tr>
      </table>

      <table class="view_access">
        <tr>
          <td colspan="2" class="archibald_table_heading"><?php print archibald_get_field_label('technical'); ?></td>
        </tr>
        <tr>
            <td width="172"><?php print t('Format'); ?></td><td>
              <?php
                $format = $lom->getTechnical()->getFormat();
                print (empty($format) ? '-' : $format);
              ?>
            </td>
        </tr>
        <tr>
            <td><?php print t('Size'); ?></td>
            <td><?php print ($lom->getTechnical()->getSize() == 0) ? '-' : $lom->getTechnical()->getSizeHuman(); ?></td>
        </tr>
        <tr>
          <td width="172">
            <?php print archibald_get_field_label('technical.otherPlattformRequirements'); ?>
          </td>
          <td>
            <?php
              $other_plattform_requirements = ArchibaldLom::getLangstringText($lom->getTechnical()->getOtherPlattformRequirements(), '', TRUE);
              if (empty($other_plattform_requirements)):
                print '-';
              else:
                print $other_plattform_requirements;
              endif;
            ?>
          </td>
        </tr>
        <tr>
          <td width="172">
            <?php print archibald_get_field_label('technical.location'); ?></td>
          <td>
            <?php
              $empty = TRUE;
              if (!empty($locations)):
                foreach ($locations as $location) {
                  if (strlen(trim($location->getValue())) > 0) {
                    $empty = FALSE;
                    $prepend = $append = '';
                    if( $location->getType() == 'url' ) {
                      $prepend = '<a target="_new" href="' . $location->getValue() . '">';
                      $append = '</a>';
                    }
                    print ('<div>' . $prepend . $location->getValue() . $append . '</div>');
                  }
                }
              endif;

              if ($empty) {
                print ('-');
              }
            ?>
          </td>
        </tr>
      </table>
      <?php
        $identifiers = $lom->getGeneral()->getIdentifier();
        if (count($identifiers) > 0 || !empty($extra_identifier)):
      ?>
        <table>
          <tr>
            <td colspan="2" class="archibald_table_heading"><?php print t('Reference to resources'); ?></td>
          </tr>
          <?php
            if (!empty($extra_identifier)):
          ?>
            <tr>
              <td valign="top" width="172">URL</td>
              <td class="ressource_link">
                <?php
                  print '<a target="new" href="' . $extra_identifier . '">' . t('Link to resource') . '</a>';
                ?>
              </td>
            </tr>
          <?php
            endif;
          ?>
          <?php
            foreach ($identifiers as $id => $identifier):
              print theme('archibald_lom_identifier', array('identifier' => $identifier));
            endforeach;
          ?>
        </table>
      <?php
        endif;
      ?>
    </div>
  </div>

  <?php
// comments module is enabled
  if (function_exists('archibald_comments_paged_list')):
    ?>
    <div class="details">
      <p><a href="JavaScript:void(0);" name="comment-show" id="archibald_view_comments" class="btndetails inactive remember_choice"><span><?php print t('Comments'); ?></span></a></p>
      <div style="display: block;" class="detailscontainer archibald_view_comments commentbox commentContainer">

  <?php
  print archibald_comments_paged_list($lom->lom_id);
  if (user_access('create archibald comments')):
    print archibald_comments_form();
  endif;
  ?>

      </div>
    </div>
<?php endif; ?>

  <small><?php
    print t('This description was recorded by @first_editor at @first_date, last updated at @last_date', array(
      '@first_editor' => $additional['first']['username'],
      '@first_date' => $additional['first']['save_time'],
      '@last_date' => $additional['last']['save_time'],
    ));
?></small>

</div>

<?php
$identifiers = $lom->getGeneral()->getIdentifier();
if (count($identifiers) > 1 || count($useful_ressource_links) != count($identifiers)) {
  print '<div class="ressource_dialog">';
  print '  <div class="title">' . t('Resource') . ' <span id="close_dialog"><img border="0" src="' . base_path() . drupal_get_path('module', 'archibald') . '/images/close.png" align="absmiddle" /></span></div>';
  print '    <div class="content">';
  foreach ($identifiers as $id => $identifier):
    print ' - ';
    if ($identifier->catalog != 'URL'):
      print t($identifier->catalog) . ': ';
    endif;
    print theme('archibald_lom_identifier', array('identifier' => $identifier, 'just_link' => TRUE)) . '<br />';
  endforeach;
  print '  </div></div>';
}
?>


<script type="text/javascript">
  (function ($) {
    $(function() {
      var ressources = $(".ressource_link > a");
      if (ressources.length > 1) {
        $(".preview_image_img").click(function() {
          if ($(".ressource_dialog").css("display") == "block") {
            $(".ressource_dialog").hide();
          }
          else {
            $(".ressource_dialog").show();
          }
        });
      }

      $("#close_dialog").click(function() {
        $(".ressource_dialog").hide();
      });
    });
  })(jQuery);
</script>

