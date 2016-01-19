(function ($) {
Drupal.viet_typing = {
	methods: {'auto': 4, 'Telex': 2, 'VNI': 1, 'VIQR': 3, 'VIQR*': 3},
	options: {}
};

Drupal.viet_typing.init = function() {
  Mudim.InitPanel();//must call this first, as we're in very early stage
  Mudim.HidePanel();
  Mudim.DISPLAY_ID = ['notfound','vim-vni','vim-telex','vim-viqr','vim-auto'];
  Mudim.SPELLCHECK_ID = 'notfound';
  Mudim.ACCENTRULE_ID = 'notfound';
}

Drupal.viet_typing.set_vim = function(vim) {
  Mudim.method = Drupal.viet_typing.methods[vim];
  $('#vim-' + vim.toLowerCase()).attr("checked", "checked");
}

Drupal.viet_typing.set_spellcheck = function(onoff) {
  CHIM.Speller.enabled(onoff ? true : false);
}

Drupal.viet_typing.set_oldrules = function(onoff) {
  Mudim.newAccentRule = !onoff;
}

Drupal.viet_typing.set_onoff = function(onoff) {
  if (onoff) {
    Drupal.viet_typing.set_vim(Drupal.viet_typing.options.vim);
  }
  else {
    Mudim.method = 0;
  }
}
})(jQuery);
