/**
 * @file
 * Instagram Feeds moderation screen js.
 */

(function($) {
  Drupal.behaviors.instagramFeedsModeration = {
    attach: function() {

      // Unselect Feed after changing Username or Hashtag fields.
      var username = $('#views-exposed-form-instagram-feeds-moderation-admin-instagram-media-items #edit-user');
      var tag = $('#views-exposed-form-instagram-feeds-moderation-admin-instagram-media-items #edit-tag');
      var feed = $('#views-exposed-form-instagram-feeds-moderation-admin-instagram-media-items #edit-feed');
      var feedVal = feed.val();
      var usernameVal = username.val();
      var tagVal = tag.val();

      username.keyup(function() {
        if ('All' != feed.val()) {
          feed.val('All');
        }
      });
      tag.keyup(function() {
        if ('All' != feed.val()) {
          feed.val('All');
        }
      });
      feed.change(function() {
        username.val('');
        tag.val('');
        if (feed.val() == feedVal) {
          username.val(usernameVal);
          tag.val(tagVal);
        }
      });

      // Absoulte location for exposed form Instagram Feeds for "Seven" theme.
      if (Drupal.settings.ajaxPageState.theme == 'seven') {
        $('.view-display-id-admin_instagram_feeds .views-exposed-form')
            .css('position', 'absolute')
            .css('right', 0)
            .css('top', 0)
            .css('z-index', 2);
      }
    }
  };
})(jQuery);
