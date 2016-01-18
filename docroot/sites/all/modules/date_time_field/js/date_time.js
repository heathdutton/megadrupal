/**
 * @file
 * Javascript file for Date and Time field Module.
 */
 
Drupal.behaviors.dateTimeField = {
  attach : function (context){
  var widgetDateFormat = Drupal.settings.date_time_field.widget_date_format ;
  var widgetTimeFormat = Drupal.settings.date_time_field.widget_time_format ;
  jQuery(".date-time-field").each(function(){
  jQuery(this).datetimepicker({ampm: true , changeMonth: true , changeYear: true, dateFormat: widgetDateFormat, timeFormat: widgetTimeFormat});
  }) ;   	
  } 
}