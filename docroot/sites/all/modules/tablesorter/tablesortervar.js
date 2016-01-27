/**
 * @file
 * jQuery Tablesorter
 */

(function ($) {
  Drupal.behaviors.tablesorter = {
    attach: function (context, settings) {

      var widgets = [];
      var widgetsZebra = [];

      if (settings.tablesorter.zebra == 1) {
          widgets.push('zebra');
          widgetsZebra.push(settings.tablesorter.odd);
          widgetsZebra.push(settings.tablesorter.even);
      }
    $('.tablesorter').once('tablesorter', function() {
      $(".tablesorter").tablesorter({
          widgets: widgets,
          widgetsZebra: {css: widgetsZebra}
      })
      if ($("#tablesorter_pager").length != 0) {
          $(".tablesorter").tablesorterPager({
              container: $("#tablesorter_pager")
          });
      }
    });

    }
  };
})(jQuery);
