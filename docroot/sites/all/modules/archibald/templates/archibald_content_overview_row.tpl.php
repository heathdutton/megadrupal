<?php
global $language;

/**
 * @file
 * archibald_content_overview_row.tpl.php
 * Display one row of archibald content overview resultset
 *
 * - $title: ArchibaldLom general.title as string
 * - $description: ArchibaldLom general.description as string (uncutted)
 * - $preview_image: ArchibaldLom technical.preview_image Image URL
 * - $preview_image_fid: ArchibaldLom technical.preview_image Image fid (file id)
 * - $preview_image_img: rendered and themed preview image
 * - $default_language: ArchibaldLom meta-metadata.language Lom Object default language
 * - $status: ArchibaldLom lifecycle.status string
 * - $lom_id: ArchibaldLom id   string(32) md5 sum
 * - $version: lom version id  string(32) md5 sum
 *
 * @parent archibald_content_overview.tpl.php
 */

?>
<div class="archibald_content_overview_row_content archibald_corc_has_img">
    <div class="img_bb" style="width: 90px; height: 90px; display: inline-block; float: left;">
       <?php
        echo $preview_image_img;
      ?>
    </div>
    <?php
    if (!empty($company_logo)) {
      echo $company_logo;
    }
    ?>
     <h3><?php echo l($title, 'archibald/' . $lom_id); ?></h3>
     <span class="description"><?php print $description; ?></span>
     <p class="info">&nbsp;</p>
     <div class="clear"></div>
</div>

