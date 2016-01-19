(function($) {
  Drupal.behaviors.recent_supporters = {};
  Drupal.behaviors.recent_supporters.attach = function (context, settings) {
    // start the recent-supporters polling
    if ($.fn.recentSupporters && settings.recentSupporters) {
      $('.block-campaignion-recent-supporters', context).each(function() {
        var $block = $(this);
        var blockId = $block.attr('id');
        var blockSettings = settings.recentSupporters.blocks[blockId];
        $block.recentSupporters({
          pollingURL: blockSettings.pollingURL,
          nodeID: blockSettings.nodeID,
          cycleSupporters: (blockSettings.cycleSupporters == "1") ? true : false,
          showCountry: (blockSettings.showCountry == "1") ? true : false,
          maxSupportersVisible: parseInt(blockSettings.visibleCount, 10),
          cycleEasing: 'easeInQuint',
          texts: settings.recentSupporters.actionTexts,
          countries: settings.recentSupporters.countries
        });
      });
    }
  };
})(jQuery);

