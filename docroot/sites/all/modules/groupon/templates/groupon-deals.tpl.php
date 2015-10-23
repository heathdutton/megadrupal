<?php
/**
 * @file
 * Default theme implementation to display groupon deals.
 *
 * Available variables:
 * - $deals: The array of groupon deals.
 */

?>
<div id='groupon-wrapper'>
  <?php
    foreach ($deals as $key => $value) :
      $title = $value['title'];
      $imageurl = check_url($value['imageurl']);
      $url = check_url($value['url']);
      $link = l($title, $url, array('attributes' => array('target' => '_blank')));
     $pitchhtml = $value['pitchhtml'];
  ?>
    <div class='groupon-row'>
      <div><h4><?php print $link;?></h4><span class='span-img'><img src=<?php echo $imageurl;?> /></span><?php echo $pitchhtml;?></div>
    </div>
  <?php endforeach; ?>
</div>
