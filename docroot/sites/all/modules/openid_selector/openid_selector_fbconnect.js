(function ($) {
openid.fbconnect_replace_i = -1;
var replace_id = null;
for (var provider_id in providers_large) {
  openid.fbconnect_replace_i++;
  replace_id = provider_id;
}

var replace_providers = {};
replace_providers[replace_id] = providers_large[replace_id];
providers_small = $.extend(replace_providers, providers_small);
delete providers_large[replace_id];

providers_large.facebook = {
  name: 'Facebook',
  url: "javascript: FB.login(function (response) { facebook_onlogin_ready(); });"
};

openid.getBoxHTML__fbconnect = openid.getBoxHTML;
openid.getBoxHTML = function (box_id, provider, box_size, index) {
  if (box_id == 'facebook') {
	  var no_sprite = this.no_sprite;
	  this.no_sprite = true;
	  var result = this.getBoxHTML__fbconnect(box_id, provider, box_size, index);
	  this.no_sprite = no_sprite;
	  return result;
  } else
  if (index >= this.fbconnect_replace_i) {
	  index--;
  }
  return this.getBoxHTML__fbconnect(box_id, provider, box_size, index);
}

Drupal.behaviors.openid_selector_fbconnect = { attach: function (context) {
  $('#fbconnect_button').hide();
}}
})(jQuery);
