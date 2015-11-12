jQuery(document).ready(function(){
  if (jQuery("#auto-display-checkbox").attr("checked")==true) {
    jQuery("#create-display-fieldset").show();
  }
  else {
    jQuery("#create-display-fieldset").hide();
  }
});