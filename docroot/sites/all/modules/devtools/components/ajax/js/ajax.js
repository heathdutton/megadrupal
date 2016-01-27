/**
 * Command to trigger a callback.
 */
Drupal.ajax.prototype.commands.callback = function(ajax, response, status) {
  Drupal.behaviors[response.behavior][response.method](response.data);
};

/**
 * Command to remove class.
 */
Drupal.ajax.prototype.commands.remove_class = function(ajax, response, status) {
  if (jQuery(response.selector)) {
    jQuery(response.selector).removeClass(response.class);
  }
};