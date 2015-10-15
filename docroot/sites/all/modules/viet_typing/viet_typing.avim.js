(function ($) {
Drupal.viet_typing = {
	methods: {'auto': 0, 'Telex': 1, 'VNI': 2, 'VIQR': 3, 'VIQR*': 4},
	options: {}
};

Drupal.viet_typing.init = function() {
  AVIMObj.radioID = "vim-auto,vim-telex,vim-vni,vim-viqr,vim-viqr,vim-off,notfound,notfound".split(",");
}

Drupal.viet_typing.set_vim = function(vim) {
  AVIMObj.setMethod(Drupal.viet_typing.methods[vim]);
  Drupal.settings.viet_typing.vim = vim;
}

Drupal.viet_typing.set_spellcheck = function(onoff) {
  AVIMObj.setSpell(onoff ? 1 : 0);
}

Drupal.viet_typing.set_oldrules = function(onoff) {
  AVIMGlobalConfig.oldAccent = onoff ? 1 : 0;
}

Drupal.viet_typing.set_onoff = function(onoff) {
  if (onoff) {
    Drupal.viet_typing.set_vim(Drupal.viet_typing.options.vim);
  }
  else {
    AVIMObj.setMethod(-1);
  }
}
})(jQuery);
