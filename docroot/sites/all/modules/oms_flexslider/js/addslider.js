(function ($) {
Drupal.behaviors.oms_flexslider = {
    attach : function(context, settings){
        flexvalues = Drupal.settings.oms_flexslider.flexvalues;
        sliderid = Drupal.settings.oms_flexslider.sliderid;
        $('#' + sliderid).flexslider(flexvalues);
    }
}

}(jQuery));
