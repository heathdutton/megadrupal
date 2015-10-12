<?php
/**
 * @file
 * Template file for RecommenderGhost
 *
 * Standard layout for picture view of recommendations.
 * To customize this block's layout just copy it into your template folder:
 * /sites/all/themes/{yourtheme}/template/recommenderghost-picture-view.tpl.php
 * and adapt it to your needs.
 */
?>

<div id="recommenderghostPictureView">
<?php
  foreach ($items as $item) :
?>
  <div class="recommenderghost-item">
    <a href="<?php print $item["url"]; ?>">
    <div class="recommenderghost-image">
      <img title="<?php print $item["description"]; ?>" src="<?php print $item["imageUrl"]; ?>">
    </div>
    <div class="recommenderghost-description">
      <?php print $item["description"]; ?><!-- value: <?php print $item["value"]; ?> -->
    </div>
    </a>
  </div>
<?php
  endforeach;
?>
</div>
