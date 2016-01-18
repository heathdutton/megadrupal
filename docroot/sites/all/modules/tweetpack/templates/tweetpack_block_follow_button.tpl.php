<?php
/**
 * @file
 * Displays a button to allow visitors to follow a specific Twitter user.
 *
 * Available variables:
 * - $screen_name: The screen_name of the Twitter account to be followed.
 *
 * @ingroup themeable
 */
 $type = variable_get('tweetpack_button_type', 0);
 $size = variable_get('tweetpack_button_size', 0) > 0 ? 'large' : 'small';
 $showcount = variable_get('tweetpack_button_showcount', 0) > 0 ? 'true' : 'false'; 
 $name = variable_get('tweetpack_button_name', '');
 $share = variable_get('tweetpack_button_share', '');
 $shareurl = variable_get('tweetpack_button_shareurl', '');
 
  if($shareurl == '' || $shareurl == '%none') {
    $url =  ' data-url=" "';
  } else if($shareurl == '%current') {
    $url =  '';
  } else {
    $url = ' data-url="' . $shareurl . '"';
  }

  switch ($type) {
    case '0': ?>
      <a href="https://twitter.com/<?php echo $name; ?>" class="twitter-follow-button" data-show-count="<?php echo $showcount; ?>" data-size="<?php echo $size; ?>">Follow @<?php echo $name; ?></a>
    <?php break;

    case '1': 
      $showcount = $showcount == 'false' ? 'none' : 'true'; ?>
      <a href="https://twitter.com/share" class="twitter-share-button" data-text="<?php echo $share; ?>" data-size="<?php echo $size; ?>" data-via="<?php echo $name; ?>" data-count="<?php echo $showcount; ?>"<?php echo $url; ?>>Tweet</a>
    <?php break;
  }
?>

<script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0];if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src="//platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>