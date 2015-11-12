(function($) {
  Drupal.behaviors.ssGooglePlus = {
    attach : function(context, settings) {
      $('body', context).bind('shareSelectionShow', function(event) {
        gapi.interactivepost.render('share-selection-google-plus', {'clientid':Drupal.settings.shareSelection.googlePlusClientId, 'cookiepolicy':'single_host_origin', 'calltoactionlabel':'COMMENT', 'contenturl':Drupal.settings.shareSelection.googlePlusContenturl, 'calltoactionurl':Drupal.settings.shareSelection.googlePlusContenturl, 'prefilltext':Drupal.shareSelection.selectedText.toString()});
      });
    }
  };
})(jQuery);
