<?php
drupal_add_js('http://cloud.yolink.com/yolinklite/js/tigr.jquery-1.4.2-min.js', 'external');
drupal_add_js('http://cloud.yolink.com/yolinklite/js/v2/yolink-2.0.js', 'external');
?>

<?php drupal_add_js('tigr.yolink.Widget.initialize(
{
    keywords: function()
    {
      var u = document.location.href;
      var mySplitURL = u.split("/");
      var lastBit = mySplitURL[mySplitURL.length-1];
      var splitLastBit = lastBit.split("?");
      var k = splitLastBit[0];
      return k;
    },
    display : \'embed\',
    getSearch : \'.title\',
    maxResults : ' . variable_get('yolink_max_results', '4') . ',
    showTools : \'result\',
    formfactor : \'mini\',
    share : ' . variable_get('yolink_show_share', 'true') . ',
    googledocs : ' . variable_get('yolink_show_google_docs', 'true') . ',
    fblike : \'local\',
    tweet : \'local\',
    apikey : \'' . variable_get('yolink_api_key', '') . '\',
    auto : true,
    checkboxes : true,
    preview : \'tab\',
    showHide : ' . variable_get('yolink_show_hide', 'true') . '
} );', 'inline'); ?>

<?php if ($search_results) : ?>
  <h2><?php print t('Search results');?></h2>
  <ol class="search-results <?php print $module; ?>-results">
    <?php print $search_results; ?>
  </ol>
  <?php print $pager; ?>
<?php else : ?>
  <h2><?php print t('Your search yielded no results');?></h2>
  <?php print search_help('search#noresults', drupal_help_arg()); ?>
<?php endif; ?>
