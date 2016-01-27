<?php
if (theme_get_setting('gradient_enable') && (theme_get_setting('gradient_opacity') > 0)) {
  $top = theme_get_setting('gradient_color_top');
  $bottom = theme_get_setting('gradient_color_bottom');
  $opacity = theme_get_setting('gradient_opacity');
  $opacity_frac = $opacity/100;

  $CSS .= <<<EOT
  .bg-gradient {
    position:fixed;
    top:0;
    left:0;
    width:100%;
    height:100%;
    background: -moz-linear-gradient({$top}, {$bottom});
    background: -ms-linear-gradient({$top}, {$bottom});
    background: -webkit-gradient(linear, left top, left bottom, color-stop(0%, {$top}), color-stop(100%, {$bottom}));
    background: -webkit-linear-gradient({$top}, {$bottom});
    background: -o-linear-gradient({$top}, {$bottom});
    filter: progid:DXImageTransform.Microsoft.gradient(startColorstr='{$top}', endColorstr='{$bottom}')progid:DXImageTransform.Microsoft.Alpha(Opacity=50);
    -ms-filter: "progid:DXImageTransform.Microsoft.gradient(startColorstr='{$top}', endColorstr='{$bottom}')progid:DXImageTransform.Microsoft.Alpha(Opacity=50)";
    background: linear-gradient({$top}, {$bottom});
    opacity: {$opacity_frac};
  }

EOT;
}

if (theme_get_setting('bg_image_enable') && (theme_get_setting('image_opacity') > 0)) {
  if (theme_get_setting('background_image') == 'none') {
    $CSS .= "html body {background-image: none;}";
  }
  else {
    $opacity = theme_get_setting('image_opacity');
    $opacity_frac = $opacity/100;
    $parts = explode('|', theme_get_setting('background_image'));
    $bg_img = $base_path . drupal_get_path('theme', $parts[0]) . '/backgrounds/' . $parts[1];
    $msfilter = $msfilter2 = '';

    if ($opacity < 100) {
      $msfilter .= "progid:DXImageTransform.Microsoft.Alpha(Opacity={$opacity})";
      $msfilter2 .= "alpha(opacity={$opacity})";
    }
    if (theme_get_setting('bg_behave') == 'cover') {
      $msfilter .= "progid:DXImageTransform.Microsoft.AlphaImageLoader(src='{$bg_img}',sizingMethod='scale')";
      $msfilter2 .= "progid:DXImageTransform.Microsoft.AlphaImageLoader(src='{$bg_img}',sizingMethod='scale')";
      $bg_behave = 'center center fixed no-repeat';
      $bg_size = '-webkit-background-size: cover;-moz-background-size: cover;-o-background-size: cover;background-size: cover;';
    }
    else {
      $bg_behave = 'top left repeat';
      $bg_size = '';
    }

    // Was experiencing browser crashes in IE7 when printing the empty filter property,
    // hence this workaround.
    if ($msfilter2) {
      $msprop2 = 'filter:';
      $msprop2_close = ';';
    } else {
      $msprop2 = $msfilter2 = $msprop2_close = '';
    }

    $CSS .= <<<EOT
  .bg-image {
    position:fixed;
    top:0;
    left:0;
    width:100%;
    height:100%;
    background: url('{$bg_img}') {$bg_behave};
    {$msprop2}{$msfilter2}{$msprop2_close}
    -ms-filter:"{$msfilter}";
    opacity: {$opacity_frac};
    {$bg_size}
  }

EOT;
  }
}
