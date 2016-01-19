(function ($) {
Drupal.behaviors.odir_acl = {
  attach: function (context, settings) {
    jQuery('.odir_acl_role_header').each(function() {
      sourcelink = jQuery(this).attr("sourcelink");
      var spanid = jQuery(this).attr("id");
      jQuery.get(sourcelink, function(data) {
        jQuery("#"+spanid).html(data);
        odir_acl.associte_mouseover_events();
      });
    });
  }
};
})(jQuery);

var odir_acl = {};
odir_acl.associte_mouseover_events = function () {
  jQuery('.odir-access-hover-me').mouseenter(function() {
    jQuery('#odir-access-view-hover-description').html("<hr>" + jQuery(this).attr('hover_text'));
  });
  jQuery('.odir-access-hover-me').mouseout(function() {
    jQuery('#odir-access-view-hover-description').html(' ');
  });
}
