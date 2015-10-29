<?php
?>
  <div class="<?php print $classes; ?> <?php if ($sticky&&$page == 0) { print " sticky"; } ?><?php if (!$status) { print " node-unpublished"; } ?>"<?php print $attributes; ?>>
    <?/*php if ($picture) {
      print $picture;
    }*/?>

 <?php if ($display_submitted): ?>
       <span class="submitted"><?php print $date; ?> &mdash; <?php print $name; ?></span>
  <?php endif; ?>

    <?php if ($page == 0) { ?><h2 class="title"><a href="<?php print $node_url?>"><?php print $title?></a></h2><?php }; ?>
   
    
    <div class="content clearfix"<?php print $content_attributes; ?>>
    <?php
      // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>

    <?php if (!empty($content['links'])): ?>
      <div class="links"><?php print '&raquo; '.render($content['links']); ?></div>
    <?php endif; ?>
    
    <?php print render($content['comments']); ?>
    <?php //var_dump($content['links'])?>
  </div>
