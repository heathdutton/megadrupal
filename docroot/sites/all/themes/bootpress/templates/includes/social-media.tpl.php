<?php
/**
 * Social media template
 * @author Pitabas Behera
*/
?>
<?php $boot_press_social = $boot_press_social_media_links; ?>
<ul class="theme-social-nav">
  <?php foreach($boot_press_social as $url => $icons): ?>
    <?php if($url): ?>
      <li><a href="<?php print url(check_plain($url)); ?>" target="_blank"><i class="<?php print $icons; ?>"></i></a></li>
    <?php endif; ?>
  <?php endforeach; ?>
  
  <?php global $base_url; if(theme_get_setting('rss')): ?>
    <li><a href="<?php print $base_url; ?>/rss.xml" target="_blank"><i class="fa fa-rss"></i></a></li>
  <?php endif; ?>
</ul>
