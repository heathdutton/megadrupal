<div class="content-display twocol-50-50 clear-block clearfix" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>

  <div class="container twocol columns">
    <div class="column col-left">
        <?php print $content['left']; ?>
    </div>

    <div class="column col-right">
      <?php print $content['right']; ?>
    </div>
  </div>

</div>
