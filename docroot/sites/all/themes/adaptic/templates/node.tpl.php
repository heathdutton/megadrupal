<?php
/**
 * @file
 * adaptIC's implementation to display a node.
 */

 ?>
 
<section>



  <?php print $user_picture; ?>

  <?php print render($title_prefix); ?>
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>>
      <a href="<?php print $node_url; ?>"><?php print $title; ?></a>
	    <?php if ($display_submitted): ?>
    <small>
      <?php print $submitted; ?>
    </small>
  <?php endif; ?>

    </h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>


  <?php
    hide($content['comments']);
    hide($content['links']);
    print render($content);
  ?>

  <?php print render($content['links']); ?>
  <?php print render($content['comments']); ?>

</section>
