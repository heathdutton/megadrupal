<?php
/**
 * @file
 * Contextual navigation template for Funnelback.
 */

foreach($contextual_nav as $category):
?>
  <div class="contextual-nav">
    <h3><em><?php print $summary['query']; ?></em> by <?php print $category['name']; ?></h3>
    <ul>
      <?php foreach($category['clusters'] as $cluster): ?>
        <li><a href="<?php print $cluster['link']; ?>"><?php print $cluster['title'] ?></a></li>
      <?php endforeach ?>
    </ul>
  </div>
<?php endforeach ?>
