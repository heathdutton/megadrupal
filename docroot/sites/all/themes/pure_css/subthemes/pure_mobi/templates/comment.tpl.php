<div class="comment clearfix">
<?php print $picture ?>
  <h3 class="title"><?php print $title ?></h3>
  <div class="submitted"><?php print t('!username - !datetime', array('!username' => $author, '!datetime' => $created)) ?></div>
  <div class="content">
  <?php // We hide the comments and links now so that we can render them later.
    hide($content['links']);
    print render($content);
  ?>

  <?php if ($signature): ?>
  <div class="signature clearfix"><?php print $signature ?></div>
  <?php endif ?>
  </div>
  <?php print render($content['links']) ?>
</div>
