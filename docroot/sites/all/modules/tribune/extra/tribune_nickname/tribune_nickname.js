(function($) {

Drupal.behaviors.tribuneNickName = {
  attach: function(context, settings) {
    $('div.form-item-nickname', context).each(function() {
      var tribune = $(this).parents('div.tribune-wrapper:first');

      tribune.bind('beforepost', function(e, data) {
        var nickname = $(this).find('div.form-item-nickname input');
        if (nickname) {
          data.nickname = nickname.val();
        }
      });

      $(this).find('input').change(function() {
        $(this).attr('title', Drupal.t('Your nickname:') + ' ' + $(this).val());
      });
    });
  }
};

})(jQuery);


