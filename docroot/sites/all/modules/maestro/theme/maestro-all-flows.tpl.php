<script type="text/javascript">
var ajax_url='<?php print $ajax_url; ?>';
var filter_url='<?php print $filter_url; ?>';
var sec_token = '<?php print drupal_get_token('maestro_user'); ?>';
</script>

<div id="maestro_error_message"></div>

<form id="maestroFilterAllFlowsFrm" style="margin-bottom:10px;">
<input type="hidden" name="sec_token" value="<?php print drupal_get_token('maestro_user'); ?>">
<div id="maestro_all_flows_filter">
<?php print t('Flow Name: '); ?>
<input type="text" name="flowNameFilter" id="flowNameFilter"></input>
<?php print t('User: '); ?>
<input type="text" name="userNameFilter" id="userNameFilter"></input>
<input type="button" value="<?php print t('Filter'); ?>" id="filterAllFlows"></input>
<span id="maestro_filter_working"></span>
</div>
</form>
<div id="maestro_all_flows_display">
<?php print theme('maestro_all_flows_display', array('ajax_url' => $ajax_url, 'database_result_set' => $database_result_set)); ?>
</div>
<?php print theme('pager'); ?>
