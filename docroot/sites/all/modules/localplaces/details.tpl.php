<?php
/**
 * @file
 * Returns HTML for the Details Page of the Local Places Module.
 */
?>

<div class="details-map">
  <a href="<?php print $mapurl ?>" target="_blank" title="Full Google Map (new window)"><img src="<?php print $mapimg ?>" alt="Map of Location"/></a>
</div>
      <div class="details"><img src="<?php print $icon ?>" align="left" title="<?php print $type ?>." alt="<?php print $type ?> icon." class="icon"/><?php print $address ?><br/>
      <?php if (isset($tel)):?>
			Tel: <?php print $tel ?>
      <?php endif; ?>
      <ul class="details">
         <?php if (isset($website)):?>
			  <li><a href="<?php print $website ?>" target="_blank" title="<?php print $name ?> website (new window)" rel="nofollow"><?php print $name ?> Website. </a></li>
      <?php endif; ?>
        <li><a href="<?php print $googleurl ?>" target="_blank" title="<?php print $name ?> Google Places Page (new window)">More Details about <?php print $name ?> from Google Places.</a></li>
      </ul>
    <?php if (isset($reviews)):?>
      </div><div class="place">
        <?php foreach ($reviews as $inner):
          foreach ($inner as $key => $value):
            if ($key == 'author'):
              echo '<h3 class="local-places">Review by ' . $value . '</h3>';
            endif;
            if ($key == 'text'):
              echo '<p>' . $value . '</p>';
            endif;
          endforeach;
        endforeach; ?>
       <?php endif; ?>
 </div>
