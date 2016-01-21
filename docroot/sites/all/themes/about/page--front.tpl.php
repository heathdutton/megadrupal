<!DOCTYPE html>
<html>
<head>
  <title><?php print render($about_name); ?></title>
  <?php print render($styles); ?>
  
</head>
<body>
  
<?php echo $messages; ?>
  
  <div id="about-name"><?php print render($about_name); ?></div>
  
  <div id="content"  class="clearfix">
    <div id="about-story">
      <?php print render($about_story); ?>
    </div>
    
    <? if ($about_links): ?>
    <div id="about-links">
      <?php print render($about_links); ?>
    </div>
    <?php endif; ?>
  </div>
  
  <?php print render($page['footer']); ?>
  <?php print render($page['bottom']); ?>
  
</body>
</html>