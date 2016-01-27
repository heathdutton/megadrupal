<div class="booklists-detail-column">
  <?php print $image; ?>
  <?php
  $counter_action_links = 0;
  foreach ($action_links as $action_link) :
    if (strstr($action_link, '&index=') || strstr($action_link, 'overdrive.com')) :
      ?>
      <a href="<?php print $action_link; ?>" target="_blank" class="button">Check it out</a>
      <?php
    elseif (strstr($action_link, '/request-purchase')) :
      $counter_action_links++;
      ?>
      <div class="action-link<?php ($counter_action_links == 1 ? print ' first' : print ''); ?>"><i class="icon-bookmark-empty"></i> <a href="<?php print $action_link; ?>">Request this title</a></div>
      <?php
    elseif (strstr($action_link, 'freegalmusic.com')) :
      $counter_action_links++;
      ?>
      <div class="action-link<?php ($counter_action_links == 1 ? print ' first' : print ''); ?>"><i class="icon-search"></i> <a href="<?php print $action_link; ?>">Search music downloads</a></div>
      <?php
    elseif (strstr($action_link, 'amazon.com')) :
      $counter_action_links++;
      ?>
      <div class="action-link<?php ($counter_action_links == 1 ? print ' first' : print ''); ?>"><i class="icon-play"></i> <a href="<?php print $action_link; ?>" target="_blank" title="Read Amazon's Description and Reviews. If you choose to buy once you're there a portion will be donated to <?php print $site_name; ?>.">Amazon</a></div>
      <?php
    elseif (strstr($action_link, 'booklists-ebooks.php')) :
      $counter_action_links++;
      ?>
      <div class="action-link<?php ($counter_action_links == 1 ? print ' first' : print ''); ?>"><i class="icon-question-sign"></i> <a href="<?php print $action_link; ?>" rel="lightmodal[why|height:200px;]">Why can&rsquo;t I get this eBook from a library?</a></div>
      <?php
    endif;
  endforeach;
  ?>
  <div class="disclaimer">You will likely be asked to place a hold on items from the best sellers lists. Hold times for <?php print $site_name; ?> are typically under 9 weeks.</div>
</div>
<h2><?php print $title; ?></h2>
<?php if ($attribution): ?>
  by <?php print $attribution; ?>
<?php endif; ?>
<br><br><?php print $description; ?>
<?php if ($unavailability_notice): ?>
  <p><?php print $unavailability_notice; ?></p>
<?php endif; ?>
<?php if ($release_date): ?>
  <br><br>Release Date: <?php print $release_date; ?>
<?php endif; ?>
<?php if ($binding): ?>
  <br><?php print $binding; ?>
<?php endif; ?>
<?php if ($publisher): ?>
  <br>Publisher: <?php print $publisher; ?>
<?php endif; ?>
<?php if ($carousel): ?>
  <br><?php print $carousel; ?>
<?php endif; ?>