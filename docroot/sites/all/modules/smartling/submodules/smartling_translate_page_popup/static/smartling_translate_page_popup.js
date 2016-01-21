Drupal.behaviors.smartlingTranslatePopup = {
    attach: function (context, settings) {
        jQuery("#smartling_translate_popup #smartling_popup_header").click(function() {
            //jQuery("#smartling_popup_content").toggle( "explode" );
            jQuery("#smartling_popup_content").toggleClass('not_visible');
            jQuery("#smartling_translate_popup").toggleClass('smartling_state_compressed');
        });
        jQuery("#smartling_popup_closebutton").click(function() {
            jQuery("#smartling_translate_popup").css("display","none");
        });

        jQuery("#smartling_popup_content .entity_progress").hide();
        jQuery("#smartling_stats a").click(function() {
            if (jQuery('#smartling_popup_content .entity_type:visible').length) {
                jQuery("#smartling_popup_content .entity_type").toggle();
                jQuery("#smartling_popup_content .entity_progress").fadeToggle();
            }
            else {
                jQuery("#smartling_popup_content .entity_progress").toggle();
                jQuery("#smartling_popup_content .entity_type").fadeToggle();
            }
        });
    }
};