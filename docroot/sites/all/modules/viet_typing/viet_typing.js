(function ($) {
Drupal.behaviors.viet_typing = {
  attach: function(context, settings) {
    if (context == document) {
      Drupal.viet_typing.init();

      // Appbar integration
      $('#viet_typing_appbar_status').click(function() {
        $('#viet_typing_appbar_control').toggle();
        $('#viet_typing_appbar_control').load(Drupal.settings.basePath +'viet_typing/control', function() {Drupal.behaviors.viet_typing.attach($('#viet_typing_appbar_control'));});
      });
    }

    if (Drupal.settings.viet_typing.vim == 'off') {
      Drupal.viet_typing.set_onoff(0);
    }
    else {
      Drupal.viet_typing.set_vim(Drupal.settings.viet_typing.vim);
    }

    // Attach handlers to buttons
    $(context).find("#vim-auto").click(function() {Drupal.viet_typing.set_vim("auto");});
    $(context).find("#vim-telex").click(function() {Drupal.viet_typing.set_vim("Telex");});
    $(context).find("#vim-vni").click(function() {Drupal.viet_typing.set_vim("VNI");});
    $(context).find("#vim-viqr").click(function() {Drupal.viet_typing.set_vim("VIQR");});
    $(context).find("#vim-off").click(function() {Drupal.viet_typing.set_onoff(0);});
    $(context).find("#vim-oldrules").click(function() {Drupal.viet_typing.set_oldrules(this.checked);});
  }
}
})(jQuery);
