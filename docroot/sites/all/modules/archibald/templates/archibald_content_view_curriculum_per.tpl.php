<?php

/**
 * @file
 * archibald_content_view_curriculum.tpl.php
 * display curriculum per
 *
 * Parent file:
 *  archibald_content_view.tpl.php
 *
 * Available variables:
 * - $entrys: list of curriculum per entry
 */
?>
<table class="curriculum_per">
  <thead>
    <tr>
        <?PHP if (empty($action) || $action != 'add') { ?><th><?PHP echo t('Context'); ?></th><?PHP } ?>
        <th><?PHP echo t('School grade'); ?></th>
        <th><?PHP echo t('Class'); ?></th>
        <th colspan="2"><?PHP echo t('Discipline and objectives'); ?></th>
        <th><?PHP echo 'C'; ?></th>
        <?PHP if (!empty($action) && $action == 'edit') { ?><th>&nbsp;</th><?PHP } ?>
    </tr>
  </thead>
  <tbody>
<?php
$last_level0    = '';
$last_level1    = '';
$last_level2    = '';
$curriculum_per = new ArchibaldCurriculumPer();
foreach (@$per_curriculum as $level1 => $entrys1) {
  $level1_rows = $entrys1['count'];
  foreach ($entrys1['data'] as $level2 => $entrys2) {
    $level2_rows = $entrys2['count'];
    foreach ($entrys2['data'] as $objectiv) {
      ?>
      <tr>
        <?PHP if ((empty($action) || $action != 'add') && !empty($level0_rows)) { ?><td class="education_level" rowspan="<?PHP echo $level0_rows; ?>"><?PHP echo t('Compulsory education'); ?></td></td><?PHP } ?>
        <?PHP if (!empty($level1_rows)) { ?><td class="school_grade"    rowspan="<?PHP echo $level1_rows; ?>"><?PHP echo $level1; ?></td><?PHP } ?>
        <?PHP if (!empty($level2_rows)) { ?><td class="class"           rowspan="<?PHP echo $level2_rows; ?>"><?PHP echo $level2; ?></td><?PHP } ?>
        <td class="subject_code">
          <?PHP
            $url = $curriculum_per->buildUrl((!empty($objectiv->url_part)) ? $objectiv->url_part : $objectiv->code);
            echo l($objectiv->code, $url, array('attributes' => array('title' => $objectiv->description, 'target' => '_blank')));
          ?>
        </td>
        <td class="subject">
          <b><?PHP echo $objectiv->discipline; ?>, <?PHP echo $objectiv->object; ?></b><br />
          <?PHP echo $objectiv->description; ?>
        </td>
        <td class="detail">
        <?PHP
          if (empty($objectiv->object_elements)) {
            echo '-';
          }
          else {
            echo theme('image', array('path' => drupal_get_path('module', 'archibald') . '/images/info.png'));
          }
        ?>
          <div class="detail_informations">
          <?PHP
            echo theme(
              'archibald_content_view_curriculum_per_details',
              array(
                'objectiv' => $objectiv,
                'action' => @$action,
              )
            );
          ?>
          </div>
        </td>
        <?PHP if (!empty($action) && $action == 'edit') { ?>
        <td class="action-edit">
             <a href="per|<?PHP echo $objectiv->code; ?>" class="archibald_classification_remove" title="<?PHP echo t('Delete curriculum entry'); ?>"><img src="<?PHP echo base_path() . drupal_get_path('module', 'archibald'); ?>/images/delete.png" border="0" /></a>
        </td>
        <?PHP } ?>
      </tr>
<?PHP
    $level0_rows = 0; $level1_rows = 0;
    $level2_rows = 0;
  }
  $last_level2 = $level2;
}
$last_level1 = $level1;
}
?>
  </tbody>
</table>

