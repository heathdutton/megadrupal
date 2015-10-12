<?php 
?>
<article class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  
  <header>
  <?php if ($new): ?>
    <span class="new"><?php print $new ?></span>
  <?php endif; ?>

  <?php print render($title_prefix); ?>
  <h3<?php print $title_attributes; ?> class="title"><?php print $title ?></h3>
  <?php print render($title_suffix); ?>
  </header>
  
  <?php print $picture ?>
  <footer class="submitted">
    <?php print $permalink; ?>
    <?php
      print t('Submitted by !username on !datetime.',
        array('!username' => $author, '!datetime' => $created));
    ?>
  </footer>

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

	<?php if (!empty($links)): ?>
		<nav class="links">
            <?php print $links; ?>
        </nav>
    <?php endif; ?>
  
</article>
