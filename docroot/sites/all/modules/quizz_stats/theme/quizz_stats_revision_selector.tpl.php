<p><?php echo $content['explanation']; ?></p>
<p>
  <?php
  array_map(function($url) {
    echo ' | ' . l(t('revision !num', array('!num' => '#' . arg(3, $url))), $url);
  }, $content['links']);
  ?>
</p>
