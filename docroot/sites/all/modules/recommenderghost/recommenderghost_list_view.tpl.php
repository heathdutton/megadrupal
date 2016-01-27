<?php
/**
 * @file
 * Template file for RecommenderGhost
 *
 * Standard layout for list view of recommendations.
 * To customize this block's layout just copy it into your template folder:
 * /sites/all/themes/{yourtheme}/template/recommenderghost-list-view.tpl.php
 * and adapt it to your needs.
 */
?>

<div id="recommenderghostList">
  <ul>

<?php
  foreach ($items as $item) :
?>
    <li class="recommenderghost recommenderghost-list-item">
      <a href="<?php print $item["url"]; ?>">
        <?php print $item["description"]; ?><!-- value: <?php print $item["value"]; ?> -->
      </a>
    </li>
<?php
  endforeach;
?>

  </ul>
</div>
