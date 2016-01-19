(function ($) {

  /*
   * Trigger event of Flags after update the link.
   */
  $(document).bind('flagGlobalAfterLinkUpdate', function(event, data) {

    var action_to_op = {
      'flagged': {
        'op': 'flag'
      },
      'unflagged': {
        'op': 'unflag'
      }
    };
    var op = action_to_op[data.flagStatus].op;

    // Check if exist a conditions for the Flag
    if (typeof(Drupal.settings.FlagConditions) !== "undefined" && typeof(Drupal.settings.FlagConditions[data.flagName]) !== "undefined") {
      $.each(Drupal.settings.FlagConditions[data.flagName], function (action, reactions) {

        // Check if the action of the Flag is the $op action
        if (action == op && typeof reactions !== "undefined") {
          $.each(reactions, function (target_name, target_action) {

            target_name = String(target_name).replace(/\_/gi, "-");

            var $flagDomLink = $('span.flag-wrapper.flag-' + target_name + '-' + data.contentId + ' a.' + target_action + '-action.flag-processed');

            // Update the Flag link to the target_action
            if (!$.isEmptyObject($flagDomLink)) {

              // Make an ajax
              $($flagDomLink).click();

              // updateLink($flaggedDom, data.newLink);
              // Drupal.attachBehaviors($flaggedDom);
            }

          });
        }

      });
    }

  });
  
})(jQuery);