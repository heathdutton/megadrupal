<div class="content-display twocol-75-25-stacked clear-block clearfix" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>
  <div class="container onecol header">
    <?php print $content['header']; ?>
  </div>

  <div class="container onecol top">
    <?php print $content['top']; ?>
  </div>

  <div class="container twocol columns">
    <div class="column col-left">
        <?php print $content['left']; ?>
    </div>

    <div class="column col-right">
      <?php print $content['right']; ?>
    </div>
  </div>

  <div class="container onecol bottom">
    <?php print $content['bottom']; ?>
  </div>
</div>
