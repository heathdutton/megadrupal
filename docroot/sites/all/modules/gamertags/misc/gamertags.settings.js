(function ($) {

Drupal.behaviors.gamertagsSettings = {
  attach: function (context, settings) {
    $('#gamertags-usage-table', context).find('.form-checkbox').click(function() {
      var count = $('#gamertags-usage-table', context).find('.form-checkbox:checked').length;
      var regexp = new RegExp("\gamertags_(.*)_collect");
      var platformMatches = regexp.exec(this.name);
      var toggleUrl = 'gamertags-' + platformMatches[1] + '-wrapper';
      $('.' + toggleUrl).toggle(this.checked);
      // Display message if nothing is checked.
      if (count == 0) {
        $("div#gamertags_message").addClass('gamertags-error').text(Drupal.t('No Gamertags are currently set to be displayed.'));
      }
      // Else ensure no message is being displayed.
      else if ($("div#gamertags_message").hasClass('gamertags-error')) {
        $("div#gamertags_message").removeClass('gamertags-error').text('');
      }
    });
  }
};

})(jQuery);