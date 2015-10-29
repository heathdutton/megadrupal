<?php
// $Id: comment.tpl.php,v 1.6 2010/11/26 06:17:01 shannonlucas Exp $
/**
 * @file
 * Comment rendering for Nitobe.
 *
 * In addition to the standard variables Drupal makes available to
 * comment.tpl.php, the follow additional variable is available:
 * - $nitobe_attribution: The formatted author link and date for the comment's
 *   meta data area.
 */

hide($content['links']);
?>
<div class="<?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <div class="content clearfix">
    <?php if ($picture) echo $picture; ?>
    <?php if ($comment->new): ?>
      <span class="new"><?php echo drupal_ucfirst($new); ?></span>
    <?php endif; ?>
    <?php echo render($title_prefix); ?>
    <?php if ($title): ?><h3<?php echo $title_attributes; ?>><?php echo $title ?></h3><?php endif; ?>
    <?php echo render($title_suffix); ?>
    <?php echo render($content); ?>
    <hr/>
    <?php if ($signature): ?>
      <div class="user-signature clearfix">
        <?php echo $signature ?>
      </div>
    <?php endif; ?>
  </div>
  <div class="comment-meta attribution">
    <span class="author-date"><?php echo $nitobe_attribution; ?></span>
    <?php echo render($content["links"]); ?>
  </div>
</div>
