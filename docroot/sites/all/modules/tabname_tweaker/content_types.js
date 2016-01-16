(function ($) {

Drupal.behaviors.TabnameTweaker = {
  attach: function (context) {
    // Provide the vertical tab summaries.
    $('fieldset#edit-tabname-tweaker', context).drupalSetSummary(function(context) {
      var vals = [];
      vals.push(Drupal.checkPlain($('#edit-tabname-tweaker--2', context).val()) || Drupal.t('Use default tab name'));
      return vals.join(', ');
    });
  }
};

})(jQuery);
