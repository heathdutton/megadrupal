<?php // $Id$ ?>
<div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?>">
  <?php print $user_picture ?>
  <?php if ($display_submitted): ?>
    <div class="date">
      <div class="textdate">
        <div class="day"><?php print format_date($created, 'custom', 'j'); ?></div>
        <div class="month"><?php print format_date($created, 'custom', 'M'); ?></div>
      </div>
    </div>
  <?php endif; ?>
  <?php print render($title_prefix); ?>
  <?php if(!$page): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <?php if($display_submitted): ?>
    <span class="submitted">
      <?php print t('Submitted by !username on !datetime', array('!username' => $name, '!datetime' => $date)); ?>
    </span>
  <?php endif; ?>
  
  <div class="content clear-block"<?php print $content_attributes; ?>>
    <?php
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>
  <div class="clear-block">
    <?php if($content['links']): ?>
      <div class="links">
        <?php print render($content['links']); ?>
      </div>
    <?php endif; ?>
  </div>
</div>
<?php print render($content['comments']); ?>