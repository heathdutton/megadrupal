<?php

global $base_url;
$path = $base_url . "/link_click_count?url=" . $url['url'] . "&nid=" . $variables['node']->nid;
$title = isset($url['title']) ? $url['title'] : $url['url'];
$target = isset($url['attributes']['target']) ? $url['attributes']['target'] : NULL;
$rel = isset($url['attributes']['rel']) ? $url['attributes']['rel'] : NULL;
$class = isset($url['attributes']['class']) ? $url['attributes']['class'] : NULL;

?>

<a href="<?php print $path; ?>" target="<?php print $target; ?>" rel="<?php print $rel; ?>" class="<?php print $class; ?>" >
    <?php print $title; ?>
</a>

