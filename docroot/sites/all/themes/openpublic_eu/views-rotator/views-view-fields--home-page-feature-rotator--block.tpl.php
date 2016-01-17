<?php
 /**
  * Custom vars defined in house_homepage_helper module
  *
  * $node_title - title linked to specified url
  * $rotator_nav - html for rotator nav 
  */
?>
<?php 
  /** Variables **/  
  $title = filter_xss($row->node_title);
  $links_url = $fields['entity_id_3']->content;
  
  $main_image = filter_xss(str_ireplace('alt=""', 'alt="' . check_plain($title) . ' ' . t('feature image') . '"', $fields['entity_id_1']->content), array('a', 'img'));
  
  $summary = filter_xss($fields['entity_id']->content, array('div'));
  $read_more = filter_xss($links_url);
  
  if ($read_more) {
    $main_image = l($main_image, $read_more, array('html' => TRUE));
  }
?>
  <div class="home-rotator-photo">
    <?php print $main_image; ?>
  </div>
  <div class="home-rotator-text-block">
    <h2><?php print l($title, $links_url); ?></h2>
    <div class="home-top-intro">
      <?php print $summary; ?>
      <?php print l(t('Read More').' &raquo;', $read_more,  array('html' => true)); ?>
    </div><!--/home top intro-->
  </div>