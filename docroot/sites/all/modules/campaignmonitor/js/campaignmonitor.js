/**
 * Enable datepicker for custom Campaign Monitor date fields.
 */
(function ($) {
  $(document).ready(function(){
    $('.campaignmonitor-date').datepicker({
      dateFormat: "yy-mm-dd",
      autoSize: true
    });
  });  
}(jQuery))
