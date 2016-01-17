
(function($) {

Drupal.behaviors.contextPanelsLayoutsReactionBlock = {};
Drupal.behaviors.contextPanelsLayoutsReactionBlock.attach = function(context) {
  $('.context-blockform-layout:not(.contextPanelsLayoutsProcessed)').each(function() {
    $(this).addClass('contextPanelsLayoutsProcessed');
    $(this).change(function() {
      var layout = $(this).val();
      if (Drupal.settings.contextLayouts.layouts[layout]) {
        $('#context-blockform div.tabledrag-toggle-weight-wrapper').hide();
        for (var key in Drupal.settings.contextLayouts.layouts[layout]) {
          var region = Drupal.settings.contextLayouts.layouts[layout][key];
          $('.context-blockform-regionlabel-'+region).next('div.tabledrag-toggle-weight-wrapper').show();
        }
      }
    });
    $(this).change();
  });
};

})(jQuery);
