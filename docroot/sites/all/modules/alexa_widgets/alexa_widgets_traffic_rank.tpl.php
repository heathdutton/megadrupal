<?php
$site_mapping = array(
'120 x 65'=>'http://xslt.alexa.com/site_stats/js/t/a?url=',
'120 x 90'=>'http://xslt.alexa.com/site_stats/js/t/b?url=',
'468 x 60'=>'http://xslt.alexa.com/site_stats/js/t/c?url='
) ;
?>
<A href="http://www.alexa.com/siteinfo/<?php print $siteurl?>">
<SCRIPT type='text/javascript' language='JavaScript' src='<?php print $site_mapping[$type].$siteurl?>'>
</SCRIPT>
</A>