(function ($) {
  "use strict";

  Drupal.behaviors.socialApprove = {
    attach: function (context, settings) {

      var $checkbox_html = '<input type="checkbox" />';
      var $checkbox_enabled = '<input type="checkbox" checked="checked" />';

      var $rows = $("body.page-admin-config-services-instagram-social-feed table tbody tr", context);
      $.each($rows, function(index, element) {
        var $td = jQuery("td", element);

        var $image = jQuery("img", $td.first());
        var approved = $image.data('approved');

        var $checkbox = $td.last();
        var id = $checkbox.html();

        if (approved == 1) {
          $checkbox.html($checkbox_enabled);
        }
        else {
          $checkbox.html($checkbox_html);
        }

        $checkbox.click(function() {
          jQuery.get(Drupal.settings.basePath + Drupal.settings.pathPrefix + "ajax/instagram_social_feed_approve?instagram_id=" + id);
        });
      });

      var $select = $("select", context);
      $select.change(function(data) {
        window.location.href = Drupal.settings.basePath + Drupal.settings.pathPrefix + "admin/config/services/instagram_social_feed/overview/" + this.value;
      });

    }
  }
}(jQuery));
