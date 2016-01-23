<?php
// $Id: 
?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>

<?php print $user_picture; ?>

<?php if (!$page): ?>
  <h2 class="node-title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
<?php endif; ?>

<?php if ($display_submitted /* || $terms */): ?>
  <div class="meta">
  <?php if ($submitted): ?>
    <span class="submitted"><?php print $submitted ?></span>
  <?php endif; ?>
  
  <!-- Removed, seems to throw errors stating that $terms is undfined
       even though the manual states $terms is the correct variable. -->
  <?php //if ($terms): ?>
    <!-- <div class="terms terms-inline"><?php //print $terms ?></div> -->
  <?php //endif; ?>
  </div>
<?php endif; ?>

  <div class="content">
      <?php
      hide($content['links']);
      hide($content['comments']);
      print render($content);
      print render($content['links']);
      print render($content['comments']);
      ?>
  </div>

  <!-- Removed - seems to not be needed any longer -->
  <?php //print render($content['links']); ?>
</div>
