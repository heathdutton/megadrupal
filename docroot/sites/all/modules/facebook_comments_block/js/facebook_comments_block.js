/**
 * @file
 * Facebook comments block module related scripts.
 */

Drupal.behaviors.facebook_comments_block = {
  attach: function (context, settings) {
    var facebook_app_id_script = Drupal.settings.facebook_comments_block.facebook_settings.facebook_app_id_script;
    (function(d, s, id) {
      var js, fjs = d.getElementsByTagName(s)[0];
      if (d.getElementById(id)) {return;}
      js = d.createElement(s); js.id = id;
      js.src = "//connect.facebook.net/en_US/all.js#xfbml=1" + facebook_app_id_script;
      fjs.parentNode.insertBefore(js, fjs);
    }(document, "script", "facebook-jssdk"));
  }
};
