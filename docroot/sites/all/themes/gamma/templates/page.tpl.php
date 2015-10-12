<?php
// $Id: page.tpl.php,v 1.2 2011/02/14 00:32:25 himerus Exp $

/**
 * @file
 * Theme implementation to display a single Drupal page.
 */
?>
<div id="page" class="clearfix">
  <?php if (isset($page['zones_above'])): ?>
    <header id="zones-above" class="clearfix"><?php print render($page['zones_above']); ?></header>
  <?php endif; ?>
  
  <section id="zones-content">
    <div id="zones-content-inner" class="clearfix">
      <div id="zones-content-background" class="clearfix">
        <?php if (isset($messages)): ?>
        <div id="message-container" class="container-<?php print $default_container_width; ?> clearfix">
          <div class="grid-<?php print $default_container_width; ?>">
            <?php print $messages; ?>
          </div>
        </div><!-- /.container-xx -->
        <?php endif; ?>
        <?php if (isset($page['content_zone'])): ?>
          <?php print render($page['content_zone']); ?>
        <?php endif; ?>
      </div>
    </div>
  </section>
  
  <?php if (isset($page['zones_below'])): ?>
    <footer id="zones-below" class="clearfix"><?php print render($page['zones_below']); ?></footer>
  <?php endif; ?>
</div><!-- /#page -->
