(function ($) {
providers_small.winliveid = {
  name: 'Windows Live ID',
  url: "javascript: window.location = $('a[href^=\"http://login.live.com/wlogin.srf?appid=\"]').attr('href');"
};

openid.getBoxHTML__winliveid = openid.getBoxHTML;
openid.getBoxHTML = function (box_id, provider, box_size, index) {
  if (box_id == 'winliveid') {
	  var no_sprite = this.no_sprite;
	  this.no_sprite = true;
	  var result = this.getBoxHTML__winliveid(box_id, provider, box_size, index);
	  this.no_sprite = no_sprite;
	  return result;
  }
  return this.getBoxHTML__winliveid(box_id, provider, box_size, index);
}

Drupal.behaviors.openid_selector_winliveid = { attach: function (context) {
  $('a[href^="http://login.live.com/wlogin.srf?appid="]').hide();
}}
})(jQuery);
