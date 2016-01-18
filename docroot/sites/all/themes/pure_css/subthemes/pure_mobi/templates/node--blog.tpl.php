<div id="node-<?php print $node->nid ?>" class="<?php print $classes ?> clearfix"<?php print $attributes ?>>

  <?php print $user_picture ?>

  <?php if (!$page): ?><h2 class="title"<?php print $title_attributes ?>><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2><?php endif ?>

  <?php if ($display_submitted): ?><div class="submitted"><?php print t('!username - !datetime', array('!username' => $name, '!datetime' => $date)) ?></div><?php endif ?>

  <div class="content clearfix"<?php print $content_attributes ?>>
    <?php // We hide the comments and links now so that we can render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>

  <?php print render($content['links']) ?>

  <?php print render($content['comments']) ?>

</div>