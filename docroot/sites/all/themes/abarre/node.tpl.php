<?php 
// $Id:$
?>
<div class="node" id="node-<?php print $node->nid; ?>">
  <?php if (!$page): ?>
    <h2<?php print $title_attributes; ?>><a href="<?php print $node_url; ?>"><?php print $title; ?></a></h2>
  <?php endif; ?>
  
  <?php if ($submitted): ?>
  	<div class="submitted">
          <?php
            print t('Posted on !datetime by !username',
              array('!username' => $name, '!datetime' => $date)) . $user_picture;
          ?>
    </div>
  <?php endif; ?>
  
  <div class="content">
    <?php
      // Hide comments and links and render them later.
      hide($content['comments']);
      hide($content['links']);
      print render($content);
    ?>
  </div>
  

    <?php if (!empty($content['links'])): ?>
      <div class="links"><?php print render($content['links']); ?></div>
    <?php endif; ?>

    <?php print render($content['comments']); ?>

</div><!-- /node -->
