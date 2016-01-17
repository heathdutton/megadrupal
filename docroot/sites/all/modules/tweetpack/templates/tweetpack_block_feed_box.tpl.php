<?php
/**
 * @file
 * Displays a Tweet Box to allow visitors to post statuses to Twitter.
 *
 * Available variables:
 */
  $widget_id = variable_get('tweetpack_feed_widget_id', '');
?>

  <?php if($widget_id !== '') { ?>
    <div id="twitter_container">
      <a class="twitter-timeline" href="#" data-widget-id="<?php echo $widget_id; ?>"></a>
    </div>
    <script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0];if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src="//platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>
  <?php } else { ?>
    <div id="twitter_container">
      <h3 class="error">No Widget ID Defined</h3>
    </div>
  <?php } ?>