<?php
// $Id: comment.tpl.php,v 1.4 2010/12/01 00:18:15 webchick Exp $

/**
 * @file
 *
 * Available variables:
 * - $author: Comment author. Can be link or plain text.
 * - $content: An array of comment items. Use render($content) to print them all, or
 *   print a subset such as render($content['field_example']). Use
 *   hide($content['field_example']) to temporarily suppress the printing of a
 *   given element.
 * - $created: Formatted date and time for when the comment was created.
 *   Preprocess functions can reformat it by calling format_date() with the
 *   desired parameters on the $comment->created variable.
 * - $changed: Formatted date and time for when the comment was last changed.
 *   Preprocess functions can reformat it by calling format_date() with the
 *   desired parameters on the $comment->changed variable.
 * - $new: New comment marker.
 * - $permalink: Comment permalink.
 * - $submitted: Submission information created from $author and $created during
 *   template_preprocess_comment().
 * - $picture: Authors picture.
 * - $signature: Authors signature.
 * - $status: Comment status. Possible values are:
 *   comment-unpublished, comment-published or comment-preview.
 * - $title: Linked title.
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default values can be one or more of the following:
 *   - comment: The current template type, i.e., "theming hook".
 *   - comment-by-anonymous: Comment by an unregistered user.
 *   - comment-by-node-author: Comment by the author of the parent node.
 *   - comment-preview: When previewing a new or edited comment.
 *   The following applies only to viewers who are registered users:
 *   - comment-unpublished: An unpublished comment visible only to administrators.
 *   - comment-by-viewer: Comment by the user currently viewing the page.
 *   - comment-new: New comment since last the visit.
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * These two variables are provided for context:
 * - $comment: Full comment object.
 * - $node: Node object the comments are attached to.
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess()
 * @see template_preprocess_comment()
 * @see template_process()
 * @see theme_comment()
 */
?>
<div class="comment<?php isset($new) ? print ' comment-new' : ''; print ' '. $status ?> <?php print $zebra; ?> <?php print $classes; ?> comment-<?php print $node->type;?> clearfix"<?php print $attributes; ?>>
	<div class="comment-top">
  <?php if ($picture) : ?><?php print $picture ?><?php endif;?>
  <?php print '<b>' . $author . '</b> ' . t('on') . ' ' . format_date($comment->created, 'custom', 'jS F, Y');?>
  <div class="clearfix"></div>
  </div>
	<div class="comment-bubble clearfix"<?php print $content_attributes; ?>>
	<div class="bubble"></div>
		<?php if (!empty($title)) : ?><h3<?php print $title_attributes; ?>><?php print $title; ?></h3><?php endif;?>
		<?php hide($content['links']); print render($content); ?>
		<?php if ($signature): ?><span class="user-signature clearfix"><?php print $signature ?></span><?php endif; ?>

		<?php print render($content['links']); ?>
	</div>
</div>
