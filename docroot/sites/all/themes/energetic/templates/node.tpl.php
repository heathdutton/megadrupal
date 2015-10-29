<?php // $Id: node.tpl.php,v 1.1 2009/07/03 07:40:11 agileware Exp $ ?>
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> Post">
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
      <?php if ($teaser): ?>
        <h2 class="PostHeaderIcon-wrapper"> <span class="PostHeader"><a href="<?php echo $node_url; ?>" title="<?php echo $title; ?>"><?php echo $title; ?></a></span></h2>
      <?php endif; ?>
      <div class="PostHeaderIcons metadata-icons">
        <?php if ($submitted) { echo art_submitted_worker($submitted, $date, $name);} ?>
      </div>
      <div class="PostContent">
        <div class="article">
	  <?php hide($content['comments']);
		hide($content['links']);
		print render($content);?>
	  <?php if (isset($content['links']['node'])) { print '<div class="read_more">'.render($content['links']['node']).'</div>'; }?>
        </div>
      </div>
      <div class="cleared"></div>
      <div class="PostFooterIcons metadata-icons">
        <?php print render($content['links']); ?>
	<?php print render ($content['comments']); ?>
        <?php if (!empty($terms)) { print render($terms); } ?>
      </div>
    </div>
  </div>
</div>
