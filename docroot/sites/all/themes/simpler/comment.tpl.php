<?php
// $Id: comment.tpl.php,v 1.7 2010/11/14 01:41:18 danprobo Exp $
?>
  <div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print $picture ?>

  <?php if ($new): ?>
    <span class="new"><?php print $new ?></span>
  <?php endif; ?>

  <?php print render($title_prefix); ?>
  <h3<?php print $title_attributes; ?> class="title"><?php print $title ?></h3>
  <?php print render($title_suffix); ?>

  <div class="submitted">
    <?php print $permalink; ?>
    <?php
      print t('Submitted by !username on !datetime.',
        array('!username' => $author, '!datetime' => $created));
    ?>
  </div>

  <div class="content"<?php print $content_attributes; ?>>
    <?php
      hide($content['links']);
      print render($content);
    ?>
    <?php if ($signature): ?>
    <div class="user-signature clearfix">
      <?php print $signature ?>
    </div>
    <?php endif; ?>
  </div>

  <?php print render($content['links']) ?>
</div>


