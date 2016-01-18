<?php
?>

<!-- BEGIN yolink -->	    
<?php
drupal_add_js('http://cloud.yolink.com/yolinklite/js/tigr.jquery-1.4.2-min.js', 'external');
drupal_add_js('http://cloud.yolink.com/yolinklite/js/v2/yolink-2.0.js', 'external');
?>

<?php drupal_add_js('tigr.yolink.Widget.initialize(
    {
    keywords : \'\',
    keywordsFromURL : \'query\',
    display : \'embed\',
    getSearch  : \'.gs-result\',
    showTools : \'result\',
    formfactor : \'mini\',
    maxResults : ' . variable_get('yolink_max_results', '4') . ',
    share : ' . variable_get('yolink_show_share', 'true') . ',
    googledocs : ' . variable_get('yolink_show_google_docs', 'true') . ',
    fblike : \'local\',
    tweet : \'local\',
    apikey : \'' . variable_get('yolink_api_key', '') . '\',
    auto : true,
    checkboxes : true,
    preview : \'tab\',
    showHide : ' . variable_get('yolink_show_hide', 'true') . '
    } );', 'inline'); 
?>

<?php if ($prefix): ?>
  <div class="google-cse-results-prefix"><?php print $prefix; ?></div>
<?php endif; ?>

<?php if ($results_searchbox_form): ?>
  <?php print render($results_searchbox_form); ?>
<?php endif; ?>

<div id="cse" style="width: 100%;"><span style="padding:0 0 0 15px;">Loading...</span></div>
  <noscript>
    <?php print $noscript; ?>
  </noscript>
  <script src="http://www.google.com/jsapi" type="text/javascript"></script>

	<script type="text/javascript">
		google.load('search', '1');
		google.setOnLoadCallback(
		function()
		{
            var csc = new
            google.search.CustomSearchControl('<?php echo variable_get('google_cse_cx', ''); ?>');
            var myDomEditor = {};
            myDomEditor.editPage = function() 
            {

                var keyword = $tigr('.form-text')[0].value;
                if(keyword && keyword.length > 0)
                {
                    tigr.yolink.Widget.options.keywords = keyword;
                    $tigr('input.form-text').val(keyword);
                }
                tigr.yolink.Widget.doSearch();
            };
            csc.setSearchCompleteCallback(myDomEditor, myDomEditor.editPage);
            csc.draw('cse');
            var u = document.location.href;
            var s = u.indexOf( 'query=' );
            var e = u.indexOf( '&', s + 1 );
            if( s > 0 && e > 0 )
            {
                csc.execute( decodeURIComponent( u.substring( s + 6, e ).split('+').join(' ') ) );
            }
		},
		true );
	</script>
<?php if ($suffix): ?>
  <div class="google-cse-results-suffix"><?php print $suffix; ?></div>
<?php endif; ?>

