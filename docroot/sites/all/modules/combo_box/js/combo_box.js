jQuery(document).ready(function () {
    var modulePath = Drupal.settings.modulepath; 
    window.dhx_globalImgPath = modulePath + "/library/codebase/imgs/";
    jQuery("select").each(function () {
        var selectmenuid = jQuery(this).attr("id"); 
        if (selectmenuid == 'combo-box-field') {
            var z = dhtmlXComboFromSelect(selectmenuid);
            z.enableFilteringMode(true);
            // empty input when - pressed
            z.attachEvent("onKeyPressed", function (keyCode) {
                if (keyCode == 189) {
                    z.DOMelem_input.value = ""; 
                }	
            });		
        }
    });	
}); 
