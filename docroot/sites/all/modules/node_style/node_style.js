(function ($) {

Drupal.behaviors.nodestyleFieldsetSummary = {
  attach: function (context) {
    $('fieldset#edit-node-style', context).drupalSetSummary(function (context) {
      return $('#edit-node-style-scheme option:selected', context).text();
    });
  }
};

})(jQuery);
