/**
 * @file
 * This javascript adds accordion-style functionality to years and months.
 */

(function($, Drupal){
  Drupal.behaviors.tardis = {
    attach: function (context, settings) {
      // The variable "year" takes the argument YYYY from the URL
      // and is passed by Drupal.
      year = settings.tardis.year;
      $("#tardis-accordion .tardis-link-year>*>a", context)
        .attr("href", "javascript:void(0);");
      $("#tardis-accordion .tardis-year-" + year + " .tardis-link-year", context)
        .addClass("active");
      $("#tardis-accordion * .tardis-link-year:not(.active)", context).hide();
      $("#tardis-accordion .tardis-link-year", context).click(function () {
        var $this = $(this);
        $this.siblings().find(".tardis-link-year").hide();
        $this.find(".tardis-link-year").addClass("active");
        $this.find(".tardis-link-year.active").show();
      });
    }
  };
})(jQuery, Drupal);
