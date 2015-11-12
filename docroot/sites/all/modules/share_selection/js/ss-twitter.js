(function($) {
  Drupal.behaviors.ssTwitter = {
    attach : function(context, settings) {
      $('#share-selection-twitter', context).mousedown(function(e) {
        if (Drupal.shareSelection.selectedText) {
          var windowWidth = $(window).width();
          var windowHeight = $(window).height();
          var popupLeft = (windowWidth / 2) - 300;
          var popupTop = (windowHeight / 2) - 110;
          window.open('https://twitter.com/intent/tweet?text=' + Drupal.shareSelection.selectedText, 'shareWindow', 'width=600, height=260, top=' + popupTop + ', left=' + popupLeft);

          // Remain the text selected.
          return false;
        };
      });
    }
  };
})(jQuery);
