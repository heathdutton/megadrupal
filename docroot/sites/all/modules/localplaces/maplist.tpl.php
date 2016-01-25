<?php
/**
 * @file
 * The homepage/results template that displays listings from Google Places.
 */
?>

<p><?php print $description ?></p>
<div class="results">  
  <?php foreach ($results as $inner): ?>
    <div class="place"><img src="<?php print $inner["icon"] ?>" align="left" title="<?php print $inner["type"] ?>." alt="<?php print $inner["type"] ?> icon." class="icon"/>
      <h3 class="local-places"><a href="<?php print $inner["detailsurl"] ?>"><?php print $inner["name"] ?></a> (<?php print $inner["letter"] ?>)</h3>
      <?php print $inner["address"] ?><br/>
      <?php if (isset($inner["rating"])):?>
        User Rating: <strong><?php print $inner["rating"] ?></strong>/5.<br/>
      <?php endif; ?>
      <br/>
    </div>
  <?php endforeach; ?>
  <?php if (isset($nexturl)):?>
    <div><a href="<?php print $nexturl ?>">Next Page >>></a></div>
  <?php endif; ?>
</div>
<div class="map"><img src="<?php print $mapimg ?>"/></div>
