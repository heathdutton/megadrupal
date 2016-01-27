(function($) {

Drupal.behaviors.drupalace = {
  attach: function (context, settings) {

    var login_block = $('#block-user-login', context);
    if (login_block.length > 0) {

      // Open login block in dialog window.
      login_block.dialog({
        autoOpen: false,
        title: Drupal.t('User login'),
        resizable: false,
        width: 220,
        maxWidth: 220,
        modal: true,
        show: "slide",
        hide: "explode"
      });

      // Open dialog with login on button click.
      $('.user-login', context).click(function() {
        login_block.dialog('open');
        return false;
      });
    }

  }
};

})(jQuery);