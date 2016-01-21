(function ($) {
providers_small.linkedin = {
  name: 'LinkedIn',
  url: "javascript: window.location = $('a[href$=\"/linkedin/login/0\"]').attr('href');"
};

openid.getBoxHTML__linkedin = openid.getBoxHTML;
openid.getBoxHTML = function (box_id, provider, box_size, index) {
  if (box_id == 'linkedin') {
	  var no_sprite = this.no_sprite;
	  this.no_sprite = true;
	  var result = this.getBoxHTML__linkedin(box_id, provider, box_size, index);
	  this.no_sprite = no_sprite;
	  return result;
  }
  return this.getBoxHTML__linkedin(box_id, provider, box_size, index);
}

Drupal.behaviors.openid_selector_linkedin = { attach: function (context) {
  $('li:has(a[href$="/linkedin/login/0"])').hide();
}}
})(jQuery);
