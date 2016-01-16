<?php
/**
 * @file
 * Zen theme's implementation to provide an HTML container for comments.
 *
 * Available variables:
 * - $content: The array of content-related elements for the node. Use
 *   render($content) to print them all, or print a subset such as
 *   render($content['comment_form']).
 * - $classes: String of classes that can be used to style contextually through
 *   CSS. It can be manipulated through the variable $classes_array from
 *   preprocess functions. The default value has the following:
 *   - comment-wrapper: The current template type, i.e., "theming hook".
 * - $title_prefix (array): An array containing additional output populated by
 *   modules, intended to be displayed in front of the main title tag that
 *   appears in the template.
 * - $title_suffix (array): An array containing additional output populated by
 *   modules, intended to be displayed after the main title tag that appears in
 *   the template.
 *
 * The following variables are provided for contextual information.
 * - $node: Node object the comments are attached to.
 * The constants below the variables show the possible values and should be
 * used for comparison.
 * - $display_mode
 *   - COMMENT_MODE_FLAT
 *   - COMMENT_MODE_THREADED
 *
 * Other variables:
 * - $classes_array: Array of html class attribute values. It is flattened
 *   into a string within the variable $classes.
 *
 * @see template_preprocess_comment_wrapper()
 * @see theme_comment_wrapper()
 */

// Render the comments and form first to see if we need headings.
$comments = render($content['comments']);

// $comment_form = render($content['comment_form']);
if (!empty($content['comment_form'])) {
  $comment_form = $content['comment_form'];
  $child = "";
  $child .= '<div class="modal-header">
          <button type="button" class="close" data-dismiss="modal">×</button>
          <h3 class="title comment-form">' . t('Add new comment') . '</h3>
       </div>
       <div class="modal-body">';
   $child .= drupal_render($comment_form['author']);
   $child .= drupal_render($comment_form['subject']);
   $child .= drupal_render($comment_form['comment_body']);      
   $child .='    </div>
       <div class="modal-footer">';
   $child .= drupal_render($comment_form['actions']);
   $child .='    </div>
    ';
  $child .= drupal_render_children($comment_form);
  $comment_form['#children'] = $child;
  $comment_form = theme('form', array('element' => $comment_form));
}
?>
<section class="comments <?php print $classes; ?>"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php if ($comments && $node->type != 'forum'): ?>
    <h2 class="title"><?php print t('Comments'); ?></h2>
  <?php endif; ?>
  <?php print render($title_suffix); ?>

  <?php print $comments; ?>

  <?php if ($comment_form): ?>
    <a class="btn" data-target="#comment-nid-<?php print $node->nid; ?>" data-toggle="modal" href="#comment-<?php print $node->nid; ?>" ><?php print t('Add Comment'); ?></a>
    <div class="modal hide" id="comment-nid-<?php print $node->nid; ?>"'>
      <?php print $comment_form; ?>
    </div>
  <?php endif; ?>
</section>
