jQuery(document).ready(function(){
var events_data = Drupal.settings.civicrm_events;
var jsonStr = JSON.stringify(events_data);
jQuery('#calendar').fullCalendar(events_data);		
});
