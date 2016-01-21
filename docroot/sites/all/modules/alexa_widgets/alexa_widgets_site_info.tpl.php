<?php
$site_mapping =array(
'120 x 95'=>'http://xslt.alexa.com/site_stats/js/s/a?url=',
'120 x 240'=>'http://xslt.alexa.com/site_stats/js/s/b?url=',
'468 x 60'=>'http://xslt.alexa.com/site_stats/js/s/c?url='
) ;
?>
<A href="http://www.alexa.com/siteinfo/<?php print $siteurl?>">
<script type='text/javascript' language='JavaScript' src='<?php print $site_mapping[$type].$siteurl?>'>
</script>
</A>