// $Id: indymedia_cities.js,v 1.1.2.1 2010/08/14 07:38:58 mfb Exp $
(function ($) {

Drupal.behaviors.indymediaCitiesAccordion = {
  attach: function (context, settings) {
    $('ul.indymedia_cities-accordion', context).once('indymedia-cities-accordion', function () {
      $(this).accordion({ header: "a.indymedia_cities-header", autoHeight: false, collapsible: true, active: false });
    });
  }
};

})(jQuery);
