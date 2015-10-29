<?php // $Id: $ ?>
<div class="Post">
  <div class="Post-tl"></div>
  <div class="Post-tr"></div>
  <div class="Post-bl"></div>
  <div class="Post-br"></div>
  <div class="Post-tc"></div>
  <div class="Post-bc"></div>
  <div class="Post-cl"></div>
  <div class="Post-cr"></div>
  <div class="Post-cc"></div>
  <div class="Post-body">
    <div class="Post-inner">
      <div class="<?php print $classes; if ($status == 'comment-unpublished') print ' comment-unpublished'; ?>">
        <h2 class="PostHeaderIcon-wrapper">
          <span class="PostHeader">
            <?php if ($picture) echo $picture; ?>
            <?php if ($title) echo $title; ?>
            <?php if ($new) echo '<span class="new-text">' . $new . '</span>'; ?>
          </span>
        </h2>
        <?php if ($submitted): ?>
          <div class="submitted">
            <?php echo $submitted; ?>
          </div>
          <div class="cleared"></div><br/>
        <?php endif; ?>
	<div class="PostContent"<?php print $content_attributes; ?>>
	  <?php hide ($content['links']); ?>
	  <?php print render($content); ?>
        </div>
        <div class="cleared"></div>
        <div class="links">
	       <?php print render($content['links']); ?>
          <div class="cleared"></div>
        </div>
      </div>
    </div>
  </div>
</div>
