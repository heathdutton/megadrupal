(function ($) {
openid.fboauth_replace_i = -1;
var replace_id = null;
for (var provider_id in providers_large) {
  openid.fboauth_replace_i++;
  replace_id = provider_id;
}

var replace_providers = {};
replace_providers[replace_id] = providers_large[replace_id];
providers_small = $.extend(replace_providers, providers_small);
delete providers_large[replace_id];

providers_large.facebook = {
  name: 'Facebook',
  url: "javascript: window.location.href = $('.facebook-action-connect').attr('href')"
};

openid.getBoxHTML__fboauth = openid.getBoxHTML;
openid.getBoxHTML = function (box_id, provider, box_size, index) {
  if (box_id == 'facebook') {
	  var no_sprite = this.no_sprite;
	  this.no_sprite = true;
	  var result = this.getBoxHTML__fboauth(box_id, provider, box_size, index);
	  this.no_sprite = no_sprite;
	  return result;
  } else
  if (index >= this.fboauth_replace_i) {
	  index--;
  }
  return this.getBoxHTML__fboauth(box_id, provider, box_size, index);
}

Drupal.behaviors.openid_selector_fboauth = { attach: function (context) {
  $('.facebook-action-connect').hide();
}}
})(jQuery);
