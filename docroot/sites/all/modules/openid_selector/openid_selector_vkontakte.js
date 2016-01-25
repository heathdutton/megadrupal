(function ($) {
providers_small.vkontakte = {
  name: 'В Контакте',
  url: "javascript: $('#vk_login').click();"
};

openid.getBoxHTML__vkontakte = openid.getBoxHTML;
openid.getBoxHTML = function (box_id, provider, box_size, index) {
  if (box_id == 'vkontakte') {
	  var no_sprite = this.no_sprite;
	  this.no_sprite = true;
	  var result = this.getBoxHTML__vkontakte(box_id, provider, box_size, index);
	  this.no_sprite = no_sprite;
	  return result;
  }
  return this.getBoxHTML__vkontakte(box_id, provider, box_size, index);
}

Drupal.behaviors.openid_selector_vkontakte = { attach: function (context) {
  $('#vk_login').hide();
}}
})(jQuery);
