<?php

/**
 * @file
 * NewsFlash node--forum.tpl.php
 * node--forum.tpl.php is a suggestion of drupal theme.inc http://drupal.org/node/1089656
 * for Default theme implementation see modules/node/node.tpl.php
 *
 */
?>
  <div id="node-<?php print $node->nid; ?>" class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>  
    <div class="nf-forum-submitted forum clearfix">
    <?php print $user_picture; ?>
      <?php if ($display_submitted): ?>
        <span class="meta forum-submitted">
        <?php print t('Submitted by !username <br /> on !datetime', array('!username' => $name, '!datetime' => $date)); ?>
        </span>
      <?php endif; ?>
    </div>
    <?php print render($title_prefix); ?>
    <?php if (!$page) { ?>
      <?php if ($title) { ?>
        <h2 <?php print $title_attributes; ?>>
          <a href="<?php print $node_url ?>" title="<?php print $title ?>">
            <?php print $title?>
          </a>
        </h2>
      <?php }; ?>
    <?php }; ?>
    <?php print render($title_suffix); ?>
    <div class="content clearfix node-forum-nf" <?php print $content_attributes ?>>
      <?php print $content_attributes?>
      <?php       
      hide($content['comments']);
      hide($content['links']);
      print render($content); ?> 
    </div>
    <div class="links-forum-nf">
      <?php if (!empty($content['links'])): ?>
        <?php print render($content['links']); ?>
      <?php endif; ?>
    </div>
    <?php if ($content['comments'] && ($page)) { ?>
      <?php print render($content['comments']); ?>
    <?php }; ?>
  </div>
