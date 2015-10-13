(function ($) {

/**
 * Chain select lists.
 */
Drupal.behaviors.chainedSelects = {
  attach: function (context) {
    var chainedSelects = Drupal.settings.chainedSelects;
    for (var key in chainedSelects) {
      $('#' + chainedSelects[key]['child'] + ':input').chained('#' + chainedSelects[key]['parent'] + ':input');
    }
  }
};

})(jQuery);
