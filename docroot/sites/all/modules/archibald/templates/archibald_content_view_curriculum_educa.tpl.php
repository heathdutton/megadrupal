<?php

/**
 * @file
 * archibald_content_view_curriculum_educa.tpl.php
 * display curriculum educa
 *
 * - $educa_curriculum pre processed curriculum tree
 *   template_preprocess_archibald_content_view_curriculum_educa
 * - $action string
 *   NULL or edit when in editing mode
 */
?>
<table class="curriculum_educa">
  <thead>
    <tr>
      <th><?PHP echo t('Context'); ?></th>
      <th colspan="2"><?PHP echo t('School level'); ?></th>
      <th><?PHP echo t('Discipline'); ?></th>
      <?PHP if (!empty($action) && $action == 'edit') { ?><th>&nbsp;</th><?PHP } ?>
    </tr>
  </thead>
  <tbody>
<?php
$last_level0 = ''; $last_level1 = '';
$last_level2 = '';

foreach ($educa_curriculum as $level0 => $entrys0) {
  $level0_rows = $entrys0['count'];

  foreach ($entrys0['data'] as $level1 => $entrys1) {
    $level1_rows = $entrys1['count'];

    foreach ($entrys1['data'] as $level2 => $entrys2) {
      $level2_rows = $entrys2['count'];

      foreach ($entrys2['data'] as $entry) {
        $exactest_discipline = end($entry->discipline);
        ?>
        <tr>
          <?PHP if (!empty($level0_rows)) { ?><td class="education_level" rowspan="<?PHP echo $level0_rows; ?>"><?PHP echo $level0; ?></td><?PHP } ?>
          <?PHP if (!empty($level1_rows)) { ?><td class="school_grade"    rowspan="<?PHP echo $level1_rows; ?>"><?PHP echo $level1; ?></td><?PHP } ?>
          <?PHP if (!empty($level2_rows)) { ?><td class="class"           rowspan="<?PHP echo $level2_rows; ?>"><?PHP echo $level2; ?></td><?PHP } ?>
          <td class="subject">
            <?PHP echo $exactest_discipline; ?>
            <div class="subject_path">
            <?PHP
            $dis_prefix = '';
            foreach ($entry->discipline as $disciplin) {
              echo $dis_prefix . $disciplin . "<br \>";
              $dis_prefix .= '&nbsp; &nbsp;';
            }
            ?>
            </div>
          </td>
          <?PHP if (!empty($action) && $action == 'edit') { ?>
          <td class="action-edit">
            <a href="educa|<?PHP echo $entry->key; ?>" class="archibald_classification_remove" title="<?PHP echo t('Delete curriculum entry'); ?>"><img src="<?PHP echo base_path() . drupal_get_path('module', 'archibald'); ?>/images/delete.png" border="0" /></a>
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
  $last_level0 = $level0;
}
?>
  </tbody>
</table>

