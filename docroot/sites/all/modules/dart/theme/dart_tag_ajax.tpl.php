<?php

/**
 * @file
 * Display the js call to display a AJAX DART ad tag.
 *
 * Variables available:
 * - $tag: The full tag object or NULL. If it's NULL, all other vars listed
 *     below will be NULL as well
 * - $json_tag: a js version of $tag.
 * - $attributes: any attributes that should be displayed on teh outer-most div.
 * - $show_script_tag: boolean.
 * - $show_noscript_tag: boolean.
 * - $noscript: the <noscript> tag for this DART tag, or empty string.
 *
 * @see template_preprocess_petside_ads_ajax()
 */
?>

<div <?php print drupal_attributes($attributes); ?>>

  <div class="<?php print $name; ?>">&nbsp;</div>

  <?php if ($show_script_tag): ?>
    <script type="text/javascript">
      Drupal.DART = Drupal.DART || {}
      Drupal.DART.ajax = Drupal.DART.ajax || {}
      Drupal.DART.ajax['<?php print $tag->machinename;?>'] = <?php print $json_tag; ?>;
    </script>
    <?php print $noscript_tag; ?>
  <?php else: ?>
    <?php print $static_tag; ?>
  <?php endif; ?>
</div>
