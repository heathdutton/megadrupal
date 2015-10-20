/**
 * @file
 */

(function ($) {

Drupal.behaviors.moduleDepToggle = {
  attach: function (context, settings) {

    /**
     * Show/hides dependencies on click.
     */
    $("a.module_dep_toggle", context).click(function(e){
      e.preventDefault();

      var textDiv = $(this).parent().parent().find("div.admin-requirements");

      if (textDiv.length) {
        if ($(this).hasClass("hide")) {
          textDiv.show();
          $(this).removeClass("hide").addClass("show").attr("title", Drupal.t("Click to hide."));
        }
        else {
          textDiv.hide();
          $(this).removeClass("show").addClass("hide").attr("title", Drupal.t("Click to show."));
        }
      }
    });

  }
};

})(jQuery);
